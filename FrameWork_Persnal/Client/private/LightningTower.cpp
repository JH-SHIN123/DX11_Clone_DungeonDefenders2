#include "stdafx.h"
#include "..\public\LightningTower.h"

CLightningTower::CLightningTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CDefenceTower(pDevice, pDevice_Context)
{
}

CLightningTower::CLightningTower(const CLightningTower & rhs)
	: CDefenceTower(rhs)
{
}

HRESULT CLightningTower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	m_pModelCom->Set_AnimationIndex(0); // 나는 애니메이션 하나에 다 있는 상황 원테이크
	m_pModelCom->Set_AnimationIndex_Start(0.f, (_float)m_eAnim_Next - 1.f);

	return S_OK;
}

_int CLightningTower::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CLightningTower::Late_Tick(_float TimeDelta)
{
	Attack_Check(TimeDelta);

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	//if (true == m_pModelCom->Get_IsFinishedAnimaion())
	//	m_IsAttack = false;

	if (true == __super::Enemy_Check(TimeDelta))
	{
		m_eAnim_Next = CLightningTower::EAnim::FireLoop;

		_float fAnimTime = m_pModelCom->Get_AnimTime();


		if (m_fAttStartTime > m_fAttCharging)
		{
			m_fAttCharging += TimeDelta;
			


		}
		else
		{
			m_IsAttack = true;

		}
		
	}
	else
	{
		m_eAnim_Next = EAnim::Idle;
		m_IsAttack = false;
	}

	if (m_eAnim_Cur != m_eAnim_Next)
	{
		m_eAnim_Cur = m_eAnim_Next;
		EAnim eAnim;

		switch (m_eAnim_Next)
		{
		case EAnim::FireLoop:
			eAnim = EAnim::Idle;
			break;
		case EAnim::Idle:
			eAnim = EAnim::End_Anim;
			break;
		default:
			break;
		}
		_float fAnimTerm = ((_float)eAnim) - (_float)m_eAnim_Next - 1.f;
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, fAnimTerm);
	}


	m_pModelCom->Update_CombindTransformationMatrix();


	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLightningTower::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	MOVESTATE_DESC Data;
	ZeroMemory(&Data, sizeof(MOVESTATE_DESC));

	_vector vPos = m_pMovementCom->Get_State(EState::Position);

	//Data.vPos

	//hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Ball"), (CComponent**)&m_pMovementCom_, &Data);
	//hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT(""), TEXT("Com_Model_Ball"), (CComponent**)&m_pModelCom);


	if (S_OK != hr)
		MSG_BOX("CLightningTower::Ready_Component Failed!");

	return hr;
}

void CLightningTower::Attack_Check(_float TimeDelta)
{
}

void CLightningTower::Anim_Check()
{
}

CLightningTower * CLightningTower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower*		pInstance = new CLightningTower(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower::Clone_GameObject(void * pArg)
{
	CLightningTower*		pInstance = new CLightningTower(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower::Free()
{
	Safe_Release(m_pModelCom_LightingBall);
	Safe_Release(m_pMovmentCom_LightingBall);

	__super::Free();
}
