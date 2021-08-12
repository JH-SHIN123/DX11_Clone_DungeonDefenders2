#include "..\public\VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer & rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_VBDesc(rhs.m_VBDesc)
	, m_VBSubresourceData(rhs.m_VBSubresourceData)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_pVertices(rhs.m_pVertices)
	, m_pIB(rhs.m_pIB)
	, m_IBDesc(rhs.m_IBDesc)
	, m_IBSubresourceData(rhs.m_IBSubresourceData)
	, m_iNumPolygons(rhs.m_iNumPolygons)
	, m_iIndicesStride(rhs.m_iIndicesStride)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, m_pEffect(rhs.m_pEffect)
	, m_InputLayouts(rhs.m_InputLayouts)
{
	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);

	for (auto iter : m_InputLayouts)
	{
		Safe_AddRef(iter.pPass);
		Safe_AddRef(iter.pLayout);
	}
}

HRESULT CVIBuffer::NativeConstruct_Prototype()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubresourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render(_uint iPassIdx)
{
	if (nullptr == m_pDevice_Context)
		return E_FAIL;

	// 여기서 상수 0 들은 어디서 부터 그릴거니에 관한것들이다.

	_uint iOffset = 0;

	m_pDevice_Context->IASetVertexBuffers(0, m_iNumVertexBuffers, &m_pVB, &m_iStride, &iOffset);
	m_pDevice_Context->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDevice_Context->IASetPrimitiveTopology(m_eTopology);
	m_pDevice_Context->IASetInputLayout(m_InputLayouts[iPassIdx].pLayout);

	if (FAILED(m_InputLayouts[iPassIdx].pPass->Apply(0, m_pDevice_Context)))
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

HRESULT CVIBuffer::SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC * pInputElementDesc, _uint iNumElement, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	_uint iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // 쉐이더 코드 디버그 가능 + 최적화 스킵
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif // _DEBUG

	ID3DBlob*	pCompileShader = nullptr;
	ID3DBlob*	pCompileErrorMsg = nullptr;

	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileErrorMsg)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByName(pTechniqueName);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC	TechniqueDesc;
	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	m_InputLayouts.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		INPUTLAYOUTDESC		InputLayoutDesc;
		InputLayoutDesc.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC	PassDesc;
		InputLayoutDesc.pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pInputElementDesc, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &InputLayoutDesc.pLayout)))
			return E_FAIL;

		m_InputLayouts.emplace_back(InputLayoutDesc);
	}

	Safe_Release(pTechnique);
	Safe_Release(pCompileErrorMsg);
	Safe_Release(pCompileShader);

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
	CComponent::Free();

	if (false == m_IsClone)
	{
		Safe_Delete_Array(m_pVertices);
		Safe_Delete_Array(m_VBSubresourceData.pSysMem);
		Safe_Delete_Array(m_IBSubresourceData.pSysMem);
	}

	for (auto& InputLayout : m_InputLayouts)
	{
		Safe_Release(InputLayout.pPass);
		Safe_Release(InputLayout.pLayout);
	}
	m_InputLayouts.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
