#pragma once

#ifndef __UI2D_HIT_EFFECT_H__
#include "UI_2D.h"

BEGIN(Client)
class CUI_Hit_Effect final : public CUI_2D
{
protected:
	explicit CUI_Hit_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_Hit_Effect(const CUI_Hit_Effect& rhs);
	virtual ~CUI_Hit_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);				
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	_bool  m_IsAlphaUp = true;
	_float m_fAlphaTime = 0.3f;
	_float2 m_vScale = { 1280.f, 720.f };

public:
	static CUI_Hit_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __UI2D_HIT_EFFECT_H__
#endif
