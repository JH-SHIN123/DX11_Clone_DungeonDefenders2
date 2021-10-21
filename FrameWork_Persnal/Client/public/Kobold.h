#pragma once

#ifndef __KOBOLD_H__

#include "Monster.h"

BEGIN(Client)

enum class EKoboldAnim
{
	Cough = 0,
	Death = 40,
	Hurt = 63,
	Idle = 81,
	Jump = 140,
	LightFuse = 172,
	Move_Backward = 208,
	Move_Forward = 236,
	Run_Suicide = 264,
	Shock = 288,
	Turn = 292,
	End_Anim = 314,
	End = -1
};

class CKobold final : public CMonster
{
private:
	explicit CKobold(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CKobold(const CKobold& rhs);
	virtual ~CKobold() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Anim_Check(_float TimeDelta);
	void Attack_Check();

	EMonsterAI AI_Check_Kobold(_float TimeDelta, _vector* pTargetPos, _bool IsContinueAnimation);


private:
	void Explosion();

private:
	HRESULT		Ready_Component(void* pArg);
	_float		Anim_Changer(EKoboldAnim eAnim);
	void		AI_Check();


private:
	EKoboldAnim	m_eAnim_Cur = EKoboldAnim::End;
	EKoboldAnim	m_eAnim_Next = EKoboldAnim::Idle;

private:
	_bool	m_IsAttack = false;
	_bool	m_IsFuseOn = false;

private:
	CCollider*				m_pColiiderCom_Push = nullptr;
	CCollider*				m_pColliderCom_Attack = nullptr;
	CCollider*				m_pColliderCom_Hurt = nullptr;
	_bool					m_IsHurt = false;
	_bool					m_IsExplosion = false;

public:
	static CKobold* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __KOBOLD_H__
#endif // !__KOBOLD_H__
