#include "stdafx.h"
#include "..\public\Point_Spread.h"

CPoint_Spread::CPoint_Spread(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPoint_Spread::CPoint_Spread(const CPoint_Spread & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPoint_Spread::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Spread::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();
	_uint iNumInstance = m_pBufferInstanceCom->Get_NumInstance();


	pInstance[0].vSize = { 10.f, 10.f };

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);

	return S_OK;
}

_int CPoint_Spread::Tick(_float TimeDelta)
{
	return _int();
}

_int CPoint_Spread::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::AlphaUI_Scecond, this);
}

HRESULT CPoint_Spread::Render()
{
	return S_OK;
}

HRESULT CPoint_Spread::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;



	if (hr != S_OK)
		MSG_BOX("CPoint_Spread::Ready_Component Failed");

	return hr;
}

void CPoint_Spread::Check_Instance(_float TimeDelta)
{
	// 0번을 주위로 퍼져나감



}

CPoint_Spread * CPoint_Spread::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Spread*		pInstance = new CPoint_Spread(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Spread) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Spread * CPoint_Spread::Clone_GameObject(void * pArg)
{
	CPoint_Spread*		pInstance = new CPoint_Spread(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Spread) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Spread::Free()
{
	__super::Free();

	Safe_Release(m_pBufferInstanceCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pMovementCom);
}
