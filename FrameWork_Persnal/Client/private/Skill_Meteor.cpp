#include "stdafx.h"
#include "..\public\Skill_Meteor.h"
#include "Point_Ex_Trail.h"
#include "Point_Spread2.h"

CSkill_Meteor::CSkill_Meteor(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CSkill_Meteor::CSkill_Meteor(const CSkill_Meteor & rhs)
	: CBullet(rhs)
{
}

HRESULT CSkill_Meteor::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Meteor::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));

	return S_OK;
}

_int CSkill_Meteor::Tick(_float TimeDelta)
{
	if (__super::Tick(TimeDelta))
	{
		Create_Explosion();
		return OBJECT_DEAD;
	}

	if (true == m_IsFall)
	{
		// 이거 몬스터 방향으로 
		m_pMovementCom->Go_Dir_Vector(TimeDelta, XMLoadFloat3(&m_vGoDir));

	}

	else
	{
		m_fCastTime -= TimeDelta;

		if (0.f >= m_fCastTime)
			m_IsFall = true;
	}

	_float fCellY = 999.f;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	m_pNaviCom->Get_Collision(&fCellY, vPos);

	_float fMyY = vPos.m128_f32[1];

	if (fCellY + 1.5f >= fMyY)
	{
		Create_Explosion();
		return OBJECT_DEAD;
	}

	m_pPoint_Trail->Set_Pos(vPos);
	m_pPoint_Trail->Tick(TimeDelta);

	return 0;
}

_int CSkill_Meteor::Late_Tick(_float TimeDelta)
{
	_int iReturn = 0;

	if (iReturn = __super::Late_Tick(TimeDelta))
	{
		Create_Explosion();

		return iReturn;
	}
	m_pPoint_Trail->Late_Tick(TimeDelta);

	return iReturn;
}

HRESULT CSkill_Meteor::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CSkill_Meteor::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_MeshLevel_1_Navi"), TEXT("Com_Navi"), (CComponent**)&m_pNaviCom);

	POINT_TRAIL_EX_DESC Effect_Data;
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 1.5f;
	Effect_Data.fSpreadDis = 2.f;
	Effect_Data.fSize = 7.f;
	Effect_Data.fTimeTerm = 0.05f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_200_50;
	Effect_Data.iShaderPass = 1;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_50");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_Red_Dark");

	m_pPoint_Trail = CPoint_Ex_Trail::Create(m_pDevice, m_pDevice_Context);
	m_pPoint_Trail->NativeConstruct(&Effect_Data);

	if (S_OK != hr)
		MSG_BOX("CSkill_Meteor::Ready_Component");

	return hr;
}

void CSkill_Meteor::Create_Explosion()
{
	BULLET_DESC Data;
	ZeroMemory(&Data, sizeof(BULLET_DESC));

	Data.fLifeTime = 1.f;
	lstrcpy(Data.szModelName, L"Component_Mesh_Skill_Meteor_Explosion");
	XMStoreFloat4(&Data.MoveState_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Skill_Meteor_Explosion"), (_uint)ELevel::Stage1, L"Layer_Effect", &Data);

	POINT_SPREAD_DESC_2 Effect_Data;
	Effect_Data.IsTime = true;
	Effect_Data.vSize = { 4.f, 4.f };
	Effect_Data.Point_Desc.fLifeTime = 1.5f;
	Effect_Data.Point_Desc.iShaderPass = 0;
	Effect_Data.Point_Desc.fSpreadDis = 10.f;
	Effect_Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Effect_Data.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	lstrcpy(Effect_Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Effect_Data.Point_Desc.szTextrueName, L"Component_Texture_Smoke");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Effect_Data);
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Effect_Data);


	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	if (60.f > fDis)
	{
		fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;
		CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING_3);
		CSound_Manager::GetInstance()->Play_Sound(L"Demon_fireball.ogg", CHANNEL_ANYTHING_3);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING_3, fDis + 0.2f);
	}

}

CSkill_Meteor * CSkill_Meteor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CSkill_Meteor* pInstance = new CSkill_Meteor(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CSkill_Meteor) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CSkill_Meteor::Clone_GameObject(void * pArg)
{
	CSkill_Meteor* pInstance = new CSkill_Meteor(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CSkill_Meteor) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CSkill_Meteor::Free()
{
	Safe_Release(m_pPoint_Trail);
	Safe_Release(m_pNaviCom);
	__super::Free();
}
