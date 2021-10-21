#include "stdafx.h"
#include "..\public\Player_Bullet.h"
#include "Point_Spread2.h"

CPlayer_Bullet::CPlayer_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CPlayer_Bullet::CPlayer_Bullet(const CPlayer_Bullet & rhs)
	: CBullet(rhs)
{
}

HRESULT CPlayer_Bullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Bullet::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CPlayer_Bullet::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;


	_vector vDir = XMLoadFloat3(&m_vGoDir);
	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);


	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
		{
			Creat_Effect();
			return OBJECT_DEAD;
		}

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	m_pPointSpread->Set_Pos(m_pMovementCom->Get_State(EState::Position));
	m_pPointSpread->Tick(TimeDelta);

	return iReturn;
}

_int CPlayer_Bullet::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
	{
		Creat_Effect();
		return OBJECT_DEAD;
	}

	m_pPointSpread->Late_Tick(TimeDelta);

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPlayer_Bullet::Render()
{
	//__super::Render();

	m_pPointSpread->Render();

#ifdef _DEBUG
//	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

void CPlayer_Bullet::Creat_Effect()
{
	POINT_SPREAD_DESC_2 Data;
	Data.InstanceValue = EInstanceValue::Point_100_10;
	Data.IsTime = true;
	Data.vSize = { 2.f,2.f };
	Data.Point_Desc.fLifeTime = 2.f;
	Data.Point_Desc.iShaderPass = 3;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Ring_Gray");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);
}

HRESULT CPlayer_Bullet::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	POINT_SPREAD_DESC Point_Desc;
	lstrcpy(Point_Desc.szTextrueName, L"Component_Texture_Ring_Gray");
	lstrcpy(Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_200_5");
	XMStoreFloat4(&Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Point_Desc.MoveDesc.vScale = { 1.f,1.f,0.f,0.f };

	m_pPointSpread = CPoint_Spread::Create(m_pDevice, m_pDevice_Context);
	m_pPointSpread->NativeConstruct(&Point_Desc);
	m_pPointSpread->SetUp_IndexDir(10);


	if (S_OK != hr)
		MSG_BOX("CPlayer_Bullet::Ready_Component Failed!");

	return hr;
}

CPlayer_Bullet * CPlayer_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPlayer_Bullet*		pInstance = new CPlayer_Bullet(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPlayer_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer_Bullet::Clone_GameObject(void * pArg)
{
	CPlayer_Bullet*		pInstance = new CPlayer_Bullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPlayer_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer_Bullet::Free()
{
	Safe_Release(m_pPointSpread);

	__super::Free();
}
