#include "stdafx.h"
#include "..\public\BackGround_Logo.h"
#include "GameInstance.h"
#include "Renderer.h"
#include "VIBuffer_Rect.h"
#include "Textures.h"

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
	if (GetAsyncKeyState('Q') & 0x8000)
		m_IsReverse = false;
	else
		m_IsReverse = true;

	return _int();
}

_int CBackGround_Logo::Late_Tick(_double TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;


			
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority, this);
  //
}

HRESULT CBackGround_Logo::Render()
{
	// 스페이스를 넣어줘야 함

	if (nullptr == m_pBuffer_Rect)
		return E_FAIL;

	m_pBuffer_Rect->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));

	m_pBuffer_Rect->Render(0);

	return S_OK;
}

HRESULT CBackGround_Logo::Ready_Component()
{
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);



	/* For.Renderer*/
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For. VIBuffer */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pBuffer_Rect)))
		return E_FAIL;











	/* For.Textures */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Logo, TEXT("Component_Texture_Devil"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
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
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pBuffer_Rect);
	Safe_Release(m_pTextureCom);

	CGameObject::Free();
}
