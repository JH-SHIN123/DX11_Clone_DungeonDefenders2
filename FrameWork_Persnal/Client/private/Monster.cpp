#include "stdafx.h"
#include "..\public\Monster.h"
#include "Masking_MeterBar_3D.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CMonster::Tick(_float TimeDelta)
{
	if (nullptr != m_pMeterBar_Hp)
		m_pMeterBar_Hp->Tick(TimeDelta);


	return _int();
}

_int CMonster::Late_Tick(_float TimeDelta)
{
	if (nullptr != m_pMeterBar_Hp)
	{
		m_pMeterBar_Hp->Set_Count((_float)m_pStatusCom->Get_Hp(), (_float)m_pStatusCom->Get_HpMax());
		m_pMeterBar_Hp->Late_Tick(TimeDelta);
	}



	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CMonster::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

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

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 0);
	}


#ifdef _DEBUG
	//m_pColliderCom->Render_Collider();
#endif


	return S_OK;
}

EMonsterAI CMonster::AI_Check()
{
	CLayer* pLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1, L"Layer_Tower");
	if (nullptr == pLayer)
		return EMonsterAI::End;

	// AI를 구상 해보자
	/*
	1. 일반적으로 몬스터는 특정 경로를 따라간다.
		 A_1. 그러다가 플레이어에게 공격을 맞으면 플레이어를 따라간다.
			A_2. 너무 멀어지면 다시 경로를 찾아 간다.
		B_1. 타워를 만나면 타워를 때린다.
			A_1. 플레이어한테 공격을 맞으면 플레이어를 따라간다.
				A_2. 너무 멀어지면 다시 경로를 찾아 간다.

	
	*/


	list<CGameObject*> listObject = pLayer->Get_GameObject_List();

	for (auto& iter : listObject)
	{

	}





	return EMonsterAI::End;
}

HRESULT CMonster::Ready_Component(void * pArg)
{
	MONSTER_DESC Data;
	memcpy(&Data, pArg, sizeof(MONSTER_DESC));
	
	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)Data.eLevel, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);





	if (S_OK != hr)
		MSG_BOX("CMonster::Ready_Component Failed");

	return hr;
}

CGameObject * CMonster::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMonster::Free()
{
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pMeterBar_Hp);

	__super::Free();
}
