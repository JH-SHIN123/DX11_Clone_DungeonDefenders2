#include "..\public\VIBuffer_RectInstance.h"

CVIBuffer_RectInstance::CVIBuffer_RectInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{

}

CVIBuffer_RectInstance::CVIBuffer_RectInstance(const CVIBuffer_RectInstance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
{
	Safe_AddRef(m_pVBInstance);

}

HRESULT CVIBuffer_RectInstance::NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance)
{
	/* For. VertexBuffer */
	m_iNumVertexBuffers = 2;
	m_iNumInstance = iNumInstance;

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(4, sizeof(VTXTEX), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	VTXTEX*		pVertices = new VTXTEX[4];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(0.f, 1.f);

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	/* For. IndexBuffer */
	if (FAILED(CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT_R16_UINT, 2 * m_iNumInstance, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0)))
		return E_FAIL;

	POLYGONINDICES16*		pPolygonIndices = new POLYGONINDICES16[2 * m_iNumInstance];

	_uint		iNumPolygons = 0;

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pPolygonIndices[iNumPolygons]._0 = 0;
		pPolygonIndices[iNumPolygons]._1 = 1;
		pPolygonIndices[iNumPolygons]._2 = 2;
		++iNumPolygons;

		pPolygonIndices[iNumPolygons]._0 = 0;
		pPolygonIndices[iNumPolygons]._1 = 2;
		pPolygonIndices[iNumPolygons]._2 = 3;
		++iNumPolygons;
	}

	if (FAILED(CVIBuffer::SetUp_IndexSubResourceData(pPolygonIndices)))
		return E_FAIL;

	if (FAILED(CVIBuffer::NativeConstruct_Prototype()))
		return E_FAIL;

	/* 인스턴스 버퍼를 생성하자. (사각형각가의 상태를 표현해주느 ㄴ행렬을 정점형태로 보관. */
	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(m_iNumInstance, sizeof(VTXMATRIX), D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE)))
		return E_FAIL;

	VTXMATRIX*		m_pInstanceVertices = new VTXMATRIX[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pInstanceVertices[i].vSize = _float2(1.f, 1.f);
		m_pInstanceVertices[i].vRight = _float4(1.0f, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, 1.0f, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		m_pInstanceVertices[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(m_pInstanceVertices)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,  D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	if (FAILED(CVIBuffer::SetUp_InputLayOuts(ElementDesc, 6, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_RectInstance::NativeConstruct(void * pArg)
{
	CVIBuffer::NativeConstruct(pArg);




	return S_OK;
}

HRESULT CVIBuffer_RectInstance::Render(_uint iPassIndex)
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	_uint		iOffset = 0;

	ID3D11Buffer* const 	pBuffers[2] = { m_pVB, m_pVBInstance };
	const _uint	 iStrides[2] = { sizeof(VTXTEX), sizeof(VTXMATRIX) };
	const _uint		iOffsets[2] = { 0, 0 };

	m_pDevice_Context->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffers, iStrides, iOffsets);
	m_pDevice_Context->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDevice_Context->IASetPrimitiveTopology(m_eTopology);
	m_pDevice_Context->IASetInputLayout(m_InputLayouts[iPassIndex].pLayout);

	if (FAILED(m_InputLayouts[iPassIndex].pPass->Apply(0, m_pDevice_Context)))
		return E_FAIL;

	_uint		iNumIndices = 0;

	switch (m_eTopology)
	{
	case D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST:
		iNumIndices = m_iNumPolygons * 3;
		break;

	case D3D_PRIMITIVE_TOPOLOGY_LINELIST:
		iNumIndices = m_iNumPolygons * 2;
		break;

	case D3D_PRIMITIVE_TOPOLOGY_POINTLIST:
		iNumIndices = m_iNumPolygons;
		break;
	}
	// m_pDevice_Context->DrawIndexed(iNumIndices, 0, 0);
	m_pDevice_Context->DrawIndexedInstanced(6, m_iNumInstance, 0, 0, 0);

	return S_OK;
}

void CVIBuffer_RectInstance::Update_Instance(_float TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;

	m_pDevice_Context->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		*((VTXMATRIX*)MappedSubResource.pData + i) = m_pInstanceVertices[i];
	}

	m_pDevice_Context->Unmap(m_pVBInstance, 0);
}

CVIBuffer_RectInstance * CVIBuffer_RectInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance)
{
	CVIBuffer_RectInstance*		pInstance = new CVIBuffer_RectInstance(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pTechniqueName, iNumInstance)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_RectInstance) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectInstance::Clone(void * pArg)
{
	CVIBuffer_RectInstance*		pInstance = new CVIBuffer_RectInstance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_RectInstance) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_RectInstance::Free()
{
	CVIBuffer::Free();

	Safe_Release(m_pVBInstance);

}
