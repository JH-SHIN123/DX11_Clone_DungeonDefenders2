#include "stdafx.h"
#include "..\public\Player.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CPlayer::Tick(_float TimeDelta)
{
	return _int();
}

_int CPlayer::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CPlayer::Render()
{
	return S_OK;
}

HRESULT CPlayer::Ready_Component()
{
	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CGameObject * CPlayer::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CPlayer::Free()
{
}
