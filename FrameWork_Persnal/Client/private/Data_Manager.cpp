#include "stdafx.h"
#include "..\public\Data_Manager.h"


IMPLEMENT_SINGLETON(CData_Manager)

CData_Manager::CData_Manager()
{
	ZeroMemory(&m_PlayerStatus, sizeof(STATUS_DESC));
	m_PlayerStatus.iLevel = 1;
}

void CData_Manager::TicK_Data()
{
	Tick_Phase();
}

void CData_Manager::Change_Scene(ELevel eNextLevel)
{
	m_eNowLevel = eNextLevel;
	m_IsChangeScene = true;
}

void CData_Manager::PlayerLevelUp_Check(_int iLevel)
{
	if (m_PlayerStatus.iLevel < iLevel)
	{
		m_IsLevelUp = true;
		m_iStatUp_Count += (iLevel - m_PlayerStatus.iLevel) * 5;
	}
}

_bool CData_Manager::Tick_Phase()
{

	if (EPhaseState::Clear == m_eNowPhase)
	{
		m_fPhaseCheck_Timer += GET_TIMER->Get_TimeDelta(L"Timer_FPS60");

		if (2.f < m_fPhaseCheck_Timer)
			Set_NowPhase(EPhaseState::Build);
	}

	else if (EPhaseState::Build == m_eNowPhase)
	{
		m_fPhaseCheck_Timer += GET_TIMER->Get_TimeDelta(L"Timer_FPS60");

		if (6.f < m_fPhaseCheck_Timer && true == m_IsLevelUp)
		{
			UI2D_DESC UI_Desc;

			lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Status");
			UI_Desc.Movement_Desc.vPos = _float4(0.f, 500.f, 0.f, 1.f);
			UI_Desc.Movement_Desc.vScale = _float4(768.f, 512.f, 0.f, 0.f);
			UI_Desc.Movement_Desc.fSpeedPerSec = 500.f;
			UI_Desc.eLevel = ELevel::Static;
			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_StatusMenu"), (_uint)ELevel::Stage1, L"Layer_StatusMenu", &UI_Desc);

			m_IsLevelUp = false;

		}
	}
	else if (EPhaseState::Combat == m_eNowPhase)
		m_fPhaseCheck_Timer = 0.f;


	return _bool();
}

void CData_Manager::Set_NowPhase(EPhaseState eNowPhase)
{
	// 시작할 때?
	_uint i = 0;
	switch (eNowPhase)
	{
	case Client::EPhaseState::Clear:
		break;
	case Client::EPhaseState::Build:
		i = 0;
		break;
	case Client::EPhaseState::Combat:
		i = 1;
		break;
	case Client::EPhaseState::Boss:
		break;
	default:
		break;
	}
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Phase_Panel"), (_uint)ELevel::Stage1, L"Layer_Phase_Panel", &i);
	m_eNowPhase = eNowPhase;
}

void CData_Manager::Minus_StatUp_Count(_int iMinusCount)
{
	m_iStatUp_Count -= iMinusCount;
	
	if (0 >= m_iStatUp_Count)
		m_iStatUp_Count = 0;
}

void CData_Manager::Set_NowScene(ELevel eNowLevel)
{
	m_eNowLevel = eNowLevel;
	m_IsChangeScene = false;
}

void CData_Manager::Free()
{
}
