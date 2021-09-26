#include "stdafx.h"
#include "..\public\Archer.h"

CArcher::CArcher(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CArcher::CArcher(const CArcher & rhs)
	: CMonster(rhs)
{
}

HRESULT CArcher::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CArcher::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);


	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	Set_Pivot(XMVectorSet(0.3f, 0.3f, 0.3f, 0.f));

	return S_OK;
}

_int CArcher::Tick(_float TimeDelta)
{
	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_State(EState::Position));


	return _int();
}

_int CArcher::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);


	return __super::Late_Tick(TimeDelta);
}

HRESULT CArcher::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();

#endif // _DEBUG


	return S_OK;
}

void CArcher::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();

	m_eAnim_Cur = m_eAnim_Next;
}

void CArcher::Attack_Check()
{
	m_IsAttack = false;
	if (EArcherAnim::Attack == m_eAnim_Next)
	{
		m_IsAttack = true;

		//if()
	}
}

HRESULT CArcher::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 7.f, 7.f, 7.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Attack_Collider"), (CComponent**)&m_pColliderCom_Attack, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_AABB"), TEXT("Com_Hurt_Collider"), (CComponent**)&m_pColliderCom_Hurt, &Data);


	return S_OK;
}

_float CArcher::Anim_Changer(EArcherAnim eAnim)
{
	EArcherAnim eAnim_Term = EArcherAnim::End;

	switch (eAnim)
	{
	case EArcherAnim::Attack:
		eAnim_Term = EArcherAnim::Cough;
		break;
	case EArcherAnim::Cough:
		eAnim_Term = EArcherAnim::Death;
		break;
	case EArcherAnim::Death:
		eAnim_Term = EArcherAnim::Hurt;
		break;
	case EArcherAnim::Hurt:
		eAnim_Term = EArcherAnim::Idle;
		break;
	case EArcherAnim::Idle:
		eAnim_Term = EArcherAnim::Jump;
		break;
	case EArcherAnim::Jump:
		eAnim_Term = EArcherAnim::Move_Backward;
		break;
	case EArcherAnim::Move_Backward:
		eAnim_Term = EArcherAnim::Move_Forward;
		break;
	case EArcherAnim::Move_Forward:
		eAnim_Term = EArcherAnim::Move_Left;
		break;
	case EArcherAnim::Move_Left:
		eAnim_Term = EArcherAnim::Move_Right;
		break;
	case EArcherAnim::Move_Right:
		eAnim_Term = EArcherAnim::Shock;
		break;
	case EArcherAnim::Shock:
		eAnim_Term = EArcherAnim::Turn_Left;
		break;
	case EArcherAnim::Turn_Left:
		eAnim_Term = EArcherAnim::End_Anim;
		break;
	default:
		eAnim_Term = EArcherAnim::Attack;
		break;
	}

	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next - 1.f;

	return fAnim;
}

void CArcher::AI_Check()
{
}

CArcher * CArcher::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CArcher* pInstance = new CArcher(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CArcher) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArcher::Clone_GameObject(void * pArg)
{
	CArcher* pInstance = new CArcher(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CArcher) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArcher::Free()
{
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
