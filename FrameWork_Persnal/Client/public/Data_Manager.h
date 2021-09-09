#pragma once

/* For. Shared Any Tiny Data */
/* 자질구레한 데이터를 처박아놓고 적당히 가져오는 짬통 중개자 입니다.*/

#ifndef __DATA_MANAGER_H__

#include "Client_Defines.h"
#include "Status.h"
#include "Base.h"

BEGIN(Client)

class CData_Manager final :	public CBase
{
	DECLARE_SINGLETON(CData_Manager)
public:
	CData_Manager();
	virtual ~CData_Manager() = default;


#pragma region Level
public:	// 씬전환 중개자,
	void Set_NowScene(ELevel eNowLevel);					// 씬의 초기화부분에서 같이 세팅
	void Change_Scene(ELevel eNextLevel);					// 오브젝트에서 씬전환 콜 
	_bool Get_IsChange_Scene() { return m_IsChangeScene; }	// 씬의 Render에서 씬전환
	
private:
	ELevel	m_NowLevel = ELevel::End;
	_bool	m_IsChangeScene = false;
#pragma endregion



#pragma region Player_Status
public:
	void Set_PlayerStatus(const STATUS_DESC& PlayerStatus) { m_PlayerStatus = PlayerStatus; }
	const STATUS_DESC Get_PlayerStatus() const { return m_PlayerStatus; }
	const _int Get_Hp()		const { return m_PlayerStatus.iHp; }
	const _int Get_HpMax()	const { return m_PlayerStatus.iHp_Max; }
	const _int Get_Mp()		const { return m_PlayerStatus.iMp; }
	const _int Get_Mp_Max() const { return m_PlayerStatus.iMp_Max; }

private:
	STATUS_DESC m_PlayerStatus;
#pragma endregion



//#pragma region Player_Skill
//public:
//	_bool Get_Tick_Stop() { return m_IsTick_Stop; }
//	void  Set_Tick_Stop(_bool IsTick) { m_IsTick_Stop = IsTick; }
//	void  Switch_Tick_Stop() { m_IsTick_Stop = !m_IsTick_Stop; }
//
//private:
//	_bool	m_IsTick_Stop = false;
//#pragma endregion




#pragma region Rank
public:
	const RANK_DESC Get_RankDesc() const	{ return m_Rank_Desc; }
	void  Set_RankDesc(const RANK_DESC& eRank)	{ memcpy(&m_Rank_Desc, &eRank, sizeof(RANK_DESC)); }
	void  Set_RankDesc_Level(const _int& iLevel) { m_Rank_Desc.iLevel = iLevel; }
	void  Set_RankDesc_Score(const _int& iScore) { m_Rank_Desc.iScore = iScore; }

private:
	RANK_DESC		m_Rank_Desc;
#pragma endregion



#pragma region UnTick
public:
	_bool Get_Tick_Stop() { return m_IsTick_Stop; }
	void  Set_Tick_Stop(_bool IsTick) { m_IsTick_Stop = IsTick; }
	void  Switch_Tick_Stop() { m_IsTick_Stop = !m_IsTick_Stop; }

private:
	_bool	m_IsTick_Stop = false;
#pragma endregion



public:
	virtual void Free();
};

END
#define __DATA_MANAGER_H__
#endif // !__DATA_MANAGER_H__
