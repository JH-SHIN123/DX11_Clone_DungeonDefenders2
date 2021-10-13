#include "stdafx.h"
#include "..\public\Goblin.h"
#include "Data_Manager.h"

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
	m_pColliderCom_Attack->Set_NotCollide(true);
	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EGoblinAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{
				CData_Manager::GetInstance()->Add_MonsterCount();
				return OBJECT_DEAD;
			}
		}
		else
			m_eAnim_Next = EGoblinAnim::Death;
	}

	_matrix Matrix = m_pMovementCom->Get_WorldMatrix();
	m_pColliderCom_Hurt->Update_Collider(Matrix);
	m_pStatusCom->Tick(TimeDelta);

	_vector vTargetPos;

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EGoblinAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EGoblinAnim::Idle;
		}

	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;


		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Direct:
			m_IsHurt = false;
			break;
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EGoblinAnim::Shock;
			break;
		default:
			break;
		}
	}
	else
	{
		switch (__super::AI_Check(TimeDelta, &vTargetPos, m_IsAttack ||m_IsHurt ))
		{
			case Client::EMonsterAI::Idle:
				m_IsAttack = false;
				m_iAttackCount = 0;
				m_eAnim_Next = EGoblinAnim::Idle;
				break;
			case Client::EMonsterAI::Attack:
				m_IsAttack = true;
				break;
			case Client::EMonsterAI::Hurt:

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


	if (true == m_IsAttack)
	{
		if(0 == m_iAttackCount)
			m_eAnim_Next = EGoblinAnim::Attack_1;

		if (m_iAttackCount % 2 == 0)
			m_eAnim_Next = EGoblinAnim::Attack_1;
		else
			m_eAnim_Next = EGoblinAnim::Attack_2;

		if (true == m_pModelCom->Get_IsFinishedAnimaion())
		{
			// ÀÌ°Å »ìÂ¦ ²÷±â³ß;

			++m_iAttackCount;
		}


	}








	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_State(EState::Position));

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
			return OBJECT_DEAD;
		}
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CGoblin::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();

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

	if (true == m_IsAttack)
	{
		//if (false == m_IsSecondAttack && true == IsFinished)
		//{
		//	m_eAnim_Next = EGoblinAnim::Attack_1;
		//	return;
		//}

		if (EGoblinAnim::Attack_1 == m_eAnim_Next && true == IsFinished)
		{
			m_IsSecondAttack = true;
			m_eAnim_Next = EGoblinAnim::Attack_2;
			return;
		}

		if (EGoblinAnim::Attack_2 == m_eAnim_Next && true == IsFinished)
		{
			m_IsSecondAttack = false;
			m_eAnim_Next = EGoblinAnim::Attack_1;
			return;
		}

		if (EGoblinAnim::Attack_1 == m_eAnim_Next && 19 == (_uint)m_pModelCom->Get_AnimTime())
		{
			m_pColliderCom_Attack->Set_NotCollide(false);
		}

		if (EGoblinAnim::Attack_2 == m_eAnim_Next && 58 == (_uint)m_pModelCom->Get_AnimTime())
		{
			m_pColliderCom_Attack->Set_NotCollide(false);
		}

	}

	else
	{
		m_IsSecondAttack = false;
		m_IsAttack = false;
	}
}

HRESULT CGoblin::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 7.f, 7.f, 7.f };
	
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hurt, &Data);

	Data.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Desc.iDamage = 30;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);


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
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
