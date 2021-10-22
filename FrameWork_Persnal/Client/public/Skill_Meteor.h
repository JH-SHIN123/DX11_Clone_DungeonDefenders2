#pragma once

#ifndef __SKILL_METEOR_H__

#include "Bullet.h"

BEGIN(Client)
class CSkill_Meteor final : public CBullet
{
private:
	explicit CSkill_Meteor(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CSkill_Meteor(const CSkill_Meteor& rhs);
	virtual ~CSkill_Meteor() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

public:
	void Create_Explosion();

private:
	_bool	m_IsFall = false;
	_float	m_fCastTime = 0.5f;

private:
	CNavigation* m_pNaviCom = nullptr;
	class CPoint_Ex_Trail* m_pPoint_Trail = nullptr;

public:
	static CSkill_Meteor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __SKILL_METEOR_H__
#endif // !__SKILL_METEOR_H__
