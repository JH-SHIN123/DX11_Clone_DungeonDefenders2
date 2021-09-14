#include "stdafx.h"
#include "..\public\StrikerTower.h"

CStrikerTower::CStrikerTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CDefenceTower(pDevice, pDevice_Context)
{
}

CStrikerTower::CStrikerTower(const CStrikerTower & rhs)
	: CDefenceTower(rhs)
{
}

HRESULT CStrikerTower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStrikerTower::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CStrikerTower::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CStrikerTower::Late_Tick(_float TimeDelta)
{

	return __super::Late_Tick(TimeDelta);
}

HRESULT CStrikerTower::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStrikerTower::Ready_Component(void * pArg)
{
	return S_OK;
}

CStrikerTower * CStrikerTower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStrikerTower*		pInstance = new CStrikerTower(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStrikerTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStrikerTower::Clone_GameObject(void * pArg)
{
	CStrikerTower*		pInstance = new CStrikerTower(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStrikerTower::Free()
{
	__super::Free();
}
