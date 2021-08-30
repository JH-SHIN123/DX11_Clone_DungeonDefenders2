#include "stdafx.h"
#include "..\public\Camera_Target.h"

CCamera_Target::CCamera_Target(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CCamera(pDevice, pDevice_Context)
{
}

CCamera_Target::CCamera_Target(const CCamera_Target & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Target::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Target::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_CameraDesc.vTargetDis = _float3(2.f, 2.f, -7.f);

	Ready_Component();

	return S_OK;
}

_int CCamera_Target::Tick(_float TimeDelta)
{
	Aim_Check();

	TargetRotate_Check((_uint)ELevel::Stage1, L"Layer_Player", L"Com_Movement");

	return __super::Tick(TimeDelta);
}

_int CCamera_Target::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera_Target::Render()
{
	return S_OK;
}

HRESULT CCamera_Target::Ready_Component()
{
	return S_OK;
}

CCamera_Target * CCamera_Target::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CCamera_Target* pInstance = new CCamera_Target(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Target) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Target::Clone_GameObject(void * pArg)
{
	CCamera_Target* pClone = new CCamera_Target(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Target) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CCamera_Target::Free()
{
	__super::Free();
}
