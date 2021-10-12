#include "stdafx.h"
#include "..\public\Player.h"
#include "PlayerSkill.h"
#include "Data_Manager.h"
#include "Camera_Target.h"
#include "StrikerTower.h"
#include "Skill_ManaBomb.h"
#include "Camera_Target.h"
#include "BlockadeTower.h"
#include "StrikerTower.h"
#include "LightningTower.h"
#include "Cursor_Manager.h"
#include "Staff_Basic.h"
#include "Calculator.h"


CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	m_pModelCom->Set_AnimationIndex(0); // 나는 애니메이션 하나에 다 있는 상황 원테이크
	m_pModelCom->Set_AnimationIndex_Start(292.f, 118.f);
	//Set_Pivot(XMVectorSet(0.25f, 0.25f, 0.25f, 0.f));

	return S_OK;
}

_int CPlayer::Tick(_float TimeDelta)
{
	if (true == m_IsMissionFailed)
	{
		if (EPlayerAnimation::Lose == m_eAnimationState_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{



				return OBJECT_DEAD;
			}
		}

		m_eAnimationState_Next = EPlayerAnimation::Lose;
	}


	__super::Tick(TimeDelta);

	SpecialAnimation_Check(TimeDelta);

	Key_Check(TimeDelta);

	m_pColliderCom_Hit->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	m_pWeapon->Tick(TimeDelta);

	Tower_Pick();

	m_pBlockadeTower ->Tick(TimeDelta);
	m_pStrikerTower  ->Tick(TimeDelta);
	m_pLightningTower->Tick(TimeDelta);

	_vector vMyLook = m_pMovementCom->Get_State(EState::Look);

	if (ETowerSpawn::End == m_eTowerSpawn)
	{
		m_pBlockadeTower->Set_TowerLookDir(vMyLook);
		m_pStrikerTower->Set_TowerLookDir(vMyLook);
		m_pLightningTower->Set_TowerLookDir(vMyLook);
	}

	return _int();
}

_int CPlayer::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	Level_Check();

	if (true == m_IsCasting_Move)
	{
		m_IsSecondAnimation = true;
		m_eAnimationState_Next_Second = EPlayerAnimation::Heal;
	}
	
	Animation_Check(TimeDelta);

	Skill_Check(TimeDelta);
	
	m_pWeapon->Weapon_Equip(m_pModelCom->Get_BoneMatrix("b_FingersR"), m_pMovementCom->Get_WorldMatrix());

	m_pWeapon->Late_Tick(TimeDelta);

	m_pBlockadeTower->Update_Anim(TimeDelta);
	m_pStrikerTower->Update_Anim(TimeDelta);
	m_pLightningTower->Update_Anim(TimeDelta);


// 	_int iIndex = m_pNaviCom->Get_CellIndex(m_pMovementCom->Get_State(EState::Position));
// 	_tchar szTitle[MAX_PATH] = L"";
// 	wsprintf(szTitle, L"%d", iIndex);
// 
// 	SetWindowText(g_hWnd, szTitle);


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CPlayer::Render()
{ 
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 0);
	}



	if (true == m_IsRenderTower[RENDER_BLOCAKE])
		m_pBlockadeTower->Render();
	if (true == m_IsRenderTower[RENDER_STRIKER])
		m_pStrikerTower->Render();
	if (true == m_IsRenderTower[RENDER_LIGHTING])
		m_pLightningTower->Render();



#ifdef _DEBUG
	m_pColliderCom_Hit->Render_Collider();
	m_pNaviCom->Render_Navigation();
#endif


	return S_OK;
}

_bool CPlayer::Get_Skill_Using(_int iSkillIndex)
{
	if (iSkillIndex >= 10)
		return false;

	return m_IsSkill_Use[iSkillIndex];
}

void CPlayer::Key_Check(_float TimeDelta)
{
	// Break First
	m_IsCasting_Move = false;

	if (true == m_IsCast_ManaBomb		|| 
		true == m_IsCast_Meteor			|| 
		true == m_IsCast_PowerUp		|| 
		true == m_IsCast_BrainWash		||
		true == m_IsCast_BrainWash_End	||
		true == m_IsCast_Healing	) return;
		
	_long dwMouseMove = 0;

	if (dwMouseMove = GET_MOUSE_X)
	{
		switch (m_eTowerSpawn)
		{
		case Client::ETowerSpawn::Locate:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::Build);
			break;
		case Client::ETowerSpawn::Rotate:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::Build);
			break;
		case Client::ETowerSpawn::Spawn:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::TPS);
			break;
		default:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::TPS);
			m_eAnimationState_Next = EPlayerAnimation::Turn_Left;
			break;
		}
	}

	if (dwMouseMove = GET_MOUSE_Y)
	{
		switch (m_eTowerSpawn)
		{
		case Client::ETowerSpawn::Locate:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::Build);
			break;
		case Client::ETowerSpawn::Rotate:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::Build);
			break;
		case Client::ETowerSpawn::Spawn:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::TPS);
			break;
		default:
			CCursor_Manager::GetInstance()->Set_MouseTexture(EMouseTexture::TPS);
			m_eAnimationState_Next = EPlayerAnimation::Turn_Left;
			break;
		}

	}

