#include "stdafx.h"
#include "..\public\Loading.h"

#include "BackGround_Logo.h"

CLoading::CLoading(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

_uint APIENTRY ThreadMain(void* pArg)
{
	CLoading*		pLoading = (CLoading*)pArg;

	EnterCriticalSection(pLoading->Get_CriticalSectionPtr());

	HRESULT		hr = 0;

	switch (pLoading->Get_NextSceneID())
	{
	case ELevel::Logo:
		hr = pLoading->LoadingForLogo();
		break;
	case ELevel::Stage1:
		hr = pLoading->LoadingForStage();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(pLoading->Get_CriticalSectionPtr());

	return 0;
}

HRESULT CLoading::NativeConstruct(ELevel eNextSceneID)
{
	InitializeCriticalSection(&m_CriticalSection);

	m_eNextSceneID = eNextSceneID;

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadMain, this, 0, nullptr);

	return S_OK;
}

HRESULT CLoading::LoadingForLogo()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel::Logo, TEXT("Prototype_BackGround"), CBackGround_Logo::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;



	m_isFinished = true;

	return S_OK;
}

HRESULT CLoading::LoadingForStage()
{
	return S_OK;
}

CLoading * CLoading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, ELevel eNextSceneID)
{
	CLoading*		pInstance = new CLoading(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct(eNextSceneID)))
	{
		MSG_BOX("Failed to Creating Instance (CLoading) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLoading::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice_Context);

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteCriticalSection(&m_CriticalSection);

	CloseHandle(m_hThread);
}
