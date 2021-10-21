#include "stdafx.h"
#include "..\public\StrikerTower_Bullet.h"
#include "Point_Spread.h"

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

	Ready_Component(pArg);

	Safe_Release(m_pModelCom);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CStrikerTower_Bullet::Tick(_float TimeDelta)
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

	m_pPointSpread->Set_Pos(m_pMovementCom->Get_State(EState::Position));
	m_pPointSpread->Tick(TimeDelta);

	return iReturn;
}

_int CStrikerTower_Bullet::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	m_pPointSpread->Late_Tick(TimeDelta);


	return __super::Late_Tick(TimeDelta);
}

HRESULT CStrikerTower_Bullet::Render()
{
	//__super::Render();

	//m_pPointSpread->Render();

	return S_OK;
}

HRESULT CStrikerTower_Bullet::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	POINT_SPREAD_DESC Point_Desc;
	lstrcpy(Point_Desc.szTextrueName, L"Component_Texture_Glow_Green");
	lstrcpy(Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_200_5");
	XMStoreFloat4(&Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Point_Desc.MoveDesc.vScale = { 1.f,1.f,0.f,0.f };

	m_pPointSpread = CPoint_Spread::Create(m_pDevice, m_pDevice_Context);
	m_pPointSpread->NativeConstruct(&Point_Desc);

	VTXMATRIX* pInstance = m_pPointSpread->Get_InstanceBuffer();
	_int iInstance_StartIndex = m_pPointSpread->Get_Instance_StartIndex();
	_int iNumInstance = m_pPointSpread->Get_Instance_Num();
	_float fSize = 4.f;

	for (_int i = iInstance_StartIndex; i < iNumInstance + iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize.x = fSize;
		pInstance[i].vSize.y = fSize;

		fSize -= 0.5f;
	}

	m_pPointSpread->Set_InstanceBuffer(pInstance);
	m_pPointSpread->SetUp_IndexDir(1);

	if (S_OK != hr)
		MSG_BOX("CStrikerTower_Bullet::Ready_Component Failed!");

	return hr;
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
	Safe_Release(m_pPointSpread);

	__super::Free();
}