#pragma region Tower


	if (GET_KEY_INPUT(DIK_6))
	{
	}

	if (GET_KEY_INPUT(DIK_7))
	{
	}

	if (GET_KEY_INPUT(DIK_8))
	{
		if (ETowerSpawn::End == m_eTowerSpawn)
		{
			m_eAnimationState_Next = EPlayerAnimation::Summon_Start;
			m_eTowerSpawn = ETowerSpawn::Locate;
		}

		m_IsRenderTower[RENDER_BLOCAKE] = true;
	}

	if (GET_KEY_INPUT(DIK_9))
	{
		if (ETowerSpawn::End == m_eTowerSpawn)
		{
			m_eAnimationState_Next = EPlayerAnimation::Summon_Start;
			m_eTowerSpawn = ETowerSpawn::Locate;
		}

		m_IsRenderTower[RENDER_STRIKER] = true;
	}

	if (GET_KEY_INPUT(DIK_0))
	{
		if (ETowerSpawn::End == m_eTowerSpawn)
		{
			m_eAnimationState_Next = EPlayerAnimation::Summon_Start;
			m_eTowerSpawn = ETowerSpawn::Locate;
		}

		m_IsRenderTower[RENDER_LIGHTING] = true;
	}

	if (GET_MOUSE_CLICK(MOUSEKEYSTATE::LB))
	{
		if (ETowerSpawn::End == m_eTowerSpawn)
		{
			m_eAnimationState_Next_Second = EPlayerAnimation::Fire;
			m_IsSecondAnimation = true;
			m_pWeapon->Create_Bullet(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)));
			return;
		}

		switch (m_eTowerSpawn)
		{
		case Client::ETowerSpawn::Locate:
			m_eTowerSpawn = ETowerSpawn::Rotate;
			break;
		case Client::ETowerSpawn::Rotate:
			m_eTowerSpawn = ETowerSpawn::Spawn;
			break;
		case Client::ETowerSpawn::Spawn:
			break;
		}
	}

	if (GET_MOUSE_CLICK(MOUSEKEYSTATE::RB))
	{
		if (ETowerSpawn::End == m_eTowerSpawn)
			return;

		static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Set_CameraView_Mode(ECameraViewMode::TopToTPS);
		
		m_eTowerSpawn = ETowerSpawn::End;
		for(_int i = 0; i < RENDER_END; ++i)
			m_IsRenderTower[i] = false;
	}



