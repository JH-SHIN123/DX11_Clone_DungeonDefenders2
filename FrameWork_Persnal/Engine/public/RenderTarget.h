#pragma once

#ifndef __RENDER_TARGET_H__

#include "Base.h"

BEGIN(Engine)
class CRenderTarget final : public CBase
{
private:
	explicit CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	virtual ~CRenderTarget() = default;
public:
	ID3D11RenderTargetView* Get_RenderTargetView() const {
		return m_pRenderTargetView;
	}

	ID3D11ShaderResourceView* Get_ShaderResourceView() const {
		return m_pShaderResourceView;// m_pShaderResourceView->Release();
	}
public:
	HRESULT NativeConstruct(_uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
public:
	HRESULT Clear_View();

public:
	HRESULT Ready_DebugBuffer(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer();


private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDevice_Context = nullptr;
private:
	ID3D11Texture2D*			m_pTargetTexture = nullptr;

	ID3D11RenderTargetView*		m_pRenderTargetView = nullptr;
	ID3D11ShaderResourceView*	m_pShaderResourceView = nullptr;
private:
	_float4				m_vClearColor;


private:
	class CVIBuffer_RectRHW*		m_pVIBuffer = nullptr;


public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	virtual void Free() override;

};

END
#define __RENDER_TARGET_H__
#endif // !__RENDER_TARGET_H__
