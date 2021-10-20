#include "..\public\VIBuffer_PointInstance.h"

CVIBuffer_PointInstance::CVIBuffer_PointInstance(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{

}

CVIBuffer_PointInstance::CVIBuffer_PointInstance(const CVIBuffer_PointInstance & rhs)
	: CVIBuffer(rhs)
	, m_pVBInstance(rhs.m_pVBInstance)
	, m_iNumInstance(rhs.m_iNumInstance)
	, m_pInstanceVtx(rhs.m_pInstanceVtx)
	, m_InstanceBufferDesc(rhs.m_InstanceBufferDesc)
	, m_InstanceBufferSubresourceData(rhs.m_InstanceBufferSubresourceData)
{
	Safe_AddRef(m_pVBInstance);

}

HRESULT CVIBuffer_PointInstance::NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance)
{
	/* For. VertexBuffer */
	m_iNumVertexBuffers = 2;
	m_iNumInstance = iNumInstance;

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(m_iNumInstance, sizeof(VTXPOINT), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	VTXPOINT* pVertices = new VTXPOINT[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		pVertices[i].vPosition = _float3(0.0f, 0.0f, 0.f);
	}

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	/* 인스턴스 버퍼를 생성하자. (사각형각가의 상태를 표현해주는 행렬을 정점형태로 보관. */
	if (FAILED(SetUp_VertexBuffer_Desc(D3D11_USAGE_DYNAMIC, D3D11_BIND_VERTEX_BUFFER, D3D11_CPU_ACCESS_WRITE)))
		return E_FAIL;

	m_pInstanceVtx = new VTXMATRIX[m_iNumInstance];

	for (_uint i = 0; i < m_iNumInstance; ++i)
	{
		m_pInstanceVtx[i].vSize = _float2(1.f, 1.f);
		m_pInstanceVtx[i].vRight = _float4(1.0f, 0.0f, 0.f, 0.f);
		m_pInstanceVtx[i].vUp = _float4(0.0f, 1.0f, 0.f, 0.f);
		m_pInstanceVtx[i].vLook = _float4(0.0f, 0.0f, 1.f, 0.f);
		m_pInstanceVtx[i].vPosition = _float4(0.f, 0.f, 0.f, 1.f);
	}

	if (FAILED(SetUp_VertexSubResourceData(m_pInstanceVtx)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_InstanceBufferDesc, &m_InstanceBufferSubresourceData, &m_pVBInstance)))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		{ "PSIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 8, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 24, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 40, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 56, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};

	if (FAILED(CVIBuffer::SetUp_InputLayOuts(ElementDesc, 6, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::NativeConstruct(void * pArg)
{
	CVIBuffer::NativeConstruct(pArg);




	return S_OK;
}

HRESULT CVIBuffer_PointInstance::Render(_uint iPassIndex, _int iDrawCount, _int iStartIndex)
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	if (-1 == iDrawCount)
		iDrawCount = m_iNumInstance;


	_uint		iOffset = 0;

	ID3D11Buffer* const 	pBuffers[2] = { m_pVB, m_pVBInstance };
	const _uint	 iStrides[2] = { sizeof(VTXPOINT), sizeof(VTXMATRIX) };
	const _uint		iOffsets[2] = { 0, 0 };

	m_pDevice_Context->IASetVertexBuffers(0, m_iNumVertexBuffers, pBuffers, iStrides, iOffsets);
	m_pDevice_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_pDevice_Context->IASetInputLayout(m_InputLayouts[iPassIndex].pLayout);

	if (FAILED(m_InputLayouts[iPassIndex].pPass->Apply(0, m_pDevice_Context)))
		return E_FAIL;

	m_pDevice_Context->DrawInstanced(1, iDrawCount, iStartIndex, iStartIndex);

	return S_OK;
}

void CVIBuffer_PointInstance::Update_Instance(_float TimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE		MappedSubResource;

	m_pDevice_Context->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedSubResource);

	for (_uint i = 0; i < m_iNumInstance; ++i)
		*((VTXMATRIX*)MappedSubResource.pData + i) = m_pInstanceVtx[i];

	m_pDevice_Context->Unmap(m_pVBInstance, 0);

}

HRESULT CVIBuffer_PointInstance::SetUp_VertexBuffer_Desc(D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag)
{
	ZeroMemory(&m_InstanceBufferDesc, sizeof(D3D11_BUFFER_DESC));

	m_InstanceBufferDesc.ByteWidth = sizeof(VTXMATRIX) * m_iNumInstance;
	m_InstanceBufferDesc.Usage = Usage;
	m_InstanceBufferDesc.BindFlags = BindFlag;
	m_InstanceBufferDesc.CPUAccessFlags = iCpuAccessFlag;
	m_InstanceBufferDesc.MiscFlags = 0;
	m_InstanceBufferDesc.StructureByteStride = sizeof(VTXMATRIX);

	return S_OK;
}

HRESULT CVIBuffer_PointInstance::SetUp_VertexSubResourceData(void * pSysMem)
{
	ZeroMemory(&m_InstanceBufferSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_InstanceBufferSubresourceData.pSysMem = pSysMem;

	return S_OK;
}

CVIBuffer_PointInstance * CVIBuffer_PointInstance::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance)
{
	CVIBuffer_PointInstance*		pInstance = new CVIBuffer_PointInstance(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pTechniqueName, iNumInstance)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_PointInstance) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_PointInstance::Clone(void * pArg)
{
	CVIBuffer_PointInstance*		pInstance = new CVIBuffer_PointInstance(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_PointInstance) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_PointInstance::Free()
{
	CVIBuffer::Free();

	if (false == m_IsClone)
	{
		Safe_Delete_Array(m_pInstanceVtx);
	}

	Safe_Release(m_pVBInstance);

}
