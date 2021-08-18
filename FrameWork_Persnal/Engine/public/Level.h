#pragma once

#ifndef __LEVEL_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLevel abstract : public CBase
{
protected:
	CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel() = default;
public:
	virtual HRESULT NativeConstruct();
	virtual int Tick(_double Timedelta);
	virtual HRESULT Render();
protected:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDevice_Context = nullptr;
public:
	virtual void Free();
};

END
#define __LEVEL_H__
#endif // !__LEVEL_H__
