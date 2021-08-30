#pragma once

#ifndef __FRAME_MANAGER_H__

#include "Engine_Defines.h"
#include "Base.h"
#include "Frame.h"

BEGIN(Engine)

class ENGINE_DLL CFrame_Manger final :	public CBase
{
	DECLARE_SINGLETON(CFrame_Manger)
private:
	explicit CFrame_Manger();
	virtual ~CFrame_Manger();

public:
	_bool IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);

public:
	HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);


private:
	CFrame*	Find_Frame(const _tchar* pFrameTag);

private:
	unordered_map<const _tchar*, CFrame*>		m_mapFrame;

public:
	virtual void Free();
};

END
#define __FRAME_MANAGER_H__
#endif // !__FRAME_MANAGER_H__
