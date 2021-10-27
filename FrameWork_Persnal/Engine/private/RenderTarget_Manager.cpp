#include "..\public\RenderTarget_Manager.h"
#include "RenderTarget.h"

IMPLEMENT_SINGLETON(CRenderTarget_Manager)

CRenderTarget_Manager::CRenderTarget_Manager()
{

}

ID3D11ShaderResourceView * CRenderTarget_Manager::Get_ShaderResourceView(const _tchar * pRenderTargetTag)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return nullptr;

	return pRenderTarget->Get_ShaderResourceView();
}

HRESULT CRenderTarget_Manager::Add_RenderTarget(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext, _tchar* pRenderTargetTag, _uint iWidth, _uint iHeight, DXGI_FORMAT eFormat, _float4 vClearColor)
{
	if (nullptr != Find_RenderTarget(pRenderTargetTag))
		return E_FAIL;

	CRenderTarget*	pRenderTarget = CRenderTarget::Create(pDevice, pDeviceContext, iWidth, iHeight, eFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(pRenderTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Add_MRT(const _tchar * pRenderTargetTag, const _tchar * pMRTTag)
{
	CRenderTarget*	pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);
		m_MRTs.emplace(pMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CRenderTarget_Manager::Begin_MRT(ID3D11DeviceContext* pDevice_Context, const _tchar * pMRTTag)
{
	list<CRenderTarget*>*	pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	pDevice_Context->OMGetRenderTargets(1, &m_pBackBufferView, &m_pDepthStencilView);
	ID3D11RenderTargetView*		RenderTargets[8] = { nullptr };

	_uint		iRenderTargetIndex = 0;

	for (auto& pRenderTarget : *pMRTList)
	{
		pRenderTarget->Clear_View();
		RenderTargets[iRenderTargetIndex++] = pRenderTarget->Get_RenderTargetView();
		//pRenderTarget->Release();
	}

	pDevice_Context->OMSetRenderTargets((_uint)pMRTList->size(), RenderTargets, m_pDepthStencilView);

	m_pDepthStencilView->Release();
	m_pBackBufferView->Release();
	return S_OK;
}

HRESULT CRenderTarget_Manager::End_MRT(ID3D11DeviceContext* pDevice_Context, const _tchar * pMRTTag)
{
	if (nullptr == pDevice_Context)
		return E_FAIL;

	pDevice_Context->OMSetRenderTargets(1, &m_pBackBufferView, m_pDepthStencilView);

	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderTarget_Manager::Ready_DebugBuffer(const _tchar * pRenderTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(pRenderTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Ready_DebugBuffer(fX, fY, fSizeX, fSizeY);
}

HRESULT CRenderTarget_Manager::Render_DebugBuffer(const _tchar * pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);
	if (nullptr == pMRTList)
		return E_FAIL;

	for (auto& pRenderTarget : *pMRTList)
	{
		if (FAILED(pRenderTarget->Render_DebugBuffer()))
			return E_FAIL;
	}

	return S_OK;
}

#endif

CRenderTarget * CRenderTarget_Manager::Find_RenderTarget(const _tchar * pRenderTargetTag)
{
	auto	iter = find_if(m_RenderTargets.begin(), m_RenderTargets.end(), CTagFinder(pRenderTargetTag));

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<CRenderTarget*>* CRenderTarget_Manager::Find_MRT(const _tchar * pMRTTag)
{
	auto	iter = find_if(m_MRTs.begin(), m_MRTs.end(), CTagFinder(pMRTTag));

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

void CRenderTarget_Manager::Free()
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			Safe_Release(pRenderTarget);
		}
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto& Pair : m_RenderTargets)
		Safe_Release(Pair.second);

}
