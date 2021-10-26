#include "stdafx.h"
#include "..\public\Monster.h"
#include "Masking_MeterBar_3D.h"
#include "Player.h"
#include "Point_Ex_Healing.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
	
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CMonster::Tick(_float TimeDelta)
{
	m_IsPushing = false;

	if (nullptr != m_pMeterBar_Hp)
	{
		_vector vMyPos = m_pMovementCom->Get_State(EState::Position) + XMLoadFloat3(&m_vHpBar_OffSet_Position);

		m_pMeterBar_Hp->Set_Position(vMyPos);

		m_pMeterBar_Hp->Tick(TimeDelta);
	}

	return _int();
}

_int CMonster::Late_Tick(_float TimeDelta)
{
	if (nullptr != m_pMeterBar_Hp && 0 < m_pStatusCom->Get_Hp())
	{
		m_pMeterBar_Hp->Set_Count((_float)m_pStatusCom->Get_Hp(), (_float)m_pStatusCom->Get_HpMax());
		m_pMeterBar_Hp->Late_Tick(TimeDelta);
	}

	if (true == m_IsPoison)
	{
		m_fPoisonTime += TimeDelta;

		if (m_fPoisonTick <= m_fPoisonTime)
		{
			_float fDamage = (_float)m_pStatusCom->Get_HpMax() * 0.0125f;

			ATTACK_DESC Attack;
			Attack.iDamage = ((_int)fDamage) >> 1;
			Attack.eDamageType = EDamageType::Direct;
			Attack.fHitTime = 0.f;

			m_fPoisonTime = 0.f;
			m_pStatusCom->Set_Damage(Attack);
		}
	}

	if (0 >= m_pStatusCom->Get_Hp())
	{
		m_fDissolveTime += TimeDelta * 0.5f;
		m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CMonster::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	_int iShader = 0;

	if (0 > m_pStatusCom->Get_Hp()) 
	{
		iShader = 12;
		//_vector vColor_Now = XMLoadFloat4(&m_vColor);
		//_vector vColor = XMVectorSet(5.f, 5.f, 5.f, 0.f);
		//
		//_vector vTerm = vColor - vColor_Now;
		//vColor_Now += vTerm * (0.008f);
		//
		//XMStoreFloat4(&m_vColor, vColor_Now);
		//m_pModelCom->Set_Variable("g_vColor", &m_vColor, sizeof(_float4));

		m_pModelCom->Set_ShaderResourceView_Direct("g_DissolveTexture", m_pTextureCom_Dissolve->Get_ShaderResourceView(0));
		m_pModelCom->Set_Variable("g_fTime", &m_fDissolveTime, sizeof(_float));

	}

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, iShader);
	}


#ifdef _DEBUG
	//m_pColliderCom->Render_Collider();
#endif


	return S_OK;
}