#pragma endregion

	if (GET_KEY_INPUT(DIK_W))
	{
		Casting_Move_Check(&TimeDelta);
		m_pMovementCom->Go_LookDir(TimeDelta, m_pNaviCom);
		m_eAnimationState_Next = EPlayerAnimation::RunForward;
	}

	if (GET_KEY_INPUT(DIK_S))
	{
		Casting_Move_Check(&TimeDelta);
		m_pMovementCom->Go_LookDir(-TimeDelta, m_pNaviCom);
		m_eAnimationState_Next = EPlayerAnimation::Move_Backward;
	}

	if (GET_KEY_INPUT(DIK_A))
	{
		Casting_Move_Check(&TimeDelta);
		m_pMovementCom->Go_Left(TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::Move_Left;
	}

	if (GET_KEY_INPUT(DIK_D))
	{
		Casting_Move_Check(&TimeDelta);
		m_pMovementCom->Go_Right(TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::Move_Right;
	}

	// Break Second
	if (true == m_IsCast_Healing)
		return;

	if (ETowerSpawn::End != m_eTowerSpawn)
		return;


#pragma region Skill

	if (GET_KEY_INPUT(DIK_1))
	{
		CPlayerSkill* pSkill = static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"));
		if (true == pSkill->Get_IsCoolDown(0))
		{
			pSkill->Set_Skill_CoolDown(0, 10.f);
			m_eAnimationState_Next = EPlayerAnimation::Detonate;
			m_IsAttack = true;
		}
	}

	if (GET_KEY_INPUT(DIK_2)) //Cam_Add_Dis
	{
		CPlayerSkill* pSkill = static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"));
		//static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Cam_Shake(_float3(0.2f,0.2f,0.2f), 0.5f);
		//static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Cam_Shake_At(_float3(0.2f, 0.2f, 0.2f), 0.5f);
		static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Cam_Add_Dis(7.f, 0.8f);

		if (true == pSkill->Get_IsCoolDown(1))
		{
			pSkill->Set_Skill_CoolDown(1, 5.f);
			m_eAnimationState_Next = EPlayerAnimation::ManaBomb;
			m_IsZoom = true;
			m_IsAttack = true;
		}
	}

	if (GET_KEY_INPUT(DIK_3))
	{
		CPlayerSkill* pSkill = static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"));
		if (true == pSkill->Get_IsCoolDown(2))
		{
			pSkill->Set_Skill_CoolDown(2, 5.f);
			m_eAnimationState_Next = EPlayerAnimation::FireMaxPower;
			m_IsAttack = true;
		}
	}

	if (GET_KEY_INPUT(DIK_4))
	{
		CPlayerSkill* pSkill = static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"));
		if (true == pSkill->Get_IsCoolDown(3))
		{
			pSkill->Set_Skill_CoolDown(3, 15.f);
			m_eAnimationState_Next = EPlayerAnimation::ChargeMax;
			m_fChargeSkill = 2.f;
			m_IsCharging = true;
			//m_IsChargeSkill_Change = false;
		}
	}

	if (GET_KEY_INPUT(DIK_5))
	{
		CPlayerSkill* pSkill = static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"));
		if (true == pSkill->Get_IsCoolDown(4))
		{
			pSkill->Set_Skill_CoolDown(4, 7.f);
			m_eAnimationState_Next = EPlayerAnimation::Heal;
			m_fChargeSkill = 5.f;
			m_IsCharging = true;
			m_fHealTime = 0.f;
			m_iHealSize = m_pStatusCom->Get_HpMax() - m_pStatusCom->Get_Hp();
		}
	}

#pragma endregion

	if (GET_MOUSE_CLICK(MOUSEKEYSTATE::LB))
	{
		//m_eAnimationState_Next_Second = EPlayerAnimation::Fire;
		//m_IsSecondAnimation = true;
		//m_pWeapon->Create_Bullet();
	}

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		m_eAnimationState_Next_Second = EPlayerAnimation::Fire;
		m_IsSecondAnimation = false;
	}


	//Idle_Check();
}

void CPlayer::Turn_Check(_float TimeDelta)
{

}

void CPlayer::Idle_Check()
{
	// 키입력 한번으로 동작을 계속 수행해야 하는 놈들

	_bool IsCurState = false;
	if (m_pModelCom->Get_IsFinishedAnimaion())
	{
		// ex. 공격이 끝났다? -> Idle
		switch (m_eAnimationState_Cur)
		{
		case Client::EPlayerAnimation::CallOut:
			break;
		case Client::EPlayerAnimation::ChargeMax:
			break;
		case Client::EPlayerAnimation::ChargeMax_KnockBack:
			break;
		case Client::EPlayerAnimation::ChargeMin:
			break;
		case Client::EPlayerAnimation::ChargeMin_KnockBack:
			break;
		case Client::EPlayerAnimation::Death:
			break;
		case Client::EPlayerAnimation::Detonate:
			break;
		case Client::EPlayerAnimation::Fire:
			IsCurState = true;
			break;
		case Client::EPlayerAnimation::FireMaxPower:
			break;
		case Client::EPlayerAnimation::Heal:
			break;
		case Client::EPlayerAnimation::Hurt:
			break;
		case Client::EPlayerAnimation::Idle_lowHp:
			break;
		case Client::EPlayerAnimation::Jump:
			break;
		case Client::EPlayerAnimation::Jump_Falling:
			break;
		case Client::EPlayerAnimation::KnockBack:
			break;
		case Client::EPlayerAnimation::LevelUp:
			break;
		case Client::EPlayerAnimation::Lose:
			break;
		case Client::EPlayerAnimation::ManaBomb:
			break;
		case Client::EPlayerAnimation::PickupItem:
			break;
		case Client::EPlayerAnimation::Repair:
			break;
		case Client::EPlayerAnimation::Spawn:
			break;
		case Client::EPlayerAnimation::Summon:
			break;
		case Client::EPlayerAnimation::Summon_Place:
			break;
		case Client::EPlayerAnimation::Summon_Start:
			break;
		case Client::EPlayerAnimation::Summon_Stop:
			break;
		case Client::EPlayerAnimation::Upgrade:
			break;
		case Client::EPlayerAnimation::Wave_Start:
			break;
		case Client::EPlayerAnimation::Win:
			break;
		case Client::EPlayerAnimation::WinWave:
			break;
		case Client::EPlayerAnimation::EndKey:
			break;

		case Client::EPlayerAnimation::Idle:
		case Client::EPlayerAnimation::Move_Backward:
		case Client::EPlayerAnimation::Move_Left:
		case Client::EPlayerAnimation::Move_Right:
		case Client::EPlayerAnimation::RunForward:
		case Client::EPlayerAnimation::Turn_Left:
		default:
			IsCurState = false;
			break;
		}

		//if (true == IsCurState)
		//	m_eAnimationState_Next = m_eAnimationState_Cur;
		m_eAnimationState_Next = EPlayerAnimation::Idle;
	}
	
}

