#include "stdafx.h"
#include "..\public\PauseMenu.h"
#include "MyButton_NoText.h"
#include "Data_Manager.h"
#include "Level_Logo.h"

CPauseMenu::CPauseMenu(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CPauseMenu::CPauseMenu(const CPauseMenu & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CPauseMenu::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPauseMenu::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_pButton.reserve(m_iButtonCount);

	XMStoreFloat4x4(&m_matAlphaBlack, XMMatrixIdentity());
	m_matAlphaBlack.m[0][0] = g_iWinCX;
	m_matAlphaBlack.m[1][1] = g_iWinCY;

	Ready_Component();

	return S_OK;
}

_int CPauseMenu::Tick(_float TimeDelta)
{
	if (false == CData_Manager::GetInstance()->Get_Tick_Stop())
		return OBJECT_DEAD;

	for (auto& iter : m_pButton)
		iter->Tick(TimeDelta);

	Button_Check();

	return _int();
}

_int CPauseMenu::Late_Tick(_float TimeDelta)
{
	for (auto& iter : m_pButton)
		iter->Late_Tick(TimeDelta);

	if (true == m_IsSceneChange)
	{
		static_cast<CLevel_Logo*>(GET_GAMEINSTANCE->Get_Scene())->Scene_Change(ELevel::Stage1); // 짬통에 이 정보를 넣어놓자

		return SCENE_CHANGE;
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::AlphaUI_Scecond, this);
}

HRESULT CPauseMenu::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	//BlackAlpha
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_matAlphaBlack)), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_AlphaBlack->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(11);

	// Frame
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);


	for (auto& iter : m_pButton)
		iter->Render(1);



	return S_OK;
}

HRESULT CPauseMenu::Ready_Component()
{
	UI2D_DESC ButtonDesc;
	ButtonDesc.Movement_Desc.vScale = { 256.f, 64.f, 0.f, 0.f };
	ButtonDesc.eLevel = ELevel::Static;
	lstrcpy(ButtonDesc.szTextureName, L"Component_Texture_PauseMenu");
	ButtonDesc.Movement_Desc.vPos = { -6.f, 60.f, 0.f, 1.f };

	_float fOffSetY = -82.f;
	for (_int i = 0; i < m_iButtonCount; ++i)
	{
		m_pButton.emplace_back(CMyButton_NoText::Create(m_pDevice, m_pDevice_Context, &ButtonDesc));

		ButtonDesc.Movement_Desc.vPos.y += fOffSetY;
	}

	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Texture_Black"), TEXT("Com_Texture_StatusPanel"), (CComponent**)&m_pTextureCom_AlphaBlack)))
		return E_FAIL;


	return S_OK;
}

void CPauseMenu::Button_Check()
{
	for (_int i = 0; i < m_iButtonCount; ++i)
	{
		if (true == m_pButton[i]->Get_IsClick())
		{
			switch (i)
			{
			case 0:	// 스탯창
			{
				UI2D_DESC UI_Desc;

				lstrcpy(UI_Desc.szTextureName, L"Component_Texture_Status");
				UI_Desc.Movement_Desc.vPos = _float4(0.f, 50.f, 0.f, 1.f);
				UI_Desc.Movement_Desc.vScale = _float4(768.f, 512.f, 0.f, 0.f);
				UI_Desc.eLevel = ELevel::Static;
				GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Static, TEXT("Prototype_StatusMenu"), (_uint)ELevel::Stage1, L"Layer_StatusMenu", &UI_Desc);
			}
				break;
			case 1:	// 게임재개
				break;
			case 2: // 다시시작
				break;
			case 3: // 메인메뉴
				m_IsSceneChange = true;
				break;
			default:
				break;
			}
		}
	}
}

CPauseMenu * CPauseMenu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPauseMenu* pInstance = new CPauseMenu(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPauseMenu) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPauseMenu::Clone_GameObject(void * pArg)
{
	CPauseMenu* pClone = new CPauseMenu(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CPauseMenu) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CPauseMenu::Free()
{
	for (auto& iter : m_pButton)
	{
		Safe_Release(iter);
	}
	m_pButton.clear();

	Safe_Release(m_pTextureCom_AlphaBlack);

	__super::Free();
}
