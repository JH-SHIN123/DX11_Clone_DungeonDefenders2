#include "stdafx.h"
#include "..\public\Boss_RepeatBall.h"
#include "Point_Ex_Trail.h"
#include "Point_Ex_Healing.h"

CBoss_RepeatBall::CBoss_RepeatBall(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CBoss_RepeatBall::CBoss_RepeatBall(const CBoss_RepeatBall & rhs)
	: CBullet(rhs)
{
}

HRESULT CBoss_RepeatBall::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_RepeatBall::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.01f, 0.01f, 0.01f, 0.f));

	return S_OK;
}

_int CBoss_RepeatBall::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	_float fDis = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vGoDir) - m_pMovementCom->Get_State(EState::Position))) * 0.25f;
	_vector vDir = XMVector3Normalize(XMLoadFloat3(&m_vGoDir) - m_pMovementCom->Get_State(EState::Position));
	vDir = XMVectorSetW(vDir, 0.f);
	vDir *= fDis;

	m_fAlphaTime -= TimeDelta * 10.f;

	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);


	if (nullptr != m_pColliderCom_Attack)
	{
		if (true == m_pColliderCom_Attack->Get_IsCollide())
		{
			POINT_EX_HEAL_DESC Point_Desc;
			Point_Desc.Point_Desc.iRandDir = 7;
			Point_Desc.Point_Desc.fLifeTime = 3.f;
			Point_Desc.Point_Desc.fSize = 5.f;
			Point_Desc.Point_Desc.fSpreadDis = 3.f;
			Point_Desc.Point_Desc.fTimeTerm = 0.05f;
			Point_Desc.Point_Desc.InstanceValue = EInstanceValue::Point_Ex_200_10;
			Point_Desc.Point_Desc.iShaderPass = 3;
			Point_Desc.Point_Desc.fAlphaTime = 1.f;
			Point_Desc.Point_Desc.fAlphaTimePower = 2.f;
			Point_Desc.Point_Desc.fScalePower = 3.f;
			XMStoreFloat4(&Point_Desc.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
			lstrcpy(Point_Desc.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
			lstrcpy(Point_Desc.Point_Desc.szTextrueName, L"Component_Texture_Purple_2");

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Particle", (_uint)ELevel::Stage1, L"Layer_Effect", &Point_Desc);

			return OBJECT_DEAD;
		}

		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}
	m_pPoint_Trail->Set_Pos(m_pMovementCom->Get_State(EState::Position));
	m_pPoint_Trail->Tick(TimeDelta);
	return iReturn;
}

_int CBoss_RepeatBall::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
	{
		POINT_EX_HEAL_DESC Point_Desc;
		Point_Desc.Point_Desc.iRandDir = 7;
		Point_Desc.Point_Desc.fLifeTime = 3.f;
		Point_Desc.Point_Desc.fSize = 5.f;
		Point_Desc.Point_Desc.fSpreadDis = 3.f;
		Point_Desc.Point_Desc.fTimeTerm = 0.05f;
		Point_Desc.Point_Desc.InstanceValue = EInstanceValue::Point_Ex_200_10;
		Point_Desc.Point_Desc.iShaderPass = 3;
		Point_Desc.Point_Desc.fAlphaTime = 1.f;
		Point_Desc.Point_Desc.fAlphaTimePower = 2.f;
		Point_Desc.Point_Desc.fScalePower = 3.f;
		XMStoreFloat4(&Point_Desc.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
		lstrcpy(Point_Desc.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_10");
		lstrcpy(Point_Desc.Point_Desc.szTextrueName, L"Component_Texture_Purple_2");

		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Ex_Particle", (_uint)ELevel::Stage1, L"Layer_Effect", &Point_Desc);

		return OBJECT_DEAD;
	}
	m_pPoint_Trail->Late_Tick(TimeDelta);

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoss_RepeatBall::Render()
{
	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	_float4 vColor = { 0.8f, 2.f, 2.f, m_fAlphaTime };
	m_pModelCom->Set_Variable("g_vColor", &vColor, sizeof(_float4));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 8);
	}

#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CBoss_RepeatBall::Ready_Component(void * pArg)
{
	POINT_TRAIL_EX_DESC Effect_Data;
	Effect_Data.iRandDir = 5;
	Effect_Data.fAlphaTime = 1.f;
	Effect_Data.fAlphaTimePower = 1.5f;
	Effect_Data.fSpreadDis = 2.f;
	Effect_Data.fSize = 6.f;
	Effect_Data.fTimeTerm = 0.05f;
	Effect_Data.fLifeTime = 10.f;
	Effect_Data.InstanceValue = EInstanceValue::Point_Ex_200_50;
	Effect_Data.iShaderPass = 1;
	XMStoreFloat4(&Effect_Data.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Effect_Data.vColor = { 1.f,1.f,1.f };
	lstrcpy(Effect_Data.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_Ex_200_50");
	lstrcpy(Effect_Data.szTextrueName, L"Component_Texture_Purple");

	m_pPoint_Trail = CPoint_Ex_Trail::Create(m_pDevice, m_pDevice_Context);
	m_pPoint_Trail->NativeConstruct(&Effect_Data);

	return S_OK;
}

CBoss_RepeatBall * CBoss_RepeatBall::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoss_RepeatBall*		pInstance = new CBoss_RepeatBall(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoss_RepeatBall) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_RepeatBall::Clone_GameObject(void * pArg)
{
	CBoss_RepeatBall*		pInstance = new CBoss_RepeatBall(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_RepeatBall::Free()
{
	Safe_Release(m_pPoint_Trail);

	__super::Free();
}
