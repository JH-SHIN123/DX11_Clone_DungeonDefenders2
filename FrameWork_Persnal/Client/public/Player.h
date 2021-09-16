#pragma once

#ifndef __PLAYER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

enum class EPlayerState
{
	CallOut = 0, // 38
	ChargeMax = 38,
	ChargeMax_KnockBack = 59,
	ChargeMin = 79,
	ChargeMin_KnockBack = 100,
	Death = 181,
	Detonate = 211,
	Fire = 231,
	Fire_MaxPower = 261,
	Heal = 281,
	Hurt = 292,
	Idle = 411,
	Idle_LowHp = 471
};

typedef struct tagGameObject_Desc
{
	STATUS_DESC		Status_Desc;
	MOVESTATE_DESC	Movement_Desc;

	wchar_t			szModelName[MAX_PATH] = L"";
	ELevel			eModelLevel = ELevel::End;
}GAMEOBJ_DESC;

class CPlayer final : public CGameObject
{
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
	_bool Get_Skill_Using(_int iSkillIndex);

private: // Tick
	void Key_Check(_float TimeDelta);
	void Turn_Check(_float TimeDelta);

private: // LateTick
	void Level_Check();

private:
	CModel*				m_pModelCom		= nullptr;
	CStatus*			m_pStatusCom	= nullptr;
	CMovement*			m_pMovementCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;

private:
	_bool		m_IsSkill_Use[10] = { false };

private: // Tower
	class CStrikerTower*	m_pStrikerTower = nullptr;
	_bool					m_IsTowerPick = false;

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

