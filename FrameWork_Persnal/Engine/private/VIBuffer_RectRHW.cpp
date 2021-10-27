#include "..\public\VIBuffer_RectRHW.h"

CVIBuffer_RectRHW::CVIBuffer_RectRHW(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{

}

CVIBuffer_RectRHW::CVIBuffer_RectRHW(const CVIBuffer_RectRHW & rhs)
	: CVIBuffer(rhs)

{

}

HRESULT CVIBuffer_RectRHW::NativeConstruct_Prototype(_float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	/* For. VertexBuffer */
	m_iNumVertexBuffers = 1;

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(4, sizeof(VTXTEXRHW), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	D3D11_VIEWPORT		ViewPort;
	_uint iNumViewPort = 1;
	m_pDevice_Context->RSGetViewports(&iNumViewPort, &ViewPort);

	/* 윈도우 상의 좌표로 넘겨받은 값들을 투영공간상의 값으로 치환하여 저장한다. */
	VTXTEXRHW*		pVertices = new VTXTEXRHW[4];

	pVertices[0].vPosition = _float4(fX / (ViewPort.Width * 0.5f) - 1.f, fY / (ViewPort.Height * -0.5f) + 1.f, 0.f, 1.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float4((fX + fSizeX) / (ViewPort.Width * 0.5f) - 1.f, fY / (ViewPort.Height * -0.5f) + 1.f, 0.f, 1.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float4((fX + fSizeX) / (ViewPort.Width * 0.5f) - 1.f, (fY + fSizeY) / (ViewPort.Height * -0.5f) + 1.f, 0.f, 1.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float4(fX / (ViewPort.Width * 0.5f) - 1.f, (fY + fSizeY) / (ViewPort.Height * -0.5f) + 1.f, 0.f, 1.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;



	/* For. IndexBuffer */
	if (FAILED(CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT_R16_UINT, 2, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0)))
		return E_FAIL;

	POLYGONINDICES16*		pPolygonIndices = new POLYGONINDICES16[2];

	pPolygonIndices[0]._0 = 0;
	pPolygonIndices[0]._1 = 1;
	pPolygonIndices[0]._2 = 2;

	pPolygonIndices[1]._0 = 0;
	pPolygonIndices[1]._1 = 2;
	pPolygonIndices[1]._2 = 3;

	if (FAILED(CVIBuffer::SetUp_IndexSubResourceData(pPolygonIndices)))
		return E_FAIL;

	if (FAILED(CVIBuffer::NativeConstruct_Prototype()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pPolygonIndices);

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(CVIBuffer::SetUp_InputLayOuts(ElementDesc, 2, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectRHW::NativeConstruct(void * pArg)
{
	CVIBuffer::NativeConstruct(pArg);

	return S_OK;
}

CVIBuffer_RectRHW * CVIBuffer_RectRHW::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, _float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	CVIBuffer_RectRHW*		pInstance = new CVIBuffer_RectRHW(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(fX, fY, fSizeX, fSizeY, pShaderFilePath, pTechniqueName)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_RectRHW) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectRHW::Clone(void * pArg)
{
	CVIBuffer_RectRHW*		pInstance = new CVIBuffer_RectRHW(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_RectRHW) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_RectRHW::Free()
{
	__super::Free();
}
