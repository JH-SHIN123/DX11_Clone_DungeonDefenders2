#pragma once

#ifndef __LIGHT_MANAGER_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)

public:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLight);
	HRESULT	Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const LIGHT_DESC& Light_Desc);

public: // Getter
	LIGHT_DESC* Get_LightDesc(_int iLightIndex);


private:
	typedef vector<class CLight*> LIGHTS;
	LIGHTS m_Lights;

public:
	virtual void Free() override;
};

END
#define __LIGHT_MANAGER_H__
#endif // !__LIGHT_MANAGER_H__
