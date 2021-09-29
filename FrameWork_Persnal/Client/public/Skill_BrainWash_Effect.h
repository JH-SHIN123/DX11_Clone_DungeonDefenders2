#pragma once

#ifndef __SKILL_BRAINWASH_EFFECT_H__

#include "Bullet.h"

BEGIN(Client)
class CSkill_BrainWash_Effect final : public CBullet
{
private:
	explicit CSkill_BrainWash_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CSkill_BrainWash_Effect(const CSkill_BrainWash_Effect& rhs);
	virtual ~CSkill_BrainWash_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);



public:
	static CSkill_BrainWash_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __SKILL_BRAINWASH_EFFECT_H__
#endif // !__SKILL_BRAINWASH_EFFECT_H__
