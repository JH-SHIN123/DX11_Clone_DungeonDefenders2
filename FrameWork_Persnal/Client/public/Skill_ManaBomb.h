#pragma once

#ifndef __SKILL_MANABOMB_H__

#include "Bullet.h"

BEGIN(Client)
class CSkill_ManaBomb final : public CBullet
{
private:
	explicit CSkill_ManaBomb(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CSkill_ManaBomb(const CSkill_ManaBomb& rhs);
	virtual ~CSkill_ManaBomb() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

private:
	_bool	m_IsFall = false;
	_float	m_fCastTime = 0.5f;

public:
	static CSkill_ManaBomb* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __SKILL_MANABOMB_H__
#endif // !__SKILL_MANABOMB_H__
