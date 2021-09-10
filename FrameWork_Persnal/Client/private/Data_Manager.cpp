#include "stdafx.h"
#include "..\public\Data_Manager.h"

IMPLEMENT_SINGLETON(CData_Manager)

CData_Manager::CData_Manager()
{
}

void CData_Manager::Change_Scene(ELevel eNextLevel)
{
	m_NowLevel = eNextLevel;
	m_IsChangeScene = true;
}

void CData_Manager::Minus_StatUp_Count(_int iMinusCount)
{
	m_iStatUp_Count -= iMinusCount;
	
	if (0 >= m_iStatUp_Count)
		m_iStatUp_Count = 0;
}

void CData_Manager::PlayerLevel_Check()
{
}

void CData_Manager::Set_NowScene(ELevel eNowLevel)
{
	m_NowLevel = eNowLevel;
	m_IsChangeScene = false;
}

void CData_Manager::Free()
{
}
