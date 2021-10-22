#pragma once

#ifndef __METEOR_EXPLOSION_EFFECT_2_H__

#include "Bullet.h"

BEGIN(Client)
class CMeteor_Explosion_Effect_2 final : public CBullet
{
private:
	explicit CMeteor_Explosion_Effect_2(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMeteor_Explosion_Effect_2(const CMeteor_Explosion_Effect_2& rhs);
	virtual ~CMeteor_Explosion_Effect_2() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

private: // 불기둥 뽱뽱


private: // 만들어지고 바로 사라져라
	_float m_fAttTime = 0.f;
	_float m_fAlphaTime = 2.f;

public:
	static CMeteor_Explosion_Effect_2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __METEOR_EXPLOSION_EFFECT_2_H__
#endif // !__METEOR_EXPLOSION_EFFECT_2_H__
