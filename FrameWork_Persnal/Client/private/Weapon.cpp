#include "stdafx.h"
#include "..\public\Weapon.h"
#include "StrikerTower_Bullet.h"
#include "Cursor_Manager.h"

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

		m_pModelCom->Render_Model(i, 0);
	}

	return S_OK;
}

void CWeapon::Weapon_Equip(_fmatrix BoneMatrix, _fmatrix ParentMatrix)
{
	// 스 자이공부
	_matrix MyMatrix = XMMatrixIdentity() * XMLoadFloat4x4(&m_RotateMatrix) * XMMatrixTranslation(-2.3f, 2.4f, 0.f) * BoneMatrix * ParentMatrix;
	 
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

void CWeapon::Create_Bullet(_fvector vDir, _int iAtt)
{
	_vector vMouseWorldPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector vMouseWorldDir = XMVectorZero();
	CCursor_Manager::GetInstance()->Get_MousePos_WorldSpace(&vMouseWorldPos, &vMouseWorldDir);

	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	vMyPos += XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * -8.f;

	BULLET_DESC Data;
	lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower_Bullet");
	Data.MoveState_Desc.fRotatePerSec = 50.f;

	_vector vvDir = vMouseWorldDir;//XMVector3Normalize(/*vTargetPos - vMyPos*/ vDir);

	XMStoreFloat3(&Data.vDir, vvDir);
	XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	Data.MoveState_Desc.fSpeedPerSec = 40.f;
	Data.fLifeTime = 10.f;

	Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
	Data.Attack_Collide_Desc.Attack_Desc.iDamage = _int(iAtt * 0.8f);
	Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
	Data.Attack_Collide_Desc.IsCenter = true;
	
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Player_Bullet", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);

	/*
	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	vMyPos += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * 8.f;

	BULLET_DESC Data;
	lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower_Bullet");
	lstrcpy(Data.szTextureName, L"Component_Texture_Glow_Green");
	Data.vTextureSize = { 5.f, 5.f };
	Data.MoveState_Desc.fRotatePerSec = 50.f;

	_vector vDir = XMVector3Normalize(vTargetPos - vMyPos);

	XMStoreFloat3(&Data.vDir, vDir);
	XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
	Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	Data.MoveState_Desc.fSpeedPerSec = 40.f;
	Data.fLifeTime = 10.f;

	Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
	Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
	Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
	//Data.Attack_Collide_Desc.vPosition = { 0.f, 50.f, 0.f };
	Data.Attack_Collide_Desc.IsCenter = true;

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_StrikerTower_Bullet", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);
	
	*/
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