void CMonster::Create_Hit_Particle(_float fOffSetY)
{
	_int iSize = rand() % 3 + 1;

	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	vPos.m128_f32[1] += fOffSetY;

	POINT_EX_HEAL_DESC Point_Desc;
	Point_Desc.Point_Desc.iRandDir = 7;
	Point_Desc.Point_Desc.fLifeTime = 3.f;
	Point_Desc.Point_Desc.fSize = ((_float)iSize / 5.f);
	Point_Desc.Point_Desc.fSpreadDis = 3.f;
	Point_Desc.Point_Desc.fTimeTerm = 0.05f;
	Point_Desc.Point_Desc.InstanceValue = EInstanceValue::Point_Ex_200_10;
	Point_Desc.Point_Desc.iShaderPass = 3;
	Point_Desc.Point_Desc.fAlphaTime = 1.f;
	Point_Desc.Point_Desc.fAlphaTimePower = 2.f;
	Point_Desc.Point_Desc.fScalePower = 3.f;
	XMStoreFloat4(&Point_Desc.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Point_Desc.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
	lstrcpy(Point_Desc.Point_Desc.szTextrueName, L"Component_Texture_Blood_Particle");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Particle", (_uint)ELevel::Stage1, L"Layer_Effect", &Point_Desc);
}

EMonsterAI CMonster::AI_Check(_float TimeDelta, _vector* pTargetPos, _bool IsContinueAnimation)
{
	// 피없음 뒤지고
	if (0 >= m_pStatusCom->Get_Hp())
		return EMonsterAI::Dead;

	if (true == m_IsPoison)
	{
		//m_fCoughTime += TimeDelta;
		//
		//if (m_fCoughTick <= m_fCoughTime)
		//{
		//	return EMonsterAI::Cough;
		//}
	}

	// 하던거 마저 하고
	if (/*EMonsterAI::Attack == m_eAI_Next &&*/ IsContinueAnimation)
		return m_eAI_Next;

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

	CMovement* pTarget_Player = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Get_Component(L"Com_Movement"));
	if (nullptr == pTarget_Player)
		return EMonsterAI::Idle;

	_vector vTargetPos = pTarget_Player->Get_State(EState::Position);

	_float fDis = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

	if (m_fAttackDis > fDis && false == m_IsTowerAttack)
	{
		// 돌다가 때려
		return m_eAI_Next = EMonsterAI::Attack;
	}

	if (m_fDetectDis > fDis)
	{
		m_IsChaseTarget = true;
		_vector vDir = XMVector3Normalize(vTargetPos - vMyPos);
		_float fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)), vDir))));

		*pTargetPos = vTargetPos;

		if (80.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 4.f, vTargetPos);

			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (60.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 3.f, vTargetPos);
			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (40.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 2.5f, vTargetPos);
			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (20.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta* 1.75f, vTargetPos);
			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (10.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, vTargetPos);
			m_pMovementCom->Go_Dir(TimeDelta, vTargetPos, m_pNaviCom);
			return m_eAI_Next = EMonsterAI::Move_Target;
		}
		else
		{
			_vector vLook = vDir;
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			_vector vRight = XMVector3Cross(vUp, vLook);

			m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
			m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
			m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);

			m_pMovementCom->Go_Dir(TimeDelta, vTargetPos, m_pNaviCom);
			return m_eAI_Next = EMonsterAI::Move_Target;
		}
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

				_vector vDir = XMVector3Normalize(*pTargetPos - vMyPos);
				_vector vLook = vDir;
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				_vector vRight = XMVector3Cross(vUp, vDir);

				m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
				m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
				m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);
				//m_pMovementCom->Go_Dir(TimeDelta, vDir, m_pNaviCom);

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

			return EMonsterAI::Attack;
		}
	}

#pragma endregion


#pragma region Navigation
	// 너 갈길 가라
	_vector vCellPos = XMVectorSet(0.f,0.f,0.f,1.f);

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
		case EMonster_MovePath::East_L:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount]);
			break;
		case EMonster_MovePath::East_R:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount]);
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
		case EMonster_MovePath::East_L:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount] + 1);
			break;
		case EMonster_MovePath::East_R:
			vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount] + 1);
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
		if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount]))
		{
			iNearCellIndex = m_iEast_R[m_iMoveCount];
			fDis = Get_DisToCell(m_iEast_R[m_iMoveCount]);
		}
		if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount]))
		{
			iNearCellIndex = m_iEast_L[m_iMoveCount];
			fDis = Get_DisToCell(m_iEast_L[m_iMoveCount]);
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

			if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iEast_R[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iEast_R[m_iMoveCount - 1]);
			}

			if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount - 1]))
			{
				iNearCellIndex = m_iEast_L[m_iMoveCount - 1];
				fDis = Get_DisToCell(m_iEast_L[m_iMoveCount - 1]);
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
			if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iEast_R[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iEast_R[m_iMoveCount + 1]);
			}
			if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount + 1]))
			{
				iNearCellIndex = m_iEast_L[m_iMoveCount + 1];
				fDis = Get_DisToCell(m_iEast_L[m_iMoveCount + 1]);
			}
		}
		// 제일 가까운 경로 찾음
		vCellPos = m_pNaviCom->Get_CellCenter_Pos(iNearCellIndex);
		m_eMovePath = Research_MovePath(iNearCellIndex);

		vNextCell_Pos = XMVectorSet(0.f,0.f,0.f,1.f);
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
			case Client::EMonster_MovePath::East_L:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount + 1]);
				break;	
			case Client::EMonster_MovePath::East_R:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount + 1]);
				break;
			}
		}
		else
			vCellPos = vMyPos_Cell;

		m_IsTowerAttack = false;
		m_IsChaseTarget = false;
	}

	// 다음꺼 검색하셈
	if (3.f >= fCellDis && 8 > m_iMoveCount + 1)
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

		if (7.f > fTurnAngle)
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
			m_pMovementCom->Go_Dir(TimeDelta * 0.75f, vCellPos, m_pNaviCom);
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, vCellPos);
			//m_pMovementCom->RotateToLookDir_Tick(TimeDelta, vCur_Next_Dir);
			return m_eAI_Next = EMonsterAI::Move_Target;
		}
	}
