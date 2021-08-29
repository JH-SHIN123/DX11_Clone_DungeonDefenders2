#pragma once
#ifndef __MAINMENU_H__

#include "UI_2D.h"

BEGIN(Client)
class CMainMenu final: public CUI_2D
{
private:
	explicit CMainMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMainMenu(const CMainMenu& rhs);
	virtual ~CMainMenu() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component(void* pArg);
	void	Key_Check();
	void	Button_Position_Check();

private:
	_bool		m_IsClick				= false;
	_bool		m_IsMenu				= false;
	_bool		m_IsMove = false;
	_bool		m_IsButtonPick[3] = { false };
	_bool		m_IsNextScene = false;
	RECT		m_tButtonRect[3];
	_float		m_fAlphaTime = 0.f;
	_float4x4	m_Mat;

	CMovement*	m_pMovementCom_UI[7]	= { nullptr };
	CTextures*	m_pTextureCom_UI		= nullptr;

public:
	static CMainMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __MAINMENU_H__
#endif // !__MAINMENU_H__