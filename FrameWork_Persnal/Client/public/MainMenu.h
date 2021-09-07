#pragma once
#ifndef __MAINMENU_H__

#include "UI_2D.h"

BEGIN(Client)

typedef struct tagStagePreViewDesc
{
	_int iStage		= 0;
	_int iDifficult = 0;
	_int iClass		= 0;
}STAGE_PREVIEW_DESC;

enum class EButtonSelect
{Stage, Option, HighScore, Exit, End};

class CMyButton;
class CMyButton_NoText;
class CMasking_MeterBar;

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
	HRESULT Ready_Button_Stage();
	HRESULT Ready_Option();

private:
	void	Key_Check();
	void	Button_Position_Check(_float TimeDelta);
	void	Button_Select_Check(_float TimeDelta);
	void	Button_Stage_Check(_float TimeDelta);
	void	Button_Option_Check(_float TimeDelta);

	void	Render_Button();
	void	Render_Button_Info(_int iIndex);
	void	Render_Button_Select(EButtonSelect eSelect);
	void	Render_Button_Stage();
	void	Render_Button_Option();

private:
	_bool		m_IsClick				= false;
	_bool		m_IsMenu				= false;
	_bool		m_IsMove				= false;
	_bool		m_IsStage_Select		= false;
	_bool		m_IsNextScene			= false;
	_float		m_fAlphaTime			= 0.f;

private:
	_bool			m_IsButtonPick[4]		= { false };
	RECT			m_tButtonRect[4];
	_float			m_fButtonScaleTime[4]	= { 0.f };
	EButtonSelect	m_eButtonSelect			= EButtonSelect::End;

private:
	_bool			m_IsButton_Select[4] = { false };
	RECT			m_tButton_Select[4];

private:
	CMovement*	m_pMovementCom_Board			= nullptr;		//백보드
	CMovement*	m_pMovementCom_UI[4]			= { nullptr };	// UI 기본 버튼
	CMovement*	m_pMovementCom_UI_Info[4]		= { nullptr };	// 설명
	CMovement*	m_pMovementCom_UI_Select[4]		= { nullptr };	// 0 뒤로 1 스테이지 진입
	CTextures*	m_pTextureCom_UI				= nullptr;
	CMyButton*	m_pButton_Start_Stage			= nullptr;	// 스테이지 진입
	CMyButton*	m_pButton_Back					= nullptr;	// 뒤로가기 폰트

private:
	CTextures*	m_pTextureCom_UI_Stage_Button = nullptr;
	CMovement*	m_pMovementCom_UI_Stage_Button[4] = { nullptr };

	CTextures*	m_pTextureCom_UI_Stage_PreView[2] = { nullptr }; // 왼쪽에 띄울 스테이지 예시
	CMovement*	m_pMovementCom_UI_Stage_PreView[2] = { nullptr };

	CMyButton*	m_pStage_Button[7] = { nullptr };// 01 Stage 234 diffi
	STAGE_PREVIEW_DESC		m_StageMakeInfo;

private: // 옵션 - 음량 조절
	CMasking_MeterBar*		m_pOption_BGM = nullptr;
	CMasking_MeterBar*		m_pOption_SFX = nullptr;
	CMyButton_NoText*		m_pButton_Option_BGM[2] = { nullptr };
	CMyButton_NoText*		m_pButton_Option_SFX[2] = { nullptr };
	_float2					m_vSound;



public:
	static CMainMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __MAINMENU_H__
#endif // !__MAINMENU_H__