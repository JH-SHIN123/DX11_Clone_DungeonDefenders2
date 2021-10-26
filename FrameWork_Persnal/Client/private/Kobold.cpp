#include "stdafx.h"
#include "..\public\Kobold.h"
#include "Data_Manager.h"
#include "Kobold_Boom.h"
#include "ManaToken.h"
#include "Point_Ex_Particle.h"
#include "Point_Ex_Healing.h"

CKobold::CKobold(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CKobold::CKobold(const CKobold & rhs)
	: CMonster(rhs)
{
}

HRESULT CKobold::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CKobold::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);



	Set_HpBar_OffSet_Position(_float3(0.f, 12.f, 0.f));

	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	Set_Pivot(XMVectorSet(1.f, 1.f, 1.f, 0.f));

	m_IsKobold = true;

	m_pColliderCom_Attack->Set_NotCollide(true);
	return S_OK;
}

_int CKobold::Tick(_float TimeDelta)
{
	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EKoboldAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{
				CData_Manager::GetInstance()->Add_MonsterCount();
				CData_Manager::GetInstance()->Add_Score(m_pStatusCom->Get_Exp() * 5);

				_vector vPos = m_pMovementCom->Get_State(EState::Position);
				vPos.m128_f32[1] += 5.f;

				MANATOKEN_DESC Data;
				XMStoreFloat4(&Data.Move_Desc.vPos, vPos);
				Data.Move_Desc.vScale = { 1.f,1.f,1.f,0.f };
				Data.Collider_Desc.vScale = { 2.f,2.f,2.f };

				GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_ManaToken"), (_uint)ELevel::Stage1, L"Layer_ManaToken", &Data);

				return OBJECT_DEAD;
			}
		}
		else
			m_eAnim_Next = EKoboldAnim::Death;
	}

	_vector vTargetPos;

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EKoboldAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EKoboldAnim::Idle;
		}
		m_IsHurt = false;
	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		Create_Hit_Particle(4.f);

		m_IsDeadSound = true;

		_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
		if (60.f > fDis)
		{
			fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

			CSound_Manager::GetInstance()->StopSound(CHANNEL_KOBOLD_E);
			CSound_Manager::GetInstance()->Play_Sound(L"Kobold_explode2.ogg", CHANNEL_KOBOLD_E);

			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_KOBOLD_E, fDis);
		}

		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;

		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EKoboldAnim::Hurt;
			break;
		default:
			m_IsHurt = false;
			break;
		}
	}
	else
	{
		switch (AI_Check_Kobold(TimeDelta, &vTargetPos, m_IsAttack))
		{
		case Client::EMonsterAI::Idle:
			m_eAnim_Next = EKoboldAnim::Idle;
			break;
		case Client::EMonsterAI::Attack:
			m_IsAttack = true;
			break;
		case Client::EMonsterAI::Cough:
			m_eAnim_Next = EKoboldAnim::Cough;
			break;
		case Client::EMonsterAI::Dead:
			m_eAnim_Next = EKoboldAnim::Death;
			break;
		case Client::EMonsterAI::Shock:
			m_eAnim_Next = EKoboldAnim::Hurt;
			break;
		case Client::EMonsterAI::Move_Cell:
			if (true == m_IsAttack)
				break;
			m_eAnim_Next = EKoboldAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Move_Target:
			if (true == m_IsAttack)
				break;
			m_eAnim_Next = EKoboldAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Turn:
			if (true == m_IsAttack)
				break;
			m_eAnim_Next = EKoboldAnim::Turn;
			break;
		default:
			break;
		}
	}

	if (true == m_pColliderCom_Attack->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsExplosion = true;
	}


	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColiiderCom_Push->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pStatusCom->Tick(TimeDelta);

	__super::Tick(TimeDelta);

	return _int();
}

