#pragma once

#ifndef __MYFONT_NOTEXT_H__
#include "UI_2D.h"

BEGIN(Client)

class CMyButton_NoText :	public CUI_2D
{
private:
	explicit CMyButton_NoText(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMyButton_NoText(const CMyButton_NoText& rhs);
	virtual ~CMyButton_NoText() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render(_uint iTextureIndex = 0, _uint ShaderPass = 1);

public:
	_bool	Get_IsClick();
	_bool	Get_IsClick_R();
	_bool	Get_IsPick();
	void	Get_Button_Pos(_float4* pPos) {	XMStoreFloat4(pPos, m_pMovementCom->Get_State(EState::Position)); }

public:
	void Set_Pos(_fvector vPos) { m_pMovementCom->Set_State(EState::Position, vPos); }

private:
	HRESULT	Ready_Component();
	void	Button_Picking_Check();

private:
	RECT			m_tPickingBox;
	_bool			m_IsPick = false;
	_bool			m_IsClicked = false;
	_bool			m_IsClicked_R = false;

public:
	static CMyButton_NoText* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void* pArg); // pArg = BUTTON_DESC
	virtual CGameObject* Clone_GameObject(void* pArg) override; 
	virtual void Free();
};

END
#define __MYFONT_NOTEXT_H__
#endif // !__MYFONT_NOTEXT_H__
