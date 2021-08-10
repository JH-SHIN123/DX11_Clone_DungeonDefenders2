#pragma once

#include "Engine_Defines.h"

/* 모든 객체들의 부모. */
/* 다르게 이야기하면, 모든 객체들의 공통적인 기능을 보유한다. */
/* 레퍼런스 카운트 관리. */

BEGIN(Engine)

class ENGINE_DLL CBase abstract
{
public:
	CBase();
	virtual ~CBase() = default;
	
public:
	unsigned long AddRef();
	unsigned long Release();

public:
	virtual void Free() = 0;
	
private:
	unsigned long			m_dwRefCnt = 0;

};

END