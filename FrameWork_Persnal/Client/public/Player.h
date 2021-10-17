#pragma once

#ifndef __PLAYER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

enum class EPlayerAnimation
{
	CallOut = 0,
	ChargeMax = 38,
	ChargeMax_KnockBack = 59,
	ChargeMin = 80,
	ChargeMin_KnockBack = 101,
	Death = 122,
	Detonate = 182,
	Fire = 212,
	FireMaxPower = 232,
	Heal = 262,
	Hurt = 282,
	Idle = 293,
	Idle_lowHp = 412,
	Jump = 472,
	Jump_Falling = 492,
	KnockBack = 511,
	LevelUp = 556,
	Lose = 646,
	ManaBomb = 708,
	Move_Backward = 782,
	Move_Left = 798,
	Move_Right = 814,
	PickupItem = 830,
	Repair = 847,
	RunForward = 876,
	Spawn = 892,
	Summon = 948,
	Summon_Place = 977,
	Summon_Start = 1022,
	Summon_Stop = 1037,
	Turn_Left = 1063,
	Upgrade = 1088,
	Wave_Start = 1109,
	Win = 1139,
	WinWave = 1216,
	EndKey = 1291,
	End = -1
};

enum class ETowerSpawn
{ Locate, Rotate, Spawn, End };

typedef struct tagGameObject_Desc
{
	STATUS_DESC		Status_Desc;
	MOVESTATE_DESC	Movement_Desc;

	wchar_t			szModelName[MAX_PATH] = L"";
	ELevel			eModelLevel = ELevel::End;
}GAMEOBJ_DESC;

class CPlayer final : public CGameObject
{
private:
	enum RENDER_TOWER
	{
		RENDER_BLOCAKE, RENDER_STRIKER, RENDER_LIGHTING, RENDER_END
	};
protected:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	_bool	Get_Skill_Using(_int iSkillIndex);

public:
	void Set_Mission_Failed(_bool IsFailed) { m_IsMissionFailed = IsFailed; }

//public:
//	void Set_Casting_BrainWash_MaxTime(const _float& fTime) { m_fBrainWash_Time_Max = fTime; }

private: // Tick
	void	Key_Check(_float TimeDelta);
	void	Turn_Check(_float TimeDelta);
	void	Skill_Check(_float TimeDelta);

private:
	void	Tower_Pick();

private: // LateTick
	void	Level_Check();
	void	Animation_Check(_float TimeDelta);
	_float	Animation_Term(EPlayerAnimation eNextAnimation);

private: 
	//void Skill_ManaBomb
	void Skill_ManaBomb();
	void Skill_Meteor();
	void Skill_BrainWash();
	void Skill_Healing(_float TimeDelta);

private:
	void Tower_Upgrade();
	void Tower_Healing();

private:
	void SpecialAnimation_Check(_float TimeDelta);
	void Casting_Move_Check(_float* TimeDelta);

private:
	class CWeapon* m_pWeapon = nullptr;

private:
	CModel*				m_pModelCom		= nullptr;
	CStatus*			m_pStatusCom	= nullptr;
	CMovement*			m_pMovementCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;
	CCollider*			m_pColliderCom_Hit = nullptr;
	CNavigation*		m_pNaviCom = nullptr;

private:
	_bool			m_IsSkill_Use[10] = { false };
	_bool			m_IsAttack = false;
	_float			m_fChargeSkill = 0.f;
	_bool			m_IsCharging = false;

private:
	_bool m_IsTowerUpgrade = false;
	_bool m_IsTowerHealing = false;
	_bool m_IsTowerClicked = false;

private: // Tower
	class CBlockadeTower*		m_pBlockadeTower			= nullptr;
	class CStrikerTower*		m_pStrikerTower				= nullptr;
	class CLightningTower*		m_pLightningTower			= nullptr;
	_bool						m_IsRenderTower[RENDER_END]	= { false };
	ETowerSpawn					m_eTowerSpawn				= ETowerSpawn::End;
	_bool						m_IsTowerSpawning			= false;
	MOVESTATE_DESC				m_CreateTower_Desc;
	POINT						m_tTowerPickPoint;

private:
	EPlayerAnimation		m_eAnimationState_Cur			= EPlayerAnimation::End;
	EPlayerAnimation		m_eAnimationState_Next			= EPlayerAnimation::Idle;
	EPlayerAnimation		m_eAnimationState_Cur_Second	= EPlayerAnimation::End;
	EPlayerAnimation		m_eAnimationState_Next_Second	= EPlayerAnimation::End;

	_bool					m_IsSecondAnimation = false;

private: // Skill
	_bool	m_IsCast_PowerUp		= false;
	_bool	m_IsCast_ManaBomb		= false;
	_bool	m_IsCast_Meteor			= false;
	_bool	m_IsCast_BrainWash		= false;
	_bool	m_IsCast_BrainWash_End	= false;
	_bool	m_IsCast_Healing		= false;
	_bool	m_IsCasting_Move		= false;

	_bool	m_IsZoom = false;

	_float	m_fHealTime = 0.f;
	_int	m_iHealSize = 0;

private:
	_bool m_IsMissionFailed = false;

private:
	HRESULT	Ready_Component(void* pArg);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __PLAYER_H__
#endif // !__PLAYER_H__

