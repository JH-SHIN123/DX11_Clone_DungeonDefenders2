#include "stdafx.h"
#include "..\public\Monster_Gate.h"
#include "Data_Manager.h"

CMonster_Gate::CMonster_Gate(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster_Gate::CMonster_Gate(const CMonster_Gate & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster_Gate::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Gate::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	m_pModelCom_Left->Set_AnimationIndex(0);
	m_pModelCom_Left->Set_AnimationIndex_Start(0.f, 1.f);

	m_pModelCom_Right->Set_AnimationIndex(0);
	m_pModelCom_Right->Set_AnimationIndex_Start(0.f, 1.f);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
	Set_Pivot_Rotate(XMVectorSet(0.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CMonster_Gate::Tick(_float TimeDelta)
{

	return _int();
}

_int CMonster_Gate::Late_Tick(_float TimeDelta)
{
	Phase_Check();
	Anim_Check(TimeDelta);
	PhaseMonster_Info_Check();

	Spawn_Monster_Check(TimeDelta);

	if(EPhaseState::Build == m_ePhase_Next)
		m_pPhaseMonster_Info->Late_Tick(TimeDelta);

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority, this);
}

HRESULT CMonster_Gate::Render()
{

	if (nullptr == m_pModelCom_Left)
		return S_OK;

	m_pModelCom_Left->Bind_VIBuffer();

	m_pModelCom_Left->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom_Left->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom_Left->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom_Left->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom_Left->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom_Left->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom_Left->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom_Left->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom_Left->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));


	m_pModelCom_Left->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	_uint iNumMaterials = m_pModelCom_Left->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom_Left->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom_Left->Render_Model(i, 0);


	}


	m_pModelCom_Right->Bind_VIBuffer();

	m_pModelCom_Right->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom_Right->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom_Right->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	m_pModelCom_Right->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom_Right->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom_Right->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom_Right->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom_Right->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom_Right->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));


	m_pModelCom_Right->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom_Right->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom_Right->Render_Model(i, 0);
	}

	return S_OK;
}

HRESULT CMonster_Gate::Ready_Component(void * pArg)
{
	GATE_DESC Data;
	memcpy(&Data, pArg, sizeof(GATE_DESC));

	HRESULT  hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveDesc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Mesh_Monster_Gate_Left"), TEXT("Com_Mesh_Door_Left"), (CComponent**)&m_pModelCom_Left);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Mesh_Monster_Gate_Right"), TEXT("Com_Mesh_Door_Right"), (CComponent**)&m_pModelCom_Right);

	UI3D_DESC UI_Desc;
	UI_Desc.eLevel = ELevel::Stage1;
	UI_Desc.Movement_Desc.vPos = Data.MoveDesc.vPos;
	UI_Desc.Movement_Desc.vScale = { 160.f, 160.f, 0.f, 0.f };
	lstrcpy(UI_Desc.szTextureName, L"Component_Texture_PhaseMonster_Info");

	m_pPhaseMonster_Info = CPhaseMonster_Info::Create(m_pDevice, m_pDevice_Context, &UI_Desc);

	m_ePath = Data.StartPath;
	
	if (hr != S_OK)
		MSG_BOX("CMonster_Gate::Ready_Component");

	return hr;
}

void CMonster_Gate::Anim_Check(_float TimeDelta)
{
	_float fAnim_Term = 0.f;
	_float fAnim = 0.f;

	if (m_eAnim_Cur != m_eAnim_Next)
	{
		EMonster_Gate_Anim eAnim;
		m_IsAnimated = true;
		m_IsAnimated_Once = false;
		m_IsChangedAnim = true;
		switch (m_eAnim_Next)
		{
		case EMonster_Gate_Anim::Closed:
			m_IsAnimated = false;
			eAnim = EMonster_Gate_Anim::Closing;
			break;
		case EMonster_Gate_Anim::Closing:
			eAnim = EMonster_Gate_Anim::Idle;
			break;
		case EMonster_Gate_Anim::Idle:
			eAnim = EMonster_Gate_Anim::Open;
			break;
		case EMonster_Gate_Anim::Open:
			m_IsAnimated = false;
			eAnim = EMonster_Gate_Anim::Opening;
			break;
		case EMonster_Gate_Anim::Opening:
			eAnim = EMonster_Gate_Anim::End_Anim;
			break;
		default:
			break;
		}

		fAnim_Term = ((_float)eAnim - (_float)m_eAnim_Next) - 1.f;

		m_pModelCom_Left->Set_AnimationIndex_Start((_float)m_eAnim_Next, fAnim_Term);
		m_pModelCom_Right->Set_AnimationIndex_Start((_float)m_eAnim_Next, fAnim_Term);

		m_eAnim_Cur = m_eAnim_Next;
	}
	
	if (m_IsChangedAnim == true)
	{
		m_pModelCom_Left->Update_AnimaionMatrix(TimeDelta);
		m_pModelCom_Left->Update_CombindTransformationMatrix();

		m_pModelCom_Right->Update_AnimaionMatrix(TimeDelta);
		m_pModelCom_Right->Update_CombindTransformationMatrix();
	}

	_bool IsEnd = m_pModelCom_Left->Get_IsFinishedAnimaion();
	_float fAnimTime = m_pModelCom_Left->Get_AnimTime();

	if (IsEnd)
		int i = 0;

	// 애니메이션 끝나면 재생X, 바뀔때만 재생하도록

	if (m_eAnim_Cur == m_eAnim_Next)
	{
		if (33 == (_uint)fAnimTime && EMonster_Gate_Anim::Closing == m_eAnim_Next)
			m_IsChangedAnim = false;

		if (69 == (_uint)fAnimTime && EMonster_Gate_Anim::Opening == m_eAnim_Next)
			m_IsChangedAnim = false;
	}
}

