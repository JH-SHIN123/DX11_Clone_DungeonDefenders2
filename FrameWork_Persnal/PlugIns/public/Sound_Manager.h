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
	void LoadSoundFile();
	void Play_Sound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, _float fVolume = 1.f);
	void PlayBGM(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, _float fVolume = 1.f);
	void Set_BGMVolume(CHANNEL_TYPE eChannel, _float fVolume);
	void UpdateSound(void);
	void StopSound(CHANNEL_TYPE eChannel);
	void StopSoundAll(void);

public:
	void Play_Sound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, _fvector vecCamEye, _fvector vecSoundPos, _float fRange, _int iSoundNum = 1, _bool bPlayOnce = false);
	void Play_RandomSound(TCHAR* pSoundKey, CHANNEL_TYPE eChannel, _int iNum, _float fVolume = 1.f); // 0~iNum 까지 중 랜덤으로 재생
	bool IsPlaying(CHANNEL_TYPE eChannel); // 해당 채널이 Play중인지 확인

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
