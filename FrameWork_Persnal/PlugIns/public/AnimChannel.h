#pragma once

#include "Base.h"

#ifndef __ANIMCHANNEL_H__
BEGIN(Engine)

class CAnimChannel final : public CBase
{
private:
	CAnimChannel();
	virtual ~CAnimChannel() = default;
public:
	HRESULT NativeConstruct(const char* pName);
	HRESULT Add_KeyFrame(KEYFRAME* pKeyFrame);
private:
	char			m_szName[MAX_PATH] = "";
private:
	vector<KEYFRAME*>			m_KeyFrames;
public:
	static CAnimChannel* Create(const char* pName);
	virtual void Free() override;
};

END
#define __ANIMCHANNEL_H__
#endif // __ANIMCHANNEL_H__