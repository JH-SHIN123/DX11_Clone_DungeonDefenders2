#pragma once

#ifndef __STATUSMENU_H__

#include "UI_2D.h"

BEGIN(Client)
class CStatusMenu final : public CUI_2D
{
private:
	explicit CStatusMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStatusMenu(const CStatusMenu& rhs);
	virtual ~CStatusMenu() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_IsCreateToPause(_bool IsCreateToPause) { m_IsCreateToPause = IsCreateToPause; }

private:
	HRESULT	Ready_Component();

private:
	CTextures*			m_pTextureCom_AlphaBlack = nullptr;
	_float4x4			m_matAlphaBlack;
	_bool				m_IsCreateToPause = false;

private: //Button 4
	vector<class CMyButton_NoText*>		m_pButton;
	_int				m_iButtonCount = 4;

public:
	static CStatusMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __STATUSMENU_H__
#endif // !__STATUSMENU_H__
