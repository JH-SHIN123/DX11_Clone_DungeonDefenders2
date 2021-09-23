#include "stdafx.h"
#include "..\public\StrikerTower_Bullet.h"

CStrikerTower_Bullet::CStrikerTower_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CStrikerTower_Bullet::CStrikerTower_Bullet(const CStrikerTower_Bullet & rhs)
	: CBullet(rhs)
{
}

HRESULT CStrikerTower_Bullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStrikerTower_Bullet::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CStrikerTower_Bullet::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	_vector vDir = XMLoadFloat3(&m_vGoDir);
	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);


	return _int();
}

_int CStrikerTower_Bullet::Late_Tick(_float TimeDelta)
{


	return __super::Late_Tick(TimeDelta);
}

HRESULT CStrikerTower_Bullet::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStrikerTower_Bullet::Ready_Component(void * pArg)
{
	return S_OK;
}

CStrikerTower_Bullet * CStrikerTower_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStrikerTower_Bullet*		pInstance = new CStrikerTower_Bullet(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStrikerTower_Bullet::Clone_GameObject(void * pArg)
{
	CStrikerTower_Bullet*		pInstance = new CStrikerTower_Bullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStrikerTower_Bullet::Free()
{
	__super::Free();
}
