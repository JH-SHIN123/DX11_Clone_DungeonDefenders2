#include "stdafx.h"
#include "..\public\Goblin.h"
#include "Data_Manager.h"
#include "ManaToken.h"

CGoblin::CGoblin(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CGoblin::CGoblin(const CGoblin & rhs)
	: CMonster(rhs)
{
}

HRESULT CGoblin::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGoblin::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_HpBar_OffSet_Position(_float3(0.f, 12.f, 0.f));

	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	Set_Pivot(XMVectorSet(0.25f, 0.25f, 0.25f, 0.f));

	return S_OK;
}

_int CGoblin::Tick(_float TimeDelta)
{
	if (0 <= m_pStatusCom->Get_Hp())
		m_pColliderCom_Attack->Set_NotCollide(true);

	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EGoblinAnim::Death == m_eAnim_Next)
		{
			if (EGoblinAnim::Death == m_eAnim_Next && false == m_IsDeadSound)
			{
				if (127 == (_uint)m_pModelCom->Get_AnimTime())
				{
					m_IsDeadSound = true;

					_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
					if (60.f > fDis)
					{
						fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

						CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN_D);

						if (rand() % 2 == 0)
							CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Dead.ogg", CHANNEL_GOBLIN_D);
						else
							CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Dead2.ogg", CHANNEL_GOBLIN_D);

						CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN_D, fDis + 0.4f);
					}
				}
			}
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
			m_eAnim_Next = EGoblinAnim::Death;
	}

	_vector vTargetPos;

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EGoblinAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EGoblinAnim::Idle;
		}


		m_IsHurt = false;
	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		Create_Hit_Particle(3.f);
		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;

		_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
		if (60.f > fDis)
		{
			fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;
			CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN_H);

			if (rand() % 2 == 0)
				CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Hurt.ogg", CHANNEL_GOBLIN_H);
			else
				CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Hurt2.ogg", CHANNEL_GOBLIN_H);

			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN_H, fDis + 0.05f);
		}

		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EGoblinAnim::Hurt;
			break;
		default:
			m_IsHurt = false;
			break;
		}
	}
	else
	{
		switch (__super::AI_Check(TimeDelta, &vTargetPos, m_IsAttack ))
		{
		case Client::EMonsterAI::Idle:
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EGoblinAnim::Idle;
			break;
		case Client::EMonsterAI::Attack:
			m_IsAttack = true;
			break;
		case Client::EMonsterAI::Cough:
			m_IsAttack = false;
			m_eAnim_Next = EGoblinAnim::Cough;
			break;
		case Client::EMonsterAI::Dead:
			m_eAnim_Next = EGoblinAnim::Death;
			break;
		case Client::EMonsterAI::Shock:
			m_eAnim_Next = EGoblinAnim::Hurt;
			break;
		case Client::EMonsterAI::Move_Cell:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EGoblinAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Move_Target:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EGoblinAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Turn:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EGoblinAnim::Turn_Left;
			break;
		default:
			break;
		}
	}

	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColliderCom_Push->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pStatusCom->Tick(TimeDelta);

	__super::Tick(TimeDelta);

	return _int();
}

_int CGoblin::Late_Tick(_float TimeDelta)
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
			m_eAnim_Next = EGoblinAnim::Death;
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CGoblin::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();
	m_pColliderCom_Push->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

void CGoblin::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EGoblinAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EGoblinAnim::Idle;
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

