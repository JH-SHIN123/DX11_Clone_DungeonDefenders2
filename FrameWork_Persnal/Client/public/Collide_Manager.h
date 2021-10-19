#pragma once

#ifndef __COLLIDE_MANAGER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CCollide_Manager final : public CBase
{
	DECLARE_SINGLETON(CCollide_Manager)
public:
	CCollide_Manager();
	virtual ~CCollide_Manager();

public:
	_bool Collide_Check(const _tchar* szDstObjectLayer, ELevel eDstObjectLevel, const _tchar* szSrcObjectLayer, ELevel eSrcObjectLevel);
	void Collide_Check_Poison(const _tchar* szDstObjectLayer, ELevel eDstObjectLevel, const _tchar* szSrcObjectLayer, ELevel eSrcObjectLevel);

	void Collide_Push(const _tchar* szDstObjectLayer, ELevel eDstObjectLevel, const _tchar* szSrcObjectLayer, ELevel eSrcObjectLevel);
	void Collide_Push_Monster(const _tchar* szDstObjectLayer, ELevel eDstObjectLevel, const _tchar* szSrcObjectLayer, ELevel eSrcObjectLevel);


public:
	virtual void Free() override;
};

END
#define __COLLIDE_MANAGER_H__
#endif // !__COLLIDE_MANAGER_H__
