#pragma once

#ifndef __LOADINGSCREEN_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CLoadingScreen final : public CGameObject
{
protected:
	explicit CLoadingScreen(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CLoadingScreen(const CLoadingScreen& rhs);
	virtual ~CLoadingScreen() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component(void* pArg);

private:
	CRenderer*				m_pRendererCom			= nullptr;
	CMovement*				m_pMovementCom			= nullptr;
	CTextures*				m_pTexturesCom			= nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom			= nullptr;
	CMovement*				m_pMovementCom_Green	= nullptr;
	CMovement*				m_pMovementCom_Blue		= nullptr;
	CMovement*				m_pMovementCom_Red		= nullptr;

private:
	_float m_fTime = 0.f;

public:
	static CLoadingScreen* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __LOADINGSCREEN_H__
#endif // !__LOADINGSCREEN_H__
