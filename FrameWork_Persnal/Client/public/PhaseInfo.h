#pragma once

#ifndef __PhaseINFO_H__

#include "UI_2D.h"

BEGIN(Client)

enum class EPhaseState
{Clear, Build, Combat, Boss, End};

class CPhaseInfo final : public CUI_2D
{
private:
	explicit CPhaseInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPhaseInfo(const CPhaseInfo& rhs);
	virtual ~CPhaseInfo() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Getter
	const EPhaseState Get_Phase() { return m_ePhaseState; }

public: // Setter
	void Set_Phase(EPhaseState ePhase) { m_ePhaseState = ePhase; }

private:
	void Phase_Check(_float TimeDelta);
	void Phase_Check_Clear(_float TimeDelta);
	void Phase_Check_Build(_float TimeDelta);
	void Phase_Check_Combat(_float TimeDelta);
	void Phase_Check_Boss(_float TimeDelta);

	void Enemy_Check();

	void Phase_Render();
	void Text_Render();


private:
	HRESULT	Ready_Component(void* pArg);

private:
	CMovement*				m_pMovementCom_Enemy = nullptr;
	CTextures*				m_pTextureCom_Enemy = nullptr;
	CTextures*				m_pTextureCom_Enemy_Mask = nullptr;
	CTextures*				m_pTextureCom_Enemy_MeterTile = nullptr;

	CMovement*				m_pMovementCom_PhaseInfo = nullptr;
	CTextures*				m_pTextureCom_PhaseInfo = nullptr;

	CMovement*				m_pMovementCom_Text[5] = { nullptr }; // Phase, Phase, Defense, Level, Score
	CTextures*				m_pTextureCom_Text[5] = { nullptr };
	class CNumber_Font*		m_pNumberFont_Score = nullptr;

private:
	_float2			m_vKillBar_Pos = _float2(205.f, 320.f);
	_float2			m_vBossBar_Pos = _float2(0.f, 310.f);
	_float			m_fBarAlphaTime = 0.f;
	_int			m_iEnemyCount = 50;
	_int			m_iEnemyCount_Max = 100;
	_float			m_fMeter_Ratio = 0.f;
	_int			m_iNowPhase = 0;

private:
	_float2		m_vInterval;
	EPhaseState	m_ePhaseState = EPhaseState::End;

private: // πÃ≈Õ±‚
	//_bool	m_Is

public:
	static CPhaseInfo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

};

END
#define __PhaseINFO_H__
#endif // !__PhaseINFO_H__
