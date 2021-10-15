#include "stdafx.h"
#include "..\public\Kobold.h"
#include "Data_Manager.h"
#include "Kobold_Boom.h"

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

	return S_OK;
}

_int CKobold::Tick(_float TimeDelta)
{
	m_pColliderCom_Attack->Set_NotCollide(true);
	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EKoboldAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{
				CData_Manager::GetInstance()->Add_MonsterCount();
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
		switch (__super::AI_Check(TimeDelta, &vTargetPos, m_IsAttack || m_IsHurt))
		{
		case Client::EMonsterAI::Idle:
			m_eAnim_Next = EKoboldAnim::Idle;
			break;
		case Client::EMonsterAI::Attack:
			m_IsAttack = true;
			break;
			//case Client::EMonsterAI::Hurt:
			//	break;
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

	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_WorldMatrix());

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
			return OBJECT_DEAD;
		}
		else
			m_eAnim_Next = EKoboldAnim::Death;
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CKobold::Render()
{
	__super::Render();


#ifdef _DEBUG
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
		m_eAnim_Next = EKoboldAnim::Run_Suicide;

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

HRESULT CKobold::Ready_Component(void * pArg)
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
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
