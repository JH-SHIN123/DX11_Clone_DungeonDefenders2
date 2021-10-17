#include "stdafx.h"
#include "..\public\Kobold_Boom.h"

CKobold_Boom::CKobold_Boom(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CKobold_Boom::CKobold_Boom(const CKobold_Boom & rhs)
	: CBullet(rhs)
{
}

HRESULT CKobold_Boom::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CKobold_Boom::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CKobold_Boom::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	if (true == m_IsBoom)
		return OBJECT_DEAD;

	m_pColliderCom_Attack->Set_Scale_Tick_Linear(_float3(10.f, 10.f, 10.f), TimeDelta * 5.f);

	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
			return OBJECT_DEAD;

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	m_IsBoom = true;

	return iReturn;
}

_int CKobold_Boom::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CKobold_Boom::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CKobold_Boom::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CKobold_Boom::Ready_Component Failed!");

	return hr;
}

CKobold_Boom * CKobold_Boom::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CKobold_Boom*		pInstance = new CKobold_Boom(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CKobold_Boom) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CKobold_Boom::Clone_GameObject(void * pArg)
{
	CKobold_Boom*		pInstance = new CKobold_Boom(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CKobold_Boom) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CKobold_Boom::Free()
{
	__super::Free();
}
