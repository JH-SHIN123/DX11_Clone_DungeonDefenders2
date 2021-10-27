#include "stdafx.h"
#include "..\public\Bullet.h"

CBullet::CBullet(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CBullet::CBullet(const CBullet & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBullet::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBullet::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CBullet::Tick(_float TimeDelta)
{
	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	if (nullptr != m_pColliderCom_Attack)
	{
		m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	}

	return _int();
}

_int CBullet::Late_Tick(_float TimeDelta)
{
	m_fLifeTime -= TimeDelta;
	if (0.f >= m_fLifeTime)
		m_IsDelete_This = true;

	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CBullet::Render()
{
#ifdef _DEBUG
//	if(nullptr != m_pColliderCom_Attack)
//		m_pColliderCom_Attack->Render_Collider();
#endif // _DEBUG


	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

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

HRESULT CBullet::Render_Rect(_bool IsBillBoard, _int iShaderPass, _int iTextureIndex)
{
	if (nullptr == m_pBufferRectCom || nullptr == m_pTextureCom)
		return S_OK;

	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();

	if (true == IsBillBoard)
		WorldMatrix = BillBorad();

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(iTextureIndex));

	m_pBufferRectCom->Render(iShaderPass);


	return S_OK;
}

HRESULT CBullet::Ready_Component(void * pArg)
{
	BULLET_DESC Data;
	memcpy(&Data, pArg, sizeof(BULLET_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.MoveState_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data.Attack_Collide_Desc);

	if(2 < lstrlen(Data.szModelName))
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szModelName, TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	if (2 < lstrlen(Data.szTextureName))
	{
		hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_Buffer"), (CComponent**)&m_pBufferRectCom);
		hr = CGameObject::Add_Component((_uint)ELevel::Stage1, Data.szTextureName, TEXT("Com_Texture_0"), (CComponent**)&m_pTextureCom);
	}

	m_vGoDir = Data.vDir;
	m_fLifeTime = Data.fLifeTime;
	m_vTextureSize = Data.vTextureSize;

	if (S_OK != hr)
		MSG_BOX("CBullet::Ready_Component Failed!");

	return hr;
}

_fmatrix CBullet::BillBorad()
{
	_vector vCamPos = GET_CAMERA_POSITION;
	_vector vLook = XMVector3Normalize(vCamPos - m_pMovementCom->Get_State(EState::Position));
	_vector vRight = XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook));
	_vector vUp = XMVector3Normalize(XMVector3Cross(vLook, vRight));

	_matrix WorldMatrix = XMMatrixIdentity();

	WorldMatrix.r[0] = vRight * m_vTextureSize.x;
	WorldMatrix.r[1] = vUp * m_vTextureSize.y;
	WorldMatrix.r[2] = vLook;
	WorldMatrix.r[3] = m_pMovementCom->Get_State(EState::Position);

	return WorldMatrix;
}

CGameObject * CBullet::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pBufferRectCom);
	Safe_Release(m_pTextureCom);
}				 
