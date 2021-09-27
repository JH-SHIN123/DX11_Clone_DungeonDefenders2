#include "stdafx.h"
#include "..\public\Weapon.h"

CWeapon::CWeapon(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CWeapon::CWeapon(const CWeapon & rhs)
	: CGameObject(rhs)
{
}

HRESULT CWeapon::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CWeapon::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(1.f, 1.f, 1.f, 0.f));
	Set_RotateMatrix(XMMatrixIdentity());

	return S_OK;
}

_int CWeapon::Tick(_float TimeDelta)
{
	return _int();
}

_int CWeapon::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CWeapon::Render()
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

	return S_OK;
}

void CWeapon::Weapon_Equip(_fmatrix BoneMatrix, _fmatrix ParentMatrix)
{
	_matrix MyMatrix = BoneMatrix * ParentMatrix  /** XMMatrixTranslation(0.f, 0.f, 0.f)*/ *XMLoadFloat4x4(&m_RotateMatrix);
	 
	//_vector vOffSet = XMVectorSet(5.f, 5.f, 0.f, 0.f);
	//MyMatrix.r[3] += vOffSet;

	m_pMovementCom->Set_WorldMatrix(MyMatrix);
}

void CWeapon::Set_RotateMatrix(_fmatrix RotateMatrix)
{
	XMStoreFloat4x4(&m_RotateMatrix, RotateMatrix);
}

void CWeapon::Set_OffSetPosition(_fmatrix TransMatrix)
{
	XMStoreFloat4x4(&m_OffSetMatrix, TransMatrix);
}

HRESULT CWeapon::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	WEAPON_DESC Data;

	memcpy(&Data, pArg, sizeof(WEAPON_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	if (2 < lstrlen(Data.szTextureName))
	{
		hr = CGameObject::Add_Component((_uint)Data.eTextureLevel, Data.szTextureName, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);
	}



	return S_OK;
}

CGameObject * CWeapon::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CWeapon::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
