#pragma once

#ifndef __STATUSPANEL_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Rect;
class CTextures;
class CTransform;
END

BEGIN(Client)

class CStatus_Panel final : public CGameObject
{
private:
	explicit CStatus_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStatus_Panel(const CStatus_Panel& rhs);
	virtual ~CStatus_Panel() = default;
public:
	/* 원형객체를 초기화하낟. */
	virtual HRESULT NativeConstruct_Prototype() override;

	/* 복제된 객체를 초기화하낟. */
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Rect*			m_pVIBufferCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	HRESULT	Ready_Component();

public:
	static CStatus_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END

#define  __STATUSPANEL_H__
#endif // !__STATUSPANEL_H__
