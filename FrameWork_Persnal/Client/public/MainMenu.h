#pragma once
#ifndef __MAINMENU_H__

#include "UI_2D.h"

BEGIN(Client)

enum class EButtonSelect
{Stage, Option, HighScore, Exit, End};

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
	void	Button_Position_Check(_float TimeDelta);
	void	Render_Button_Info(_int iIndex);

private:
	_bool		m_IsClick				= false;
	_bool		m_IsMenu				= false;
	_bool		m_IsMove				= false;
	_bool		m_IsStage_Select		= false;
	_bool		m_IsNextScene			= false;
	_float		m_fAlphaTime = 0.f;

private:
	_bool			m_IsButtonPick[4]		= { false };
	RECT			m_tButtonRect[4];
	_float			m_fButtonScaleTime[4]	= { 0.f };
	EButtonSelect	m_eButtonSelect			= EButtonSelect::End;

private:
	CMovement*	m_pMovementCom_Board = nullptr;
	CMovement*	m_pMovementCom_UI[7]	= { nullptr };
	CMovement*	m_pMovementCom_UI_Info[7] = { nullptr };
	CTextures*	m_pTextureCom_UI		= nullptr;

public:
	static CMainMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __MAINMENU_H__
#endif // !__MAINMENU_H__