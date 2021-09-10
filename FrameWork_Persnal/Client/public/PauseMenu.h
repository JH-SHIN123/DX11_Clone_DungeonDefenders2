#pragma once

#ifndef __PAUSEMENU_H__

#include "UI_2D.h"

BEGIN(Client)
class CPauseMenu final : public CUI_2D
{
private:
	explicit CPauseMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPauseMenu(const CPauseMenu& rhs);
	virtual ~CPauseMenu() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component();
	void Button_Check();

private:
	CTextures*			m_pTextureCom_AlphaBlack = nullptr;
	_float4x4			m_matAlphaBlack;

private: //스탯, 계속, 다시하기, 메인메뉴
	vector<class CMyButton_NoText*>		m_vecButton;
	_int				m_iButtonCount = 4;
	_bool				m_IsDestroyOption_UI = false;
private:
	_bool				m_IsSceneChange = false;

public:
	static CPauseMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __PAUSEMENU_H__
#endif // !__PAUSEMENU_H__
