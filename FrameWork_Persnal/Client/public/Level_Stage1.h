#pragma once

#ifndef __LEVEL_STAGE1_H__


#include "Client_Defines.h"
#include "Level.h"
#include "PhaseInfo.h"

BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
private:
	CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel_Stage1() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_float Timedelta) override;
	virtual HRESULT Render() override;

public:
	void Scene_Change(ELevel eLevel);

private:
	HRESULT	Ready_Light();
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster_Gate(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera_Free(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_CrystalCore(const _tchar* pLayerTag);
	
private:
	void Make_Monster_Phase_1();
	void Phase_Check();
	void Cheet_Monster_Spawn();

private:
	_int m_iCombatPhase_CountMax = 5;

private:
	_bool m_IsChange = false;
	ELevel 	m_eNextLevel = ELevel::End;

private:
	_bool m_IsKey = false;

private:
	class CMonster_Gate* m_pMonsterGate[6] = { nullptr };
	_int m_iWaveCount = 0;
	//EPhaseState m_ePhase = EPhaseState::End;

private:
	_bool m_IsIntro = false;
	_bool m_IsBGM = false;

public:
	static CLevel_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual void Free();
};

END
#define __LEVEL_STAGE1_H__
#endif // !__LEVEL_STAGE1_H__