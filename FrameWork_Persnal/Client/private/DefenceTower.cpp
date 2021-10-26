#include "stdafx.h"
#include "..\public\DefenceTower.h"
#include "Masking_MeterBar_3D.h"
#include "Point_Spread2.h"
#include "Point_Spread3.h"
#include "Ring_Explosion.h"

CDefenceTower::CDefenceTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CDefenceTower::CDefenceTower(const CDefenceTower & rhs)
	: CGameObject(rhs)
{
}

HRESULT CDefenceTower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CDefenceTower::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);
	//m_eTowerState_Next = ETowerState::Idle;
	return S_OK;
}

_int CDefenceTower::Tick(_float TimeDelta)
{
	if (0 >= m_pStatusCom->Get_Hp())
	{
		Create_Dead_Effect();
		return OBJECT_DEAD;
	}


	m_pHpBar->Tick(TimeDelta);
	Spawn_Check(TimeDelta);
	TowerState_Check();

	return _int();
}

_int CDefenceTower::Late_Tick(_float TimeDelta)
{
	if (ETowerState::Pick == m_eTowerState_Next ||
		ETowerState::Rotate == m_eTowerState_Next ||
		ETowerState::Spawn == m_eTowerState_Next)
		return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);

	m_pHpBar->Set_Count((_float)m_pStatusCom->Get_Hp(), (_float)m_pStatusCom->Get_HpMax());

	if (0.f >= m_pStatusCom->Get_Hp())
	{
		Create_Dead_Effect();
		return OBJECT_DEAD;
	}

	HpBar_Render_Check(TimeDelta);

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CDefenceTower::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	_matrix World = m_pMovementCom->Get_WorldMatrix();
	_matrix View = XMMatrixTranspose(GET_VIEW_SPACE);
	_matrix Proj = XMMatrixTranspose(GET_PROJ_SPACE);

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();
	_int iShaderPass = 0;

	if (ETowerState::Pick == m_eTowerState_Next || 
		ETowerState::Rotate == m_eTowerState_Next || 
		ETowerState::Spawn == m_eTowerState_Next)
	{
		iShaderPass = 7;

		m_pModelCom->Set_Variable("g_vColor", &m_vColor, sizeof(_float4));

		if (ETowerState::Pick == m_eTowerState_Next)
		{
			_vector vRight = XMVector3Normalize(World.r[0]) * 24.f;
			_vector vUp = XMVector3Normalize(World.r[2]) * 24.f;
			_vector vLook = XMVector3Normalize(World.r[1]);
			_vector vPos = World.r[3];

			World.r[0] = vRight;
			World.r[1] = vUp;
			World.r[2] = vLook;
			World.r[3] = vPos;

			m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(World), sizeof(_matrix));
			m_pVIBufferCom->Set_Variable("ViewMatrix", &View, sizeof(_matrix));
			m_pVIBufferCom->Set_Variable("ProjMatrix", &Proj, sizeof(_matrix));

			m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView((_uint)m_eTowerRange));
			m_pVIBufferCom->Render(14);
		}
	}
	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &View, sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &Proj, sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;



		m_pModelCom->Render_Model(i, iShaderPass);
	}

	return S_OK;	
}

void CDefenceTower::Set_TowerLookDir(_fvector vLookDir)
{
	_vector vNewLookDir = XMVector3Normalize(vLookDir) * m_pMovementCom->Get_Scale(EState::Look);
	_vector vRightDir = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vNewLookDir)) * m_pMovementCom->Get_Scale(EState::Right);
	_vector vUpDir = XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_pMovementCom->Get_Scale(EState::Up);

	m_pMovementCom->Set_State(EState::Right, vRightDir);
	m_pMovementCom->Set_State(EState::Up, vUpDir);
	m_pMovementCom->Set_State(EState::Look, vNewLookDir);
}

void CDefenceTower::Set_TowerRotatePosition(_fvector vDir)
{
	_vector vMyLookDir = XMVector3Normalize(XMVectorSetY(vDir, 0.f)) * m_pMovementCom->Get_Scale(EState::Look);
	_vector vRightDir = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vMyLookDir)) ;
	_vector vUpDir = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	if (0 != XMVectorGetX(XMVector3Length(vMyLookDir)))
	{
		m_pMovementCom->Set_State(EState::Right, vRightDir * m_pMovementCom->Get_Scale(EState::Right));
		m_pMovementCom->Set_State(EState::Up, vUpDir * m_pMovementCom->Get_Scale(EState::Up));
		m_pMovementCom->Set_State(EState::Look, vMyLookDir);
	}
}

void CDefenceTower::TowerState_Check()
{
	if (m_eTowerState_Cur != m_eTowerState_Next)
	{
		m_eTowerState_Cur = m_eTowerState_Next;
	}
}

