#pragma once

#ifndef __LIGHTNINGTOWER_BULLET_H__

#include "Bullet.h"

BEGIN(Client)
class CLightningTower_Bullet final : public CBullet
{
private:
	explicit CLightningTower_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CLightningTower_Bullet(const CLightningTower_Bullet& rhs);
	virtual ~CLightningTower_Bullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT		Ready_Component(void* pArg);
	void		Spawn_Check(_float TimeDelta);
	void		Create_Effect();

private:
	_int		m_iBoundCount = 3;
	_bool		m_IsMoveable = false;
	_float		m_fScaleTime = 0.f;
	_float3		m_vScale_SizeUp = { 0.05f ,0.05f ,0.05f };

private:
	vector<class CLightningTower_Bullet_Effect*>	m_vecEffectMesh = { nullptr };


public:
	static CLightningTower_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __LIGHTNINGTOWER_BULLET_H__
#endif // !__LIGHTNINGTOWER_BULLET_H__
