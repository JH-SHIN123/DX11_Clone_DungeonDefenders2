#pragma once

#ifndef __BASE_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CComponent abstract : public CBase
{
public:
	CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CComponent(const CComponent& rhs);
	virtual ~CComponent() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();
	virtual HRESULT NativeConstruct(void* pArg);

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() PURE;

};

END

#define __BASE_H__
#endif // !__BASE_H__
