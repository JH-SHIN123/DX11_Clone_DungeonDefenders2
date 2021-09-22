#include "stdafx.h"
#include "..\public\LightningTower_Bullet.h"

CLightningTower_Bullet::CLightningTower_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CLightningTower_Bullet::CLightningTower_Bullet(const CLightningTower_Bullet & rhs)
	: CBullet(rhs)
{
}

HRESULT CLightningTower_Bullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower_Bullet::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CLightningTower_Bullet::Tick(_float TimeDelta)
{
	return _int();
}

_int CLightningTower_Bullet::Late_Tick(_float TimeDelta)
{
	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower_Bullet::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLightningTower_Bullet::Ready_Component(void * pArg)
{
	return S_OK;
}

CLightningTower_Bullet * CLightningTower_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower_Bullet::Clone_GameObject(void * pArg)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower_Bullet::Free()
{
	__super::Free();
}
