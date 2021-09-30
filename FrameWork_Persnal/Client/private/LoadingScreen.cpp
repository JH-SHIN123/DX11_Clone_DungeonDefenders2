#include "stdafx.h"
#include "..\public\LoadingScreen.h"

CLoadingScreen::CLoadingScreen(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CLoadingScreen::CLoadingScreen(const CLoadingScreen & rhs)
	: CGameObject(rhs)
{
}

HRESULT CLoadingScreen::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLoadingScreen::NativeConstruct(void * pArg)
{
	Ready_Component(nullptr);

	return S_OK;
}

_int CLoadingScreen::Tick(_float TimeDelta)
{
	_vector vAxis_Z = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	m_pMovementCom_Red->RotateToAxis_Tick(TimeDelta * 0.4f, vAxis_Z);

	m_pMovementCom_Green->RotateToAxis_Tick(TimeDelta * 1.2f, vAxis_Z);
	m_pMovementCom_Blue->RotateToAxis_Tick(TimeDelta * -0.95f, vAxis_Z);

	m_fTime += TimeDelta;
	//if (m_fTime >= 3.f)
	//	m_fTime = 0.f;
	return _int();
}

_int CLoadingScreen::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::AlphaUI, this);
}

HRESULT CLoadingScreen::Render()
{
	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	//g_MaskTexture g_TextureTime_UV
	_float2 Time = { m_fTime, m_fTime * -0.3333f };
	m_pVIBufferCom->Set_Variable("g_TextureTime_UV", &Time, sizeof(_float2));
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(1));
	m_pVIBufferCom->Set_ShaderResourceView("g_MaskTexture", m_pTexturesCom->Get_ShaderResourceView(1));
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Render(19);

	_float3 fColor = { 0.690196097f, 0.878431439f, 0.901960850f };
	m_pVIBufferCom->Set_Variable("g_Color", &fColor, sizeof(_float3));
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(0));
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Green->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Render(17);
	fColor = { 1.000000000f, 0.854902029f, 0.725490212f };
	m_pVIBufferCom->Set_Variable("g_Color", &fColor, sizeof(_float3));
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Blue->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Render(18);
	fColor = { 0.933333397f, 0.509803951f, 0.933333397f };
	m_pVIBufferCom->Set_Variable("g_Color", &fColor, sizeof(_float3));
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Red->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Render(16);



	return S_OK;
}

HRESULT CLoadingScreen::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	MOVESTATE_DESC Data;

	Data.vPos = { 0.f, 0.f, 0.f, 1.f };
	Data.fRotatePerSec = 1.5f;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Loading_Ring"), TEXT("Com_Texture"), (CComponent**)&m_pTexturesCom);

	Data.vScale = { 1280.f, 720.f, 0.f, 0.f };
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"),	(CComponent**)&m_pMovementCom, &Data);
	Data.vScale = { 700.f, 700.f, 0.f, 0.f};
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_R"),	(CComponent**)&m_pMovementCom_Red, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_G"),	(CComponent**)&m_pMovementCom_Green, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_B"),	(CComponent**)&m_pMovementCom_Blue, &Data);




	if (hr != S_OK)
		MSG_BOX("CLoadingScreen::Ready_Component");

	return hr;
}

CLoadingScreen * CLoadingScreen::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLoadingScreen* pInstance = new CLoadingScreen(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLoadingScreen) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLoadingScreen::Clone_GameObject(void * pArg)
{
	CLoadingScreen* pInstance = new CLoadingScreen(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CLoadingScreen) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLoadingScreen::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pMovementCom_Green);
	Safe_Release(m_pMovementCom_Blue);
	Safe_Release(m_pMovementCom_Red);

	__super::Free();
}
