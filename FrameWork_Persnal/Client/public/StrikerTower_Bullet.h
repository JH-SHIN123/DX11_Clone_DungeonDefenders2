#pragma once

#ifndef __STRIKERTOWER_BULLET_H__

#include "Bullet.h"
class CStrikerTower_Bullet final : public CBullet
{
private:
	explicit CStrikerTower_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStrikerTower_Bullet(const CStrikerTower_Bullet& rhs);
	virtual ~CStrikerTower_Bullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);



public:
	static CStrikerTower_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

#define __STRIKERTOWER_BULLET_H__
#endif // !__STRIKERTOWER_BULLET_H__
