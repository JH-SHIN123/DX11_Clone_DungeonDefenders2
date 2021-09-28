#pragma once

#ifndef __ARCHER_H__

#include "Monster.h"

BEGIN(Client)
enum class EArcherAnim
{
	Attack = 0,
	Cough = 90,
	Death = 135,
	Hurt = 195,
	Idle = 224,
	Jump = 269,
	Move_Backward = 286,
	Move_Forward = 334,
	Move_Left = 382,
	Move_Right = 430,
	Shock = 478,
	Turn_Left = 482,
	End_Anim = 530,
	End = -1
};

class CArcher final : public CMonster
{
private:
	explicit CArcher(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CArcher(const CArcher& rhs);
	virtual ~CArcher() = default;

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
	_float		Anim_Changer(EArcherAnim eAnim);
	void		AI_Check();


private:
	EArcherAnim	m_eAnim_Cur = EArcherAnim::End;
	EArcherAnim	m_eAnim_Next = EArcherAnim::Attack;

private:
	CCollider*				m_pColliderCom_Attack = nullptr;
	CCollider*				m_pColliderCom_Hurt = nullptr;
	_bool					m_IsHurt = false;

private:
	_bool		m_IsAttack = false;
	_bool		m_IsPickArrow = false;
	_bool		m_IsBowPulling = false;


public:
	static CArcher* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __ARCHER_H__
#endif // !__ARCHER_H__
