#include "stdafx.h"
#include "..\public\Masking_UI.h"

CMasking_UI::CMasking_UI(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CMasking_UI::CMasking_UI(const CMasking_UI & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CMasking_UI::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMasking_UI::NativeConstruct(void * pArg)
{

	if (nullptr != pArg)
		memcpy(&m_Mask_Desc, pArg, sizeof(MASK_UI_DESC));

	__super::NativeConstruct(&m_Mask_Desc.UI_Desc);

	if (true == m_Mask_Desc.HasFrame)
	{
		_vector vFramePos = XMLoadFloat3(&m_Mask_Desc.vFrameOffSetPos);
		vFramePos += m_pMovementCom->Get_State(EState::Position);
		XMStoreFloat3(&m_Mask_Desc.vFrameOffSetPos, vFramePos);

		_matrix FrameMatrix = GET_INDENTITY_MATRIX;
		FrameMatrix = XMMatrixSet(m_Mask_Desc.vFrameScale.x, 0.f, 0.f, 0.f
			, 0.f, m_Mask_Desc.vFrameScale.y, 0.f, 0.f
			, 0.f, 0.f, m_Mask_Desc.vFrameScale.z, 0.f
			, m_Mask_Desc.vFrameOffSetPos.x, m_Mask_Desc.vFrameOffSetPos.y, m_Mask_Desc.vFrameOffSetPos.z, 1.f);

		XMStoreFloat4x4(&m_FrameMatrix, FrameMatrix);
	}
	
	return S_OK;
}

_int CMasking_UI::Tick(_float TimeDelta)
{
	_float3 vPos;
	XMStoreFloat3(&vPos, m_pMovementCom->Get_State(EState::Position));
	m_FrameMatrix._41 = vPos.x += m_Mask_Desc.vFrameOffSetPos.x;
	m_FrameMatrix._42 = vPos.y += m_Mask_Desc.vFrameOffSetPos.y;
	m_FrameMatrix._43 = vPos.z += m_Mask_Desc.vFrameOffSetPos.z;

	return _int();
}

_int CMasking_UI::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CMasking_UI::Render(_uint MaskShaderPass, _uint UIFramePass)
{
	if (nullptr == m_pBufferRectCom)
		return E_FAIL;

	_float2 Time = { 0.f,0.f };

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	_matrix FrameMatrix = XMLoadFloat4x4(&m_FrameMatrix);

	if (true == m_Mask_Desc.HasFrame)
	{
		if (true == m_Mask_Desc.IsFrameFirst)
		{
			m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
			m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(FrameMatrix), sizeof(_matrix));
			m_pBufferRectCom->Render(UIFramePass);

		}

		m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &Time, sizeof(_float2));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom->Get_ShaderResourceView(0));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(1));
		m_pBufferRectCom->Render(MaskShaderPass);

		if (false == m_Mask_Desc.IsFrameFirst)
		{
			m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
			m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(FrameMatrix), sizeof(_matrix));
			m_pBufferRectCom->Render(UIFramePass);
		}
	}

	else
	{
		m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &Time, sizeof(_float2));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom->Get_ShaderResourceView(0));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(1));
		m_pBufferRectCom->Render(MaskShaderPass);
	}


	return S_OK;
}

CMasking_UI * CMasking_UI::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMasking_UI* pInstance = new CMasking_UI(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMasking_UI) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMasking_UI::Clone_GameObject(void * pArg)
{
	CMasking_UI* pClone = new CMasking_UI(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CMasking_UI) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CMasking_UI::Free()
{
	__super::Free();
}