void CPlayer::Skill_Check(_float TimeDelta)
{
	Skill_ManaBomb();
	Skill_Meteor();
	Skill_BrainWash();
	Skill_Healing(TimeDelta);
}

void CPlayer::Tower_Pick()
{
	switch (m_eTowerSpawn)
	{
	case Client::ETowerSpawn::Locate:
	{
		m_eAnimationState_Next = EPlayerAnimation::Summon_Place;

		static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Set_CameraView_Mode(ECameraViewMode::TopView);
		_vector vMouseWorldPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		_vector vMouseWorldDir = XMVectorZero();
		m_tTowerPickPoint = CCursor_Manager::GetInstance()->Get_MousePos_WorldSpace(&vMouseWorldPos, &vMouseWorldDir);

		_bool bGetCell = false;
		_vector vOutPos = XMVectorZero();

		if (0 == XMVectorGetX(XMVectorIsNaN(vMouseWorldDir)))
			bGetCell = m_pNaviCom->Get_CellPos(vMouseWorldDir, vMouseWorldPos, &vOutPos);

		if (true == bGetCell)
		{			
			if (true == m_IsRenderTower[RENDER_BLOCAKE])
				m_pBlockadeTower->Set_TowerPos(vOutPos);

			if (true == m_IsRenderTower[RENDER_STRIKER])
				m_pStrikerTower->Set_TowerPos(vOutPos);

			if (true == m_IsRenderTower[RENDER_LIGHTING])
				m_pLightningTower->Set_TowerPos(vOutPos);
		}
	}
		break;

	case Client::ETowerSpawn::Rotate:
	{
		_vector vMouseWorldPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
		_vector vMouseWorldDir = XMVectorZero();
		CCursor_Manager::GetInstance()->Get_MousePos_WorldSpace(&vMouseWorldPos, &vMouseWorldDir, m_tTowerPickPoint);
		m_IsTowerSpawning = true;

		m_eAnimationState_Next = EPlayerAnimation::Summon_Place;

		if (true == m_IsRenderTower[RENDER_BLOCAKE])
		{
			if (0 == XMVectorGetX(XMVectorIsNaN(vMouseWorldDir)))
				m_pBlockadeTower->Set_TowerRotatePosition(vMouseWorldDir);
		}

		if (true == m_IsRenderTower[RENDER_STRIKER])
		{
			if (0 == XMVectorGetX(XMVectorIsNaN(vMouseWorldDir)))
				m_pStrikerTower->Set_TowerRotatePosition(vMouseWorldDir);
		}		
		
		if (true == m_IsRenderTower[RENDER_LIGHTING])
		{
			if (0 == XMVectorGetX(XMVectorIsNaN(vMouseWorldDir)))
				m_pLightningTower->Set_TowerRotatePosition(vMouseWorldDir);
		}
	}
		break;
	case Client::ETowerSpawn::Spawn:

		if (false == m_IsTowerSpawning)
		{
			static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Set_CameraView_Mode(ECameraViewMode::TopToTPS);

			m_eTowerSpawn = ETowerSpawn::End;
		}
		else
		{
			m_eAnimationState_Next = EPlayerAnimation::Summon;
			_tchar szPrototypeName[MAX_PATH] = L"";
			TOWER_DESC Data;

			if (true == m_IsRenderTower[RENDER_BLOCAKE])
			{
				lstrcpy(szPrototypeName, L"Prototype_BlockadeTower");
				m_pBlockadeTower->Get_TowerDesc(&Data);
			}

			if (true == m_IsRenderTower[RENDER_STRIKER])
			{
				lstrcpy(szPrototypeName, L"Prototype_StrikerTower");
				m_pStrikerTower->Get_TowerDesc(&Data);
			}

			if (true == m_IsRenderTower[RENDER_LIGHTING])
			{
				lstrcpy(szPrototypeName, L"Prototype_LightningTower");
				m_pLightningTower->Get_TowerDesc(&Data);
			}

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, szPrototypeName, (_uint)ELevel::Stage1, L"Layer_Tower", &Data);
		}
		m_IsTowerSpawning = false;

		break;
	default:

		for (_int i = 0; i < RENDER_END; ++i)
			m_IsRenderTower[i] = false;
		break;
	}
}

