#pragma once

#ifndef __MONSTER_GATE_H__

#include "Client_Defines.h"
#include "GameObject.h"
#include "PhaseInfo.h"
#include "PhaseMonster_Info.h"
#include "Monster.h"

BEGIN(Client)
enum class EMonster_Gate_Anim
{
	Closed = 0,
	Closing = 1,
	Idle = 35,
	Open = 36,
	Opening = 37,
	End_Anim = 71,
	End = -1		
};

typedef struct tagMonsterGate_Desc
{
	MOVESTATE_DESC		MoveDesc;
	EMonster_MovePath	StartPath;
}GATE_DESC;

class CMonster_Gate final :	public CGameObject
{
protected:
	explicit CMonster_Gate(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMonster_Gate(const CMonster_Gate& rhs);
	virtual ~CMonster_Gate() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // PHASEINFO_DESC
	void PhaseMonster_Info_Check();
	void Set_PhaseMonster_Info(const PHASEINFO_DESC& PhaseMonster_Info);

private:
	HRESULT	Ready_Component(void* pArg);
	void Anim_Check(_float TimeDelta);
	void Phase_Check();

private:
	void Spawn_Monster_Check(_float TimeDelta);
	void Spawn_Monster();

protected:
	CModel*				m_pModelCom_Left	= nullptr;
	CModel*				m_pModelCom_Right	= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CMovement*			m_pMovementCom		= nullptr;
	
private:
	_bool m_IsAnimated = true;
	_bool m_IsAnimated_Once = true;
	_bool m_IsChangedAnim = false;

private:
	EPhaseState			m_ePhase_Cur	= EPhaseState::End;
	EPhaseState			m_ePhase_Next	= EPhaseState::End;
	EMonster_Gate_Anim	m_eAnim_Cur		= EMonster_Gate_Anim::End;
	EMonster_Gate_Anim	m_eAnim_Next	= EMonster_Gate_Anim::Closing;

private:
	CPhaseMonster_Info* m_pPhaseMonster_Info = nullptr;

private: // Monster_Wave
	const _float	m_fSpawnTime_Max = 3.f;
	_float			m_fSpawnTime = 0.f;

	EMonster_List	m_eSpawnList = EMonster_List::Goblin;
	_int			m_iSpawnCount = 0;
	PHASEINFO_DESC	m_SpawnMonster;

private:
	EMonster_MovePath m_ePath = EMonster_MovePath::End;

public:
	static CMonster_Gate* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_GATE_H__
#endif // !__MONSTER_GATE_H__
