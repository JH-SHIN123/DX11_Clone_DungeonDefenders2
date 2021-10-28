#include "..\public\RenderTarget.h"
#include "VIBuffer_RectRHW.h"

CRenderTarget::CRenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDeviceContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CRenderTarget::NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	D3D11_TEXTURE2D_DESC		TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = eFormat;

	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTargetTexture)))
	{
		MSG_BOX("Failed to Creating Texture2D For RenderTarget");
		return E_FAIL;
	}

	D3D11_RENDER_TARGET_VIEW_DESC		RenderTargetViewDesc;
	ZeroMemory(&RenderTargetViewDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	RenderTargetViewDesc.Format = eFormat;
	RenderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	RenderTargetViewDesc.Texture2D.MipSlice = 0;

	if (m_pDevice->CreateRenderTargetView(m_pTargetTexture, &RenderTargetViewDesc, &m_pRenderTargetView))
	{
		MSG_BOX("Failed to Creating RenderTargetView For RenderTarget");
		return E_FAIL;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC		ShaderResourceViewDesc;
	ZeroMemory(&ShaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	ShaderResourceViewDesc.Format = eFormat;
	ShaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ShaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTargetTexture, &ShaderResourceViewDesc, &m_pShaderResourceView)))
	{
		MSG_BOX("Failed to Creating ShaderResourceView For RenderTarget");
		return E_FAIL;
	}

	m_vClearColor = vClearColor;

	return S_OK;
}

HRESULT CRenderTarget::Clear_View()
{
	if (nullptr == m_pDevice_Context ||
		nullptr == m_pRenderTargetView)
		return E_FAIL;

	m_pDevice_Context->ClearRenderTargetView(m_pRenderTargetView, (_float*)&m_vClearColor);

	return S_OK;
}

HRESULT CRenderTarget::Ready_DebugBuffer(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	m_pVIBuffer = CVIBuffer_RectRHW::Create(m_pDevice, m_pDevice_Context, fX, fY, fSizeX, fSizeY, TEXT("../Bin/Shader/Shader_RenderTarget.hlsl"), "DefaultTechnique");
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderTarget::Render_DebugBuffer()
{
	m_pVIBuffer->Set_ShaderResourceView("g_DiffuseTexture", m_pShaderResourceView);

	m_pVIBuffer->Render(0);

	return S_OK;
}

CRenderTarget * CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	CRenderTarget*		pInstance = new CRenderTarget(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct(iWidth, iHeight, eFormat, vClearColor)))
	{
		MSG_BOX("Failed to Creating Instance (CRenderTarget) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRenderTarget::Free()
{
	Safe_Release(m_pShaderResourceView);//
	Safe_Release(m_pRenderTargetView);//
	Safe_Release(m_pTargetTexture);//

	Safe_Release(m_pDevice_Context);//
	Safe_Release(m_pDevice);//

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);//
#endif // _DEBUG

}
