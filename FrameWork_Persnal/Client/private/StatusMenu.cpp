#include "stdafx.h"
#include "..\public\StatusMenu.h"
#include "Data_Manager.h"
#include "MyButton_NoText.h"
#include "Masking_MeterBar.h"
#include "Masking_UI.h"

CStatusMenu::CStatusMenu(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CStatusMenu::CStatusMenu(const CStatusMenu & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CStatusMenu::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStatusMenu::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_pButton_HpMp.reserve(m_iButtonCount_HpMp);
	m_pTransform_HpMpFrame.resize(m_iButtonCount_HpMp);

	m_pButton_Status.reserve(m_iButtonCount_Status);
	m_pTransform_StatusFrame.resize(m_iButtonCount_Status);

	Ready_Component();

	return S_OK;
}

_int CStatusMenu::Tick(_float TimeDelta)
{
	if (false == CData_Manager::GetInstance()->Get_Tick_Stop())
		return OBJECT_DEAD;

	m_pExpBar->Tick(TimeDelta);

	return _int();
}

_int CStatusMenu::Late_Tick(_float TimeDelta)
{
	if (true == m_IsExit)
		return OBJECT_DEAD;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Option_UI_2, this);
}

HRESULT CStatusMenu::Render()
{
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	m_pExpBar->Render();

	Button_Render();
	ButtonFrame_Render();

	Status_Render();
	StatusFrame_Render();


	m_pPlayerPortrait->Render(13);

	return S_OK;
}