#pragma endregion

	return m_eAI_Next = EMonsterAI::Idle;
}

EMonsterAI CMonster::AI_BrainWashed(_float TimeDelta, _vector * pTargetPos, _bool IsContinueAnimation)
{
	m_pMeterBar_Hp->Set_ShaderPass(15);

	if (true == IsContinueAnimation ||
		true == m_IsBrainWashed)
		return EMonsterAI::Shock;

	if (true == m_IsBrainWashed_Complete)
	{
		_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
		_vector vMyPos_Cell = XMVectorSetY(vMyPos, 0.f);

		CMovement* pTarget_Monster = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Monster"))->Get_Component(L"Com_Movement"));
		if (nullptr == pTarget_Monster)
			return EMonsterAI::Idle;

		_vector vTargetPos = pTarget_Monster->Get_State(EState::Position);

		_float fDis = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

		if (m_fAttackDis > fDis && false == m_IsTowerAttack)
		{

			return m_eAI_Next = EMonsterAI::Attack;
		}

		if (m_fDetectDis > fDis)
		{
			m_IsChaseTarget = true;
			_vector vDir = XMVector3Normalize(vTargetPos - vMyPos);
			_float fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)), vDir))));

			*pTargetPos = vTargetPos;

			if (80.f < fTurnAngle)
			{
				m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 4.f, vTargetPos);

				return m_eAI_Next = EMonsterAI::Turn;
			}
			else if (60.f < fTurnAngle)
			{
				m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 3.f, vTargetPos);
				return m_eAI_Next = EMonsterAI::Turn;
			}
			else if (40.f < fTurnAngle)
			{
				m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 2.5f, vTargetPos);
				return m_eAI_Next = EMonsterAI::Turn;
			}
			else if (20.f < fTurnAngle)
			{
				m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta* 1.75f, vTargetPos);
				return m_eAI_Next = EMonsterAI::Turn;
			}
			else if (10.f < fTurnAngle)
			{
				m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, vTargetPos);
				m_pMovementCom->Go_Dir(TimeDelta, vTargetPos, m_pNaviCom);
				return m_eAI_Next = EMonsterAI::Move_Target;
			}
			else
			{
				_vector vLook = vDir;
				_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
				_vector vRight = XMVector3Cross(vUp, vLook);

				m_pMovementCom->Set_State(EState::Right, vRight * m_vScale.x);
				m_pMovementCom->Set_State(EState::Up, vUp *m_vScale.y);
				m_pMovementCom->Set_State(EState::Look, vLook * m_vScale.z);

				m_pMovementCom->Go_Dir(TimeDelta, vTargetPos, m_pNaviCom);
				return m_eAI_Next = EMonsterAI::Move_Target;
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
			case Client::EMonster_MovePath::East_L:
				vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount]);
				break;
			case Client::EMonster_MovePath::East_R:
				vCellPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount]);
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
			case Client::EMonster_MovePath::East_L:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount + 1]);
				break;
			case Client::EMonster_MovePath::East_R:
				vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount + 1]);
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
			if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount]))
			{
				iNearCellIndex = m_iEast_R[m_iMoveCount];
				fDis = Get_DisToCell(m_iEast_R[m_iMoveCount]);
			}
			if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount]))
			{
				iNearCellIndex = m_iEast_L[m_iMoveCount];
				fDis = Get_DisToCell(m_iEast_L[m_iMoveCount]);
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
				if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount - 1]))
				{
					iNearCellIndex = m_iEast_R[m_iMoveCount - 1];
					fDis = Get_DisToCell(m_iEast_R[m_iMoveCount - 1]);
				}
				if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount - 1]))
				{
					iNearCellIndex = m_iEast_L[m_iMoveCount - 1];
					fDis = Get_DisToCell(m_iEast_L[m_iMoveCount - 1]);
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
				if (fDis > Get_DisToCell(m_iEast_R[m_iMoveCount + 1]))
				{
					iNearCellIndex = m_iEast_R[m_iMoveCount + 1];
					fDis = Get_DisToCell(m_iEast_R[m_iMoveCount + 1]);
				}
				if (fDis > Get_DisToCell(m_iEast_L[m_iMoveCount + 1]))
				{
					iNearCellIndex = m_iEast_L[m_iMoveCount + 1];
					fDis = Get_DisToCell(m_iEast_L[m_iMoveCount + 1]);
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
				case Client::EMonster_MovePath::East_L:
					vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[m_iMoveCount] + 1);
					break;
				case Client::EMonster_MovePath::East_R:
					vNextCell_Pos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[m_iMoveCount] + 1);
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

	return EMonsterAI::Idle;
}

