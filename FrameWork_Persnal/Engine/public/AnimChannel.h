#pragma once

#include "Base.h"

#ifndef __ANIMCHANNEL_H__
BEGIN(Engine)

class CAnimChannel final : public CBase
{
private:
	CAnimChannel();
	virtual ~CAnimChannel() = default;

public: // Getter
	const char* Get_Name() const { return m_szName; }
	vector<KEYFRAME*> Get_KeyFrames() { return m_KeyFrames; }
	_uint	Get_CurrentKeyFrame() const { return m_iCurrentKeyFrame; }
	_matrix Get_TransformationMatrix() { return XMLoadFloat4x4(&m_TransformationMatrix); }

public:
	void Set_CurrentKeyFrame(_uint iCurrentKeyFrame);
	void Set_TransformationMatrix(_fmatrix TransformationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix); }
	void Set_Parent_Name(const char* szParentName);

public:
	HRESULT NativeConstruct(const char* pName);
	HRESULT Add_KeyFrame(KEYFRAME* pKeyFrame);

private:
	char			m_szName[MAX_PATH] = "";
	char			m_szParentName[MAX_PATH] = "";

private:
	vector<KEYFRAME*>	m_KeyFrames;
	_uint				m_iCurrentKeyFrame = 0;
	_float4x4			m_TransformationMatrix;

public:
	static CAnimChannel* Create(const char* pName);
	virtual void Free() override;
};

END
#define __ANIMCHANNEL_H__
#endif // __ANIMCHANNEL_H__