void CPlayer::Level_Check()
{
	_bool IsLevelUp = m_pStatusCom->Level_Check();
	if (true == IsLevelUp)
	{
		CData_Manager::GetInstance()->PlayerLevelUp_Check(m_pStatusCom->Get_Level());
		CData_Manager::GetInstance()->Set_PlayerStatus(m_pStatusCom->Get_Stat());
	}
}

void CPlayer::Animation_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if(m_eAnimationState_Cur != m_eAnimationState_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnimationState_Next, Animation_Term(m_eAnimationState_Next));

	if (true == m_IsCasting_Move)
		TimeDelta *= 0.4f;

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	if (true == m_IsSecondAnimation)
	{
		if (m_eAnimationState_Cur_Second != m_eAnimationState_Next_Second)
			m_pModelCom->Set_AnimationIndex_Start_SecondNode((_float)m_eAnimationState_Next_Second, Animation_Term(m_eAnimationState_Next_Second));

		if(true == m_IsCasting_Move)
			m_pModelCom->Update_AnimaionMatrix_Second("b_Torso", TimeDelta);

		else
			m_pModelCom->Update_AnimaionMatrix_Second("b_Torso", TimeDelta);



		if (true == m_pModelCom->Get_IsFinishedAnimaion_Second())
		{
			m_IsSecondAnimation = false;
			m_eAnimationState_Next_Second = EPlayerAnimation::End;
			m_eAnimationState_Cur_Second = m_eAnimationState_Next_Second;
		}
	}


	m_pModelCom->Update_CombindTransformationMatrix();

	m_eAnimationState_Cur		 = m_eAnimationState_Next;
	m_eAnimationState_Cur_Second = m_eAnimationState_Next_Second;


	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		m_IsCast_ManaBomb = false;
		m_IsCast_Meteor = false;
		m_IsCast_BrainWash_End = false;
		m_IsZoom = false;
	}
}

HRESULT CPlayer::Ready_Component(void* pArg)
{
	GAMEOBJ_DESC Data;
	memcpy(&Data, pArg, sizeof(GAMEOBJ_DESC));

	HRESULT hr = S_OK;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Status_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(ColliderDesc));

	ColliderDesc.vScale = XMFLOAT3(3.f, 3.f, 3.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hit, &ColliderDesc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_MeshLevel_1_Navi"), TEXT("Com_Navi"), (CComponent**)&m_pNaviCom);


	WEAPON_DESC Weapon_Data;
	ZeroMemory(&Weapon_Data, sizeof(WEAPON_DESC));

	lstrcpy(Weapon_Data.szModelName, L"Component_Mesh_Staff_1");

	m_pWeapon = CStaff_Basic::Create(m_pDevice, m_pDevice_Context);
	m_pWeapon->NativeConstruct(&Weapon_Data);


	// BLOCKADE TOWER
	TOWER_DESC TowerData;
	_vector vPos = m_pMovementCom->Get_State(EState::Position) + m_pMovementCom->Get_State(EState::Look) * 5.f;
	lstrcpy(TowerData.szModelName, L"Component_Mesh_BlockcadeTower");
	TowerData.eTowerRange = ETowerRange::Half;
	TowerData.MoveState_Desc.fRotatePerSec = 0.5f;
	TowerData.MoveState_Desc.vScale = _float4(1.f, 1.f, 1.f, 0.f);
	XMStoreFloat4(&TowerData.MoveState_Desc.vPos, vPos);
	m_CreateTower_Desc.vPos = TowerData.MoveState_Desc.vPos;
	TowerData.Stat_Desc.iHp_Max = 400;

	m_pBlockadeTower = CBlockadeTower::Create(m_pDevice, m_pDevice_Context);
	m_pBlockadeTower->NativeConstruct(&TowerData);
	m_pBlockadeTower->Set_OnEnemyCheck(false);
	m_pBlockadeTower->Set_IsSpawn(false);

	// STRIKER
	TowerData.Stat_Desc.iHp_Max = 200;
	lstrcpy(TowerData.szModelName, L"Component_Mesh_StrikerTower");
	TowerData.eTowerRange = ETowerRange::Quarter;

	m_pStrikerTower = CStrikerTower::Create(m_pDevice, m_pDevice_Context);
	m_pStrikerTower->NativeConstruct(&TowerData);
	m_pStrikerTower->Set_OnEnemyCheck(false);
	m_pStrikerTower->Set_IsSpawn(false);

	// LIGHTNING
	TowerData.Stat_Desc.iHp_Max = 130;
	lstrcpy(TowerData.szModelName, L"Component_Mesh_LightningTower");
	TowerData.eTowerRange = ETowerRange::Pi;

	m_pLightningTower = CLightningTower::Create(m_pDevice, m_pDevice_Context);
	m_pLightningTower->NativeConstruct(&TowerData);
	m_pLightningTower->Set_OnEnemyCheck(false);
	m_pLightningTower->Set_IsSpawn(false);

	return S_OK;
}