void CMonster_Gate::Phase_Check()
{
	m_ePhase_Next = CData_Manager::GetInstance()->Get_NowPhase();

	if (m_ePhase_Cur != m_ePhase_Next)
	{
		switch (m_ePhase_Next)
		{
		case Client::EPhaseState::Clear:
		case Client::EPhaseState::Build:
			m_eAnim_Next = EMonster_Gate_Anim::Closing;
			m_IsChangedAnim = true;
			break;
		case Client::EPhaseState::Combat:
		case Client::EPhaseState::Boss:
			m_eAnim_Next = EMonster_Gate_Anim::Opening;
			m_IsChangedAnim = true;
			break;
		default:
			m_eAnim_Next = EMonster_Gate_Anim::Idle;
			break;
		}
		m_ePhase_Cur = m_ePhase_Next;
	}
}

void CMonster_Gate::Spawn_Monster_Check(_float TimeDelta)
{
	if (EPhaseState::Combat != m_ePhase_Next && EPhaseState::Boss != m_ePhase_Next)
	{
		m_iSpawnCount = 0;
		return;
	}

	// 웨이브 시작!
	m_fSpawnTime += TimeDelta;
	if (m_fSpawnTime >= m_fSpawnTime_Max)
	{
		// 시간 됬다

		if (m_iSpawnCount < m_SpawnMonster.iMonsterCount[(_uint)m_eSpawnList])
		{
			m_fSpawnTime = 0.f;

			Spawn_Monster();
		}

		m_eSpawnList = EMonster_List((_uint)m_eSpawnList + 1);

		if (EMonster_List::End <= m_eSpawnList)
		{
			m_eSpawnList = EMonster_List(0);
			++m_iSpawnCount;
		}
	}
}

void CMonster_Gate::Spawn_Monster()
{
	_tchar szPrototypeName[MAX_PATH] = L"";
	tagMonsterDesc MonData;

	MonData.eLevel = ELevel::Stage1;
	MonData.eMovePath = m_ePath;
	MonData.Movement_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
	MonData.Movement_Desc.fRotatePerSec = XMConvertToRadians(120.f);
	MonData.Movement_Desc.fSpeedPerSec = 10.f;

	switch (m_eSpawnList)
	{
	case EMonster_List::Goblin:
		MonData.fAttackDis = 8.f;
		MonData.fDetectDis = 15.f;
		MonData.Stat_Desc.iExp = 15;
		MonData.Stat_Desc.iHp_Max = 300;
		MonData.Movement_Desc.fSpeedPerSec = 15.f;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Goblin");
		lstrcpy(szPrototypeName, L"Prototype_Goblin");
		break;
	case EMonster_List::Ogre:
		MonData.fAttackDis = 10.f;
		MonData.fDetectDis = 15.f;
		MonData.Stat_Desc.iExp = 53;
		MonData.Stat_Desc.iHp_Max = 500;
		MonData.Movement_Desc.fSpeedPerSec = 9.f;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Ogre");
		lstrcpy(szPrototypeName, L"Prototype_Ogre");
		break;
	case EMonster_List::Kamikaze:
		MonData.fAttackDis = 40.f;
		MonData.fDetectDis = 40.f;
		MonData.Stat_Desc.iExp = 20;
		MonData.Stat_Desc.iHp_Max = 200;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Kobold");
		lstrcpy(szPrototypeName, L"Prototype_Kobold");
		break;
	case EMonster_List::Boss:
		MonData.fAttackDis = 10.f;
		MonData.fDetectDis = 15.f;
		MonData.Stat_Desc.iExp = 999;
		MonData.Stat_Desc.iHp_Max = 1200;
		lstrcpy(MonData.szModelName, L"Component_Mesh_Boss_Djinn");
		lstrcpy(szPrototypeName, L"Prototype_Boss_Djinn");
		break;
	default:
		m_eSpawnList = EMonster_List(0);
		break;
	}

	MonData.Stat_Desc.iHp = MonData.Stat_Desc.iHp_Max;

	GET_GAMEINSTANCE->Add_GameObject((_uint)MonData.eLevel, szPrototypeName, (_uint)MonData.eLevel, L"Layer_Monster", &MonData);

}

void CMonster_Gate::PhaseMonster_Info_Check()
{
	_vector vPos = m_pMovementCom->Get_State(EState::Position) - XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * 3.f;

	m_pPhaseMonster_Info->Set_Pos(vPos);
	
}

void CMonster_Gate::Set_PhaseMonster_Info(const PHASEINFO_DESC & PhaseMonster_Info)
{
	m_pPhaseMonster_Info->Set_PhaseInfo(PhaseMonster_Info);
	m_SpawnMonster = PhaseMonster_Info;
}

CMonster_Gate * CMonster_Gate::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMonster_Gate* pInstance = new CMonster_Gate(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMonster_Gate) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMonster_Gate::Clone_GameObject(void * pArg)
{
	CMonster_Gate* pInstance = new CMonster_Gate(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMonster_Gate) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster_Gate::Free()
{
	Safe_Release(m_pModelCom_Left);
	Safe_Release(m_pModelCom_Right);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);

	Safe_Release(m_pPhaseMonster_Info);

	__super::Free();
}
