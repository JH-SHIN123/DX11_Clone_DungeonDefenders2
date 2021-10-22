#pragma once

#ifndef __RING_EXPLOSION_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagRing_Desc
{
	_float3 vScale;
	_float fLifeTime = 0.f;
	MOVESTATE_DESC Move_Desc;
	_tchar szModelName[MAX_PATH] = L"";

}RING_EXPLOSION_DESC;

class CRing_Explosion final : public CGameObject
{
public:
	explicit CRing_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CRing_Explosion(const CRing_Explosion& rhs);
	virtual ~CRing_Explosion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component(void* pArg);

private:
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;
	CModel*					m_pModelCom		= nullptr;

private:
	RING_EXPLOSION_DESC m_Desc;

public:
	static CRing_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __RING_EXPLOSION_H__
#endif // !__RING_EXPLOSION_H__
