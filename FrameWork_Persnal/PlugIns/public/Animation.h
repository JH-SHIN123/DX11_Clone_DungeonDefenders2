#pragma once

#ifndef __ANIMATION_H__

#include "Base.h"

BEGIN(Engine)
class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;
public:
	HRESULT NativeConstruct(const char* pName, _double Duration, _double TickPerSecond);
	HRESULT Add_Channel(class CAnimChannel* pAnimChannel);

private:
	char			m_szName[MAX_PATH] = "";
	_double			m_Duration = 0.0;
	_double			m_TickPerSecond = 0.0;

private:
	vector<class CAnimChannel*>			m_Channels;

public:
	static CAnimation* Create(const char* pName, _double Duration, _double TickPerSecond);
	virtual void Free() override;
};
END
#define __ANIMATION_H__
#endif // !__ANIMATION_H__
