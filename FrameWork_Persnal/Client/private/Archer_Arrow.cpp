#include "stdafx.h"
#include "..\public\Archer_Arrow.h"

CArcher_Arrow::CArcher_Arrow(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CArcher_Arrow::CArcher_Arrow(const CArcher_Arrow & rhs)
	: CBullet(rhs)
{
}

HRESULT CArcher_Arrow::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CArcher_Arrow::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));

	return S_OK;
}

_int CArcher_Arrow::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;


	_vector vDir = XMLoadFloat3(&m_vGoDir);
	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);


	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
			return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return iReturn;
}

_int CArcher_Arrow::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CArcher_Arrow::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CArcher_Arrow::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CArcher_Arrow::Ready_Component Failed!");

	return hr;
}

CArcher_Arrow * CArcher_Arrow::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CArcher_Arrow*		pInstance = new CArcher_Arrow(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CArcher_Arrow) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArcher_Arrow::Clone_GameObject(void * pArg)
{
	CArcher_Arrow*		pInstance = new CArcher_Arrow(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CArcher_Arrow) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArcher_Arrow::Free()
{
	__super::Free();
}
