#include "stdafx.h"
#include "LightningTower_Bullet.h"
#include "LightningTower_Bullet_Effect.h"
#include "Point_Spread2.h"

CLightningTower_Bullet::CLightningTower_Bullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CBullet(pDevice, pDevice_Context)
{
}

CLightningTower_Bullet::CLightningTower_Bullet(const CLightningTower_Bullet & rhs)
	: CBullet(rhs)
{
}

HRESULT CLightningTower_Bullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower_Bullet::NativeConstruct(void * pArg)
{
	LIGHTNING_BULLET_DESC Data;
	memcpy(&Data, pArg, sizeof(LIGHTNING_BULLET_DESC));

	//m_iBoundCount = Data.iBoundCount;

	__super::NativeConstruct(&Data.Bullet_Desc);

	//m_vecEffectMesh.resize(m_iBoundCount);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));

	Target_Check(0.f);

	Ready_Component(&Data.Bullet_Desc);

	return S_OK;
}

_int CLightningTower_Bullet::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
	{
		Create_Effect();
		return iReturn;
	}
	if (true == m_pColliderCom_Attack->Get_IsCollide())
	{
		Create_Effect();
		return OBJECT_DEAD;
	}

	Spawn_Check(TimeDelta);

	//for (auto& iter : m_vecEffectMesh)
	//{
	//	if (nullptr != iter)
	//	{
	//		iter->Late_Tick(TimeDelta);
	//		iter->Set_Rotate(m_pMovementCom->Get_WorldMatrix());
	//		iter->Set_Position(m_pMovementCom->Get_State(EState::Position));
	//		iter->Tick(TimeDelta);
	//	}
	//}
	Move_Check(TimeDelta);


	m_pPointSpread->Set_Pos(m_pMovementCom->Get_State(EState::Position));
	m_pPointSpread->Tick(TimeDelta);
	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return _int();
}

_int CLightningTower_Bullet::Late_Tick(_float TimeDelta)
{
	m_pPointSpread->Late_Tick(TimeDelta);

	if (true == m_pColliderCom_Attack->Get_IsCollide() || true == m_IsDelete_This)
	{
		Create_Effect();
		return OBJECT_DEAD;
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower_Bullet::Render()
{
	//__super::Render();


	return S_OK;

	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
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


	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 1);
	}
	

	return S_OK;
}

