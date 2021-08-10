#include "stdafx.h"
#include "..\public\BackGround_Logo.h"
#include "GameInstance.h"
#include "Renderer.h"

CBackGround_Logo::CBackGround_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)	
{
}

CBackGround_Logo::CBackGround_Logo(const CBackGround_Logo & rhs)
	: CGameObject(rhs)
{
}

HRESULT CBackGround_Logo::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBackGround_Logo::NativeConstruct(void* pArg)
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CBackGround_Logo::Tick(_double TimeDelta)
{
	return _int();
}

_int CBackGround_Logo::Late_Tick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority, this);
}

HRESULT CBackGround_Logo::Render()
{
	return S_OK;
}

HRESULT CBackGround_Logo::Ready_Component()
{
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 렌더러컴포넌트를 추가하자.*/
	/* For.Renderer*/
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Transform */

	Safe_Release(pGameInstance);

	return S_OK;
}

CBackGround_Logo * CBackGround_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBackGround_Logo*		pInstance = new CBackGround_Logo(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBackGround_Logo) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround_Logo::Clone_GameObject(void * pArg)
{
	CBackGround_Logo*		pInstance = new CBackGround_Logo(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CBackGround_Logo) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround_Logo::Free()
{
	CGameObject::Free();

	Safe_Release(m_pRendererCom);
}
