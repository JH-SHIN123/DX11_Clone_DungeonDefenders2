#pragma once

#ifndef __LEVEL_MANAGER_H__

/* 현재 게임내에서 보여줘야할 레벨의 주소를 가지고 있는다. */
#include "Base.h"

BEGIN(Engine)

class CLevel_Manager final : public CBase
{
	DECLARE_SINGLETON(CLevel_Manager)
private:
	CLevel_Manager();
	virtual ~CLevel_Manager() = default;
public:
	HRESULT SetUp_CurrentLevel(class CLevel* pCurrentLevel);
	_int Tick(_double TimeDelta);
	HRESULT Render();

private:
	class CLevel*				m_pCurrentLevel = nullptr;
public:
	virtual void Free() override;
};

END
#define __LEVEL_MANAGER_H__
#endif // !__LEVEL_MANAGER_H__