void CGoblin::Attack_Check()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();

	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;


	if (true == m_IsAttack)
	{
		if (0 == m_iAttackCount)
			m_eAnim_Next = EGoblinAnim::Attack_1;

		if (m_iAttackCount % 2 == 0)
			m_eAnim_Next = EGoblinAnim::Attack_1;
		else
			m_eAnim_Next = EGoblinAnim::Attack_2;

		if (true == m_pModelCom->Get_IsFinishedAnimaion())
			++m_iAttackCount;

		if (EGoblinAnim::Attack_1 == m_eAnim_Next && 16 == (_uint)m_pModelCom->Get_AnimTime())
		{
			if (false == m_IsAttackSound)
			{
				if (60.f > fDis)
				{
					m_IsAttackSound = true;
					CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN);
					CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN_A);

					CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Attack.ogg", CHANNEL_GOBLIN_A);

					if (rand() % 2 == 0)
						CSound_Manager::GetInstance()->Play_Sound(L"Goblin_attack2.ogg", CHANNEL_GOBLIN);
					else
						CSound_Manager::GetInstance()->Play_Sound(L"Goblin_attack4.ogg", CHANNEL_GOBLIN);


					CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN_A, fDis * 1.5f);
					CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN, fDis);
				}
			}

		}		


		if (EGoblinAnim::Attack_1 == m_eAnim_Next && 19 == (_uint)m_pModelCom->Get_AnimTime())
			m_pColliderCom_Attack->Set_NotCollide(false);

		if (EGoblinAnim::Attack_2 == m_eAnim_Next && 56 == (_uint)m_pModelCom->Get_AnimTime())
		{
			if (false == m_IsAttackSound)
			{
				if (60.f > fDis)
				{
					m_IsAttackSound = true;
					CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN);
					CSound_Manager::GetInstance()->StopSound(CHANNEL_GOBLIN_A);

					CSound_Manager::GetInstance()->Play_Sound(L"Goblin_Attack.ogg", CHANNEL_GOBLIN_A);

					if (rand() % 2 == 0)
						CSound_Manager::GetInstance()->Play_Sound(L"Goblin_attack2.ogg", CHANNEL_GOBLIN);
					else
						CSound_Manager::GetInstance()->Play_Sound(L"Goblin_attack4.ogg", CHANNEL_GOBLIN);


					CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN_A, fDis * 1.5f);
					CSound_Manager::GetInstance()->Set_Volume(CHANNEL_GOBLIN, fDis);
				}
			}
		}


		if (EGoblinAnim::Attack_2 == m_eAnim_Next && 58 == (_uint)m_pModelCom->Get_AnimTime())
			m_pColliderCom_Attack->Set_NotCollide(false);
	}

	else
	{
		m_IsSecondAttack = false;
		m_IsAttack = false;
		m_IsAttackSound = false;
	}
}

HRESULT CGoblin::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 4.f, 4.f, 4.f };
	
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hurt, &Data);

	Data.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Desc.iDamage = 10;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);


	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 2.f, 2.f, 2.f };
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Push"), (CComponent**)&m_pColliderCom_Push, &Data);


	return S_OK;
}

_float CGoblin::Anim_Changer(EGoblinAnim eAnim)
{
	EGoblinAnim eAnim_Term = EGoblinAnim::End;
	switch (eAnim)
	{
	case Client::EGoblinAnim::Attack_1:
		eAnim_Term = EGoblinAnim::Attack_2;
		break;
	case Client::EGoblinAnim::Attack_2:
		eAnim_Term = EGoblinAnim::Cough;
		break;
	case Client::EGoblinAnim::Cough:
		eAnim_Term = EGoblinAnim::Death;
		break;
	case Client::EGoblinAnim::Death:
		eAnim_Term = EGoblinAnim::Hurt;
		break;
	case Client::EGoblinAnim::Hurt:
		eAnim_Term = EGoblinAnim::Idle;
		break;
	case Client::EGoblinAnim::Idle:
		eAnim_Term = EGoblinAnim::Jump;
		break;
	case Client::EGoblinAnim::Jump:
		eAnim_Term = EGoblinAnim::Move_Forward;
		break;
	case Client::EGoblinAnim::Move_Forward:
		eAnim_Term = EGoblinAnim::Shock;
		break;
	case Client::EGoblinAnim::Shock:
		eAnim_Term = EGoblinAnim::Turn_Left;
		break;
	case Client::EGoblinAnim::Turn_Left:
		eAnim_Term = EGoblinAnim::End_Anim;
		break;
	default:
		break;
	}

	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next  - 1.f;

	return fAnim;
}

void CGoblin::AI_Check()
{


}

CGoblin * CGoblin::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CGoblin* pInstance = new CGoblin(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CGoblin) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CGoblin::Clone_GameObject(void * pArg)
{
	CGoblin* pInstance = new CGoblin(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CGoblin) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGoblin::Free()
{
	Safe_Release(m_pColliderCom_Push);
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
