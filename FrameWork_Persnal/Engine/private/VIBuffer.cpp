#include "..\public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(m_pDevice, m_pDevice_Context)
{
}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
{
	Safe_AddRef(m_pVB);
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	// 여기서 상수 0 들은 어디서 부터 그릴거니에 관한것들이다.

	_uint iOffset = 0;

	m_pDevice_Context->IASetVertexBuffers(0, m_iNumVertexBuffers, &m_pVB, &m_iStride, &iOffset);
	m_pDevice_Context->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDevice_Context->IASetPrimitiveTopology(m_eTopology);
	//m_pDevice_Context->IASetInputLayout(); 여기서 쉐이더 사용

	_uint iNumIndices = 0;
	_uint iIndexBufferSize = sizeof(m_IBSubresourceData.pSysMem);

	if (m_eIndexFormat == DXGI_FORMAT_R16_UINT)
		iNumIndices = iIndexBufferSize >> 1;
	else
		iNumIndices = iIndexBufferSize >> 2;

	m_pDevice_Context->DrawIndexed(iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::SetUp_VertexBuffer_Desc(_uint iNumVertices, _uint iStride, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag)
{
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = iNumVertices;
	m_iStride = iStride;

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = Usage;
	m_VBDesc.BindFlags = BindFlag;
	m_VBDesc.CPUAccessFlags = iCpuAccessFlag;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	return S_OK;
}

HRESULT CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT eFormat, _uint iNumPolygons, D3D11_PRIMITIVE_TOPOLOGY eTopology, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag)
{
	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_eIndexFormat = eFormat;
	m_iIndicesStride = eFormat == DXGI_FORMAT_R16_UINT ? sizeof(POLYGONINDICES16) : sizeof(POLYGONINDICES32); // 개같은 삼항연산자
	m_iNumPolygons = iNumPolygons;
	m_eTopology = eTopology;

	m_IBDesc.ByteWidth = m_iIndicesStride * m_iNumPolygons;
	m_IBDesc.Usage = Usage;
	m_IBDesc.BindFlags = BindFlag;
	m_IBDesc.CPUAccessFlags = iCpuAccessFlag;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	return S_OK;
}

HRESULT CVIBuffer::SetUp_VertexSubResourceData(void * pSysMem)
{
	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_VBSubresourceData.pSysMem = pSysMem;

	m_pVertices = new _byte[m_iStride * m_iNumVertices];

	memcpy(m_pVertices, pSysMem, m_iStride * m_iNumVertices);

	return S_OK;
}

HRESULT CVIBuffer::SetUp_IndexSubResourceData(void * pSysMem)
{
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_IBSubresourceData.pSysMem = pSysMem;

	return S_OK;
}

void CVIBuffer::Free()
{
	Safe_Delete_Array(m_pVertices);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);

	CComponent::Free();
}
