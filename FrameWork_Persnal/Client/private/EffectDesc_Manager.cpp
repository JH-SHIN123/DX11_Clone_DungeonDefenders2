#include "stdafx.h"
#include "..\public\EffectDesc_Manager.h"
#include "Point_Spread.h"


IMPLEMENT_SINGLETON(CEffectDesc_Manager)

CEffectDesc_Manager::CEffectDesc_Manager()
{
}

const _int CEffectDesc_Manager::Get_Instance_StartIndex(EInstanceValue eIndex_Contanier)
{
	_int iNumIndex = m_vecInstance[(_uint)eIndex_Contanier].iStartIndex;

	m_vecInstance[(_uint)eIndex_Contanier].iStartIndex += m_vecInstance[(_uint)eIndex_Contanier].iTerm;

	if (m_vecInstance[(_uint)eIndex_Contanier].iStartIndex >= m_vecInstance[(_uint)eIndex_Contanier].iStartIndex_Max)
		m_vecInstance[(_uint)eIndex_Contanier].iStartIndex = 0;

	return iNumIndex;
}

const _int CEffectDesc_Manager::Get_Instance_DrawCount(EInstanceValue eIndex_Contanier)
{
	return m_vecInstance[(_uint)eIndex_Contanier].iTerm;
}

void CEffectDesc_Manager::Set_Instance(EInstanceValue eIndex_Contanier, const _int& iStartIndex_Max, const _int& iInstance_Term)
{
	m_vecInstance[(_uint)eIndex_Contanier].iStartIndex_Max = iStartIndex_Max;
	m_vecInstance[(_uint)eIndex_Contanier].iTerm = iInstance_Term;
}

void CEffectDesc_Manager::Free()
{
	m_vecInstance.clear();
}
