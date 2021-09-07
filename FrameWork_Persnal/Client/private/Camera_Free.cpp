#include "stdafx.h"
#include "Camera_Free.h"

CCamera_Free::CCamera_Free(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CCamera(pDevice, pDevice_Context)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_Free::Tick(_float TimeDelta)
{
	/* TODO. */
	if (nullptr == m_pMovementCom)
		return -1;


	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pMovementCom->Go_Left(TimeDelta);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pMovementCom->Go_Right(TimeDelta);

	if (GetKeyState(VK_UP) & 0x8000)
		m_pMovementCom->Go_Straight(TimeDelta);

	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pMovementCom->Go_Backward(TimeDelta);

	if (GetKeyState('T') & 0x8000)
		__super::Set_ZoomIn(80.f);
		//__super::Set_ZoomOut(100.f);

	return __super::Tick(TimeDelta); // ÀÌ°Ô »ÇÀÎÆ®
}

_int CCamera_Free::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

HRESULT CCamera_Free::Ready_Component()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Free) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone_GameObject(void * pArg)
{
	CCamera_Free* pClone = new CCamera_Free(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Free) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CCamera_Free::Free()
{
	__super::Free();
}
