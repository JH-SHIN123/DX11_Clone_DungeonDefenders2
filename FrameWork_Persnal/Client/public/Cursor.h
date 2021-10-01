#pragma once

// For Render Mouse Cursor Image

#ifndef __CURSOR_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)
enum class EMouseMode
{Default, Reverse_X, Reverse_Y, Reverse_XY ,End};

enum class EMouseTexture
{Default, Build, Repair, TPS, End};

class CCursor final : public CGameObject
{
private:
	explicit CCursor(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CCursor(const CCursor& rhs);
	virtual ~CCursor() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* TextureName);
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Getter
	POINT	Get_Mouse() const { return m_tMouse; }
	_bool	Get_IsShowCursor() { return m_IsShowCursor; }

public: // Setter
	void	Set_MouseMode(EMouseMode eMode) { m_eMouseMode = eMode; }
	void	Set_MouseTexture(EMouseTexture eMode) { m_eMouseTexture = eMode; }
	void	Set_ShowCursor(_bool IsShowCursor);

public:
	void	Tracking_Mouse();
	HRESULT	Ready_Component(const _tchar* TextureName);

private:
	CRenderer*				m_pRendererCom	= nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom	= nullptr;
	CTextures*				m_pTextureCom	= nullptr;
	CTransform*				m_pTransformCom = nullptr;

	CTextures*				m_pTextureCom_TPSAim	= nullptr;
	CTransform*				m_pTransformCom_TPSAim	= nullptr;

private:
	POINT			m_tMouse;
	_int			m_iShowCount	= 0;
	_bool			m_IsShowCursor;
	EMouseMode		m_eMouseMode	= EMouseMode::Default;
	EMouseTexture	m_eMouseTexture = EMouseTexture::Default;

public:
	static CCursor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* TextureName);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};
END
#define __CURSOR_H__
#endif // !__CURSOR_H__
