#include "..\public\Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CRenderer::CRenderer(const CComponent & rhs)
	: CComponent(rhs)
{
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CRenderer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Add_GameObjectToRenderer(ERenderGroup eGroup, CGameObject * pGameObject)
{
	// Add_GameObjecetToRenderer((ERenderGroup)7, nullptr); 이따위 정신나간 enum class가 가능

	if (ERenderGroup::End <= eGroup ||
		nullptr == pGameObject)
		return E_FAIL;

	m_pRenderObjects[(_uint)eGroup].emplace_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Draw_Renderer()
{
	Render_Priority();
	Render_NoneAlpha();
	Render_Alpha();
	Render_UI();
	Render_AlphaUI();

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Priority])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Priority].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NoneAlpha()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::NoneAlpha])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::NoneAlpha].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Alpha()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Alpha])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Alpha].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::UI])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaUI()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::AlphaUI])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::AlphaUI].clear();

	return S_OK;
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CRenderer* pInstance = new CRenderer(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CRenderer)");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	CBase::AddRef();

	return this;
}

void CRenderer::Free()
{
	CComponent::Free();

	for (_uint i = 0; i < (_uint)ERenderGroup::End; ++i)
	{
		for (auto& iter : m_pRenderObjects[i])
			Safe_Release(iter);

		m_pRenderObjects[i].clear();
	}
}
