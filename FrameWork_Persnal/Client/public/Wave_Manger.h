#pragma once

#ifndef __WAVE_MANAGER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
class CWave_Manger final : public CBase
{

private:
	CWave_Manger();
	virtual ~CWave_Manger() = default;

public:
	virtual void Free() override;

};
END
#define __WAVE_MANAGER_H__
#endif // !__WAVE_MANAGER_H__
