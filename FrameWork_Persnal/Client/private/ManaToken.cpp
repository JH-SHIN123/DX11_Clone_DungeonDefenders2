#include "stdafx.h"
#include "..\public\ManaToken.h"

CManaToken::CManaToken(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CManaToken::CManaToken(const CManaToken & rhs)
	: CGameObject(rhs)
{
}

HRESULT CManaToken::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CManaToken::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CManaToken::Tick(_float TimeDelta)
{
	// 따라간다
	CGameObject* pTarget = GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player");

	if (nullptr == pTarget)
		return OBJECT_DEAD;

	CMovement* pMovement = (CMovement*)pTarget->Get_Component(L"Com_Movement");

	if (nullptr == pMovement)
		return OBJECT_DEAD;

	_vector vDir = pMovement->Get_State(EState::Position) - m_pMovementCom->Get_State(EState::Position);

	_float fDis = XMVectorGetX(XMVector3Length(vDir)) * 0.5f;
	vDir = XMVector3Normalize(vDir);
	vDir = XMVectorSetW(vDir, 0.f);
	vDir *= fDis;

	m_fDropTime -= TimeDelta;

	if (0.f >= m_fDropTime)
		m_fDropTime = 0.f;

	m_pMovementCom->Go_Up(TimeDelta* (m_fDropTime * 2.f));

	m_fMoveRatio += TimeDelta;

	if (1.f <= m_fMoveRatio)
		m_fMoveRatio = 1.f;

	m_pMovementCom->Go_Dir_Vector(TimeDelta * m_fMoveRatio, vDir);
	m_pMovementCom->RotateToAxis_Tick(TimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return _int();
}

_int CManaToken::Late_Tick(_float TimeDelta)
{
	if (true == m_pColliderCom_Attack->Get_IsCollide())
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CManaToken::Render()
{
#ifdef _DEBUG
	if (nullptr != m_pColliderCom_Attack)
		m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


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

HRESULT CManaToken::Ready_Component(void * pArg)
{
	MANATOKEN_DESC Data;
	memcpy(&Data, pArg, sizeof(MANATOKEN_DESC));

	HRESULT hr = S_OK;

	Data.Move_Desc.fSpeedPerSec = 10.f;
	Data.Move_Desc.fRotatePerSec = 1.f;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Move_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data.Collider_Desc);

	switch (m_eManaType)
	{
	case Client::EManaType::Default:
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, L"Component_Mesh_ManaToken", TEXT("Com_Model"), (CComponent**)&m_pModelCom);
		Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
		Set_Pivot_Rotate_Radian(XMVectorSet(180.f, 0.f, 90.f, 0.f));
		m_iMp = 15;
		break;
	case Client::EManaType::Large:
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, L"Component_Mesh_ManaToken_L", TEXT("Com_Model"), (CComponent**)&m_pModelCom);
		Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));
		m_iMp = 25;
		break;
	default:
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, L"Component_Mesh_ManaToken", TEXT("Com_Model"), (CComponent**)&m_pModelCom);
		Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
		Set_Pivot_Rotate_Radian(XMVectorSet(180.f, 0.f, 90.f, 0.f));
		m_iMp = 15;
		break;
	}



	if (hr)
		MSG_BOX("CManaToken::Ready_Component");

	return hr;
}

CManaToken * CManaToken::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CManaToken*		pInstance = new CManaToken(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CManaToken) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CManaToken::Clone_GameObject(void * pArg)
{
	CManaToken*		pInstance = new CManaToken(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CManaToken) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CManaToken::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom_Attack);

	Safe_Release(m_pBufferRectCom);
	Safe_Release(m_pTextureCom);
}
