#include "..\public\Renderer.h"
#include "GameObject.h"
#include "RenderTarget.h"
#include "Light_Manager.h"
#include "RenderTarget_Manager.h"
#include "VIBuffer_RectRHW.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
	, m_pTarget_Manager(CRenderTarget_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
}

CRenderer::CRenderer(const CComponent & rhs)
	: CComponent(rhs)
{
}

HRESULT CRenderer::NativeConstruct_Prototype()
{
	_uint		iNumViewPorts = 1;

	D3D11_VIEWPORT		ViewPortDesc;

	m_pDevice_Context->RSGetViewports(&iNumViewPorts, &ViewPortDesc);

	_uint View_Width = (_uint)ViewPortDesc.Width;
	_uint View_Height = (_uint)ViewPortDesc.Height;


	/* Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDevice_Context, TEXT("Target_Diffuse"), View_Width, View_Height, DXGI_FORMAT_R8G8B8A8_SNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDevice_Context, TEXT("Target_Normal"), View_Width, View_Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Depth */ // 깊이값의 소수점은 너무 자세하니까 크기를 더 크게 잡음
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDevice_Context, TEXT("Target_Depth"), View_Width, View_Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDevice_Context, TEXT("Target_Shade"), View_Width, View_Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pDevice_Context, TEXT("Target_Specular"), View_Width, View_Height, DXGI_FORMAT_R16G16B16A16_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;



	/* MRT_Deferred */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("Target_Diffuse"), TEXT("MRT_Deferred"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("Target_Normal"), TEXT("MRT_Deferred"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("Target_Depth"), TEXT("MRT_Deferred"))))
		return E_FAIL;

	/* MRT_LightAcc */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("Target_Shade"), TEXT("MRT_LightAcc"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("Target_Specular"), TEXT("MRT_LightAcc"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_RectRHW::Create(m_pDevice, m_pDevice_Context, 0.f, 0.f, ViewPortDesc.Width, ViewPortDesc.Height, TEXT("../Bin/Shader/Shader_Blend.hlsl"), "DefaultTechnique");
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	// 디버그용 렌더링 Shader_RenderTarget 텍스처 그대로 렌더함
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(TEXT("Target_Diffuse"), 0.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(TEXT("Target_Normal"), 0.f, 200.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(TEXT("Target_Depth"), 0.f, 400.f, 200.f, 200.f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(TEXT("Target_Shade"), 200.f, 0.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_DebugBuffer(TEXT("Target_Specular"), 200.f, 200.f, 200.f, 200.f)))
		return E_FAIL;




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
	Render_Priority_Second(); // 알림판


	 // 디퍼드
	Render_NoneAlpha();

	// 빛연산을 몰아서 한번에 처리 하겠다 라는 굳은 의지
	Render_LightAcc();

	// 지금까지 그려놓은 MRT들의 픽셀들을 섞자
	Render_Blend();

	Render_NoneAlpha_NotDeffed();
	Render_Alpha();

	Render_UI();
	Render_AlphaUI();
	Render_FrameUI();
	Render_AlphaUI_Second();
	Render_Option_UI_1();
	Render_Option_UI_2();
	Render_SceneChange();
	Render_Cursor();


	m_pTarget_Manager->Render_DebugBuffer(TEXT("MRT_Deferred"));
	m_pTarget_Manager->Render_DebugBuffer(TEXT("MRT_LightAcc"));

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

HRESULT CRenderer::Render_Priority_Second()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Priority_Second])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Priority_Second].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NoneAlpha()
{
	// 멀티 렌더 타겟_디퍼드에 같이 그려진다.
	m_pTarget_Manager->Begin_MRT(m_pDevice_Context, TEXT("MRT_Deferred"));


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

	// 그만 그렸다. 현재 MRT_Deferred에는 논알파 그룹에 속해 있는 애들만 그려져있음
	// 여기서 그려지는 애들은 클라의 Shader_Mesh 쉐이더를 쓰고 있으며
	// Shader_Mesh의 픽셀 쉐이더 아웃풋으로 SV_TARGET은 3개를 쓴다. 순서대로
	// 디퓨즈, 노멀, 깊이값
	m_pTarget_Manager->End_MRT(m_pDevice_Context, TEXT("MRT_Deferred"));

	// 이제 논알파 그룹의 디퓨즈 노멀 깊이를 각각 하나의 렌더타겟(이미지)에 담고있다.

	return S_OK;
}

HRESULT CRenderer::Render_NoneAlpha_NotDeffed()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::NoneAlpha_NotDeffed])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::NoneAlpha_NotDeffed].clear();
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

HRESULT CRenderer::Render_FrameUI()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::FrameUI])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::FrameUI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaUI_Second()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::AlphaUI_Scecond])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::AlphaUI_Scecond].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Option_UI_1()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Option_UI_1])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Option_UI_1].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Option_UI_2()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Option_UI_2])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Option_UI_2].clear();

	return S_OK;
}

HRESULT CRenderer::Render_SceneChange()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::SceneChange])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::SceneChange].clear();
	
	return S_OK;
}

HRESULT CRenderer::Render_Cursor()
{
	HRESULT hr = S_OK;
	for (auto& iter : m_pRenderObjects[(_uint)ERenderGroup::Cursor])
	{
		if (nullptr != iter)
			hr = iter->Render();

		Safe_Release(iter);

		if (FAILED(hr))
			return E_FAIL;
	}

	m_pRenderObjects[(_uint)ERenderGroup::Cursor].clear();

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	CLight_Manager*	pLight_Manager = CLight_Manager::GetInstance();

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pDevice_Context, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	// 빛에 대한 렌더 타겟을 뽑아내자
	// Shader_LightAcc 엔진내의 쉐이더를 씀
	// 픽셀 쉐이더의 아웃풋으로 렌더 타겟 두개를 씀
	// 명암 렌더타겟, 스펙큘러 렌더타겟 
	pLight_Manager->Render_Lights();



	if (FAILED(m_pTarget_Manager->End_MRT(m_pDevice_Context, TEXT("MRT_LightAcc"))))
		return E_FAIL;

	// 빛정보를 기반으로 MRT_LightAcc의 렌더 타겟 두개를 각각 그렸다


	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	// 하나의 버퍼에 지금까지 세팅했던 MRT들을 블렌딩하는 작업이다.
	// 기본색과, 빛의 명암, 빛의 스펙큘러들을 섞는다.
	// Shader_Blend 엔진내의 쉐이더를 사용한 RHW버퍼에 그리자
	m_pVIBuffer->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Diffuse")));
	m_pVIBuffer->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Shade")));
	m_pVIBuffer->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_ShaderResourceView(TEXT("Target_Specular")));

	m_pVIBuffer->Render(0);

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

	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pVIBuffer);
}