EMonster_MovePath CMonster::Research_MovePath(const _int & iCellIndex)
{
	for (_int i = 0; i < 7; ++i)
	{
		if (iCellIndex == m_iNorth_L[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::North_L;
		}
		if (iCellIndex == m_iNorth_R[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::North_R;
		}
		if (iCellIndex == m_iWest_L[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::West_L;
		}
		if (iCellIndex == m_iWest_R[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::West_R;
		}

		if (iCellIndex == m_iEast_L[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::East_L;
		}

		if (iCellIndex == m_iEast_R[i])
		{
			m_iMoveCount = i;
			return EMonster_MovePath::East_R;
		}
	}

	return EMonster_MovePath::End;
}

HRESULT CMonster::Ready_Component(void * pArg)
{
	MONSTER_DESC Data;
	memcpy(&Data, pArg, sizeof(MONSTER_DESC));
	
	HRESULT hr = S_OK;
	// m_pTextureCom_Dissolve
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_Dissolve"), TEXT("Com_Tex"), (CComponent**)&m_pTextureCom_Dissolve);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)Data.eLevel, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_MeshLevel_1_Navi"), TEXT("Com_Navi"), (CComponent**)&m_pNaviCom);

	m_eMovePath = Data.eMovePath;

	_vector vPos = XMVectorZero();
	switch (m_eMovePath)
	{
	case Client::EMonster_MovePath::North_L:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_L[0]);
		break;
	case Client::EMonster_MovePath::North_R:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iNorth_R[0]);
		break;
	case Client::EMonster_MovePath::West_L:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_L[0]);
		break;
	case Client::EMonster_MovePath::West_R:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iWest_R[0]);
		break;
	case Client::EMonster_MovePath::East_L:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_L[0]);
		break;
	case Client::EMonster_MovePath::East_R:
		vPos = m_pNaviCom->Get_CellCenter_Pos(m_iEast_R[0]);
		break;
	}
	m_pNaviCom->SetUp_NavigationIndex(vPos);
	vPos = m_pNaviCom->Get_CellY(m_pNaviCom->Get_CellIndex(), vPos);
	m_pMovementCom->Set_State(EState::Position, vPos);


	MASK_METERBAR_DESC_3D HP_Bar;
	HP_Bar.eFillMode = EMeterBar_FillMode::ZeroToFull;
	HP_Bar.eFrame_Render = ECastingBar_Frame_Render::Second;
	HP_Bar.HasFrameBar = true;
	HP_Bar.fCount = 100.f;
	HP_Bar.fCount_Max = 100.f;
	HP_Bar.UI_Desc.eLevel = ELevel::Static;
	HP_Bar.UI_Desc.Movement_Desc.vScale = { 8.f, 2.f, 0.1f, 0.f };
	lstrcpy(HP_Bar.UI_Desc.szTextureName, L"Component_Texture_ExpBar");
	XMStoreFloat4(&HP_Bar.UI_Desc.Movement_Desc.vPos, vPos);

	m_pMeterBar_Hp = CMasking_MeterBar_3D::Create(m_pDevice, m_pDevice_Context, &HP_Bar);

	m_fDetectDis = Data.fDetectDis;
	m_fAttackDis = Data.fAttackDis;

	m_vScale.x = Data.Movement_Desc.vScale.x;
	m_vScale.y = Data.Movement_Desc.vScale.y;
	m_vScale.z = Data.Movement_Desc.vScale.z;

	if (S_OK != hr)
		MSG_BOX("CMonster::Ready_Component Failed");

	return hr;
}