_bool CDefenceTower::Enemy_Check(_float TimeDelta, _vector* vTargetPos)
{
	// 몬스터가 내 공격범위안에 있다.
	// 돌아라

	if (false == m_IsEnemyCheck)
		return false;

	CLayer* pLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1, L"Layer_Monster");
	if (nullptr == pLayer)
		return false;

	list<CGameObject*> listObject = pLayer->Get_GameObject_List();
	if (true == listObject.empty())
		return false;

	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	_float	fTargetDis = -1.f;
	_float	fTurnAngle = 0.f;

	for (auto& iter : listObject)
	{
		CMovement* pMovement = (CMovement*)iter->Get_Component(L"Com_Movement");
		_vector vPos = pMovement->Get_State(EState::Position);

		_vector vDir = vPos - vMyPos;
		_float fDis = XMVectorGetX(XMVector3Length(vDir));

		vDir = XMVector3Normalize(vDir);
		//_float fAngle = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vDir, vMyAngle))));
		_float fAngle_Axis = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vDir, XMVectorSet(1.f,0.f,0.f,0.f)))));

		if (-1.f == fTargetDis)
		{
			fTargetDis = fDis;
			*vTargetPos = vPos;
			fTurnAngle = fAngle_Axis;
		}

		else if (fTargetDis >= fDis)
		{
			if (m_fTowerRangeMin <= fAngle_Axis && fAngle_Axis <= m_fTowerRangeMax)
			{

				fTargetDis = fDis;
				*vTargetPos = vPos;
				fTurnAngle = fAngle_Axis;
			}
		}
	}

	if (m_fTowerRangeMin <= fTurnAngle && fTurnAngle <= m_fTowerRangeMax)
	{
		_float fDstDis = XMVectorGetX(XMVector3Length(*vTargetPos - vMyPos));
		_float fSrcDis = XMVectorGetX(XMVector3Length(*vTargetPos - (vMyPos + XMLoadFloat3(&m_vFirstLook_Dir) * 10.f)));

		//_float fTurnAngle_Break = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vTargetPos, m_pMovementCom->Get_State(EState::Look)))));

		// 여기서 좌우회전 반복을 그만 하고싶음 지금은 회전속도가 아주 적어서 티는 안나지만...
		if (fDstDis > fSrcDis /*&& 25.f < fTurnAngle_Break*/)
		{
			m_pMovementCom->RotateToTargetOnLand_Tick(TimeDelta, *vTargetPos);

			return true;
		}
	}

	return false;
}

void CDefenceTower::Get_TowerDesc(TOWER_DESC * pOutTowerDesc)
{
	memcpy(pOutTowerDesc, &m_TowerDesc, sizeof(TOWER_DESC));

	XMStoreFloat4(&pOutTowerDesc->MoveState_Desc.vPos, m_pMovementCom->Get_State(EState::Position));
	XMStoreFloat4(&pOutTowerDesc->MoveState_Desc.vRotateLook, m_pMovementCom->Get_State(EState::Look));

}

void CDefenceTower::Create_Dead_Effect()
{
	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	vPos.m128_f32[1] += 3.f;

	POINT_SPREAD_DESC_2 Data;
	Data.IsTime = true;
	Data.vSize = { 5.f,5.f };
	Data.Point_Desc.fLifeTime = 2.f;
	Data.Point_Desc.iShaderPass = 0;
	Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Explosion_Smoke");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);

	Data.IsTime = true;
	Data.vSize = { 5.f,5.f };
	Data.Point_Desc.fLifeTime = 2.f;
	Data.Point_Desc.iShaderPass = 1;
	Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Explosion");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);

	Data.IsTime = true;
	Data.vSize = { 4.f,4.f };
	Data.Point_Desc.fLifeTime = 2.f;
	Data.Point_Desc.iShaderPass = 0;
	Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, vPos);
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Smoke");
	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);


	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	if (60.f > fDis)
	{
		fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

		CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING);
		CSound_Manager::GetInstance()->Play_Sound(L"TowerDestroy2.ogg", CHANNEL_ANYTHING);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING, fDis);
	}
}

void CDefenceTower::Upgrade_Tower()
{
	STATUS_DESC Stat = m_pStatusCom->Get_Stat();

	_float fHp_Max = (_float)Stat.iHp_Max;
	fHp_Max *= 1.5f;
	Stat.iHp_Max = (_int)fHp_Max;

	fHp_Max = (_float)Stat.iHp;
	fHp_Max *= 1.5f;
	Stat.iHp = (_int)fHp_Max;

	Stat.iAtt_Basic += 20;

	_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
	if (60.f > fDis)
	{
		fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

		CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING);
		CSound_Manager::GetInstance()->Play_Sound(L"Tower_upgrade.ogg", CHANNEL_ANYTHING);
		CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING, fDis + 0.2f);
	}
	

	m_pStatusCom->Set_Stat(Stat);
}

void CDefenceTower::Healing_Tower()
{
	m_IsHealTic = true;

	m_fHealTime = 0.f;
	m_fHealTime_Total = 0.f;
}

