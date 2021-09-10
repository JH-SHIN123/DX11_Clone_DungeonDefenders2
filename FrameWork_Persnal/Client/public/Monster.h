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

	// 지금 당장은 쓰자구 ㅇㅇ
	_tchar  szTextureName[MAX_PATH] = L"";
	ELevel	eLevel = ELevel::Static;
	
}MONSTER_DESC;


class CMonster final : public CGameObject
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

private:
	CStatus*				m_pStatus		= nullptr;
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;
	CMasking_MeterBar_3D*	m_pMeterBar_Hp	= nullptr;

	// 지금 당장은 쓰자구 ㅇㅇ
	CTextures*			m_pTextureCom		= nullptr;
	CVIBuffer_Rect*		m_pBufferRectCom	= nullptr;


public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_H__
#endif // !__MONSTER_H__
