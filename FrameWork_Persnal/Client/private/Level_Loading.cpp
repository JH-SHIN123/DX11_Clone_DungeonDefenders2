#include "stdafx.h"

#include "..\public\Level_Loading.h"

#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Stage1.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Loading::NativeConstruct(ELevel eNextSceneID)
{
	CLevel::NativeConstruct();

	m_eNextLevelID = eNextSceneID;

	/* 로딩씬을 보여줘야할 객체를 생성한다. */


	/* 다음씬에 필요한 자원들을 생성한다. */
	// 멀티스레드로 로딩하는건 얘가 함
	m_pLoading = CLoading::Create(m_pDevice, m_pDevice_Context, eNextSceneID);
	
	if (nullptr == m_pLoading)
		return E_FAIL;

	return S_OK;
}

int CLevel_Loading::Tick(_double Timedelta)
{
	CLevel::Tick(Timedelta);

	/* 해당객체를 렌더그룹에 추가하고. */
	if (true == m_pLoading->Get_FinishedtoLoading())
	{
		CLevel*		pLevel = nullptr;

		switch (m_eNextLevelID)
		{
		case ELevel::Logo:
			pLevel = CLevel_Logo::Create(m_pDevice, m_pDevice_Context);
			break;
		case ELevel::Stage1:
			pLevel = CLevel_Stage1::Create(m_pDevice, m_pDevice_Context);
			break;
		}		

		if (nullptr == pLevel)
			return -1;

		CGameInstance*		pGameInstance = CGameInstance::GetInstance();
		if (nullptr == pGameInstance)
			return -1;

		pGameInstance->SetUp_CurrentLevel(pLevel);
	}	
	return 0;
}

HRESULT CLevel_Loading::Render()
{
	CLevel::Render();

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, ELevel eNextSceneID)
{
	CLevel_Loading*		pInstance = new CLevel_Loading(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct(eNextSceneID)))
	{
		MSG_BOX("Failed to Creating Instance (CLevel_Loading) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	CLevel::Free();

	Safe_Release(m_pLoading);


}
