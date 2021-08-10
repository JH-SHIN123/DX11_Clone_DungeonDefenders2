#include "..\public\VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Rect::NativeConstruct_Prototype()
{
	// For.VertexBuffer
	m_iNumVertexBuffers = 1;

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(4, sizeof(VTXTEX), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	VTXTEX* pVertices = new VTXTEX[4]; // 정점 몇개 필요함(Rect)

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f); // test

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	// For.IndicesBuffer
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

	return S_OK;
}

HRESULT CVIBuffer_Rect::NativeConstruct(void * pArg)
{
	CVIBuffer::NativeConstruct(pArg);

	return S_OK;
}

CVIBuffer_Rect * CVIBuffer_Rect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_Rect) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Rect::Clone(void * pArg)
{
	CVIBuffer_Rect*		pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_Rect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	CVIBuffer::Free();
}
