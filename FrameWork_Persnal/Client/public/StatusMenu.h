#pragma once

#ifndef __STATUSMENU_H__

#include "UI_2D.h"

BEGIN(Client)

class CMyButton_NoText;
class CMasking_MeterBar;
class CMasking_UI;

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

private:
	HRESULT	Ready_Component();
	void	Button_Render();
	void	ButtonFrame_Render();

private: // Exp
	CMasking_MeterBar*		m_pExpBar = nullptr;

private: // Portrait
	CMasking_UI*		m_pPlayerPortrait = nullptr;

private: // HpMp
	vector<class CMyButton_NoText*>		m_pButton_HpMp;
	vector<CTransform*>					m_pTransform_HpMpFrame;
	_int								m_iButtonCount_HpMp = 2;

private:


private: // Exit
	CMyButton_NoText*		m_pButton_Exit = nullptr;
	_bool					m_IsExit = false;

public:
	static CStatusMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __STATUSMENU_H__
#endif // !__STATUSMENU_H__
