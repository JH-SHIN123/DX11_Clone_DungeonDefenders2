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

	m_pModelCom->Set_AnimationIndex(0); // 애니메이션 인덱스는 한개밖에 안들어가는 상황
	m_pModelCom->Set_CurrentTime(38.f);

	return S_OK;
}

_int CPlayer::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	Key_Check(TimeDelta);

	if (GetAsyncKeyState('V') & 0x8000)
		m_pStatusCom->Add_Exp(10);


	//static_cast<CCamera*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Camera_Free"))->TargetRotate_Check(m_pMovementCom);

	if (nullptr != m_pStrikerTower)
		m_pStrikerTower->Tick(TimeDelta);

	return _int();
}

_int CPlayer::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	Level_Check();
	
	if (nullptr != m_pStrikerTower)
	{
		_vector vPos = m_pMovementCom->Get_State(EState::Position);
		_vector vDir = m_pMovementCom->Get_State(EState::Look);
		vPos += vDir * 5.f;

		m_pStrikerTower->Set_TowerPos(vPos);

		//m_pStrikerTower->
	}

	if (nullptr != m_pModelCom)
		m_pModelCom->Update_CombindTransformationMatrix(TimeDelta, 181.f, 100.f);

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
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (nullptr != m_pStrikerTower)
		{
			Safe_Release(m_pStrikerTower);
			m_IsTowerPick = false;
		}
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
		m_pMovementCom->Go_LookDir(TimeDelta);

	if (GET_KEY_INPUT(DIK_S))
		m_pMovementCom->Go_LookDir(-TimeDelta);

	if (GET_KEY_INPUT(DIK_A))
		m_pMovementCom->Go_Left(TimeDelta);

	if (GET_KEY_INPUT(DIK_D))
		m_pMovementCom->Go_Right(TimeDelta);

	if (GET_KEY_INPUT(DIK_1))
	{
		static_cast<CPlayerSkill*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_UI"))->Set_Skill_CoolDown(0, 10.f);
	}

}

void CPlayer::Turn_Check(_float TimeDelta)
{
	_long dwMouseMove = 0;

	if (dwMouseMove = GET_MOUSE_X)
	{
		
	}

	if (dwMouseMove = GET_MOUSE_Y)
	{

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
