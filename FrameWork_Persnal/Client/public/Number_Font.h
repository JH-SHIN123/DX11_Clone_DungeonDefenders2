#pragma once

#ifndef __NUMBER_FONT_H__

#include "UI_2D.h"

BEGIN(Client)

typedef struct tagNumberFontDesc
{
	UI2D_DESC UI_Desc;
	_int*	pNumberBuffer = nullptr;
	_int	iBufferSize = 0;

}NUMBERFONT_DESC;

class CNumber_Font final : public CUI_2D
{
private:
	explicit CNumber_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CNumber_Font(const CNumber_Font& rhs);
	virtual ~CNumber_Font() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pos(_fvector vPos) { m_pMovementCom->Set_State(EState::Position, vPos); }
	void Set_Number(_int* pNumberBuffer);
	void Set_Number(char pNumber[MAX_PATH]);

private:
	HRESULT	Ready_Component(NUMBERFONT_DESC* pArg);
	void	Move_Check(_float TimeDelta);



private:
	_int*		m_pNumberBuffer = nullptr;
	_int		m_iBufferSize = 0;
	_float2		m_vOffSetPos;

public:
	static CNumber_Font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __NUMBER_FONT_H__
#endif // !__NUMBER_FONT_H__
