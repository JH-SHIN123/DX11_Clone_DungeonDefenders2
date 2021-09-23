#include "stdafx.h"
#include "..\public\Goblin.h"

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


	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	return S_OK;
}

_int CGoblin::Tick(_float TimeDelta)
{
	return _int();
}

_int CGoblin::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);


	return __super::Late_Tick(TimeDelta);
}

HRESULT CGoblin::Render()
{
	__super::Render();

	return S_OK;
}

void CGoblin::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();

	m_eAnim_Cur = m_eAnim_Next;
}

HRESULT CGoblin::Ready_Component(void * pArg)
{
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

	_float fAnim = (_float)m_eAnim_Next - (_float)eAnim_Term - 1.f;

	return fAnim;
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
	__super::Free();
}
