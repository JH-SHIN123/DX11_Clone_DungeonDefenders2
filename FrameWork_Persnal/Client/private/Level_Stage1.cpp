#include "stdafx.h"
#include "..\public\Level_Stage1.h"
#include "GameInstance.h"
#include "Loading.h"
#include "Camera_Free.h"
#include "UI_2D.h"
#include "Data_Manager.h"
#include "Player.h"
#include "DefenceTower.h"
#include "Collide_Manager.h"
#include "Level_Loading.h"
#include "Monster.h"
#include "Monster_Gate.h"

#include "Point_Spread.h"
#include "Point_Spread2.h"

#include "Sound_Manager.h"

CLevel_Stage1::CLevel_Stage1(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CLevel(pDevice, pDevice_Context)
{

}

HRESULT CLevel_Stage1::NativeConstruct()
{
	CSound_Manager::GetInstance()->StopSoundAll();

	CData_Manager::GetInstance()->Set_NowPhase(EPhaseState::Build);

	CLevel::NativeConstruct();

	GET_GAMEINSTANCE->Clear_This_Level((_uint)ELevel::Loading);

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(TEXT("Layer_UI"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera_Free(TEXT("Layer_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(L"Layer_Player")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_Gate(L"Layer_Gate")))
		return E_FAIL;

	if (FAILED(Ready_Layer_CrystalCore(L"Layer_CrystalCore")))
		return E_FAIL;
		

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, L"Prototype_Fade", (_uint)ELevel::Stage1, L"Layer_Fade");

	PHASEINFO_DESC Phase;
	Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
	Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 3;
	Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 2;
	Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;
	m_pMonsterGate[(_uint)EMonster_MovePath::North_L]->Set_PhaseMonster_Info(Phase);

	Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
	Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 2;
	Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 3;
	Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 1;
	m_pMonsterGate[(_uint)EMonster_MovePath::North_R]->Set_PhaseMonster_Info(Phase);

	Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
	Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 3;
	Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;
	m_pMonsterGate[(_uint)EMonster_MovePath::West_L]->Set_PhaseMonster_Info(Phase);

	Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
	Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 2;
	Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;
	m_pMonsterGate[(_uint)EMonster_MovePath::West_R]->Set_PhaseMonster_Info(Phase);


	Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
	Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
	Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 0;
	Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;
	m_pMonsterGate[(_uint)EMonster_MovePath::East_L]->Set_PhaseMonster_Info(Phase);
	m_pMonsterGate[(_uint)EMonster_MovePath::East_R]->Set_PhaseMonster_Info(Phase);

	CData_Manager::GetInstance()->Set_MonsterCount_Max(15);


	return S_OK;
}

_int CLevel_Stage1::Tick(_float Timedelta)
{
	CLevel::Tick(Timedelta);
	//CSound_Manager::GetInstance()->PlayBGM(L"BGM2.mp3", Engine::CHANNEL_BGM, 1.f);
	//CSound_Manager::GetInstance()->Play_Sound(L"button.wav", Engine::CHANNEL_PLAYER, GET_CAMERA_POSITION, XMVectorSet(0.f,0.f,0.f,1.f), 10.f);

	if (false == m_IsIntro)
	{
		m_IsIntro = true;
		CSound_Manager::GetInstance()->StopSound(CHANNEL_CORE);
		CSound_Manager::GetInstance()->Play_Sound(L"Phase_Intro.ogg", CHANNEL_CORE);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_CORE, CData_Manager::GetInstance()->Get_SoundVolume_BGM());
	}
	else
	{
		if (false == m_IsBGM)
		{
			if (true == CSound_Manager::GetInstance()->IsPlaying(CHANNEL_CORE))
			{
				m_IsBGM = true;
				CSound_Manager::GetInstance()->StopSound(CHANNEL_BGM);
				CSound_Manager::GetInstance()->PlayBGM(L"Phase_Bulid_BGM.ogg", CHANNEL_BGM);
				CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());
			}
		}
	}
	

	if (GET_KEY_INPUT(DIK_F1) && true == CData_Manager::GetInstance()->Get_Tick_Stop())
	{
		CData_Manager::GetInstance()->Set_Tick_Stop(false);
	}

	if (GET_KEY_INPUT(DIK_F2) && false == CData_Manager::GetInstance()->Get_Tick_Stop())
	{
		// 탭키 퍼즈
		UI2D_DESC UI_Desc;

		lstrcpy(UI_Desc.szTextureName, L"Component_Texture_PauseMenu");
		UI_Desc.Movement_Desc.vPos = _float4(0.f, 50.f, 0.f, 1.f);
		UI_Desc.Movement_Desc.vScale = _float4(512.f, 512.f, 0.f, 0.f);
		UI_Desc.eLevel = ELevel::Static;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PauseMenu"), (_uint)ELevel::Stage1, L"Layer_Pause", &UI_Desc);

		CData_Manager::GetInstance()->Set_Tick_Stop(true);
	}

	// 맞을놈, 때릴놈
	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_CrystalCore", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);
	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_Monster", ELevel::Stage1, L"Layer_Bullet", ELevel::Stage1);

	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_Player", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);
	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_Player", ELevel::Stage1, L"Layer_Bullet_Monster", ELevel::Stage1);

	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_Tower", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);
	CCollide_Manager::GetInstance()->Collide_Check(L"Layer_Tower", ELevel::Stage1, L"Layer_Bullet_Monster", ELevel::Stage1);

	CCollide_Manager::GetInstance()->Collide_Check_Poison(L"Layer_Monster", ELevel::Stage1, L"Layer_Bullet_Posion", ELevel::Stage1);

	CCollide_Manager::GetInstance()->Collide_Check_Always(L"Layer_Player", ELevel::Stage1, L"Layer_ManaToken", ELevel::Stage1);

	CCollide_Manager::GetInstance()->Collide_Push(L"Layer_Monster", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);
	CCollide_Manager::GetInstance()->Collide_Push(L"Layer_Player", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);
	CCollide_Manager::GetInstance()->Collide_Push(L"Layer_Player", ELevel::Stage1, L"Layer_Monster", ELevel::Stage1);

	//Collide_Push


	if (EPhaseState::Combat == CData_Manager::GetInstance()->Get_NowPhase())
	{
		if (true == CData_Manager::GetInstance()->Is_PhaseClear())
		{
			CData_Manager::GetInstance()->Set_NowPhase(EPhaseState::Build);
			CSound_Manager::GetInstance()->StopSound(CHANNEL_BGM);
			CSound_Manager::GetInstance()->PlayBGM(L"Phase_Bulid_BGM.ogg", CHANNEL_BGM);
			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());

			++m_iWaveCount;
		}
	}

	switch (m_iWaveCount)
	{
	case 0:
		break;
	case 1:
	{
		PHASEINFO_DESC Phase;
		Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
		Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 4;
		Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 2;
		Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 1;
		Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;

		m_pMonsterGate[(_uint)EMonster_MovePath::North_L]->Set_PhaseMonster_Info(Phase); //6
		m_pMonsterGate[(_uint)EMonster_MovePath::North_R]->Set_PhaseMonster_Info(Phase); //6

		Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
		Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 3;
		Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 3;
		Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 1;
		Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;

		m_pMonsterGate[(_uint)EMonster_MovePath::West_L]->Set_PhaseMonster_Info(Phase); //6
		m_pMonsterGate[(_uint)EMonster_MovePath::West_R]->Set_PhaseMonster_Info(Phase); //6

		CData_Manager::GetInstance()->Reset_MonsterCount();
		CData_Manager::GetInstance()->Set_MonsterCount_Max(24);
		//CData_Manager::GetInstance()->Set_BossDead(false);
		++m_iWaveCount;

		m_IsCombat = true;
	}
		break;
	case 2:
		if (true == m_IsCombat)
		{
			m_IsBulid = true;
			m_IsCombat = false;
// 			CSound_Manager::GetInstance()->PlayBGM(L"Phase_Combat_BGM.mp3", CHANNEL_BGM);
// 			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());
		}
		break;
	case 3:
	{
		if (true == m_IsBulid)
		{
			m_IsBulid = false;
			m_IsCombat = true;
// 			CSound_Manager::GetInstance()->PlayBGM(L"Phase_Bulid_BGM.mp3", CHANNEL_BGM);
// 			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());
		}

		PHASEINFO_DESC Phase;
		Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
		Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 2;
		Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 1;
		Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 1;
		Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 1;

		m_pMonsterGate[(_uint)EMonster_MovePath::North_L]->Set_PhaseMonster_Info(Phase);

		Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 5;
		Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 1;
		Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 2;
		Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;

		m_pMonsterGate[(_uint)EMonster_MovePath::North_R]->Set_PhaseMonster_Info(Phase);

		Phase.IsAddMonster[(_uint)EMonster_List::Goblin] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Ogre] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Kamikaze] = true;
		Phase.IsAddMonster[(_uint)EMonster_List::Boss] = true;
		Phase.iMonsterCount[(_uint)EMonster_List::Goblin] = 3;
		Phase.iMonsterCount[(_uint)EMonster_List::Ogre] = 2;
		Phase.iMonsterCount[(_uint)EMonster_List::Kamikaze] = 2;
		Phase.iMonsterCount[(_uint)EMonster_List::Boss] = 0;

		m_pMonsterGate[(_uint)EMonster_MovePath::West_L]->Set_PhaseMonster_Info(Phase);
		m_pMonsterGate[(_uint)EMonster_MovePath::West_R]->Set_PhaseMonster_Info(Phase);

		CData_Manager::GetInstance()->Reset_MonsterCount();
		CData_Manager::GetInstance()->Set_MonsterCount_Max(15);
		CData_Manager::GetInstance()->Set_BossDead(false);
		CData_Manager::GetInstance()->Set_BossPhase(true);
		++m_iWaveCount;
	}
		break;
	case 4:
		// 클리어 했다
		CSound_Manager::GetInstance()->StopSound(CHANNEL_BGM);
		CSound_Manager::GetInstance()->PlayBGM(L"Phase_Complete.ogg", CHANNEL_BGM);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_BGM, CData_Manager::GetInstance()->Get_SoundVolume_BGM());
		++m_iWaveCount;
		break;

	case 5:
		m_fTime -= Timedelta;
		if (0.f >= m_fTime)
			++m_iWaveCount;
		break;
	default:
		break;
	}
	
	Cheet_Monster_Spawn();

	return 0;
}

