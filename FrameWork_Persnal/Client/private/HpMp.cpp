#include "stdafx.h"
#include "..\public\HpMp.h"

CHpMp::CHpMp(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CHpMp::CHpMp(const CHpMp & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CHpMp::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CHpMp::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_vInterval = _float2(69.f, 0.f);

	Ready_Component(pArg);



	return S_OK;
}

_int CHpMp::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	//m_pMovementCom->Set_Scale(XMVectorSet())
	Damage_Check();

	return _int();
}

_int CHpMp::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CHpMp::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	// Back
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(2);

	// Hp Mask
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Mask->Get_WorldMatrix()), sizeof(_matrix));
	//m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	//m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	//m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom->Get_ShaderResourceView(2));
	m_pBufferRectCom->Render(4);

	// 마스크 이미지를 움직여야함

	// Hp Mask
	//_float4x4 MaskMatrix;
	//XMStoreFloat4x4(&MaskMatrix, m_pMovementCom->Get_WorldMatrix());
	//MaskMatrix.m[4][0] += 30.f;
	//m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&MaskMatrix)), sizeof(_matrix));
	//m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Mask->Get_ShaderResourceView(0));
	//m_pBufferRectCom->Render(1);


	// Mp
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_MP->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);





	return S_OK;
}

void CHpMp::Damage_Check()
{
	_float4 vMaskPos;
	XMStoreFloat4(&vMaskPos, m_pMovementCom_Mask->Get_State(EState::Position));
	//XMStoreFloat3(&vMaskPos, m_pMovementCom_Mask->Get_Scale(EState::Up));

	//if (GetAsyncKeyState('Q') & 0x8000)
	//	vMaskPos.y -= 0.5f;
	//if (GetAsyncKeyState('W') & 0x8000)
	//	vMaskPos.y += 0.5f;

	m_pMovementCom_Mask->Set_State(EState::Position, XMVectorSet(vMaskPos.x, vMaskPos.y, 0.f, 1.f));
}

HRESULT CHpMp::Ready_Component(void* pArg)
{
	UI2D_DESC* pData = new UI2D_DESC;
	memcpy(pData, pArg, sizeof(UI2D_DESC));

	HRESULT hr = S_OK;

	/* For.Transform */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Mp"), (CComponent**)&m_pMovementCom_MP);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Mask"), (CComponent**)&m_pMovementCom_Mask);
	//hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_HpMp_Mask"), TEXT("Com_Texture_HpMp_Mask"), (CComponent**)&m_pTextureCom_Mask);


	/* 	UI_Desc.vPos = _float2(-512.f, -212.f);
	UI_Desc.vScale = _float2(64.f, 128.f);*/
	_vector vPosMp = XMVectorSet(pData->vPos.x + m_vInterval.x, pData->vPos.y + m_vInterval.y, 0.f, 1.f);
	_vector vPosMask = XMVectorSet(pData->vPos.x, pData->vPos.y + 32.f, 0.f, 1.f);


	m_pMovementCom_MP->Set_Scale(XMVectorSet(pData->vScale.x, pData->vScale.y, 0.f, 0.f));
	m_pMovementCom_MP->Set_State(EState::Position, vPosMp);


	m_pMovementCom_Mask->Set_Scale(XMVectorSet(pData->vScale.x, pData->vScale.y, 0.f, 0.f));
	m_pMovementCom_Mask->Set_State(EState::Position, vPosMask);

	Safe_Delete(pData);
	return S_OK;
}

CHpMp * CHpMp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CHpMp* pInstance = new CHpMp(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CHpMp) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHpMp::Clone_GameObject(void * pArg)
{
	CHpMp* pInstance = new CHpMp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CHpMp) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHpMp::Free()
{
	Safe_Release(m_pMovementCom_Mask);

	Safe_Release(m_pMovementCom_MP);
	Safe_Release(m_pBufferRectCom_MP);
	//Safe_Release(m_pTextureCom_Mask);

	__super::Free();
}
