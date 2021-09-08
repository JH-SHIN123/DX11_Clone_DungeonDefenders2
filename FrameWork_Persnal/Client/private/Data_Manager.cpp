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

void CData_Manager::Set_NowScene(ELevel eNowLevel)
{
	m_NowLevel = eNowLevel;
	m_IsChangeScene = false;
}

void CData_Manager::Free()
{
}
