#pragma once

#ifndef __GOBLIN_H__

#include "Monster.h"

BEGIN(Client)

enum class EGoblinAnim
{
	Attack_1 = 0,
	Attack_2 = 40,
	Cough = 80,
	Death = 124,
	Hurt = 174,
	Idle = 189,
	Jump = 233,
	Move_Forward = 248,
	Shock = 289,
	Turn_Left = 293,
	End_Anim = 334,
	End = -1
};

class CGoblin final : public CMonster
{
private:
	explicit CGoblin(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CGoblin(const CGoblin& rhs);
	virtual ~CGoblin() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Anim_Check(_float TimeDelta);
	void Attack_Check();

private:
	HRESULT		Ready_Component(void* pArg);
	_float		Anim_Changer(EGoblinAnim eAnim);
	void		AI_Check();


private:
	EGoblinAnim	m_eAnim_Cur			= EGoblinAnim::End;
	EGoblinAnim	m_eAnim_Next		= EGoblinAnim::Idle;

private:
	_int	m_iAttackCount = 0;
	_bool	m_IsSecondAttack = false;
	_bool	m_IsAttack = false;

private:
	CCollider*				m_pColliderCom_Attack = nullptr;
	CCollider*				m_pColliderCom_Hurt = nullptr;
	_bool					m_IsHurt = false;


public:
	static CGoblin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __GOBLIN_H__
#endif // !__GOBLIN_H__
