#include "..\public\Graphic_Device.h"

IMPLEMENT_SINGLETON(CGraphic_Device)

CGraphic_Device::CGraphic_Device()
{
}

HRESULT CGraphic_Device::Ready_Graphic_Device(WINMODE eWinMode, HWND hWnd, _uint iSizeX, _uint iSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDevice_Context)
{
	_uint			iCreateFlag = 0;

#ifdef _DEBUG
	iCreateFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif	
	D3D_FEATURE_LEVEL			FeatureLevel;

	if (FAILED(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, iCreateFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLevel, &m_pDevice_Context)))
		return E_FAIL;

	if (FAILED(Ready_SwapChain(hWnd, eWinMode, iSizeX, iSizeY)))
		return E_FAIL;

	if (FAILED(Ready_BackBuffer_RenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_Depth_Stencil_RenderTargetView(iSizeX, iSizeY)))
		return E_FAIL;	

	m_pDevice_Context->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	D3D11_VIEWPORT				Viewport;

	/*FLOAT TopLeftX;
	FLOAT TopLeftY;
	FLOAT Width;
	FLOAT Height;
	FLOAT MinDepth;
	FLOAT MaxDepth;*/

	Viewport.TopLeftX = 0.0f;
	Viewport.TopLeftY = 0.0f;

	Viewport.Width = (_float)iSizeX;
	Viewport.Height = (_float)iSizeY;

	Viewport.MinDepth = 0.0f;
	Viewport.MaxDepth = 1.0f;

	m_pDevice_Context->RSSetViewports(1, &Viewport);

	if (nullptr != ppDevice)
	{
		*ppDevice = m_pDevice;
		Safe_AddRef(m_pDevice);

	}

	if (nullptr != ppDevice_Context)
	{
		*ppDevice_Context = m_pDevice_Context;
		Safe_AddRef(m_pDevice_Context);
	}
		
	return S_OK;
}

HRESULT CGraphic_Device::Clear_Back_Buffer(_float4 vClearColor)
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	m_pDevice_Context->ClearRenderTargetView(m_pBackBufferView, (_float*)&vClearColor);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_Depth_Stencil_Buffer()
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	m_pDevice_Context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	m_pSwapChain->Present(0, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, WINMODE eWinMode, _uint iSizeX, _uint iSizeY)
{
	IDXGIDevice*			pDevice = nullptr;

	if (FAILED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice)))
		return E_FAIL;

	IDXGIAdapter*			pAdapter = nullptr;

	if (FAILED(pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter)))
		return E_FAIL;

	IDXGIFactory*			pFactory = nullptr;

	if (FAILED(pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory)))
		return E_FAIL;

	DXGI_SWAP_CHAIN_DESC			SwapchainDesc;

	/* 생성하고자하는 백버퍼의 정보를 채우낟. */
	SwapchainDesc.BufferDesc.Width = iSizeX;
	SwapchainDesc.BufferDesc.Height = iSizeY;
	SwapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	/* 전체화면 처리시에 사용되는 정보. */
	SwapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapchainDesc.BufferDesc.RefreshRate.Denominator = 1;

	SwapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	SwapchainDesc.BufferCount = 1;
	SwapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapchainDesc.OutputWindow = hWnd;
	SwapchainDesc.Windowed = eWinMode;
	SwapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	SwapchainDesc.SampleDesc.Count = 1;
	SwapchainDesc.SampleDesc.Quality = 0;
	SwapchainDesc.Flags = 0;	

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapchainDesc, &m_pSwapChain)))
		return E_FAIL;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_BackBuffer_RenderTargetView()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	ID3D11Texture2D*			pBackBuffer = nullptr;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pBackBufferView)))
		return E_FAIL;

	Safe_Release(pBackBuffer);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_Depth_Stencil_RenderTargetView(_uint iSizeX, _uint iSizeY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;

	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	ID3D11Texture2D*		pDepthStencil = nullptr;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencil)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencil, nullptr, &m_pDepthStencilView)))
		return E_FAIL;


	Safe_Release(pDepthStencil);

	return S_OK;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferView);
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
