#include "..\public\Light.h"
#include "VIBuffer_RectRHW.h"
#include "Pipeline_Manager.h"
#include "RenderTarget_Manager.h"
#include "GameInstance.h"

CLight::CLight(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

HRESULT CLight::Native_Construct(const LIGHT_DESC & LightDesc, _bool isActive)
{
	m_LightDesc = LightDesc;

	D3D11_VIEWPORT	Viewport;
	_uint			iNumViewports = 1;

	m_pDevice_Context->RSGetViewports(&iNumViewports, &Viewport);

	m_pVIBuffer = CVIBuffer_RectRHW::Create(m_pDevice, m_pDevice_Context, 0.0f, 0.f, Viewport.Width, Viewport.Height, TEXT("../Bin/Shader/Shader_LightAcc.hlsl"), "DefaultTechnique");
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	return S_OK;
}

HRESULT CLight::Render_Light()
{

	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	ID3D11ShaderResourceView* pNormalShaderResourceView = CRenderTarget_Manager::GetInstance()->Get_ShaderResourceView(TEXT("Target_Normal"));
	if (nullptr == pNormalShaderResourceView)
		return E_FAIL;

	ID3D11ShaderResourceView* pDepthShaderResourceView = CRenderTarget_Manager::GetInstance()->Get_ShaderResourceView(TEXT("Target_Depth"));
	if (nullptr == pDepthShaderResourceView)
		return E_FAIL;

	// MRT_Deferred에 있던 렌더 타겟을 마치 텍스처 처럼 쓸거임
	if (FAILED(m_pVIBuffer->Set_ShaderResourceView("g_NormalTexture", pNormalShaderResourceView)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_ShaderResourceView("g_DepthTexture", pDepthShaderResourceView)))
		return E_FAIL;

	// 빛의 정보를 떤짐
	if (FAILED(m_pVIBuffer->Set_Variable("g_vLightDiffuse", &m_LightDesc.vDiffuse, sizeof(_float4)))) // 기본광
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_Variable("g_vLightAmbient", &m_LightDesc.vAmbient, sizeof(_float4)))) // 반사광
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_Variable("g_vLightSpecular", &m_LightDesc.vSpecular, sizeof(_float4)))) // 하이라이트
		return E_FAIL;

	CPipeline_Manager* pPipeLine = CPipeline_Manager::GetInstance();

	// Specular를 위해서 카메라 위치
	_vector		vCamPosition = pPipeLine->Get_CamPosition();
	if (FAILED(m_pVIBuffer->Set_Variable("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;

	_matrix		ProjMatrixInverse = XMMatrixInverse(nullptr, GET_PROJ_SPACE);
	_matrix		ViewMatrixInverse = XMMatrixInverse(nullptr, GET_VIEW_SPACE);

	// 점광원 처리시 픽셀의 위치를 월드상태까지 내려야 하기 때문에 역행렬 던짐
	if (FAILED(m_pVIBuffer->Set_Variable("g_ProjMatrixInverse", &XMMatrixTranspose(ProjMatrixInverse), sizeof(_matrix))))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Set_Variable("g_ViewMatrixInverse", &XMMatrixTranspose(ViewMatrixInverse), sizeof(_matrix))))
		return E_FAIL;

	_uint		iPassIndex = 0;

	if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
	{
		// 방향성 광원이면 당연히 방향
		if (FAILED(m_pVIBuffer->Set_Variable("g_vLightDir", &m_LightDesc.vDirection, sizeof(XMFLOAT3))))
			return E_FAIL;

		iPassIndex = 0;
	}
	else if (LIGHT_DESC::TYPE_POINT == m_LightDesc.eType)
	{
		// 점광원 위치
		if (FAILED(m_pVIBuffer->Set_Variable("g_vLightPos", &m_LightDesc.vPosition, sizeof(XMFLOAT3))))
			return E_FAIL;

		// 반경
		if (FAILED(m_pVIBuffer->Set_Variable("g_fRadius", &m_LightDesc.fRadius, sizeof(float))))
			return E_FAIL;

		iPassIndex = 1;
	}

	m_pVIBuffer->Render(iPassIndex);


	return S_OK;
}

CLight * CLight::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const LIGHT_DESC & LightDesc, _bool isActive)
{

	CLight*		pInstance = new CLight(pDevice, pDevice_Context);

	if (FAILED(pInstance->Native_Construct(LightDesc, isActive)))
	{
		MSG_BOX("Failed to Creating Instance (CLight) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
