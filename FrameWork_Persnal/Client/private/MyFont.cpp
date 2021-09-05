#include "stdafx.h"
#include "..\public\MyFont.h"
#include "VIBuffer_Font.h"

CMyFont::CMyFont(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMyFont::CMyFont(const CMyFont & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMyFont::NativeConstruct_Prototype()
{
	return E_FAIL;
}

HRESULT CMyFont::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);


	return S_OK;
}

_int CMyFont::Tick(_float TimeDelta)
{
	return _int();
}

_int CMyFont::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CMyFont::Render()
{
	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));

	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));

	m_pVIBufferCom->Render(1);

	return S_OK;
}

HRESULT CMyFont::Ready_Component(void * pArg)
{
	memcpy(&m_Font, pArg, sizeof(FONT_DESC));

	_float4 vTexUV = { m_Font.TextUV_Left.x, m_Font.TextUV_Left.y, m_Font.TextUV_Right.x, m_Font.TextUV_Right.y };
	m_pVIBufferCom = CVIBuffer_Font::Create(m_pDevice, m_pDevice_Context, TEXT("../Bin/Shader/Shader_Default.hlsl"), "DefaultTechnique", &vTexUV);

	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Font"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);

	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom);
	m_pMovementCom->Set_Scale(XMLoadFloat4(&m_Font.vScale));
	m_pMovementCom->Set_State(EState::Position, XMLoadFloat4(&m_Font.vPos));

	return S_OK;
}

CMyFont * CMyFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void * pArg)
{
	CMyFont* pInstance = new CMyFont(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CMonster) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMyFont::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMyFont::Free()
{
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);

	__super::Free();
}
