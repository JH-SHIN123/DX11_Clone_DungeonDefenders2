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

private:
	CTextures*			m_pTextureCom_AlphaBlack = nullptr;
	CTransform*			m_pTransformCom_AlphaBlack = nullptr;
	_int				m_iButtonCount = 3;

private: //Button 3
	vector<class CMyButton_NoText*>		m_pButton;

public:
	static CPauseMenu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __PAUSEMENU_H__
#endif // !__PAUSEMENU_H__
