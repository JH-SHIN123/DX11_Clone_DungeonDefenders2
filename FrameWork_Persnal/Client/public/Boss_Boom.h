#pragma once

#ifndef __BOSS_BOOM_H__

#include "Bullet.h"

BEGIN(Client)
class CBoss_Boom final : public CBullet
{
private:
	explicit CBoss_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoss_Boom(const CBoss_Boom& rhs);
	virtual ~CBoss_Boom() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);



private:
	_float m_fAlphaTime = 2.f;

public:
	static CBoss_Boom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOSS_BOOM_H__
#endif // !__BOSS_BOOM_H__
