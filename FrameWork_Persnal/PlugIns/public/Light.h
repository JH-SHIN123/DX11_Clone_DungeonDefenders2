#pragma once

#ifndef __LIGHT_H__

#include "Base.h"

BEGIN(Engine)
class CLight final : public CBase
{
private:
	explicit CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLight() = default;

public:
	LIGHT_DESC* Get_LightDesc() {return &m_LightDesc;}

public:
	HRESULT Native_Construct(const LIGHT_DESC& LightDesc);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

private:
	LIGHT_DESC				m_LightDesc;
	_bool					m_isActive = true;

public:
	static CLight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const LIGHT_DESC& LightDesc);
	virtual void Free() override;

};
END
#define __LIGHT_H__
#endif // !__LIGHT_H__
