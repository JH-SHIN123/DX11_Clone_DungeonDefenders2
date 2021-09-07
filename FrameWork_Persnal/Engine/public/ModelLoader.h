#pragma once

#ifndef __MODELLOADER_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CModelLoader  final : public CBase
{
	DECLARE_SINGLETON(CModelLoader)
public:
	CModelLoader();
	virtual ~CModelLoader() = default;

public:
	virtual void Free() override;
};

END
#define __MODELLOADER_H__
#endif // !__MODELLOADER_H__
