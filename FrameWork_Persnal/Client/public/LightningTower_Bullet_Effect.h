#pragma once

#ifndef __CLIGHTNINGTOWER_BULLET_EFFECT__

#include "Effect.h"

BEGIN(Client)

class CLightningTower_Bullet_Effect final :	public CEffect
{
protected:
	explicit CLightningTower_Bullet_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context);
	explicit CLightningTower_Bullet_Effect(const CLightningTower_Bullet_Effect& rhs);
	virtual ~CLightningTower_Bullet_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Ready_Component(void* pArg);

private:
	_float	m_fTimeColor = 0.f;

public:
	static  CLightningTower_Bullet_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __CLIGHTNINGTOWER_BULLET_EFFECT__
#endif // !__CLIGHTNINGTOWER_BULLET_EFFECT__
