#pragma once

#ifndef __SKILL_BRAINWASH_H__

#include "Bullet.h"
BEGIN(Client)
class CSkill_BrainWash final : public CBullet
{
private:
	explicit CSkill_BrainWash(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CSkill_BrainWash(const CSkill_BrainWash& rhs);
	virtual ~CSkill_BrainWash() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);

	class CPoint_Ex_Trail* m_pPointTrail = nullptr;

public:
	static CSkill_BrainWash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __SKILL_BRAINWASH_H__
#endif // !__SKILL_BRAINWASH_H__
