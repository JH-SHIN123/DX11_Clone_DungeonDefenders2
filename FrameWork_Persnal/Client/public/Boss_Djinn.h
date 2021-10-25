#pragma once

#ifndef __BOSS_DJINN_H__

#include "Monster.h"

BEGIN(Client)

enum class EDjinnAnim
{
	Attack_1 = 0,
	Charge = 59,
	Coughing = 118,
	Death = 158,
	Hurt = 248,
	Idle = 266,
	Interrupt = 325,
	Move_Backward = 345,
	Move_Forward = 404,
	Move_Left = 463,
	Move_Right = 522,
	Move_Up = 581,
	Shock = 640,
	Spell_1_Enter = 669,
	Spell_1_Enter_Alt = 716,
	Spell_1_Loop = 764,
	Spell_1_Loop_Alt = 786,
	Spell_1_Start = 847,
	Spell_1_Stop = 895,
	Spell_2_Loop = 932,
	Spell_2_Start = 956,
	Spell_2_Stop = 971,
	Turn_Left = 987,
	Turn_Right = 1046,
	End_Anim = 1105,
	End = -1
};

enum class EDjinn_Attack
{
	Attack,
	EnergyBall,
	WideRange,
	TrapBall,
	RepeatBall,
	End
};

class CBoss_Djinn final : public CMonster
{
private:
	explicit CBoss_Djinn(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoss_Djinn(const CBoss_Djinn& rhs);
	virtual ~CBoss_Djinn() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Anim_Check(_float TimeDelta);
	void Attack_Check(_float TimeDelta);

private:
	HRESULT		Ready_Component(void* pArg);
	_float		Anim_Changer(EDjinnAnim eAnim);
	void		AI_Check();

private:
	void SetUp_GemColor();
	void GemColor_Check(_float TimeDelta);

private:
	void Attack_Default();
	void Attack_EnergyBall();
	void Attack_WideRange();
	void Attack_TrapBall(_float TimeDelta);
	void Attack_RepeatBall(_float TimeDelta);

private:
	_fvector Get_TargetPos();

private:
	_int	m_iAttackCount = 0;
	_bool	m_IsAttack = false;

private:
	_bool					m_IsHurt = false;
	CCollider*				m_pColliderCom_Hurt = nullptr;
	CCollider*				m_pColliderCom_Attack = nullptr;
	CCollider*				m_pColliderCom_LeftHand = nullptr;
	CCollider*				m_pColliderCom_Push = nullptr;

private:
	CTextures*				m_pTextureCom_Specular = nullptr;

private:
	EDjinnAnim		m_eAnim_Cur			= EDjinnAnim::End;
	EDjinnAnim		m_eAnim_Next		= EDjinnAnim::Attack_1;
	EDjinn_Attack	m_eAttack_Value		= EDjinn_Attack::Attack;
	_float4			m_vGemColor[(_uint)EDjinn_Attack::End];
	_float4			m_vGemColor_Now;
	_float			m_fColorChange_Time = 0.f;

	_float			m_fTime_TrapBall = 0.f;
	const _float	m_fTime_TrapBall_Max = 2.f;
	_float			m_fTime_RepeatBall = 0.f;
	const _float	m_fTime_RepeatBall_Max = 1.5f;

	class CStrikerTower_Bullet* m_pTestHand = nullptr;
	_float4x4 m_RightHand_Matrix;
	_float4x4 m_LeftHand_Matrix;

	_float fTimeTest[4] = {0.f,0.f,0.f,0.f};

private:
	_float4 m_vTargetPosition;
	class CRing_Effect* m_pRing_Effect = nullptr;
	class CPoint_Ex_Particle* m_pEffect_RightHand = nullptr;
	class CPoint_Ex_Trail* m_pTrail_RightHand = nullptr;

private:
	_bool m_IsSkillSound[(_uint)EDjinn_Attack::End] = { false };

public:
	static CBoss_Djinn* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOSS_DJINN_H__
#endif //!__BOSS_DJINN_H__

/*

에너지볼 HandL - 노란색
2.f, 1.8f, 0.f
Spell_2_Start
Spell_2_Stop

일반 공격 Joint24 - 회색
1.f, 1.f, 1.f
Attack_1

광역 공격 - 붉은색
1.5f, 0.2f, 0.2f
Spell_1_Start
Spell_1_Stop

설치형 에너지볼(지연 폭발) HandL - 초록색
0.8f, 2.f, 0.8f
Spell_2_Start
Spell_2_Loop
Spell_2_Stop

연속 에너지볼 - 보라색
0.8f, 0.f, 1.7f
Spell_1_Enter_Alt
Spell_1_Loop_Alt
Spell_1_Stop


*/