_float CMonster::Get_HpRatio() const
{
	return m_pMeterBar_Hp->Get_Ratio(); 
}

_float CMonster::Get_DisToCell(const _int & iCellIndex)
{
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	_vector vDis = m_pNaviCom->Get_CellCenter_Pos(iCellIndex);

	return XMVectorGetX(XMVector3Length(vDis - vPos));
}

void CMonster::Set_IsBrainWash(_bool IsBrainWash)
{
	m_IsBrainWashed = IsBrainWash;

	m_fBrainWashTime = m_pMeterBar_Hp->Get_Ratio() * 2.f;

	//static_cast<CPlayer*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Set_Casting_BrainWash_MaxTime(m_pMeterBar_Hp->Get_Ratio() * 2.f);
}

void CMonster::Set_IsBrainWash_Complete(_bool IsBrainWash)
{
	m_IsBrainWashed = !m_IsBrainWashed;
	//m_IsBrainWashed_Complete = IsBrainWash;
}

void CMonster::Set_Poison(_bool IsPoison)
{
	m_pMeterBar_Hp->Set_ShaderPass(15);

	m_IsPoison = IsPoison;
}

CGameObject * CMonster::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMonster::Free()
{
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNaviCom);

	Safe_Release(m_pMeterBar_Hp);

	Safe_Release(m_pTextureCom_Dissolve);

	__super::Free();
}

//else if (60.f < fTurnAngle)
//{
//	m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 2.5f, vCur_Next_Dir);
//	return m_eAI_Next = EMonsterAI::Turn;
//}
//else if (40.f < fTurnAngle)
//{
//	m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 2.f, vCur_Next_Dir);
//	return m_eAI_Next = EMonsterAI::Turn;
//}
//else if (20.f < fTurnAngle)
//{
//	m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta* 1.5f, vCur_Next_Dir);
//	return m_eAI_Next = EMonsterAI::Turn;
//}
//else if (10.f < fTurnAngle)
//{
//	m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, vCur_Next_Dir);
//	m_pMovementCom->Go_Dir_NoUp(TimeDelta, vCur_Next_Dir, m_pNaviCom);
//	return m_eAI_Next = EMonsterAI::Move_Target;
//}
//else if (5.f < fTurnAngle)
//{
//	_vector vLook = vCur_Next_Dir;
//	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
//	_vector vRight = XMVector3Cross(vUp, vCur_Next_Dir);
//
//	m_pMovementCom->Set_State(EState::Right, vRight * m_pMovementCom->Get_Scale(EState::Right));
//	m_pMovementCom->Set_State(EState::Up, vUp * m_pMovementCom->Get_Scale(EState::Up));
//	m_pMovementCom->Set_State(EState::Look, vLook * m_pMovementCom->Get_Scale(EState::Look));
//}
//
//m_pMovementCom->Go_Dir_NoUp(TimeDelta, vCur_Next_Dir, m_pNaviCom);