HRESULT CLightningTower_Bullet::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	POINT_SPREAD_DESC Point_Desc;
	lstrcpy(Point_Desc.szTextrueName, L"Component_Texture_Flare");
	lstrcpy(Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_200_5");
	XMStoreFloat4(&Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	Point_Desc.MoveDesc.vScale = { 1.f,1.f,0.f,0.f };

	m_pPointSpread = CPoint_Spread::Create(m_pDevice, m_pDevice_Context);
	m_pPointSpread->NativeConstruct(&Point_Desc);

	VTXMATRIX* pInstance = m_pPointSpread->Get_InstanceBuffer();
	_int iInstance_StartIndex = m_pPointSpread->Get_Instance_StartIndex();
	_int iNumInstance = m_pPointSpread->Get_Instance_Num();
	_float fSize = 8.f;

	for (_int i = iInstance_StartIndex; i < iNumInstance + iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize.x = fSize;
		pInstance[i].vSize.y = fSize;

		fSize -= 0.5f;
	}

	m_pPointSpread->Set_InstanceBuffer(pInstance);
	m_pPointSpread->SetUp_IndexDir(1);

	m_pPointSpread->Set_ShaderPass(2);
	m_pPointSpread->SetUp_Color({ 0.8f, 2.f, 3.2f, 1.f });



	if (S_OK != hr)
		MSG_BOX("CLightningTower_Bullet::Ready_Component Filed!");

	return S_OK;
}

void CLightningTower_Bullet::Spawn_Check(_float TimeDelta)
{
	//m_fScaleTime += TimeDelta;
	//_float3 vScale;
	//vScale.x = TimeDelta * 0.05f;

	//if (vScale.x >= m_vScale_SizeUp.x)
	//	m_IsMoveable = true;

	//if (nullptr == m_vecEffectMesh[0] && 0.25f <= m_fScaleTime)
	//{
	//	Create_Effect();
	//}

	//if (false == m_IsMoveable)
	//{
	//	vScale.y = TimeDelta * 0.05f;
	//	vScale.z = TimeDelta * 0.05f;

	//	Set_Pivot(XMVectorSet(vScale.x, vScale.y, vScale.z, 0.f));
	//}

	//else
	//{
	//	m_pMovementCom->Go_Up(-TimeDelta);
	//}


}

void CLightningTower_Bullet::Create_Effect()
{
	POINT_SPREAD_DESC_2 Data;
	Data.Point_Desc.fSpreadDis = 2.f;
	Data.IsTime = true;
	Data.vSize = { 2.f,2.f };
	Data.Point_Desc.fLifeTime = 1.f;
	Data.Point_Desc.iShaderPass = 3;
	Data.Point_Desc.InstanceValue = EInstanceValue::Point_100_10;
	XMStoreFloat4(&Data.Point_Desc.MoveDesc.vPos, m_pMovementCom->Get_State(EState::Position));
	lstrcpy(Data.Point_Desc.szPointInstance_PrototypeName, L"Component_VIBuffer_PointInstance_100_10");
	lstrcpy(Data.Point_Desc.szTextrueName, L"Component_Texture_Particle");

	GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Point_Spread2", (_uint)ELevel::Stage1, L"Layer_Effect", &Data);
}

void CLightningTower_Bullet::Target_Check(_float TimeDelta)
{

	CLayer* pDstLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1, L"Layer_Monster");
	if (nullptr == pDstLayer)
	{
		m_IsCopyMonster = false;
		return;
	}

	list<CGameObject*> pTarget = pDstLayer->Get_GameObject_List();

	_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
	vector<SORT_DESC> vecPos;

	SORT_DESC Sort_Data;
	vecPos.reserve(pTarget.size());

	for (auto& iter : pTarget)
	{
		_vector vPos = static_cast<CMovement*>(iter->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
		_float fDis = XMVectorGetX(XMVector3Length(vPos - vMyPos));


		XMStoreFloat3(&Sort_Data.vPos, vPos);
		Sort_Data.fDis = fDis;
		Sort_Data.pTarget = iter;

		vecPos.push_back(Sort_Data);
	}

	auto& iter = vecPos.begin();
	sort(vecPos.begin(), vecPos.end(), [](SORT_DESC &a, SORT_DESC &b) -> _bool { return a.fDis < b.fDis; });

	for (_int i = 0; i < 3; ++i)
	{
		if (i < vecPos.size() && nullptr != vecPos[i].pTarget)
		{
			m_vTargetPos[i] = vecPos[i].vPos;
			m_vTargetPos[i].y += 7.f;
		}
	}

	m_IsCopyMonster = true;
}

void CLightningTower_Bullet::Move_Check(_float TimeDelta)
{
	_float fDis = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vGoDir) - m_pMovementCom->Get_State(EState::Position))) * 0.25f;
	_vector vDir = XMVector3Normalize(XMLoadFloat3(&m_vGoDir) - m_pMovementCom->Get_State(EState::Position));
	vDir = XMVectorSetW(vDir, 0.f);
	vDir *= fDis;

	m_pMovementCom->Go_Dir_Vector(TimeDelta, vDir);
}

CLightningTower_Bullet * CLightningTower_Bullet::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower_Bullet::Clone_GameObject(void * pArg)
{
	CLightningTower_Bullet*		pInstance = new CLightningTower_Bullet(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower_Bullet) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower_Bullet::Free()
{
	//Safe_Release_Vector(m_vecEffectMesh);

	//if (true == m_IsCopyMonster)
	//{
	//	for (_int i = 0; i < 3; ++i)
	//		Safe_Release(m_pMonsterList[i]);
	//}
	Safe_Release(m_pPointSpread);

	__super::Free();
}
