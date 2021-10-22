#include "stdafx.h"
#include "..\public\Skill_ManaBomb.h"
#include "Point_Spread2.h"
#include "Point_Ex_Trail.h"

CSkill_ManaBomb::CSkill_ManaBomb(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_ManaBomb::CSkill_ManaBomb(const CSkill_ManaBomb & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_ManaBomb::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_ManaBomb::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CSkill_ManaBomb::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{
		Create_Explosion();
		return OBJECT_DEAD;
	}

	if (true == m_IsFall)
	{
		m_pMovementCom->Go_Up(-TimeDelta * 13.f);

	}

	else
	{
		m_fCastTime -= TimeDelta;
		m_pMovementCom->Go_Up(TimeDelta);
		
		if (0.f >= m_fCastTime)
			m_IsFall = true;
	}

	if (1) // ºÎ‹HÇû´Ù¸é
	{
		//Æã
	}

	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	m_pPoint_Trail->Set_Pos(vPos);
	m_pPoint_Trail->Tick(TimeDelta);
	return 0;
}

_int CSkill_ManaBomb::Late_Tick(_float TimeDelta)
{
	int iReturn = 0;
	if (iReturn = __super::Late_Tick(TimeDelta))
	{
		Create_Explosion();
		return iReturn;
	}

	m_pPoint_Trail->Late_Tick(TimeDelta);
	return iReturn;
}

HRESULT CSkill_ManaBomb::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSkill_ManaBomb::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;


	POINT_TRAIL_EX_DESC Effect_Data;
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 1.5f;
	Effect_Data.fSpreadDis = 1.f;
	Effect_Data.fSize = 5.f;
	Effect_Data.fTimeTerm = 0.2f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_200_10;
	Effect_Data.iShaderPass = 1;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_BlueGiant");


	m_pPoint_Trail = CPoint_Ex_Trail::Create(m_pDevice, m_pDevice_Context);
	m_pPoint_Trail->NativeConstruct(&Effect_Data);
	if (S_OK != hr)
		MSG_BOX("CSkill_ManaBomb::Ready_Component");

	return hr;
}

void CSkill_ManaBomb::Create_Explosion()
{
	_vector vDir;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	BULLET_DESC Data;
	Data.fLifeTime = 0.3f;
	XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
	Data.MoveState_Desc.fSpeedPerSec = 30.f;
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

	Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
	Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	Data.Attack_Collide_Desc.vScale = { 5.f, 5.f, 5.f };
	Data.Attack_Collide_Desc.IsCenter = false;

	_float fAngle = 0;
	_matrix RotateMatrix;

	for (_int i = 0; i < 10; ++i)
	{
		RotateMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fAngle));
		vDir = XMVector3Normalize(RotateMatrix.r[2]);
		XMStoreFloat3(&Data.vDir, vDir);
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Boom"), (_uint)ELevel::Stage1, TEXT("Layer_Bullet"), &Data);

		fAngle += 360.f / 10.f;
	}

	POINT_TRAIL_EX_DESC Effect_Data;
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 1.5f;
	Effect_Data.fSpreadDis = 7.f;
	Effect_Data.fSize = 2.f;
	Effect_Data.fTimeTerm = 0.01f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_200_50;
	Effect_Data.iShaderPass = 1;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_50");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_BlueSmall");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Trail", (_uint)ELevel::Stage1, L"Layer_Effect", &Effect_Data);

	ZeroMemory(&Effect_Data, sizeof(POINT_TRAIL_EX_DESC));
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 15.f;
	Effect_Data.fSpreadDis = 5.f;
	Effect_Data.fSize = 7.0f;
	Effect_Data.fTimeTerm = 0.025f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_200_10;
	Effect_Data.iShaderPass = 1;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_BlueBig");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Trail", (_uint)ELevel::Stage1, L"Layer_Effect", &Effect_Data);

	Effect_Data.fAlphaTimePower = 5.f;
	Effect_Data.fSize = 4.0f;
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_BlueSmall");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Trail", (_uint)ELevel::Stage1, L"Layer_Effect", &Effect_Data);
}

CSkill_ManaBomb * CSkill_ManaBomb::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_ManaBomb* pInstance = new CSkill_ManaBomb(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_ManaBomb) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_ManaBomb::Clone_GameObject(void * pArg)
{
	CSkill_ManaBomb* pInstance = new CSkill_ManaBomb(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CSkill_ManaBomb) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_ManaBomb::Free()
{
	Safe_Release(m_pPoint_Trail);
	__super::Free();
}