void CDefenceTower::HpBar_Render_Check(_float TimeDelta)
{
	_vector vPlayerPos = static_cast<CMovement*>(GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player")->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);

	_float fDis = XMVectorGetX(XMVector3Length(vPlayerPos - vMyPos));

	if (fDis <= 15.f)
	{
		vMyPos.m128_f32[1] += 2.f;
		m_pHpBar->Set_Position(vMyPos);
		m_pHpBar->Late_Tick(TimeDelta);
	}

}

void CDefenceTower::Healing_Check(_float TimeDelta)
{
	if (false == m_IsHealTic)
	{
		m_IsSound_Heal = false;
		return;
	}

	m_fHealTime += TimeDelta;
	m_fHealTime_Total += TimeDelta;

	if (false == m_IsSound_Heal)
	{
		_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
		if (60.f > fDis)
		{
			m_IsSound_Heal = true;
			fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

			CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING);
			CSound_Manager::GetInstance()->Play_Sound(L"TowerRepair.ogg", CHANNEL_ANYTHING);
			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING, fDis + 0.2f);
		}
	}


	if (0.125f <= m_fHealTime)
	{
		m_fHealTime = 0.f;

		STATUS_DESC Stat = m_pStatusCom->Get_Stat();

		_float fHp_Max	= (_float)Stat.iHp_Max * 0.01f;
		_float fHp		= (_float)Stat.iHp;

		fHp += fHp_Max;

		m_pStatusCom->Set_Stat(Stat);
	}

	if (m_fHealTime_Max <= m_fHealTime_Total)
		m_IsHealTic = false;
}

void CDefenceTower::Spawn_Check(_float TimeDelta)
{
	if (ETowerState::Spawn != m_eTowerState_Next)
		return;

	_vector vColor_Now = XMLoadFloat4(&m_vColor);
	_vector vColor = XMVectorSet(1.f,1.f,1.f,1.f);

	_vector vTerm = vColor - vColor_Now;
	vColor_Now += vTerm * (TimeDelta * 5.f);

	XMStoreFloat4(&m_vColor, vColor_Now);

	m_fSpawnTime -= TimeDelta;
	if (0 >= m_fSpawnTime)
		m_eTowerState_Next = ETowerState::Idle;
}

void CDefenceTower::Set_TowerPos(_fvector vPosition)
{
	//if (ETowerState::Pick != m_eTowerState_Next)
	//	return;

	m_pMovementCom->Set_State(EState::Position, vPosition);
}

void CDefenceTower::Set_TowerRotation(_fvector vRotation)
{
	if (ETowerState::Rotate != m_eTowerState_Next)
		return;

}

HRESULT CDefenceTower::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	memcpy(&m_TowerDesc, pArg, sizeof(TOWER_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, L"Component_VIBuffer_Rect", TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_TowerDesc.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &m_TowerDesc.Stat_Desc);
	
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_TowerRange"), TEXT("Com_Textures"), (CComponent**)&m_pTexturesCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_TowerDesc.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);


	XMStoreFloat3(&m_vFirstLook_Dir, m_pMovementCom->Get_State(EState::Look));
	m_fSpawnTime_Max	= m_TowerDesc.fSpawnTime_Max;
	m_eTowerRange		= m_TowerDesc.eTowerRange;
	m_IsTurnable		= m_TowerDesc.IsTurnable;
	m_fTowerRangeDis	= m_TowerDesc.fTowerRangeDis;

	m_fTowerRangeCenter = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(1.f,0.f,0.f,0.f), XMLoadFloat3(&m_vFirstLook_Dir)))));

	m_fTowerRangeMax += m_fTowerRangeCenter;
	m_fTowerRangeMin -= m_fTowerRangeCenter;


	MASK_METERBAR_DESC_3D HP_Bar;
	HP_Bar.eFillMode = EMeterBar_FillMode::ZeroToFull;
	HP_Bar.eFrame_Render = ECastingBar_Frame_Render::Second;
	HP_Bar.HasFrameBar = true;
	HP_Bar.fCount = 100.f;
	HP_Bar.fCount_Max = 100.f;
	HP_Bar.UI_Desc.eLevel = ELevel::Static;
	HP_Bar.UI_Desc.Movement_Desc.vScale = { 4.f, 1.f, 0.1f, 0.f };
	lstrcpy(HP_Bar.UI_Desc.szTextureName, L"Component_Texture_ExpBar");
	XMStoreFloat4(&HP_Bar.UI_Desc.Movement_Desc.vPos, m_pMovementCom->Get_State(EState::Position));

	m_pHpBar = CMasking_MeterBar_3D::Create(m_pDevice, m_pDevice_Context, &HP_Bar);

	m_pHpBar->Set_ShaderPass(22);
	m_pHpBar->Set_ShaderPass_Frame(20);

	m_pHpBar->Set_Color({ 0.5f, 1.3f, 2.f });

	if (S_OK != hr)
		MSG_BOX("");

	return hr;
}

CGameObject * CDefenceTower::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CDefenceTower::Free()
{
	__super::Free();

	Safe_Release(m_pHpBar);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);

	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pVIBufferCom);
}
