#pragma once

#ifndef __BOSS_TRAPBALL_H__

#include "Bullet.h"

BEGIN(Client)

class CBoss_TrapBall final : public CBullet
{
private:
	explicit CBoss_TrapBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoss_TrapBall(const CBoss_TrapBall& rhs);
	virtual ~CBoss_TrapBall() = default;

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
	static CBoss_TrapBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOSS_TRAPBALL_H__
#endif // __BOSS_TRAPBALL_H__