_int CKobold::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (true == m_pModelCom->Get_IsFinishedAnimaion())
		{
			CData_Manager::GetInstance()->Add_MonsterCount();
			CData_Manager::GetInstance()->Add_Score(m_pStatusCom->Get_Exp() * 5);

			_vector vPos = m_pMovementCom->Get_State(EState::Position);
			vPos.m128_f32[1] += 5.f;

			MANATOKEN_DESC Data;
			XMStoreFloat4(&Data.Move_Desc.vPos, vPos);
			Data.Move_Desc.vScale = { 1.f,1.f,1.f,0.f };
			Data.Collider_Desc.vScale = { 2.f,2.f,2.f };

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_ManaToken"), (_uint)ELevel::Stage1, L"Layer_ManaToken", &Data);

			return OBJECT_DEAD;
		}
		else
			m_eAnim_Next = EKoboldAnim::Death;
	}

	if (true == m_IsExplosion)
	{
		Explosion();
		CData_Manager::GetInstance()->Add_MonsterCount();
		return OBJECT_DEAD;
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CKobold::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColiiderCom_Push->Render_Collider();
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

void CKobold::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (true == m_IsFuseOn)
	{
		m_eAnim_Next = EKoboldAnim::Run_Suicide;
		_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
		if (60.f > fDis)
		{
			fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

			if (false == CSound_Manager::GetInstance()->IsPlaying(CHANNEL_KOBOLD_A))
			{			
				CSound_Manager::GetInstance()->Play_Sound(L"Kobold_screamloop2.ogg", CHANNEL_KOBOLD_A);
			}
				CSound_Manager::GetInstance()->Set_Volume(CHANNEL_KOBOLD_A, fDis - 0.7f);
		}

	}

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EKoboldAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EKoboldAnim::Idle;
		}
		m_IsAttack = false;
	}

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();

	_matrix Matrix = m_pMovementCom->Get_WorldMatrix();
	Matrix.r[3] += XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * 5.f;
	m_pColliderCom_Attack->Update_Collider(Matrix);

	m_eAnim_Cur = m_eAnim_Next;
}

void CKobold::Attack_Check()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	_int iAnimTime = (_int)m_pModelCom->Get_AnimTime();
	if (true == m_IsAttack && false == m_IsFuseOn)
	{
		m_eAnim_Next = EKoboldAnim::LightFuse;

		if (205 == iAnimTime)
		{
			m_IsFuseOn = true;
			m_IsAttack = false;
			MOVESTATE_DESC Data = m_pMovementCom->Get_Movestate_Desc();

			m_pColliderCom_Attack->Set_NotCollide(false);

			_float fSpeed = Data.fSpeedPerSec;
			fSpeed *= 1.3f;

			Data.fSpeedPerSec = fSpeed;

			m_pMovementCom->Set_Movestate_Desc(Data);
		}
	}

	else
	{
		m_IsAttack = false;
	}
}

EMonsterAI CKobold::AI_Check_Kobold(_float TimeDelta, _vector * pTargetPos, _bool IsContinueAnimation)
{
	// 피없음 뒤지고
	if (0 >= m_pStatusCom->Get_Hp())
		return EMonsterAI::Dead;

	if (true == m_IsPoison)
	{
		m_fCoughTime += TimeDelta;

		if (m_fCoughTick <= m_fCoughTime)
		{
			m_fCoughTime = 0.f;
			return EMonsterAI::Cough;
		}
	}

	// 세뇌중이면 이거
	if (true == m_IsBrainWashed)
		return AI_BrainWashed(TimeDelta, pTargetPos, IsContinueAnimation);

	// 때린거 마저 때리고
	if (EMonsterAI::Attack == m_eAI_Next && IsContinueAnimation)
		return EMonsterAI::Attack;

	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	_vector vMyPos_Cell = XMVectorSetY(vMyPos, 0.f);

#pragma region Core
	// 무조건 때림
	_vector vCorePos = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_CrystalCore"))->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
	_float fCoreDis = XMVectorGetX(XMVector3Length(vCorePos - vMyPos));

	if (m_fAttackDis > fCoreDis)
	{
		_vector vDir = XMVector3Normalize(vCorePos - vMyPos);
		_vector vLook = vDir;
		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector vRight = XMVector3Cross(vUp, vDir);

		m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
		m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
		m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);

		return EMonsterAI::Attack;
	}
#pragma endregion


#pragma region Player

	if (false == m_IsFuseOn)
	{

		CMovement* pTarget_Player = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Get_Component(L"Com_Movement"));
		if (nullptr == pTarget_Player)
			return EMonsterAI::Idle;

		_vector vTargetPos = pTarget_Player->Get_State(EState::Position);

		_float fDis = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

		if (m_fAttackDis > fDis && false == m_IsTowerAttack)
		{
			// 돌다가 때려
			if (true == m_IsFuseOn)
				return m_eAI_Next = EMonsterAI::Move_Target;
			return m_eAI_Next = EMonsterAI::Attack;
		}

		if (m_fDetectDis > fDis)
		{
			return m_eAI_Next = EMonsterAI::Attack;
		}
#pragma endregion


