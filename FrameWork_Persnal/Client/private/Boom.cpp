#include "stdafx.h"
#include "..\public\Boom.h"


CBoom::CBoom(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CBoom::CBoom(const CBoom & rhs)
	: CBullet(rhs)
{
}

HRESULT CBoom::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoom::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	//Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CBoom::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{

		return OBJECT_DEAD;
	}

	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;


	m_pMovementCom->Go_Dir_Vector(TimeDelta, XMLoadFloat3(&m_vGoDir));

	return 0;
}

_int CBoom::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoom::Render()
{
	__super::Render();

#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG

	return S_OK;
}

HRESULT CBoom::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (S_OK != hr)
		MSG_BOX("CBoom::Ready_Component");

	return hr;
}

CBoom * CBoom::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoom* pInstance = new CBoom(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoom) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoom::Clone_GameObject(void * pArg)
{
	CBoom* pInstance = new CBoom(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CBoom) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoom::Free()
{
	__super::Free();
}
