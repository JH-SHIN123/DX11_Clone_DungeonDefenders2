#pragma once

/* For. Shared Any Tiny Data*/

#ifndef __DATA_MANAGER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CData_Manager final :	public CBase
{
	DECLARE_SINGLETON(CData_Manager)
public:
	CData_Manager();
	virtual ~CData_Manager() = default;


#pragma region Rank

public:
	const RANK_DESC Get_RankDesc() const	{ return m_Rank_Desc; }
	void  Set_RankDesc(const RANK_DESC& eRank)	{ memcpy(&m_Rank_Desc, &eRank, sizeof(RANK_DESC)); }
	void  Set_RankDesc_Level(const _int& iLevel) { m_Rank_Desc.iLevel = iLevel; }
	void  Set_RankDesc_Score(const _int& iScore) { m_Rank_Desc.iScore = iScore; }
private:
	RANK_DESC		m_Rank_Desc;
#pragma endregion




public:
	virtual void Free();
};

END
#define __DATA_MANAGER_H__
#endif // !__DATA_MANAGER_H__
