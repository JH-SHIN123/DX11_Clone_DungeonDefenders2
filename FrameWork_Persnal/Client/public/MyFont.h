#pragma once

#ifndef __MYFONT_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CVIBuffer_Font;
class CTextures;
class CMovement;
END

BEGIN(Client) // 왼쪽부터 시작하자

typedef struct tagFontDesc
{
	_int	iFontID;
	_float4 vPos;
	_float4 vScale;
	_float2 TextUV_Left;
	_float2 TextUV_Right;
	CVIBuffer_Font* VIBufferCom = nullptr;
}FONT_DESC;

class CMyFont final : public CGameObject
{
private:
	explicit CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMyFont(const CMyFont& rhs);
	virtual ~CMyFont() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Font*			m_pVIBufferCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	FONT_DESC				m_Font;

private:
	HRESULT	Ready_Component(void* pArg);

public:
	static CMyFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void * pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#define __MYFONT_H__
#endif // !__MYFONT_H__