HRESULT CLevel_Stage1::Render()
{
#ifdef _DEBUG
	//SetWindowText(g_hWnd, TEXT("지금 이거 로고씬."));
#endif // _DEBUG

	if (m_IsChange == true)
	{
		CGameInstance* pGameInstance = GET_GAMEINSTANCE;

		if (nullptr == pGameInstance)
			return -1;

		if (FAILED(pGameInstance->SetUp_CurrentLevel(CLevel_Loading::Create(m_pDevice, m_pDevice_Context, m_eNextLevel))))
			return -1;
		
		pGameInstance->Clear_This_Level((_uint)ELevel::Stage1);
	}



	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Light()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	pGameInstance->Reserve_Container_Light(3);

	/* For.Directional */
	LIGHT_DESC			LightDesc;
	LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
	LightDesc.vDirection = XMFLOAT3(0.f, -1.f, -0.3f);
	LightDesc.vDiffuse = XMFLOAT4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDevice_Context, LightDesc)))
		return E_FAIL;


	LightDesc.eType = tagLightDesc::TYPE_POINT;
	LightDesc.vPosition = XMFLOAT3(0.f, 5.f, 0.f);
	LightDesc.fRadius = 10.f;
	LightDesc.vDiffuse = XMFLOAT4(1.f, 0.5f, 0.5f, 1.f);
	LightDesc.vAmbient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vSpecular = XMFLOAT4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pDevice_Context, LightDesc)))
		return E_FAIL;



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Terrain(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Terrain"), (_uint)ELevel::Stage1, pLayerTag);

	//pGameInstance->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Terrain_TestNavi"), (_uint)ELevel::Stage1, L"Layer_Terrain_Navi");

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Monster_Gate(const _tchar * pLayerTag)
{
	GATE_DESC Data;
	Data.MoveDesc.vScale = { 1.25f, 1.f, 1.f, 0.f };

	// 북좌
	Data.StartPath = EMonster_MovePath::North_L;
	Data.MoveDesc.vPos = { -238.f,-32.f,-40.5f,1.f };
	Data.MoveDesc.vRotateLook = { -1.f,0.f,0.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_NL", &Data);

	Data.StartPath = EMonster_MovePath::North_R;
	Data.MoveDesc.vPos = { -238.f, -32.f, 39.5f, 1.f };
	Data.MoveDesc.vRotateLook = { -1.f,0.f,0.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_NR", &Data);

	Data.MoveDesc.vScale = { 1.f,1.f,1.f,0.f };

	// 서좌
	Data.StartPath = EMonster_MovePath::West_L;
	Data.MoveDesc.vPos = { 31.5f,-31.5f,-228.5f,1.f };
	Data.MoveDesc.vRotateLook = { 0.f,0.f,-1.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_WL", &Data);

	Data.StartPath = EMonster_MovePath::West_R;
	Data.MoveDesc.vPos = { -19.5f,-31.5f,-228.5f,1.f };
	Data.MoveDesc.vRotateLook = { 0.f,0.f,-1.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_WR", &Data);

	// 동좌
	Data.StartPath = EMonster_MovePath::East_L;
	Data.MoveDesc.vPos = { -19.5f,-31.5f,228.5f,1.f };
	Data.MoveDesc.vRotateLook = { 0.f,0.f,1.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_EL", &Data);

	Data.StartPath = EMonster_MovePath::East_R;
	Data.MoveDesc.vPos = { 31.5f,-31.5f,228.5f,1.f };
	Data.MoveDesc.vRotateLook = { 0.f,0.f,1.f,0.f };
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_Monster_Gate"), (_uint)ELevel::Stage1, L"Layer_Gate_ER", &Data);



	m_pMonsterGate[(_uint)EMonster_MovePath::North_L] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_NL"));
	m_pMonsterGate[(_uint)EMonster_MovePath::North_R] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_NR"));

	m_pMonsterGate[(_uint)EMonster_MovePath::West_L] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_WL"));
	m_pMonsterGate[(_uint)EMonster_MovePath::West_R] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_WR"));

	m_pMonsterGate[(_uint)EMonster_MovePath::East_L] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_EL"));
	m_pMonsterGate[(_uint)EMonster_MovePath::East_R] = (CMonster_Gate*)GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, TEXT("Layer_Gate_ER"));


	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	// 스킬창 프레임
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_StatusPanel"), (_uint)ELevel::Stage1, L"Layer_FrameUI");

	// X 67 Y 67
	// 스킬창
	UI2D_DESC UI_Desc;
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_SkillMage");
	UI_Desc.Movement_Desc.vPos = _float4(-435.2f, -211.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(64.f, 64.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PlayerSkill_UI"), (_uint)ELevel::Stage1, L"Layer_UI", &UI_Desc);

	// HPMP따로 만듬
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_HpMp");
	UI_Desc.Movement_Desc.vPos = _float4(-581.f, -213.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(64.f, 128.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Player_HpMp"), (_uint)ELevel::Stage1, L"Layer_UI_HpMp", &UI_Desc);

	// 해당 스테이지의 정보를 보여줌
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Panel_Level");
	UI_Desc.Movement_Desc.vPos = _float4(510.f, -305.f, 0.f, 1.f);
	UI_Desc.Movement_Desc.vScale = _float4(256.f, 100.f, 0.f, 0.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_PhaseInfo"), (_uint)ELevel::Stage1, L"Layer_PhaseInfo", &UI_Desc);



	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Camera_Free(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	CAMERA_DESC			CameraDesc;
	CameraDesc.vEye = _float3(0.f, 10.f, -7.f);
	CameraDesc.vAt = _float3(0.f, -5.f, 0.f);
	CameraDesc.vAxisY = _float3(0.f, 1.f, 0.f);

	CameraDesc.vTargetAxis = _float3(0.3f, 0.9f, -1.1f);
	CameraDesc.fXRotationLock_Min = 70.f;
	CameraDesc.fXRotationLock_Max = 155.f;
	CameraDesc.fDis = 10.f;
	//CameraDesc.fDis = 30.f;
	CameraDesc.fAspect = _float(g_iWinCX) / g_iWinCY;
	CameraDesc.fFovy = XMConvertToRadians(90.0f);
	CameraDesc.fNear = 0.3f;
	CameraDesc.fFar = 430.f;

	CameraDesc.StateDesc.fSpeedPerSec = 50.f;
	CameraDesc.StateDesc.fRotatePerSec = XMConvertToRadians(90.0f);

	//pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Camera_Free"), (_uint)ELevel::Stage1, pLayerTag, &CameraDesc);
	CameraDesc.vAt = _float3(5.f, 5.f, 10.f);
	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Camera_Target"), (_uint)ELevel::Stage1, pLayerTag, &CameraDesc);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_Player(const _tchar * pLayerTag)
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	GAMEOBJ_DESC Data;
	Data.Movement_Desc.vPos = { 0.f, 0.f, -9.f, 1.f };
	Data.Movement_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	Data.Movement_Desc.fSpeedPerSec = 20.f;

	Data.Status_Desc.fAttSpeed = 2.f;
	Data.Status_Desc.fCritical = 10.f;
	Data.Status_Desc.iAtt_Basic = 50;
	Data.Status_Desc.iExp = 0;
	Data.Status_Desc.iExp_Max = 100;
	Data.Status_Desc.iHp_Max = 400;
	Data.Status_Desc.iLevel = 1;
	Data.Status_Desc.iMp_Max = 250;

	lstrcpy(Data.szModelName, L"Component_Mesh_Mage");

	pGameInstance->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_Player"), (_uint)ELevel::Stage1, pLayerTag, &Data);

	return S_OK;
}

HRESULT CLevel_Stage1::Ready_Layer_CrystalCore(const _tchar * pLayerTag)
{
	GAMEOBJ_DESC Data;
	Data.Movement_Desc.vPos = { 0.f, 0.f, 0.f, 1.f };
	Data.Movement_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	Data.Movement_Desc.fRotatePerSec = 0.5f;

	Data.Status_Desc.fAttSpeed = 2.f;
	Data.Status_Desc.fCritical = 10.f;
	Data.Status_Desc.iAtt_Basic = 50;
	Data.Status_Desc.iExp = 0;
	Data.Status_Desc.iExp_Max = 100;
	Data.Status_Desc.iHp_Max = 5000;
	Data.Status_Desc.iLevel = 1;

	

	lstrcpy(Data.szModelName, L"Component_Mesh_CrystalCore");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, TEXT("Prototype_CrystalCore"), (_uint)ELevel::Stage1, pLayerTag, &Data);

	return S_OK;
}

void CLevel_Stage1::Make_Monster_Phase_1()
{
}

void CLevel_Stage1::Phase_Check()
{

}

void CLevel_Stage1::Cheet_Monster_Spawn()
{

#ifdef _DEBUG

	if (GET_KEY_INPUT(DIK_H))
	{
		MONSTER_DESC MonData;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Kobold");
		MonData.eLevel = ELevel::Stage1;
		MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
		MonData.Movement_Desc.fSpeedPerSec = 10.f;
		MonData.eMovePath = EMonster_MovePath::West_L;
		MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		MonData.fAttackDis = 30.f;
		MonData.fDetectDis = 0.f;
		MonData.Stat_Desc.iExp = 20;
		MonData.Stat_Desc.iHp_Max = 200;
		//for(int i = 0; i < 20; ++i)
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Kobold", (_uint)ELevel::Stage1, L"Layer_Monster", &MonData);
	}
	if (GET_KEY_INPUT(DIK_J))
	{
		MONSTER_DESC MonData;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Ogre");
		MonData.eLevel = ELevel::Stage1;
		MonData.fDetectDis = 15.f;
		MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
		MonData.Movement_Desc.fSpeedPerSec = 10.f;
		MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		MonData.Stat_Desc.iHp_Max = 10000;
		MonData.Stat_Desc.iHp = MonData.Stat_Desc.iHp_Max;
		MonData.Stat_Desc.iExp = 15;

		MonData.eMovePath = EMonster_MovePath::West_R;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Ogre", (_uint)ELevel::Stage1, L"Layer_Monster", &MonData);
	}
	if (GET_KEY_INPUT(DIK_K))
	{
		MONSTER_DESC MonData;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Ogre");
		MonData.eLevel = ELevel::Stage1;
		MonData.fDetectDis = 15.f;
		MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
		MonData.Movement_Desc.fSpeedPerSec = 10.f;
		MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		MonData.Stat_Desc.iHp_Max = 10000;
		MonData.Stat_Desc.iHp = MonData.Stat_Desc.iHp_Max;
		MonData.Stat_Desc.iExp = 15;

		MonData.eMovePath = EMonster_MovePath::North_L;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Ogre", (_uint)ELevel::Stage1, L"Layer_Monster", &MonData);
	}
	if (GET_KEY_INPUT(DIK_L))
	{
		MONSTER_DESC MonData;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Ogre");
		MonData.eLevel = ELevel::Stage1;
		MonData.fDetectDis = 15.f;
		MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
		MonData.Movement_Desc.fSpeedPerSec = 10.f;
		MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		MonData.Stat_Desc.iHp_Max = 10000;
		MonData.Stat_Desc.iHp = MonData.Stat_Desc.iHp_Max;
		MonData.Stat_Desc.iExp = 15;

		MonData.eMovePath = EMonster_MovePath::North_R;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Ogre", (_uint)ELevel::Stage1, L"Layer_Monster", &MonData);


	}
	if (GET_KEY_INPUT(DIK_SEMICOLON))
	{
		MONSTER_DESC MonData;

		lstrcpy(MonData.szModelName, L"Component_Mesh_Boss_Djinn");
		MonData.eLevel = ELevel::Stage1;
		MonData.fDetectDis = 15.f;
		MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
		MonData.Movement_Desc.fSpeedPerSec = 10.f;
		MonData.eMovePath = EMonster_MovePath::West_L;
		MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		MonData.Stat_Desc.iHp_Max = 700;
		MonData.Stat_Desc.iHp = MonData.Stat_Desc.iHp_Max;
		MonData.Stat_Desc.iExp = 15;
		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Boss_Djinn", (_uint)ELevel::Stage1, L"Layer_Boss", &MonData);

	}
#endif // _DEBUG



}

void CLevel_Stage1::Scene_Change(ELevel eLevel)
{
	m_IsChange = true;
	m_eNextLevel = eLevel;
}

CLevel_Stage1 * CLevel_Stage1::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLevel_Stage1*		pInstance = new CLevel_Stage1(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct()))
	{
		MSG_BOX("Failed to Creating Instance (CLevel_Stage1) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Stage1::Free()
{
	CLevel::Free();

	//Safe_Release(m_pMonsterGate[(_uint)EMonster_MovePath::North_R]);
	//Safe_Release(m_pMonsterGate[(_uint)EMonster_MovePath::North_L]);
	//Safe_Release(m_pMonsterGate[(_uint)EMonster_MovePath::West_L]);	
	//Safe_Release(m_pMonsterGate[(_uint)EMonster_MovePath::West_R]);	
	//Safe_Release();
	//Safe_Release();
}
