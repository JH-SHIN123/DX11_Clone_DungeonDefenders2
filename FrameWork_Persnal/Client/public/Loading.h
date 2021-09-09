#pragma once

#ifndef __LOADING_H__

/* 씬별로 로딩해야할 리소스들을 정의한다. */
/* 객체원형을 생성한다. */
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

enum class ELevel
{Static, Logo, Stage1, End };

class CLoading final : public CBase
{
private:
	CLoading(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLoading() = default;

public:
	CRITICAL_SECTION* Get_CriticalSectionPtr() {
		return &m_CriticalSection;
	}

	ELevel Get_NextSceneID() const {
		return m_eNextSceneID;
	}

	bool Get_FinishedtoLoading() const {
		return m_isFinished;
	}
public:
	HRESULT NativeConstruct(ELevel eNextSceneID);
	HRESULT LoadingForLogo();
	HRESULT LoadingForStage();

public:
	HRESULT Loading_UI();

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_CriticalSection;
	ELevel				m_eNextSceneID;
private:
	CGameInstance*		m_pGameInstance = nullptr;

private:
	_bool				m_isFinished = false;


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;




public:
	static CLoading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ELevel eNextSceneID);
	virtual void Free();
};

END
#define __LOADING_H__
#endif // !__LOADING_H__