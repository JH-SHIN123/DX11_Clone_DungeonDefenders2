#pragma once

#ifndef __MONSTER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMasking_MeterBar_3D;

enum class EMonsterAI
{
	Idle, Attack, Hurt, Dead, Shock,Cough, Move_Cell, Move_Target, Turn, End
};

enum class EMonster_MovePath
{
	North_L, North_R, West_L, West_R, East_L, East_R, End
};

typedef struct tagMonsterDesc
{
	_float fDetectDis = 30.f;
	_float fAttackDis = 10.f;
	STATUS_DESC Stat_Desc;
	
	MOVESTATE_DESC Movement_Desc;
	EMonster_MovePath eMovePath = EMonster_MovePath::North_L;

	_tchar  szModelName[MAX_PATH] = L"";
	ELevel	eLevel = ELevel::Static;
	
}MONSTER_DESC;

class CMonster abstract : public CGameObject
{
protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Create_Hit_Particle(_float fOffSetY);

public:
	_float Get_HpRatio() const;
	_float Get_DisToCell(const _int& iCellIndex);

public:
	void Set_IsBrainWash(_bool IsBrainWash);
	void Set_IsBrainWash_Complete(_bool IsBrainWash);
	void Set_Poison(_bool IsPoison);


protected:
	EMonsterAI AI_Check(_float TimeDelta, _vector* pTargetPos, _bool IsContinueAnimation);
	EMonsterAI AI_BrainWashed(_float TimeDelta, _vector* pTargetPos, _bool IsContinueAnimation);

protected:
	void Set_HpBar_OffSet_Position(_float3 vOffSet_Pos) { m_vHpBar_OffSet_Position = vOffSet_Pos; }
	EMonster_MovePath Research_MovePath(const _int& iCellIndex);

private:
	HRESULT	Ready_Component(void* pArg);

protected:
	CMasking_MeterBar_3D*	m_pMeterBar_Hp	= nullptr;
	_float3 m_vHpBar_OffSet_Position;

protected:
	CModel*					m_pModelCom		= nullptr;
	CStatus*				m_pStatusCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;
	CNavigation*			m_pNaviCom		= nullptr;
	_bool					m_IsDead = false;
	CTextures*				m_pTextureCom_Dissolve	= nullptr;

protected:
	_bool	m_IsBrainWashed				= false;
	_bool	m_IsBrainWashed_Complete = false;

	_float	m_fBrainWashTime = 0.f;

protected: // 스케일 값 보존용
	_float3 m_vScale = { 0.f,0.f,0.f };

protected:
	EMonsterAI		m_eAI_Cur	= EMonsterAI::End;
	EMonsterAI		m_eAI_Next	= EMonsterAI::Idle;
	_float			m_fDetectDis = 0.f;
	_float			m_fAttackDis = 0.f;

protected: //Path_Index
	const _int m_iNorth_L[7]	= { 83,		81,		77,		73,		159,	17,		0 };
	const _int m_iNorth_R[7]	= { 113,	177,	179,	165,	158,	17,		0 };
	const _int m_iWest_L[7]		= { 87,		85,		36,		31,		24,		12,		0 };
	const _int m_iWest_R[7]		= { 93,		53,		50,		44,		25,		12,		0 };
	const _int m_iEast_L[7]		= { 186,	118,	105,	106,	101,	99,		0 };
	const _int m_iEast_R[7]		= { 190,	114,	103,	126,	100,	99,		0 };
	// 190, 114, 103, 126, 100, 99, 0
	// = { 186,	118,	105,	106,	101,	99,		0 };
	_int m_iMoveCount = 0;
	_bool m_IsChaseTarget = false;
	_bool m_IsTowerAttack = false;
	EMonster_MovePath m_eMovePath = EMonster_MovePath::End;

protected:
	_bool	m_IsKobold = false;

public:
	void Set_Pushing(_bool IsPushing) { m_IsPushing = IsPushing; }
	_bool Get_Pushing() { return m_IsPushing; }
protected:
	_bool m_IsPushing = false;

protected:
	_bool	m_IsPoison = false;
	_float	m_fPoisonTime = 0.f;
	const _float	m_fPoisonTick = 0.125f;
	_float			m_fCoughTime = 0.f;
	_float			m_fCoughTick = 5.f;

private:
	_float4		m_vColor = { 1.f, 1.f, 1.f, 1.f };
	_float m_fDissolveTime = 0.f;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_H__
#endif // !__MONSTER_H__
