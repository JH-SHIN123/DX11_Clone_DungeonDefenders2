#include "stdafx.h"
#include "..\public\CrystalCore.h"
#include "CrystalCore_Ring.h"
#include "Ortho3D.h"
#include "Data_Manager.h"

CCrystalCore::CCrystalCore(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CCrystalCore::CCrystalCore(const CCrystalCore & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrystalCore::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CCrystalCore::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));
	Set_Pivot_Rotate_Radian(XMVectorSet(90.f, 0.f, 0.f, 0.f));

	return S_OK;
}

_int CCrystalCore::Tick(_float TimeDelta)
{
	if (0 >= m_pStatusCom->Get_Hp())
	{
		static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Get_Component(L"Com_Movement"));

		return OBJECT_DEAD;
	}


	m_pStatusCom->Tick(TimeDelta);

	m_pMovementCom->RotateToAxis_Tick(TimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	//Set_Pivot_Rotate_Radian(XMVectorSet(XMConvertToRadians(-180.f), 0.f, 0.f, 0.f));

	m_pRing_Up->Tick(TimeDelta);
	m_pRing_Down->Tick(TimeDelta);

	m_pColliderCom_Hit->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return _int();
}

_int CCrystalCore::Late_Tick(_float TimeDelta)
{
	Button_Dis_Check();

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority, this);
}

HRESULT CCrystalCore::Render()
{
	if (nullptr == m_pModelCom)
		return E_FAIL;

	_float4 vColor = { 2.f, 3.f, 5.f, 1.f };

	m_pModelCom->Bind_VIBuffer();

	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);
	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));
	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	m_pModelCom->Set_Variable("g_vColor", &vColor, sizeof(_float4));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();
	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;

		m_pModelCom->Render_Model(i, 5);
	}

	m_pRing_Up->Render();
	m_pRing_Down->Render();

#ifdef _DEBUG
	m_pColliderCom_Hit->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

HRESULT CCrystalCore::Ready_Component(void * pArg)
{
	GAMEOBJ_DESC Data;
	memcpy(&Data, pArg, sizeof(GAMEOBJ_DESC));

	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Status"), (CComponent**)&m_pStatusCom, &Data.Status_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Mesh_CrystalCore"), TEXT("Com_Model"), (CComponent**)&m_pModelCom);

	COLLIDER_DESC		ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof(ColliderDesc));

	ColliderDesc.vScale = XMFLOAT3(10.f, 10.f, 10.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hit, &ColliderDesc);

	Data.Movement_Desc.fRotatePerSec = -0.18f;
	lstrcpy(Data.szModelName, L"Component_Mesh_CrystalCore_Ring_Up");
	m_pRing_Up		= CCrystalCore_Ring::Create(m_pDevice, m_pDevice_Context, &Data);

	Data.Movement_Desc.fRotatePerSec = 0.33f;
	lstrcpy(Data.szModelName, L"Component_Mesh_CrystalCore_Ring_Down");
	m_pRing_Down	= CCrystalCore_Ring::Create(m_pDevice, m_pDevice_Context, &Data);

	UI3D_DESC UIData;
	UIData.eLevel = ELevel::Stage1;
	UIData.Movement_Desc.vPos = Data.Movement_Desc.vPos;
	UIData.Movement_Desc.vPos.y += 7.f;
	UIData.Movement_Desc.vScale = { 512.f, 64.f, 0.f, 0.f };
	lstrcpy(UIData.szTextureName, L"Component_Texture_ActivateCrystal");
	m_pOrtho3D_Text = COrtho3D::Create(m_pDevice, m_pDevice_Context, &UIData);


	if (S_OK != hr)
		MSG_BOX("CCrystalCore::Ready_Component Failed");

	return hr;
}

void CCrystalCore::Button_Dis_Check()
{
	if (EPhaseState::Build == CData_Manager::GetInstance()->Get_NowPhase())
	{
		_vector vPos = m_pMovementCom->Get_State(EState::Position);
		_vector vTargetPos = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, L"Layer_Player"))->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
	
		_float fDis = XMVectorGetX(XMVector3Length(vPos - vTargetPos));
		if (fDis <= 25.f)
		{
			vPos += XMVectorSet(0.f,1.f,0.f,0.f) * 7.f;
			m_pOrtho3D_Text->Set_Pos(vPos);
			m_pOrtho3D_Text->Late_Tick(0.f);

			if (GET_KEY_INPUT(DIK_E))
			{
				CData_Manager::GetInstance()->Set_NowPhase(EPhaseState::Combat);
			}
		}
	}
}

CCrystalCore * CCrystalCore::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CCrystalCore*		pInstance = new CCrystalCore(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CCrystalCore) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCrystalCore::Clone_GameObject(void * pArg)
{
	CCrystalCore*		pInstance = new CCrystalCore(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CCrystalCore) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCrystalCore::Free()
{
	__super::Free();

	Safe_Release(m_pRing_Up);
	Safe_Release(m_pRing_Down);

	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pColliderCom_Hit);

	Safe_Release(m_pOrtho3D_Text);
}
