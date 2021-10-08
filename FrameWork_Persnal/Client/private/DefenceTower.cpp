#include "stdafx.h"
#include "..\public\DefenceTower.h"

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

	return S_OK;
}

_int CDefenceTower::Tick(_float TimeDelta)
{
	if (0 >= m_pStatusCom->Get_Hp())
		return OBJECT_DEAD;

	return _int();
}

_int CDefenceTower::Late_Tick(_float TimeDelta)
{
	if(ETowerState::Pick == m_eTowerState_Next ||
		ETowerState::Rotate == m_eTowerState_Next)
		return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CDefenceTower::Render()
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
	m_eTowerState_Next = ETowerState::Idle;
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

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_TowerDesc.MoveState_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &m_TowerDesc.Stat_Desc);
	
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_TowerRange"), TEXT("Com_Textures"), (CComponent**)&m_pTexturesCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_TowerDesc.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	XMStoreFloat3(&m_vFirstLook_Dir, m_pMovementCom->Get_State(EState::Look));
	m_fSpawnTime_Max	= m_TowerDesc.fSpawnTime_Max;
	m_eTowerRange		= m_TowerDesc.eTowerRange;
	m_IsTurnable		= m_TowerDesc.IsTurnable;
	m_fTowerRangeDis	= m_TowerDesc.fTowerRangeDis;

	m_fTowerRangeCenter = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(XMVectorSet(1.f,0.f,0.f,0.f), XMLoadFloat3(&m_vFirstLook_Dir)))));

	m_fTowerRangeMax += m_fTowerRangeCenter;
	m_fTowerRangeMin -= m_fTowerRangeCenter;



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

	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pTexturesCom);
}
