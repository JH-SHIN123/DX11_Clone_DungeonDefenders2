#pragma once

#ifndef __SOUND_MANAGER_H__

#include "Base.h"
#include "Engine_Defines.h"

#include "io.h"
#include "../public/Fmod/fmod.h"
#include "../public/Fmod/fmod.hpp"
#include "../public/Fmod/fmod_dsp.h"
#include "../public/Fmod/fmod_common.h"
#include "../public/Fmod/fmod_errors.h"

BEGIN(Engine)

enum ENGINE_DLL CHANNEL_TYPE
{
	CHANNEL_BGM, CHANNEL_PLAYER ,CHANNEL_END
};

class ENGINE_DLL CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)
public:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	void Initialize(void);
	void UpdateSound(void);

public:
	void Play_Sound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel);
	void PlayBGM(TCHAR* pSoundKey, CHANNEL_TYPE eChannel);
	void Set_Volume(CHANNEL_TYPE eChannel, _float fVolume);
	void StopSound(CHANNEL_TYPE eChannel);
	void StopSoundAll(void);
	bool IsPlaying(CHANNEL_TYPE eChannel);

public:
	void Play_Sound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, _fvector vecCamEye, _fvector vecSoundPos, _float fRange, _int iSoundNum = 1, _bool bPlayOnce = false);

private:
	FMOD_SYSTEM*				m_pSystem = nullptr;
	FMOD_CHANNEL*				m_pChannel[CHANNEL_END];

	map<TCHAR*, FMOD_SOUND*>	m_MapSound;

public:
	virtual void Free() override;
};

END
#define __SOUND_MANAGER_H__
#endif
