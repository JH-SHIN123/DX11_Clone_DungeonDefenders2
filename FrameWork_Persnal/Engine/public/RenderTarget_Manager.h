#pragma once

#ifndef __RENDER_TARGET_MANAGER_H__

#include "Base.h"

BEGIN(Engine)
class CRenderTarget;

class CRenderTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CRenderTarget_Manager)
public:
	CRenderTarget_Manager();
	virtual ~CRenderTarget_Manager() = default;

public:
	ID3D11ShaderResourceView* Get_ShaderResourceView(const _tchar* pRenderTargetTag);

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, _tchar* pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor);
	HRESULT Add_MRT(const _tchar* pRenderTargetTag, const _tchar* pMRTTag);
	HRESULT Begin_MRT(ID3D11DeviceContext* pDevice_Context, const _tchar* pMRTTag);
	HRESULT End_MRT(ID3D11DeviceContext* pDevice_Context, const _tchar* pMRTTag);

#ifdef _DEBUG
public:
	HRESULT Ready_DebugBuffer(const _tchar* pRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_DebugBuffer(const _tchar* pMRTTag);
#endif
private:
	unordered_map<const _tchar*, CRenderTarget*>			m_RenderTargets;
	unordered_map<const _tchar*, list<CRenderTarget*>>		m_MRTs; // 멀티 렌더 타겟이라는 뜻ㅎ

private: // 오리지날 버퍼, 게임 시작 시 엔진 초기화 부분에서 만들어지는 유서깊은 친구들이다.
	ID3D11RenderTargetView*		m_pBackBufferView = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;


private:
	CRenderTarget* Find_RenderTarget(const _tchar* pRenderTargetTag);
	list<CRenderTarget*>* Find_MRT(const _tchar* pMRTTag);
public:
	virtual void Free() override;
};

END
#define __RENDER_TARGET_MANAGER_H__
#endif // !__RENDER_TARGET_MANAGER_H__
