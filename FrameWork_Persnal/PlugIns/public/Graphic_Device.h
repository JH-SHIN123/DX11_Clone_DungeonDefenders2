#pragma once

#include "Base.h"

/* 장치객체를 초기화한다. */

/* m_pDevice, m_pDevice_Context 를 생성한다. */
/* 하드웨어의 정보를 조사한다. */
/* 앨리어싱 처리를 어떻게할까?! (x) */
/* 백버퍼 정보 생성 및 렌더타겟뷰를 생성한다. */
/* 뎁스버퍼, 스텐실버퍼를 렌더타겟뷰를 생성한다. */
/* 렌더타겟 뷰들을 장치에 엮는다. */
/* 뷰포트 설정. */


BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)
public:
	enum WINMODE { TYPE_FULLMODE, TYPE_WINMODE, TYPE_END };

public:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT Ready_Graphic_Device(WINMODE eWinMode, HWND hWnd, _uint iSizeX, _uint iSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDevice_Context);
	HRESULT Clear_Back_Buffer(_float4 vClearColor);
	HRESULT Clear_Depth_Stencil_Buffer();
	HRESULT Present();


private:
	/* 각종 컴객체를 생성하는데 활용. 할당 */
	ID3D11Device*					m_pDevice = nullptr;

	/* 함수를 제공한다. */
	ID3D11DeviceContext*			m_pDevice_Context = nullptr;

	IDXGISwapChain*					m_pSwapChain = nullptr;

	ID3D11RenderTargetView*			m_pBackBufferView = nullptr;

	ID3D11DepthStencilView*			m_pDepthStencilView = nullptr;

private:
	HRESULT Ready_SwapChain(HWND hWnd, WINMODE eWinMode, _uint iSizeX, _uint iSizeY);
	HRESULT Ready_BackBuffer_RenderTargetView();
	HRESULT Ready_Depth_Stencil_RenderTargetView(_uint iSizeX, _uint iSizeY);

public:
	virtual void Free() override;
};

END