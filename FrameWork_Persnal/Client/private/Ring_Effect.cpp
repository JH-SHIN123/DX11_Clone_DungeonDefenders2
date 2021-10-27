#include "stdafx.h"
#include "..\public\Ring_Effect.h"


CRing_Effect::CRing_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CRing_Effect::CRing_Effect(const CRing_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CRing_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRing_Effect::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.1f, 0.1f, 0.1f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CRing_Effect::Tick(_float TimeDelta)
{
	return _int();
}

_int CRing_Effect::Late_Tick(_float TimeDelta)
{
	m_fLifeTime -= TimeDelta * 1.f;

	if (0.f >= m_fLifeTime)
		m_fLifeTime = 0.f;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CRing_Effect::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	_float4 vColor = { 2.f, 1.5f, 0.8f, 1.f };
	m_pModelCom->Set_Variable("g_vColor", &vColor, sizeof(_float4));

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 11);
	}

	return S_OK;
}

void CRing_Effect::Set_WorldMatrix(_fmatrix WorldMatrix)
{
	_vector vRight = XMVector3Normalize(WorldMatrix.r[0]);
	_vector vUp = XMVector3Normalize(WorldMatrix.r[1]);
	_vector vLook = XMVector3Normalize(WorldMatrix.r[2]);
	_vector vPos = XMVector3Normalize(WorldMatrix.r[3]); 


	m_pMovementCom->Set_State(EState::Right, vRight * m_pMovementCom->Get_Scale(EState::Right));
	m_pMovementCom->Set_State(EState::Up, vUp * m_pMovementCom->Get_Scale(EState::Up));
	m_pMovementCom->Set_State(EState::Look, vLook * m_pMovementCom->Get_Scale(EState::Look));

	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CRing_Effect::Set_LifeTime(_float fTime)
{
	m_fLifeTime = fTime;
}

HRESULT CRing_Effect::Ready_Component(void * pArg)
{
	RING_EFFECT_DESC Data;
	memcpy(&Data, pArg, sizeof(RING_EFFECT_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	m_fLifeTime = Data.fLifeTime;

	if (S_OK != hr)
		MSG_BOX("CRing_Effect::Ready_Component Failed!");

	return hr;
}

CRing_Effect * CRing_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CRing_Effect*		pInstance = new CRing_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CRing_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CRing_Effect::Clone_GameObject(void * pArg)
{
	CRing_Effect*		pInstance = new CRing_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CRing_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CRing_Effect::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