_float CPlayer::Animation_Term(EPlayerAnimation eNextAnimation)
{
	EPlayerAnimation NewtAnimaion = EPlayerAnimation::EndKey;

	switch (eNextAnimation)
	{
	case Client::EPlayerAnimation::CallOut: 
		NewtAnimaion = EPlayerAnimation::ChargeMax;
		break;
	case Client::EPlayerAnimation::ChargeMax:
		NewtAnimaion = EPlayerAnimation::ChargeMax_KnockBack;
		break;
	case Client::EPlayerAnimation::ChargeMax_KnockBack:
		NewtAnimaion = EPlayerAnimation::ChargeMin;
		break;
	case Client::EPlayerAnimation::ChargeMin:
		NewtAnimaion = EPlayerAnimation::ChargeMin_KnockBack;
		break;
	case Client::EPlayerAnimation::ChargeMin_KnockBack:
		NewtAnimaion = EPlayerAnimation::Death;
		break;
	case Client::EPlayerAnimation::Death:
		NewtAnimaion = EPlayerAnimation::Detonate;
		break;
	case Client::EPlayerAnimation::Detonate:
		NewtAnimaion = EPlayerAnimation::Fire;
		break;
	case Client::EPlayerAnimation::Fire:
		NewtAnimaion = EPlayerAnimation::FireMaxPower;
		break;
	case Client::EPlayerAnimation::FireMaxPower:
		NewtAnimaion = EPlayerAnimation::Heal;
		break;
	case Client::EPlayerAnimation::Heal:
		NewtAnimaion = EPlayerAnimation::Hurt;
		break;
	case Client::EPlayerAnimation::Hurt:
		NewtAnimaion = EPlayerAnimation::Idle;
		break;
	case Client::EPlayerAnimation::Idle:
		NewtAnimaion = EPlayerAnimation::Idle_lowHp;
		break;
	case Client::EPlayerAnimation::Idle_lowHp:
		NewtAnimaion = EPlayerAnimation::Jump;
		break;
	case Client::EPlayerAnimation::Jump:
		NewtAnimaion = EPlayerAnimation::Jump_Falling;
		break;
	case Client::EPlayerAnimation::Jump_Falling:
		NewtAnimaion = EPlayerAnimation::KnockBack;
		break;
	case Client::EPlayerAnimation::KnockBack:
		NewtAnimaion = EPlayerAnimation::LevelUp;
		break;
	case Client::EPlayerAnimation::LevelUp:
		NewtAnimaion = EPlayerAnimation::Lose;
		break;
	case Client::EPlayerAnimation::Lose:
		NewtAnimaion = EPlayerAnimation::ManaBomb;
		break;
	case Client::EPlayerAnimation::ManaBomb:
		NewtAnimaion = EPlayerAnimation::Move_Backward;
		break;
	case Client::EPlayerAnimation::Move_Backward:
		NewtAnimaion = EPlayerAnimation::Move_Left;
		break;
	case Client::EPlayerAnimation::Move_Left:
		NewtAnimaion = EPlayerAnimation::Move_Right;
		break;
	case Client::EPlayerAnimation::Move_Right:
		NewtAnimaion = EPlayerAnimation::PickupItem;
		break;
	case Client::EPlayerAnimation::PickupItem:
		NewtAnimaion = EPlayerAnimation::Repair;
		break;
	case Client::EPlayerAnimation::Repair:
		NewtAnimaion = EPlayerAnimation::RunForward;
		break;
	case Client::EPlayerAnimation::RunForward:
		NewtAnimaion = EPlayerAnimation::Spawn;
		break;
	case Client::EPlayerAnimation::Spawn:
		NewtAnimaion = EPlayerAnimation::Summon;
		break;
	case Client::EPlayerAnimation::Summon:
		NewtAnimaion = EPlayerAnimation::Summon_Place;
		break;
	case Client::EPlayerAnimation::Summon_Place:
		NewtAnimaion = EPlayerAnimation::Summon_Start;
		break;
	case Client::EPlayerAnimation::Summon_Start:
		NewtAnimaion = EPlayerAnimation::Summon_Stop;
		break;
	case Client::EPlayerAnimation::Summon_Stop:
		NewtAnimaion = EPlayerAnimation::Turn_Left;
		break;
	case Client::EPlayerAnimation::Turn_Left:
		NewtAnimaion = EPlayerAnimation::Upgrade;
		break;
	case Client::EPlayerAnimation::Upgrade:
		NewtAnimaion = EPlayerAnimation::Wave_Start;
		break;
	case Client::EPlayerAnimation::Wave_Start:
		NewtAnimaion = EPlayerAnimation::Win;
		break;
	case Client::EPlayerAnimation::Win:
		NewtAnimaion = EPlayerAnimation::WinWave;
		break;
	case Client::EPlayerAnimation::WinWave:
		NewtAnimaion = EPlayerAnimation::EndKey;
		break;
	default:
		NewtAnimaion = EPlayerAnimation::Idle_lowHp;
		m_eAnimationState_Next = EPlayerAnimation::Idle;
		break;
	}

	_float fAnimationTime = (_float)NewtAnimaion - (_float)eNextAnimation;

	return fAnimationTime - 1.f;
}

