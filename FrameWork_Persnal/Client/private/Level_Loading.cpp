#include "stdafx.h"

#include "..\public\Level_Loading.h"

#include "GameInstance.h"
#include "Level_Logo.h"
#include "Level_Stage1.h"
#include "Fade.h"

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


	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, L"Prototype_Fade", (_uint)ELevel::Loading, L"Layer_FadeOut");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, L"Prototype_LoadingScreen", (_uint)ELevel::Loading, L"Layer_LoadingScreen");

	
	if (nullptr == m_pLoading)
		return E_FAIL;

	return S_OK;
}

int CLevel_Loading::Tick(_float Timedelta)
{
	CLevel::Tick(Timedelta);

	/* 해당객체를 렌더그룹에 추가하고. */
	if (true == m_pLoading->Get_FinishedtoLoading())
	{
		CLevel*		pLevel = nullptr;

		if (false == m_IsCreateFadeIn)
		{
			FADE_DESC Data;
			Data.IsFadeIn = true;
			Data.eNowLevel = ELevel::Loading;
			Data.eNextLevel = m_eNextLevelID;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, L"Prototype_Fade", (_uint)ELevel::Loading, L"Layer_FadeIn", &Data);
			m_IsCreateFadeIn = true;
		}


		if (true == m_IsRealFinish)
		{
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

			GET_GAMEINSTANCE->Clear_This_Level((_uint)ELevel::Loading);

			pGameInstance->SetUp_CurrentLevel(pLevel);
		}
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
