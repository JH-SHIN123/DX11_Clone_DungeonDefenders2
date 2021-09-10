#pragma once

#ifndef __PLAYER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

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

private:
	void Key_Check(_float TimeDelta);
	void Turn_Check(_float TimeDelta);
	void Level_Check();

private:
	CStatus*			m_pStatusCom	= nullptr;
	CRenderer*			m_pRendererCom	= nullptr;
	CMovement*			m_pMovementCom	= nullptr;
	CTextures*			m_pTextureCom	= nullptr;
	CVIBuffer_Cube*		m_pBufferRectCom = nullptr;

private:
	_bool		m_IsSkill_Use[10] = { false };

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