#pragma region Tower

		// 타워 거리 탐색
		CLayer* pLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1, L"Layer_Tower");

		if (nullptr != pLayer)
		{
			list<CGameObject*> listObject = pLayer->Get_GameObject_List();

			_float fTowerDis = m_fAttackDis;
			for (auto& iter : listObject)
			{
				_vector vTowerPos = static_cast<CMovement*>(iter->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
				_float	fDis = XMVectorGetX(XMVector3Length(vTowerPos - vMyPos));
				if (fTowerDis > fDis)
				{
					// 타워 때릴 준비 완료
					fTowerDis = fDis;
					*pTargetPos = vTowerPos;
					if (true == m_IsFuseOn)
						return m_eAI_Next = EMonsterAI::Move_Target;

				}
			}

			if (m_fAttackDis > fTowerDis)
			{
				// 내 눈앞에 있는가
				m_IsTowerAttack = true;
				m_IsChaseTarget = false;
				_vector vDir = XMVector3Normalize(*pTargetPos - vMyPos);
				_float fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)), vDir))));

				_vector vLookDir = XMVector3Normalize(vDir - m_pMovementCom->Get_State(EState::Look));

				if (3.f > fTurnAngle)
				{
					_vector vLook = vDir;
					_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
					_vector vRight = XMVector3Cross(vUp, vDir);

					m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
					m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
					m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);
					//m_pMovementCom->Go_Dir(TimeDelta, vDir, m_pNaviCom);
				}

				else
				{
					m_pMovementCom->RotateToLookDir_Tick(TimeDelta * 2.f, vLookDir);

				}
				if (true == m_IsFuseOn)
					return m_eAI_Next = EMonsterAI::Move_Target;

				return EMonsterAI::Attack;
			}
		}
	}
#pragma endregion


