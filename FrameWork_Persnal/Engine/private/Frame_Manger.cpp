#include "Frame_Manger.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrame_Manger)

Engine::CFrame_Manger::CFrame_Manger(void)
{

}

Engine::CFrame_Manger::~CFrame_Manger(void)
{
	Free();
}

Engine::_bool Engine::CFrame_Manger::IsPermit_Call(const _tchar* pFrameTag,
	const _float& fTimeDelta)
{
	CFrame*		pInstance = Find_Frame(pFrameTag);
	if (nullptr == pInstance)
		return false;

	return pInstance->IsPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrame_Manger::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame*	pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	if (nullptr == pFrame)
		return false;

	m_mapFrame.emplace(pFrameTag, pFrame);

	return S_OK;
}

CFrame* Engine::CFrame_Manger::Find_Frame(const _tchar* pFrameTag)
{
	auto	iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTagFinder(pFrameTag));

	if (iter == m_mapFrame.end())
		return nullptr;

	return iter->second;
}

void Engine::CFrame_Manger::Free(void)
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
	m_mapFrame.clear();
}

