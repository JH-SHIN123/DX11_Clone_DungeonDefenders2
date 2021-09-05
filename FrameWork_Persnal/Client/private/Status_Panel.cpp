#include "stdafx.h"
#include "..\public\Status_Panel.h"
#include "GameInstance.h"
#include "Masking_MeterBar.h"
#include "MyText.h"

CStatus_Panel::CStatus_Panel(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CStatus_Panel::CStatus_Panel(const CStatus_Panel & rhs)
	: CGameObject(rhs)
{
}

HRESULT CStatus_Panel::NativeConstruct_Prototype()
{
	CGameObject::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CStatus_Panel::NativeConstruct(void * pArg)
{
	CGameObject::NativeConstruct(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(XMVectorSet(512.f, 256.f, 0.f, 0.f));
	m_pTransformCom->Set_State(EState::Position, XMVectorSet(-380.f, -236.f, 0.f, 1.f));

	return S_OK;
}

_int CStatus_Panel::Tick(_float TimeDelta)
{
	m_pMeterBar_Exp->Tick(TimeDelta);

	return _int();
}

_int CStatus_Panel::Late_Tick(_float TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return UPDATE_ERROR;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::FrameUI, this);
}

HRESULT CStatus_Panel::Render()
{
	if (nullptr == m_pVIBufferCom)
		return UPDATE_ERROR;

	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	m_pMeterBar_Exp->Render();

	// UI ÇÁ·¹ÀÓ
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pVIBufferCom->Render(1);


	m_pText->Render();
	return S_OK;
}

HRESULT CStatus_Panel::Ready_Component()
{
	/* For.Renderer*/
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.VIBuffer */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Transform */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Texture */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Texture_StatusPanel"), TEXT("Com_Texture_StatusPanel"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;


	MASK_METERBAR_DESC* pData = new MASK_METERBAR_DESC;
	pData->eFillMode = EMeterBar_FillMode::ZeroToFull;
	pData->HasFrameBar = false;
	pData->fCount = 10.f;
	pData->fCount_Max = 10.f;
	pData->UI_Desc.eLevel = ELevel::Static;
	pData->UI_Desc.Movement_Desc.vPos = { -397.f, -334.f, 0.f, 1.f };
	pData->UI_Desc.Movement_Desc.vScale = { 522.f, 64.f, 0.f, 0.f };
	lstrcpy((pData->UI_Desc.szTextureName), L"Component_Texture_ExpBar");
	m_pMeterBar_Exp = CMasking_MeterBar::Create(m_pDevice, m_pDevice_Context);
	m_pMeterBar_Exp->NativeConstruct(pData);

	Safe_Delete(pData);


	// test
	TEXT_DESC Data;
	Data.iScaleCount = 300;
	Data.MoveDesc.vPos = { 0.f,0.f,0.f,1.f };
	lstrcpy(Data.szText, L"HELLO");
	m_pText = CMyText::Create(m_pDevice, m_pDevice_Context, &Data);

	return S_OK;
}

CStatus_Panel * CStatus_Panel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStatus_Panel* pInstance = new CStatus_Panel(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStatus_Panel) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStatus_Panel::Clone_GameObject(void * pArg)
{
	CStatus_Panel* pClone = new CStatus_Panel(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CStatus_Panel) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CStatus_Panel::Free()
{
	CGameObject::Free();

	Safe_Release(m_pMeterBar_Exp);
	Safe_Release(m_pText);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
}
