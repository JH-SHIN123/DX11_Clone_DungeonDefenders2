#pragma once

#ifndef __CACULATOR_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CCalculator final
{
private:
	CCalculator() = default;
	~CCalculator() = default;

public:
	static _vector Convert_LayToAim(_fvector vPos = XMVectorSet(0.f,0.f,0.f,1.f));

	static void Convert_MouseRay(_fvector vPos, _vector* vOutDir);

};

END
#define __CACULATOR_H__
#endif // !__CACULATOR_H__