#pragma region Navigation
	// 너 갈길 가라
	_vector vCellPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);

	if (7 > m_iMoveCount)
	{
		switch (m_eMovePath)
		{
		case Client::EMonster_MovePath::North_L:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_L[m_iMoveCount]);
			break;
		case Client::EMonster_MovePath::North_R:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_R[m_iMoveCount]);
			break;
		case Client::EMonster_MovePath::West_L:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_L[m_iMoveCount]);
			break;
		case Client::EMonster_MovePath::West_R:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_R[m_iMoveCount]);
			break;
		}
	}
	else
		vCellPos = m_pNaviCom->Get_CellCenter_Pos(0);

	// 다음 루트가 멀어져버렸다
	_vector vNextCell_Pos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	if (8 > m_iMoveCount + 1)
	{
		switch (m_eMovePath)
		{
		case Client::EMonster_MovePath::North_L:
			vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_L[m_iMoveCount + 1]);
			break;
		case Client::EMonster_MovePath::North_R:
			vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_R[m_iMoveCount + 1]);
			break;
		case Client::EMonster_MovePath::West_L:
			vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_L[m_iMoveCount + 1]);
			break;
		case Client::EMonster_MovePath::West_R:
			vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_R[m_iMoveCount + 1]);
			break;
		}
	}
	else
		vNextCell_Pos = vCellPos;

	_float fCellDis = XMVectorGetX(XMVector3Length(vCellPos - vMyPos_Cell));
	_float fNextCellDis = XMVectorGetX(XMVector3Length(vCellPos - vNextCell_Pos));

	// 내가 딴길로 샛다 == 플레이어 따라댕겼다
	if ((true == m_IsChaseTarget) || (true == m_IsTowerAttack))
	{
		// 가까운곳으로만 가면 그만
		_int iNearCellIndex = -1;
		_float fDis = -1.f;
		fDis = Get_DisToCell(m_iNorth_L[m_iMoveCount]);
		iNearCellIndex = m_iNorth_L[m_iMoveCount];

		if (fDis > Get_DisToCell(m_iNorth_R[m_iMoveCount]))
		{
			iNearCellIndex = m_iNorth_R[m_iMoveCount];
			fDis = Get_DisToCell(m_iNorth_R[m_iMoveCount]);
		}
		if (fDis > Get_DisToCell(m_iWest_L[m_iMoveCount]))
		{
			iNearCellIndex = m_iWest_L[m_iMoveCount];
			fDis = Get_DisToCell(m_iWest_L[m_iMoveCount]);
		}
		if (fDis > Get_DisToCell(m_iWest_R[m_iMoveCount]))
		{
			iNearCellIndex = m_iWest_R[m_iMoveCount];
			fDis = Get_DisToCell(m_iWest_R[m_iMoveCount]);
		}

		if (-1 < m_iMoveCount - 1)
		{
			if (fDis > Get_DisToCell(m_iNorth_L[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iNorth_L[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iNorth_L[m_iMoveCount - 1]);
			}
			if (fDis > Get_DisToCell(m_iWest_R[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iWest_R[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iWest_R[m_iMoveCount - 1]);
			}
			if (fDis > Get_DisToCell(m_iNorth_R[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iNorth_R[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iNorth_R[m_iMoveCount - 1]);
			}
			if (fDis > Get_DisToCell(m_iWest_L[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iWest_L[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iWest_L[m_iMoveCount - 1]);
			}
		}

		if (8 > m_iMoveCount + 1)
		{
			if (fDis > Get_DisToCell(m_iNorth_L[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iNorth_L[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iNorth_L[m_iMoveCount + 1]);
			}
			if (fDis > Get_DisToCell(m_iWest_R[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iWest_R[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iWest_R[m_iMoveCount + 1]);
			}
			if (fDis > Get_DisToCell(m_iNorth_R[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iNorth_R[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iNorth_R[m_iMoveCount + 1]);
			}
			if (fDis > Get_DisToCell(m_iWest_L[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iWest_L[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iWest_L[m_iMoveCount + 1]);
			}
		}
		// 제일 가까운 경로 찾음
		vCellPos = m_pNaviCom->Get_CellCenter_Pos(iNearCellIndex);
		m_eMovePath = Research_MovePath(iNearCellIndex);

		vNextCell_Pos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		if (8 > m_iMoveCount + 1)
		{
			switch (m_eMovePath)
			{
			case Client::EMonster_MovePath::North_L:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_L[m_iMoveCount + 1]);
				break;
			case Client::EMonster_MovePath::North_R:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_R[m_iMoveCount + 1]);
				break;
			case Client::EMonster_MovePath::West_L:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_L[m_iMoveCount + 1]);
				break;
			case Client::EMonster_MovePath::West_R:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_R[m_iMoveCount] + 1);
				break;
			}
		}
		else
			vCellPos = vMyPos_Cell;

		m_IsTowerAttack = false;
		m_IsChaseTarget = false;
	}

	// 다음꺼 검색하셈
	if (0.5f >= fCellDis && 8 > m_iMoveCount + 1)
	{
		++m_iMoveCount;
		return EMonsterAI::Idle;
	}

	*pTargetPos = vCellPos;

	// 방향 바꿔주자
	_vector vCur_Next_Dir = XMVector3Normalize(vCellPos - vMyPos_Cell);
	_vector vTest = XMVectorZero();

	if (false == XMVector3Equal(vCur_Next_Dir, vTest))
	{
		_float fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)), vCur_Next_Dir))));

		_vector vDir = XMVector3Normalize(vCur_Next_Dir - m_pMovementCom->Get_State(EState::Look));

		if (5.f > fTurnAngle)
		{
			_vector vLook = vCur_Next_Dir;
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector vRight = XMVector3Cross(vUp, vCur_Next_Dir);

			m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
			m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
			m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);

			vCellPos = XMVectorSetY(vCellPos, XMVectorGetY(vMyPos));
			m_pMovementCom->Go_Dir(TimeDelta, vCellPos, m_pNaviCom);

			return m_eAI_Next = EMonsterAI::Move_Target;
		}

		else // 이함수가 이상하게 반복되면 몬스터의 X스케일이 점점 작아짐 ㅋㅋㅋㅋㅋㅋ
		{
			m_pMovementCom->RotateToLookDir_Tick(TimeDelta, vCur_Next_Dir);
			return m_eAI_Next = EMonsterAI::Move_Target;
		}
	}
#pragma endregion

	return m_eAI_Next = EMonsterAI::Idle;
}

void CKobold::Explosion()
{
	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);

	BULLET_DESC Data;
	lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower_Bullet");

	XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	Data.MoveState_Desc.fSpeedPerSec = 40.f;
	Data.fLifeTime = 10.f;

	Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Collide_Desc.Attack_Desc.iDamage = 30;
	Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	Data.Attack_Collide_Desc.vScale = { 7.f, 7.f, 7.f };
	//Data.Attack_Collide_Desc.vPosition = { 0.f, 50.f, 0.f };
	Data.Attack_Collide_Desc.IsCenter = true;

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Kobold_Boom", (_uint)ELevel::Stage1, L"Layer_Bullet_Monster", &Data);


	_int iSize = 10;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	vPos.m128_f32[1] += 4.f;


	POINT_EX_HEAL_DESC Point_Desc;
	Point_Desc.Point_Desc.iRandDir = 7;
	Point_Desc.Point_Desc.fLifeTime = 3.f;
	Point_Desc.Point_Desc.fSize = ((_float)iSize);
	Point_Desc.Point_Desc.fSpreadDis = 7.f;
	Point_Desc.Point_Desc.fTimeTerm = 0.05f;
	Point_Desc.Point_Desc.InstanceValue = EInstanceValue::Point_Ex_200_10;
	Point_Desc.Point_Desc.iShaderPass = 3;
	Point_Desc.Point_Desc.fAlphaTime = 1.f;
	Point_Desc.Point_Desc.fAlphaTimePower = 2.f;
	Point_Desc.Point_Desc.fScalePower = 3.f;
	XMStoreFloat4(&Point_Desc.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Point_Desc.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
	lstrcpy(Point_Desc.Point_Desc.szTextrueName, L"Component_Texture_Explosion");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Particle", (_uint)ELevel::Stage1, L"Layer_Effect", &Point_Desc);

	iSize = 7;
	Point_Desc.Point_Desc.iRandDir = 7;
	Point_Desc.Point_Desc.fLifeTime = 3.f;
	Point_Desc.Point_Desc.fSize = ((_float)iSize);
	Point_Desc.Point_Desc.fSpreadDis = 3.f;
	Point_Desc.Point_Desc.fTimeTerm = 0.05f;
	Point_Desc.Point_Desc.InstanceValue = EInstanceValue::Point_Ex_200_10;
	Point_Desc.Point_Desc.iShaderPass = 3;
	Point_Desc.Point_Desc.fAlphaTime = 1.f;
	Point_Desc.Point_Desc.fAlphaTimePower = 3.f;
	Point_Desc.Point_Desc.fScalePower = 4.f;
	XMStoreFloat4(&Point_Desc.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Point_Desc.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
	lstrcpy(Point_Desc.Point_Desc.szTextrueName, L"Component_Texture_Explosion");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Particle", (_uint)ELevel::Stage1, L"Layer_Effect", &Point_Desc);


	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	if (60.f > fDis)
	{
		fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;
		_int i = rand() % 2;

		CSound_Manager::GetInstance()->StopSound(CHANNEL_KOBOLD);
		if(i == 0)
			CSound_Manager::GetInstance()->Play_Sound(L"Kobold_explode1.ogg", CHANNEL_KOBOLD);
		else
			CSound_Manager::GetInstance()->Play_Sound(L"Kobold_explode2.ogg", CHANNEL_KOBOLD);

		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_KOBOLD, fDis - 0.7f);
	}

}

HRESULT CKobold::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 6.f, 6.f, 6.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hurt, &Data);

	Data.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Desc.iDamage = 30;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);

	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 3.f, 3.f, 3.f };
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Push"), (CComponent**)&m_pColiiderCom_Push, &Data);

	return S_OK;
}

