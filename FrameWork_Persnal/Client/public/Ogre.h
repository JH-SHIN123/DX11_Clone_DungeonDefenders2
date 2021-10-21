#pragma once

#ifndef __OGRE_H__

#include "Monster.h"

BEGIN(Client)

enum class EOgreAnim
{
	Cough = 0,
	Death = 86,
	Hurt = 199,
	Idle = 285,
	Pound = 371,
	Punch = 422,
	Sweep = 468,
	Turn = 519,
	Walk = 550,
	End_Anim = 611,
	End = -1
};

class COgre final :	public CMonster
{
private:
	explicit COgre(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit COgre(const COgre& rhs);
	virtual ~COgre() = default;

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
	_float		Anim_Changer(EOgreAnim eAnim);
	void		AI_Check();

private:
	EOgreAnim	m_eAnim_Cur = EOgreAnim::End;
	EOgreAnim	m_eAnim_Next = EOgreAnim::Idle;

private:
	_int	m_iAttackCount = 0;
	_bool	m_IsSpecialAttack = false;
	_bool	m_IsAttack = false;

private:
	CCollider*				m_pColliderCom_Push = nullptr;
	CCollider*				m_pColliderCom_Attack = nullptr;
	CCollider*				m_pColliderCom_Hurt = nullptr;
	_bool					m_IsHurt = false;
	//_bool					m_HasCollide_Attack = false;


public:
	static COgre* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __OGRE_H__
#endif // !__OGRE_H__
