#include "stdafx.h"
#include "..\public\Player_Bullet.h"
#include "Point_Spread.h"
#include "Point_Spread2.h"
#include "Data_Manager.h"
#include "Point_Ex_Trail.h"

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
	m_pPoint_Trail->Set_Pos(m_pMovementCom->Get_State(EState::Position));
	m_pPoint_Trail->Tick(TimeDelta);

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
	m_pPoint_Trail->Late_Tick(TimeDelta);

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPlayer_Bullet::Render()
{
	//__super::Render();

	//m_pPointSpread->Render();

#ifdef _DEBUG
//	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

void CPlayer_Bullet::Creat_Effect()
{
	POINT_SPREAD_DESC_2 Data;
	Data.IsTime = true;
	Data.vSize = { 2.f,2.f };
	Data.Point_Desc.fLifeTime = 2.f;
	Data.Point_Desc.iShaderPass = 3;
	Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Ring_Gray");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);

	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	if (60.f > fDis)
	{
		fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;
		CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING_4);
		CSound_Manager::GetInstance()->Play_Sound(L"Energyball.wav", CHANNEL_ANYTHING_4);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING_4, fDis + 0.2f);
	}
}

HRESULT CPlayer_Bullet::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	POINT_SPREAD_DESC Point_Desc;
	lstrcpy(Point_Desc.szTextrueName, L"Component_Texture_Ring_Gray");
	lstrcpy(Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_200_5");
	XMStoreFloat4(&Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Point_Desc.MoveDesc.vScale = { 1.f,1.f,0.f,0.f };
	Point_Desc.InstanceValue = EInstanceValue::Point_200_5;


	m_pPointSpread = CPoint_Spread::Create(m_pDevice, m_pDevice_Context);
	m_pPointSpread->NativeConstruct(&Point_Desc);
	m_pPointSpread->SetUp_IndexDir(10);

	// 이거 시간값 좀만 손보고 인스턴싱 따로 만들자
	POINT_TRAIL_EX_DESC Effect_Data;
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 1.5f;
	Effect_Data.fSpreadDis = 3.f;
	Effect_Data.fSize = 3.f;
	Effect_Data.fTimeTerm = 0.1f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_1000_100;
	Effect_Data.iShaderPass = 4;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_1000_100");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_Ring_Gray");

	m_pPoint_Trail = CPoint_Ex_Trail::Create(m_pDevice, m_pDevice_Context);
	m_pPoint_Trail->NativeConstruct(&Effect_Data);

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
	Safe_Release(m_pPoint_Trail);

	__super::Free();
}
