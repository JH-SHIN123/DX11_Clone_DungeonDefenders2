#pragma once

#ifndef __PHASEMONSTER_INFO_H__

#include "UI_3D.h"

enum class EMonster_List
{ Goblin = 0, Ogre, Kamikaze, Boss, End };

typedef struct tagPhaseMonster_Info_Desc
{
	_bool IsAddMonster[(_int)(EMonster_List::End)];
	_int iMonsterCount[(_int)(EMonster_List::End)] = { 0, 0, 0, 0 };

}PHASEINFO_DESC;

class CPhaseMonster_Info final : public CUI_3D
{
private:
	explicit CPhaseMonster_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPhaseMonster_Info(const CPhaseMonster_Info& rhs);
	virtual ~CPhaseMonster_Info() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pos(_fvector vPos);

public:
	void Set_PhaseInfo(const PHASEINFO_DESC& PhaseInfo_Desc);

	void Monster_Check();

private:
	PHASEINFO_DESC	m_Monster_Info;
	_int			m_iSpawnMonster_Value = 0;	// 스폰 할 몬스터의 종류
	_float3			m_vScale[(_int)EMonster_List::End]; // 이미지 크기
	_float3			m_vPos[(_int)EMonster_List::End];	// 렌더 할 이미지 위치
	_int			m_iMonsterIcon[(_int)EMonster_List::End];
	_int			m_iMonsterCount[(_int)EMonster_List::End];

	CTextures*		m_pTextureCom_Icon = nullptr;
	CTextures*		m_pTextureCom_Num = nullptr;

	//_float3			m_vStartPos_Icon;
	//_float3			m_vOffSetPos_Icon;
	//
	//_float3			m_vStartPos_Count;
	//_float3			m_vOffSetPos_Count;

public:
	static CPhaseMonster_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void * pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

#define __PHASEMONSTER_INFO_H__
#endif //!__PHASEMONSTER_INFO_H__
