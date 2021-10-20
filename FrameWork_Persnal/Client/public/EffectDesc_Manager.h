#pragma once

#ifndef __EFFECTDESC_MANAGER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

// 인스턴스 버퍼의 그리기 시작할 특정 인덱스(int)를 던져주는 역할을 함

typedef struct tagInstanceData
{
	_int iStartIndex = 0;
	_int iStartIndex_Max = 1;
	_int iTerm = 5;

}INSTANCE_DATA;

class CEffectDesc_Manager final : public CBase
{
	DECLARE_SINGLETON(CEffectDesc_Manager);

public:
	CEffectDesc_Manager();
	virtual ~CEffectDesc_Manager() = default;

public:
	const _int	Get_PointSpread_StartIndex();
	const _int  Get_PointSpread_DrawCount() { return m_iPointSpread_Term; }
	void		Set_PointSpread(const _int& iStartIndex_Max, const _int& iInstance_Term);
private:
	_int m_iPointSpread_StartIndex = 0;
	_int m_iPointSpread_StartIndex_Max = 1;
	_int m_iPointSpread_Term = 5;

public:
	virtual void Free();
};

END
#define __EFFECTDESC_MANAGER_H__
#endif // !__EFFECTDESC_MANAGER_H__
