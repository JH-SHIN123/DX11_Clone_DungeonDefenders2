#include "stdafx.h"
#include "..\public\Monster.h"
#include "Masking_MeterBar_3D.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CMonster::Tick(_float TimeDelta)
{
	if (nullptr != m_pMeterBar_Hp)
	{
		_vector vMyPos = m_pMovementCom->Get_State(EState::Position) + XMLoadFloat3(&m_vHpBar_OffSet_Position);

		m_pMeterBar_Hp->Set_Position(vMyPos);

		m_pMeterBar_Hp->Tick(TimeDelta);
	}




	return _int();
}

_int CMonster::Late_Tick(_float TimeDelta)
{
	if (nullptr != m_pMeterBar_Hp && 0 < m_pStatusCom->Get_Hp())
	{
		m_pMeterBar_Hp->Set_Count((_float)m_pStatusCom->Get_Hp(), (_float)m_pStatusCom->Get_HpMax());
		m_pMeterBar_Hp->Late_Tick(TimeDelta);
	}



	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CMonster::Render()
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


#ifdef _DEBUG
	//m_pColliderCom->Render_Collider();
#endif


	return S_OK;
}

EMonsterAI CMonster::AI_Check(_float TimeDelta, _vector* pTargetPos, _bool IsContinueAnimation)
{
	// 먼저 플레이어 거리 탐색
	if (EMonsterAI::Attack == m_eAI_Next && IsContinueAnimation)
		return EMonsterAI::Attack;


	if (0 >= m_pStatusCom->Get_Hp())
		return EMonsterAI::Dead;

	CMovement* pTarget = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Get_Component(L"Com_Movement"));

	if (nullptr == pTarget)
		return EMonsterAI::End;

	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	_vector vTargetPos = pTarget->Get_State(EState::Position);

	_float fDis = XMVectorGetX(XMVector3Length(vTargetPos - vMyPos));

	if (m_fAttackDis > fDis)
	{
		return m_eAI_Next = EMonsterAI::Attack;
	}


	if (m_fDetectDis > fDis)
	{
		_vector vDir = XMVector3Normalize(vTargetPos - vMyPos);
		_float fTurnAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)), vDir))));

		*pTargetPos = vTargetPos;

		if (80.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 2.f, vTargetPos);

			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (60.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 1.5f, vTargetPos);
			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (40.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta * 1.5f, vTargetPos);
			//m_pMovementCom->Go_Dir(TimeDelta, vTargetPos);
			return m_eAI_Next = EMonsterAI::Turn;
		}
		else if (20.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta* 1.5f, vTargetPos);
			m_pMovementCom->Go_Dir(TimeDelta, vTargetPos);
			return m_eAI_Next = EMonsterAI::Move;
		}
		else if (10.f < fTurnAngle)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, vTargetPos);
			m_pMovementCom->Go_Dir(TimeDelta, vTargetPos);
			return m_eAI_Next = EMonsterAI::Move;
		}



		m_pMovementCom->Go_Dir(TimeDelta, vTargetPos);
		return m_eAI_Next = EMonsterAI::Move;

	}













	CLayer* pLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1, L"Layer_Tower");
	if (nullptr == pLayer)
		return m_eAI_Next = EMonsterAI::Idle;

	// AI를 구상 해보자
	/*
	1. 일반적으로 몬스터는 특정 경로를 따라간다.
		 A_1. 그러다가 플레이어에게 공격을 맞으면 플레이어를 따라간다.
			A_2. 너무 멀어지면 다시 경로를 찾아 간다.
		B_1. 타워를 만나면 타워를 때린다.
			A_1. 플레이어한테 공격을 맞으면 플레이어를 따라간다.
				A_2. 너무 멀어지면 다시 경로를 찾아 간다.

	
	*/




	list<CGameObject*> listObject = pLayer->Get_GameObject_List();


	for (auto& iter : listObject)
	{

	}





	return m_eAI_Next = EMonsterAI::Idle;
}

HRESULT CMonster::Ready_Component(void * pArg)
{
	MONSTER_DESC Data;
	memcpy(&Data, pArg, sizeof(MONSTER_DESC));
	
	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)Data.eLevel, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);


	MASK_METERBAR_DESC_3D HP_Bar;
	HP_Bar.eFillMode = EMeterBar_FillMode::ZeroToFull;
	HP_Bar.eFrame_Render = ECastingBar_Frame_Render::Second;
	HP_Bar.HasFrameBar = true;
	HP_Bar.fCount = 100.f;
	HP_Bar.fCount_Max = 100.f;
	HP_Bar.UI_Desc.eLevel = ELevel::Static;
	HP_Bar.UI_Desc.Movement_Desc.vScale = { 8.f, 2.f, 0.1f, 0.f };
	HP_Bar.UI_Desc.Movement_Desc.vPos = { 0.f, 0.f, 0.f, 1.f };
	lstrcpy(HP_Bar.UI_Desc.szTextureName, L"Component_Texture_ExpBar");

	m_pMeterBar_Hp = CMasking_MeterBar_3D::Create(m_pDevice, m_pDevice_Context, &HP_Bar);


	m_fDetectDis = Data.fDetectDis;
	m_fAttackDis = Data.fAttackDis;

	if (S_OK != hr)
		MSG_BOX("CMonster::Ready_Component Failed");

	return hr;
}

CGameObject * CMonster::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMonster::Free()
{
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);

	Safe_Release(m_pMeterBar_Hp);

	__super::Free();
}