HRESULT CStatusMenu::Ready_Component()
{
	// Exp Bar
	MASK_METERBAR_DESC Data;
	Data.eFrame_Render = ECastingBar_Frame_Render::Second;
	Data.eFillMode = EMeterBar_FillMode::ZeroToFull;
	Data.HasFrameBar = true;
	Data.fCount = 10.f;
	Data.fCount_Max = 10.f;
	Data.UI_Desc.eLevel = ELevel::Static;
	Data.UI_Desc.Movement_Desc.vScale = { 480.f, 36.f, 0.f, 0.f };
	Data.UI_Desc.Movement_Desc.vPos = { 0.f, 224.f, 0.f, 1.f };
	lstrcpy((Data.UI_Desc.szTextureName), L"Component_Texture_ExpBar");
	m_pExpBar = CMasking_MeterBar::Create(m_pDevice, m_pDevice_Context);
	m_pExpBar->NativeConstruct(&Data);


	// Portrait
	MASK_UI_DESC Mask_UI_Desc;
	Mask_UI_Desc.HasFrame = true;
	Mask_UI_Desc.IsFrameFirst = false;
	Mask_UI_Desc.vFrameOffSetPos = { 0.f, 28.f, 0.f };
	Mask_UI_Desc.vFrameScale = { 128.f, 176.f,0.f };
	Mask_UI_Desc.UI_Desc.eLevel = ELevel::Static;
	Mask_UI_Desc.UI_Desc.Movement_Desc.vScale = { 100.5f, 100.f, 0.f, 0.f };
	Mask_UI_Desc.UI_Desc.Movement_Desc.vPos = { 0.f, 100.f, 0.f, 1.f };
	lstrcpy((Mask_UI_Desc.UI_Desc.szTextureName), L"Component_Texture_Status_Portrait");
	m_pPlayerPortrait = CMasking_UI::Create(m_pDevice, m_pDevice_Context);
	m_pPlayerPortrait->NativeConstruct(&Mask_UI_Desc);


	// HpMp
	UI2D_DESC ButtonDesc;
	ButtonDesc.Movement_Desc.vScale = { 74.f, 74.f, 0.f, 0.f };
	ButtonDesc.eLevel = ELevel::Static;
	lstrcpy(ButtonDesc.szTextureName, L"Component_Texture_Status_HpMp");
	ButtonDesc.Movement_Desc.vPos = { -180.f, 130.f, 0.f, 1.f };

	TRANSFORM_DESC Trans_Desc;
	Trans_Desc.vScale = { 256.f, 104.f, 0.f, 0.f };
	Trans_Desc.vPos = ButtonDesc.Movement_Desc.vPos;
	_float fOffSetX = 360.f;

	for (_int i = 0; i < m_iButtonCount_HpMp; ++i)
	{
		m_pButton_HpMp.emplace_back(CMyButton_NoText::Create(m_pDevice, m_pDevice_Context, &ButtonDesc));

		ButtonDesc.Movement_Desc.vPos.x += fOffSetX;
	}

	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), TEXT("Com_Transform_Hp"), (CComponent**)&m_pTransform_HpMpFrame[0], &Trans_Desc);
	Trans_Desc.vPos.x += fOffSetX;
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), TEXT("Com_Transform_Mp"), (CComponent**)&m_pTransform_HpMpFrame[1], &Trans_Desc);



	// Status
	ButtonDesc.Movement_Desc.vScale = { 64.f, 64.f, 0.f, 0.f };
	ButtonDesc.eLevel = ELevel::Static;
	lstrcpy(ButtonDesc.szTextureName, L"Component_Texture_Status_Icon");
	ButtonDesc.Movement_Desc.vPos = { -230.f, -15.f, 0.f, 1.f };
	Trans_Desc.vScale = { 220.f, 128.f, 0.f, 0.f };
	Trans_Desc.vPos = ButtonDesc.Movement_Desc.vPos;
	fOffSetX = 150.f;

	_tchar szCom[MAX_PATH] = L"Com_Transform_StatusFrame_%d";
	for (_int i = 0; i < m_iButtonCount_Status; ++i)
	{
		_tchar szName[MAX_PATH] = L"";

		wsprintf(szName, szCom, i);
		m_pButton_Status.emplace_back(CMyButton_NoText::Create(m_pDevice, m_pDevice_Context, &ButtonDesc));
		ButtonDesc.Movement_Desc.vPos.x += fOffSetX;
	}
	
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), (L"Com_Transform_StatusFrame_0"), (CComponent**)&m_pTransform_StatusFrame[0], &Trans_Desc);
	Trans_Desc.vPos.x += fOffSetX;
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), (L"Com_Transform_StatusFrame_1"), (CComponent**)&m_pTransform_StatusFrame[1], &Trans_Desc);
	Trans_Desc.vPos.x += fOffSetX;
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), (L"Com_Transform_StatusFrame_2"), (CComponent**)&m_pTransform_StatusFrame[2], &Trans_Desc);
	Trans_Desc.vPos.x += fOffSetX;
	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), (L"Com_Transform_StatusFrame_3"), (CComponent**)&m_pTransform_StatusFrame[3], &Trans_Desc);
	Trans_Desc.vPos.x += fOffSetX;


	return S_OK;
}

void CStatusMenu::Button_Render()
{
	m_pButton_HpMp[0]->Render(1);
	m_pButton_HpMp[1]->Render(2);
}

void CStatusMenu::ButtonFrame_Render()
{
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(3));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransform_HpMpFrame[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(1);

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransform_HpMpFrame[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(1);
}

void CStatusMenu::Status_Render()
{
	m_pButton_Status[0]->Render(0);
	m_pButton_Status[1]->Render(1);
	m_pButton_Status[2]->Render(2);
	m_pButton_Status[3]->Render(3);

}

void CStatusMenu::StatusFrame_Render()
{
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(4));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransform_StatusFrame[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(1);

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransform_StatusFrame[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(1);

}

CStatusMenu * CStatusMenu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStatusMenu* pInstance = new CStatusMenu(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStatusMenu) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStatusMenu::Clone_GameObject(void * pArg)
{
	CStatusMenu* pClone = new CStatusMenu(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CStatusMenu) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CStatusMenu::Free()
{
	__super::Free();

	Safe_Release_Vector(m_pTransform_HpMpFrame);
	Safe_Release_Vector(m_pButton_HpMp);
	Safe_Release_Vector(m_pTransform_StatusFrame);
	Safe_Release_Vector(m_pButton_Status);

	Safe_Release(m_pPlayerPortrait);
	Safe_Release(m_pExpBar);
}
