#pragma once

#ifndef __WAVEINFO_H__

#include "UI_2D.h"

BEGIN(Client)

enum class EWaveState
{Clear, Build, Combat, Boss, End};

class CWaveInfo final :	public CUI_2D
{
private:
	explicit CWaveInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CWaveInfo(const CWaveInfo& rhs);
	virtual ~CWaveInfo() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Getter
	const EWaveState Get_Wave() { return m_eWaveState; }

public: // Setter
	void Set_Wave(EWaveState eWave) { m_eWaveState = eWave; }

private:
	void Wave_Check();

	void Wave_Render();
	void Text_Render();


private:
	HRESULT	Ready_Component(void* pArg);

private:
	CMovement*				m_pMovementCom_Enemy = nullptr;
	CTextures*				m_pTextureCom_Enemy = nullptr;

	CMovement*				m_pMovementCom_Enemy_Mask = nullptr;
	CTextures*				m_pTextureCom_Enemy_Mask = nullptr;

	CMovement*				m_pMovementCom_WaveInfo = nullptr;
	CTextures*				m_pTextureCom_WaveInfo = nullptr;

	CMovement*				m_pMovementCom_Text[5] = { nullptr }; // Wave, Phase, Defense, Level, Score
	CTextures*				m_pTextureCom_Text[5] = { nullptr };


private:
	_float2			m_vKillBar_Pos = _float2(205.f, 320.f);
	_float2			m_vBossBar_Pos = _float2(0.f, 310.f);
	_float			m_fBarAlphaTime = 0.f;


private:
	_float2		m_vInterval;
	EWaveState	m_eWaveState = EWaveState::End;

public:
	static CWaveInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

};

END
#define __WAVEINFO_H__
#endif // !__WAVEINFO_H__
