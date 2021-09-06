#pragma once

#ifndef __MYBUTTON_H__

#include "UI_2D.h"
#include "MyText.h"

BEGIN(Client)

typedef struct tagButtonDesc
{
	TEXT_DESC Text_Desc;
	UI2D_DESC UI_Desc;
}BUTTON_DESC;


class CMyButton final : public CUI_2D
{
private:
	explicit CMyButton(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMyButton(const CMyButton& rhs);
	virtual ~CMyButton() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(void* pArg);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	Get_IsPick();

private:
	HRESULT	Ready_Component();
	void	Button_Picking_Check();

private:
	class CMyText*	m_pText = nullptr;
	RECT			m_tPickingBox;
	_bool			m_IsPick = false;

public:
	static CMyButton* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void* pArg);
	virtual CGameObject* Clone_GameObject(void* pArg) override; // pArg = BUTTON_DESC
	virtual void Free();
};

END
#define __MYBUTTON_H__
#endif // !__MYBUTTON_H__
