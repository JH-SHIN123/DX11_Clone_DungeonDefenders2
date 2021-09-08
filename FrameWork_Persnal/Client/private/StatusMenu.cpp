#include "stdafx.h"
#include "..\public\StatusMenu.h"

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
	return S_OK;
}

_int CStatusMenu::Tick(_float TimeDelta)
{
	return _int();
}

_int CStatusMenu::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CStatusMenu::Render()
{
	return S_OK;
}

HRESULT CStatusMenu::Ready_Component()
{
	return S_OK;
}

CStatusMenu * CStatusMenu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CGameObject * CStatusMenu::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CStatusMenu::Free()
{
	__super::Free();
}
