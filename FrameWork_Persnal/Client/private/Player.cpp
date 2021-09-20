#include "stdafx.h"
#include "..\public\Player.h"
#include "PlayerSkill.h"
#include "Data_Manager.h"
#include "Camera_Target.h"
#include "StrikerTower.h"

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

	return S_OK;
}

_int CPlayer::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	Turn_Check(TimeDelta);
	Key_Check(TimeDelta);


	//static_cast<CCamera*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera_Free"))->TargetRotate_Check(m_pMovementCom);

	if (nullptr != m_pStrikerTower)
		m_pStrikerTower->Tick(TimeDelta);

	return _int();
}

_int CPlayer::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	Level_Check();
	
	Animation_Check(TimeDelta);

	if (nullptr != m_pStrikerTower)
	{
		_vector vPos = m_pMovementCom->Get_State(EState::Position);
		_vector vDir = m_pMovementCom->Get_State(EState::Look);
		vPos += vDir * 5.f;

		m_pStrikerTower->Set_TowerPos(vPos);

		//m_pStrikerTower->
	}


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CPlayer::Render()
{ 
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

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


	return S_OK;
	if (nullptr != m_pStrikerTower)
	{
		m_pStrikerTower->Render();
	}

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
	m_eAnimationState_Next = EPlayerAnimation::Idle;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (nullptr != m_pStrikerTower)
		{
			Safe_Release(m_pStrikerTower);
			m_IsTowerPick = false;
		}
	}

	_long dwMouseMove = 0;

	if (dwMouseMove = GET_MOUSE_X)
	{
		m_eAnimationState_Next = EPlayerAnimation::Turn_Left;
	}

	if (dwMouseMove = GET_MOUSE_Y)
	{

	}

	if (GET_KEY_INPUT(DIK_8))
	{
		//m_IsTowerPick = true;
		static_cast<CCamera*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera_Free"))->Set_CameraView_Mode(ECameraViewMode::TopView);

		TOWER_DESC Data;
		lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower");
		Data.eTowerRange = ETowerRange::Quarter;
		XMStoreFloat4(&Data.MoveState_Desc.vPos, m_pMovementCom->Get_State(EState::Position));

		m_pStrikerTower = CStrikerTower::Create(m_pDevice, m_pDevice_Context);
		m_pStrikerTower->NativeConstruct(&Data);
	}

	if (GET_KEY_INPUT(DIK_W))
	{
		m_pMovementCom->Go_LookDir(TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::RunForward;
	}

	if (GET_KEY_INPUT(DIK_S))
	{
		m_pMovementCom->Go_LookDir(-TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::Move_Backward;
	}

	if (GET_KEY_INPUT(DIK_A))
	{
		m_pMovementCom->Go_Left(TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::Move_Left;
	}

	if (GET_KEY_INPUT(DIK_D))
	{
		m_pMovementCom->Go_Right(TimeDelta);
		m_eAnimationState_Next = EPlayerAnimation::Move_Right;
	}

	if (GET_KEY_INPUT(DIK_1))
	{
		static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"))->Set_Skill_CoolDown(0, 10.f);
	}

	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_eAnimationState_Next = EPlayerAnimation::Fire;


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
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnimationState_Next, Animation_Term());

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Set_AnimationIndex_Start_SecondNode( "b_HipL", TimeDelta, (_float)EPlayerAnimation::Fire, (_float)EPlayerAnimation::FireMaxPower - 1.f, 1.f);


	m_pModelCom->Update_CombindTransformationMatrix();

	m_eAnimationState_Cur = m_eAnimationState_Next;
}

HRESULT CPlayer::Ready_Component(void* pArg)
{
	GAMEOBJ_DESC Data;
	memcpy(&Data, pArg, sizeof(GAMEOBJ_DESC));

	HRESULT hr = S_OK;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Status_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom, &Data.Status_Desc);


	return S_OK;
}

_float CPlayer::Animation_Term()
{
	EPlayerAnimation NewtAnimaion = EPlayerAnimation::EndKey;

	switch (m_eAnimationState_Next)
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

	_float fAnimationTime = (_float)NewtAnimaion - (_float)m_eAnimationState_Next;

	return fAnimationTime - 1.f;
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

	Safe_Release(m_pStrikerTower);

	__super::Free();
}
