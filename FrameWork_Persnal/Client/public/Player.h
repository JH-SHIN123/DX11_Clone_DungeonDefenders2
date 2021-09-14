#pragma once

#ifndef __PLAYER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagGameObject_Desc
{
	STATUS_DESC		Status_Desc;
	MOVESTATE_DESC	Movement_Desc;

	_tchar			szModelName[MAX_PATH] = L"";
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
	CStatus*			m_pStatusCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;
	CTextures*			m_pTextureCom	= nullptr;
	// 이제 이거 안쓸거임
	CMovement*			m_pMovementCom	= nullptr;
	CVIBuffer_Cube*		m_pBufferRectCom = nullptr;

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

