#pragma once

#ifndef __MONSTER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMasking_MeterBar_3D;

typedef struct tagMonsterDesc
{
	_float fDetectDis = 30.f;
	_float fAttackDis = 10.f;
	STATUS_DESC Stat_Desc;
	
	MOVESTATE_DESC Movement_Desc;

	_tchar  szModelName[MAX_PATH] = L"";
	ELevel	eLevel = ELevel::Static;
	
}MONSTER_DESC;

enum class EMonsterAI
{ Idle, Attack, Hurt, Dead, Shock, Move, Turn, End };

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

protected:
	EMonsterAI AI_Check(_float TimeDelta, _vector* pTargetPos);

private:
	HRESULT	Ready_Component(void* pArg);



protected:
	void Set_HpBar_OffSet_Position(_float3 vOffSet_Pos) { m_vHpBar_OffSet_Position = vOffSet_Pos; }


private:
	CMasking_MeterBar_3D*	m_pMeterBar_Hp	= nullptr;
	_float3 m_vHpBar_OffSet_Position;

protected:
	CModel*					m_pModelCom		= nullptr;
	CStatus*				m_pStatusCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;



private:
	EMonsterAI		m_eAI_Cur	= EMonsterAI::End;
	EMonsterAI		m_eAI_Next	= EMonsterAI::Idle;
	_float			m_fDetectDis = 0.f;
	_float			m_fAttackDis = 0.f;
	_bool			m_IsDead = false;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_H__
#endif // !__MONSTER_H__
