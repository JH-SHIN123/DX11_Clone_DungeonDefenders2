#pragma once

#ifndef __LIGHT_MANAGER_H__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class CLight_Manager final : public CBase
{
	DECLARE_SINGLETON(CLight_Manager)

private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	HRESULT Reserve_LightManager(_int iNumLight);
	HRESULT Add_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const LIGHT_DESC& LightDesc, _bool isActive = true);
	const LIGHT_DESC& Get_LightDesc(_int iLightIndex) const;
	/* 빛 연산한 결과를 쉐이드타겟에 그리기위한 함수. */
	HRESULT Render_Lights();


private:
	vector<class CLight*>				m_Lights;
	typedef vector<class CLight*>		LIGHTS;



public:
	virtual void Free() override;

};

END
#define __LIGHT_MANAGER_H__
#endif // !__LIGHT_MANAGER_H__
