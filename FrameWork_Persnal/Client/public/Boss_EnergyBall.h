#pragma once

#ifndef __BOSS_ENERGYBALL_H__

#include "Bullet.h"

BEGIN(Client)
class CBoss_EnergyBall final : public CBullet
{
private:
	explicit CBoss_EnergyBall(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoss_EnergyBall(const CBoss_EnergyBall& rhs);
	virtual ~CBoss_EnergyBall() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);

private:
	void Create_Effect();

	_float m_fAlphaTime = 2.f;
	class CPoint_Ex_Trail* m_pPoint_Trail = nullptr;

public:
	static CBoss_EnergyBall* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOSS_ENERGYBALL_H__
#endif // __BOSS_ENERGYBALL_H__

