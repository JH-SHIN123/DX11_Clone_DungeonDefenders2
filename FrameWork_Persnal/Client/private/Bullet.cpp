#include "stdafx.h"
#include "..\public\Bullet.h"

CBullet::CBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBullet::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CBullet::Tick(_float TimeDelta)
{
	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	m_pColliderCom->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return _int();
}

_int CBullet::Late_Tick(_float TimeDelta)
{
	m_fLifeTime -= TimeDelta;
	if (0.f >= m_fLifeTime)
		m_IsDelete_This = true;

	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CBullet::Render()
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

		m_pModelCom->Render_Model(i, 1);
	}

	if (nullptr != m_pTextureCom)
	{

	}

	return S_OK;
}

HRESULT CBullet::Ready_Component(void * pArg)
{
	BULLET_DESC Data;
	memcpy(&Data, pArg, sizeof(BULLET_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);




	//if (ELevel::End != Data.eTextureLevel)
	//{
	//	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect_Model"), TEXT("Com_Buffer"), (CComponent**)&m_pBufferRectCom);
	//	hr = CGameObject::Add_Component((_uint)Data.eTextureLevel, Data.szTextureName, TEXT("Com_Texture_0"), (CComponent**)&m_pTextureCom);
	//}

	m_vGoDir = Data.vDir;
	m_fLifeTime = Data.fLifeTime;

	if (S_OK != hr)
		MSG_BOX("CBullet::Ready_Component Failed!");

	return hr;
}

CGameObject * CBullet::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CBullet::Free()
{
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pBufferRectCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}				 
