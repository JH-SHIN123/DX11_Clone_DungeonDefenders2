#pragma once

#ifndef __METEOR_EXPLOSION_H__

#include "Bullet.h"

BEGIN(Client)
class CMeteor_Explosion final :	public CBullet
{
private:
	explicit CMeteor_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMeteor_Explosion(const CMeteor_Explosion& rhs);
	virtual ~CMeteor_Explosion() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

private: // ºÒ±âµÕ –í–í
	void Create_FirePillar();

private:
	_float m_fAttTime = 0.5f;
	_int m_iFireCount = 4;

public:
	static CMeteor_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __METEOR_EXPLOSION_H__
#endif // !__METEOR_EXPLOSION_H__
