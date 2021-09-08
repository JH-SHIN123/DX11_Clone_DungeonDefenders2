#include "stdafx.h"
#include "..\public\PauseMenu.h"
#include "MyButton_NoText.h"

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

	Ready_Component();

	return S_OK;
}

_int CPauseMenu::Tick(_float TimeDelta)
{


	return _int();
}

_int CPauseMenu::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CPauseMenu::Render()
{
	return S_OK;
}

HRESULT CPauseMenu::Ready_Component()
{
	for (_int i = 0; i < m_iButtonCount; ++i)
	{
		UI2D_DESC ButtonDesc;
		ButtonDesc.Movement_Desc.vPos = { -345.f, 28.f, 0.f, 1.f };
		ButtonDesc.Movement_Desc.vScale = { 256.f, 64.f, 0.f, 0.f };
		ButtonDesc.eLevel = ELevel::Logo;
		lstrcpy(ButtonDesc.szTextureName, L"Component_Texture_PauseMenu");
		m_pButton.emplace_back(CMyButton_NoText::Create(m_pDevice, m_pDevice_Context, &ButtonDesc));
	}


	return S_OK;
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