void CPlayer::Skill_ManaBomb()
{
	if (EPlayerAnimation::ManaBomb == m_eAnimationState_Next)
	{
		_float fAnimTime = m_pModelCom->Get_AnimTime();

		if (true == m_IsZoom && 700 == (_uint)fAnimTime)
		{
			static_cast<CCamera_Target*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera"))->Set_ZoomOut(110.f);
			m_IsZoom = false;
		}

		if (false == m_IsCast_ManaBomb && 720 == (_uint)fAnimTime)
		{
			_vector vPos = m_pMovementCom->Get_State(EState::Position) + m_pMovementCom->Get_State(EState::Up) * 13.f;
			BULLET_DESC Data;
			Data.fLifeTime = 0.85f;
			lstrcpy(Data.szModelName, L"Component_Mesh_Skill_ManaBomb");
			XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
			Data.MoveState_Desc.fSpeedPerSec = 3.f;
			Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

			Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Direct;
			Data.Attack_Collide_Desc.Attack_Desc.iDamage = 0;
			Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
			Data.Attack_Collide_Desc.vScale = { 3.f, 3.f, 3.f };
			Data.Attack_Collide_Desc.IsCenter = true;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Skill_ManaBomb", (_uint)ELevel::Stage1, L"Layer_Skill", &Data);

			m_IsCast_ManaBomb = true;
		}
	}
}

void CPlayer::Skill_Meteor()
{
	if (EPlayerAnimation::FireMaxPower == m_eAnimationState_Next)
	{
		_float fAnimTime = m_pModelCom->Get_AnimTime();

		if (false == m_IsCast_Meteor && 242 == (_uint)fAnimTime)
		{
			_vector vDir = XMVector3Normalize(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) + XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * -1.f);
			_vector vPos = m_pMovementCom->Get_State(EState::Position) + m_pMovementCom->Get_State(EState::Up) * 13.f;
			BULLET_DESC Data;
			Data.fLifeTime = 2.f;
			lstrcpy(Data.szModelName, L"Component_Mesh_Skill_Meteor");
			XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
			XMStoreFloat3(&Data.vDir, vDir);
			Data.MoveState_Desc.fSpeedPerSec = 30.f;
			Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

			Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
			Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
			Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
			Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
			Data.Attack_Collide_Desc.IsCenter = true;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Skill_Meteor", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);

			m_IsCast_Meteor = true;
		}
	}
}

