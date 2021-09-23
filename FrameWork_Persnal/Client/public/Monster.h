#pragma once

#ifndef __MONSTER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CMasking_MeterBar_3D;

typedef struct tagMonsterDesc
{
	STATUS_DESC Stat_Desc;
	
	MOVESTATE_DESC Movement_Desc;

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

private:
	HRESULT	Ready_Component(void* pArg);

protected:
	CModel*					m_pModelCom		= nullptr;
	CStatus*				m_pStatusCom	= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;

	// 아마 이걸 세분화 해야 할것임
	CCollider*				m_pColliderCom	= nullptr;

	CMasking_MeterBar_3D*	m_pMeterBar_Hp	= nullptr;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_H__
#endif // !__MONSTER_H__