_float CKobold::Anim_Changer(EKoboldAnim eAnim)
{
	EKoboldAnim eAnim_Term = EKoboldAnim::End;

	switch (eAnim)
	{
	case Client::EKoboldAnim::Cough:
		eAnim_Term = EKoboldAnim::Death;
		break;
	case Client::EKoboldAnim::Death:
		eAnim_Term = EKoboldAnim::Hurt;
		break;
	case Client::EKoboldAnim::Hurt:
		eAnim_Term = EKoboldAnim::Idle;
		break;
	case Client::EKoboldAnim::Idle:
		eAnim_Term = EKoboldAnim::Jump;
		break;
	case Client::EKoboldAnim::Jump:
		eAnim_Term = EKoboldAnim::LightFuse;
		break;
	case Client::EKoboldAnim::LightFuse:
		eAnim_Term = EKoboldAnim::Move_Backward;
		break;
	case Client::EKoboldAnim::Move_Backward:
		eAnim_Term = EKoboldAnim::Move_Forward;
		break;
	case Client::EKoboldAnim::Move_Forward:
		eAnim_Term = EKoboldAnim::Run_Suicide;
		break;
	case Client::EKoboldAnim::Run_Suicide:
		eAnim_Term = EKoboldAnim::Shock;
		break;
	case Client::EKoboldAnim::Shock:
		eAnim_Term = EKoboldAnim::Turn;
		break;
	case Client::EKoboldAnim::Turn:
		eAnim_Term = EKoboldAnim::End_Anim;
		break;
	default:
		eAnim_Term = EKoboldAnim::End_Anim;
		break;
	}
	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next - 1.f;

	return fAnim;
}

void CKobold::AI_Check()
{


}

CKobold * CKobold::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CKobold* pInstance = new CKobold(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CKobold) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CKobold::Clone_GameObject(void * pArg)
{
	CKobold* pInstance = new CKobold(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CKobold) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKobold::Free()
{
	Safe_Release(m_pColiiderCom_Push);
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