void CPlayer::Skill_BrainWash()
{
	//if (0 < m_fBrainWash_Time_Max && false == m_IsChargeSkill_Change)
	//{
	//	m_IsChargeSkill_Change = true;
	//	m_fChargeSkill = m_fBrainWash_Time_Max;
	//}

	if (EPlayerAnimation::ChargeMax == m_eAnimationState_Next)
	{
		_float fAnimTime = m_pModelCom->Get_AnimTime();

		if (false == m_IsCast_BrainWash && 37 == (_uint)fAnimTime /*&& 5 == (_uint)m_fChargeSkill * 10*/)
		{
			//m_eAnimationState_Next = EPlayerAnimation::KnockBack;


			// Dir은 나중에 네비메쉬가 추가되면 바꾼다 
			_vector vDir = XMVector3Normalize(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) + XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * -1.f);
			_vector vPos = m_pWeapon->Get_State(EState::Position);
			vPos += XMVector3Normalize(m_pWeapon->Get_State(EState::Look)) * -8.f;

			BULLET_DESC Data;
			Data.fLifeTime = 2.f;
			lstrcpy(Data.szModelName, L"Component_Mesh_Skill_BrainWash");
			XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
			XMStoreFloat3(&Data.vDir, vDir);
			Data.MoveState_Desc.fSpeedPerSec = 30.f;
			Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };

			Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
			Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
			Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
			Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
			Data.Attack_Collide_Desc.IsCenter = true;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Skill_BrainWash", (_uint)ELevel::Stage1, L"Layer_Bullet_BrainWash", &Data);


			m_IsCast_BrainWash = true;
			//m_IsCast_BrainWash_End = false;
		}
	}

	//if (false == m_IsCast_BrainWash_End && true == m_IsCast_BrainWash)
	//{
	//	if (EPlayerAnimation::ChargeMax == m_eAnimationState_Next)
	//	{
	//
	//		_float fAnimTime = m_pModelCom->Get_AnimTime();
	//
	//		if (false == m_IsCast_BrainWash_End && 536 == (_uint)fAnimTime)
	//		{
	//			m_IsCast_BrainWash_End = true;
	//
	//			// Dir은 나중에 네비메쉬가 추가되면 바꾼다 
	//			_vector vDir = XMVector3Normalize(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) + XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * -1.f);
	//			_vector vPos = m_pWeapon->Get_State(EState::Position);
	//			BULLET_DESC Data;
	//			Data.fLifeTime = 2.f;
	//			lstrcpy(Data.szModelName, L"Component_Mesh_Skill_BrainWash");
	//			XMStoreFloat4(&Data.MoveState_Desc.vPos, vPos);
	//			XMStoreFloat3(&Data.vDir, vDir);
	//			Data.MoveState_Desc.fSpeedPerSec = 30.f;
	//			Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	//
	//			Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
	//			Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
	//			Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
	//			Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
	//			Data.Attack_Collide_Desc.IsCenter = true;
	//
	//			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Skill_BrainWash", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);
	//		}
	//	}
	//}
}

void CPlayer::Skill_Healing(_float TimeDelta)
{
	if (EPlayerAnimation::Heal == m_eAnimationState_Next)
	{
		m_fHealTime += TimeDelta;

		if (0.125 <= m_fHealTime)
		{
			m_fHealTime = 0.f;

			_int iHp = m_pStatusCom->Get_Hp() + 5/*(m_iHealSize / 500.f)*/;

			if ((_int)iHp > m_pStatusCom->Get_HpMax())
				iHp = m_pStatusCom->Get_HpMax();

			m_pStatusCom->Set_Hp((_int)iHp);

		}
	}
	//else
	//	m_fHealTime = 0.f;
}

void CPlayer::SpecialAnimation_Check(_float TimeDelta)
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	if (false == m_IsAttack)
	{
		if(false == m_IsCharging)
			m_eAnimationState_Next = EPlayerAnimation::Idle;
	}


	if (true == m_IsAttack && true == IsFinished)
	{
		m_eAnimationState_Next = EPlayerAnimation::Idle;
		m_IsAttack = false;
	}

	if (true == m_IsCharging)
	{
		m_fChargeSkill -= TimeDelta;
		if (0.f >= m_fChargeSkill)
		{
			m_IsAttack = false;
			m_fChargeSkill = 0.f;
			m_IsCharging = false;
			m_IsCast_Healing = false;
			m_IsCast_BrainWash = false;
			m_eAnimationState_Next = EPlayerAnimation::Idle;
		}
	}

	if (ETowerSpawn::End != m_eTowerSpawn)
	{
		if (ETowerSpawn::Locate == m_eTowerSpawn && true == IsFinished)
		{
			m_eAnimationState_Next = EPlayerAnimation::Summon;
		}


	}

}

void CPlayer::Casting_Move_Check(_float* TimeDelta)
{
	if (true == m_IsCast_Healing)
	{
		if (false == m_IsCasting_Move)
			*TimeDelta *= 0.4f;
		m_IsCasting_Move = true;
	}
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPlayer* pInstance = new CPlayer(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPlayer) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CPlayer) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pColliderCom_Hit);
	Safe_Release(m_pNaviCom);

	Safe_Release(m_pBlockadeTower);
	Safe_Release(m_pStrikerTower);
	Safe_Release(m_pLightningTower);

	Safe_Release(m_pWeapon);

	__super::Free();
}
