#include "stdafx.h"
#include "..\public\Ogre.h"

COgre::COgre(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

COgre::COgre(const COgre & rhs)
	: CMonster(rhs)
{
}

HRESULT COgre::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT COgre::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_HpBar_OffSet_Position(_float3(0.f, 12.f, 0.f));

	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
	Set_Pivot_Rotate(XMVectorSet(0.f, XMConvertToDegrees(180.f), 0.f, 0.f));

	return S_OK;
}

_int COgre::Tick(_float TimeDelta)
{
	m_pColliderCom_Attack->Set_NotCollide(true);
	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EOgreAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
				return OBJECT_DEAD;
		}
	}

	_vector vTargetPos;

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EOgreAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EOgreAnim::Idle;
		}
		m_IsHurt = false;
	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;


		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EOgreAnim::Hurt;
			break;
		default:
			break;
		}
	}
	else
	{
		
		switch (__super::AI_Check(TimeDelta, &vTargetPos, m_IsAttack || m_IsSpecialAttack || m_IsHurt))
		{
		case Client::EMonsterAI::Idle:
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EOgreAnim::Idle;
			break;
		case Client::EMonsterAI::Attack:
			m_IsAttack = true;
			break;
		case Client::EMonsterAI::Hurt:

			break;
		case Client::EMonsterAI::Dead:
			m_eAnim_Next = EOgreAnim::Death;
			break;
		case Client::EMonsterAI::Shock:
			m_eAnim_Next = EOgreAnim::Hurt;
			break;
		case Client::EMonsterAI::Move_Cell:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EOgreAnim::Walk;
			break;
		case Client::EMonsterAI::Move_Target:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EOgreAnim::Walk;
			break;
		case Client::EMonsterAI::Turn:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EOgreAnim::Turn;
			break;
		default:
			break;
		}
	}

	//m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	m_pStatusCom->Tick(TimeDelta);

	__super::Tick(TimeDelta);

	return _int();
}

_int COgre::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (true == m_pModelCom->Get_IsFinishedAnimaion())
			return OBJECT_DEAD;
	}


	return __super::Late_Tick(TimeDelta);
}

HRESULT COgre::Render()
{
	__super::Render();


#ifdef _DEBUG
	if(nullptr != m_pColliderCom_Attack)
		m_pColliderCom_Attack->Render_Collider();
	
	m_pColliderCom_Hurt->Render_Collider();

#endif // _DEBUG


	return S_OK;
}

void COgre::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;


	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EOgreAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EOgreAnim::Idle;
		}
		m_IsAttack = false;
		m_IsSpecialAttack = false;
	}

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();



	_matrix Matrix = m_pMovementCom->Get_WorldMatrix();
	Matrix.r[3] += XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * 7.f;
	m_pColliderCom_Attack->Update_Collider(Matrix);

	m_eAnim_Cur = m_eAnim_Next;

}

void COgre::Attack_Check()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();

	if (true == m_IsAttack)
	{
		if (0 == m_iAttackCount)
			m_eAnim_Next = EOgreAnim::Pound;

		if (m_iAttackCount % 2 == 0)
			m_eAnim_Next = EOgreAnim::Pound;
		else
			m_eAnim_Next = EOgreAnim::Sweep;

		if (true == m_pModelCom->Get_IsFinishedAnimaion())
		{
			// ÀÌ°Å »ìÂ¦ ²÷±â³ß;

			++m_iAttackCount;
		}


		if (EOgreAnim::Pound == m_eAnim_Next && 400 == (_uint)m_pModelCom->Get_AnimTime())
		{
			m_pColliderCom_Attack->Set_NotCollide(false);
		}
		
		if (EOgreAnim::Sweep == m_eAnim_Next && 491 == (_uint)m_pModelCom->Get_AnimTime())
		{
			m_pColliderCom_Attack->Set_NotCollide(false);
		}
	}
}

HRESULT COgre::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 7.f, 7.f, 7.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hurt, &Data);


	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 4.f, 4.f, 4.f };

	Data.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Desc.iDamage = 100;
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);

	//COLLIDER_DESC Data;
	//ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	//Data.vScale = { 100.f, 100.f, 100.f };
	//
	//Data.Attack_Desc.eDamageType = EDamageType::Direct;
	//Data.Attack_Desc.iDamage = 100;
	//CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);

	return S_OK;
}

_float COgre::Anim_Changer(EOgreAnim eAnim)
{
	EOgreAnim eAnim_Term = EOgreAnim::End;

	switch (eAnim)
	{
	case Client::EOgreAnim::Cough:
		eAnim_Term = EOgreAnim::Death;
		break;
	case Client::EOgreAnim::Death:
		eAnim_Term = EOgreAnim::Hurt;
		break;
	case Client::EOgreAnim::Hurt:
		eAnim_Term = EOgreAnim::Idle;
		break;
	case Client::EOgreAnim::Idle:
		eAnim_Term = EOgreAnim::Pound;
		break;
	case Client::EOgreAnim::Pound:
		eAnim_Term = EOgreAnim::Punch;
		break;
	case Client::EOgreAnim::Punch:
		eAnim_Term = EOgreAnim::Sweep;
		break;
	case Client::EOgreAnim::Sweep:
		eAnim_Term = EOgreAnim::Turn;
		break;
	case Client::EOgreAnim::Turn:
		eAnim_Term = EOgreAnim::Walk;
		break;
	case Client::EOgreAnim::Walk:
		eAnim_Term = EOgreAnim::End_Anim;
		break;
	case Client::EOgreAnim::End_Anim:
		break;
	default:
		break;
	}

	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next - 1.f;

	return fAnim;
}

void COgre::AI_Check()
{
}

COgre * COgre::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	COgre* pInstance = new COgre(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (COgre) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * COgre::Clone_GameObject(void * pArg)
{
	COgre* pInstance = new COgre(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (COgre) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void COgre::Free()
{
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();

}
