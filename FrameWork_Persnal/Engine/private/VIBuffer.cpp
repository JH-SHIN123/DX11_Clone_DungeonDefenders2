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

HRESULT CVIBuffer::Set_Variable(const char * pConstanceName, void * pData, _int iByteSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*	pVariable = m_pEffect->GetVariableByName(pConstanceName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iByteSize);
	// pConstanceName 쉐이더의 전역변수명
	// pData 어떤 값을 줄것인지
	// iByteSize 넘기려는 데이터의 사이즈
}

HRESULT CVIBuffer::Set_ShaderResourceView(const char * pConstanceName, ID3D11ShaderResourceView * pResourceView)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	// 얻어온 쉐이더리소스뷰를 통해서 리소스를 세팅하는 모씁
	// 문자열은 쉐이더의 전역변수
	// 이 단계를 거치고 Render를 하니 텍스처를 통해서 픽셀의 색이 정해질 것이다
	ID3DX11EffectShaderResourceVariable*	pVariable = m_pEffect->GetVariableByName(pConstanceName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetResource(pResourceView);
}

HRESULT CVIBuffer::SetUp_VertexBuffer_Desc(_uint iNumVertices, _uint iStride, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag)
{
	// Rect 자식 클래스 기준
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = iNumVertices;							// 4 찍을 정점의 개수
	m_iStride = iStride;									// 사용 할 구조체의 크기 VTXTEX

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;			// 바이트 길이(메모리 크기인가) 4 * 20
	m_VBDesc.Usage = Usage;										// GPU에서만 사용 할 것임 (하지만 직접 정점을 제어 할 필요가 있다면 DYNAMIC을 써야함)
	m_VBDesc.BindFlags = BindFlag;								// 이 버퍼의 사용 용도 VERTEX_BUFFER
	m_VBDesc.CPUAccessFlags = iCpuAccessFlag;					// CPU 액세스 유무
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;					// 구조체 크기

	return S_OK;
}

HRESULT CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT eFormat, _uint iNumPolygons, D3D11_PRIMITIVE_TOPOLOGY eTopology, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag)
{
	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_eIndexFormat = eFormat; // 포맷 형식
							  // 인덱스의 구조체의 크기
	m_iIndicesStride = eFormat == DXGI_FORMAT_R16_UINT ? sizeof(POLYGONINDICES16) : sizeof(POLYGONINDICES32); // 못생긴 삼항연산자
	m_iNumPolygons = iNumPolygons; // 폴리곤 개수 == 삼각형 개수
	m_eTopology = eTopology;	// 그리기 방식

	m_IBDesc.ByteWidth = m_iIndicesStride * m_iNumPolygons; // 인덱스가 사용 할 버퍼의 크기 (인덱스 구조체 * 폴리곤 개수)
	m_IBDesc.Usage = Usage;				// GPU에서만 사용 할 것임 (하지만 직접 정점을 제어 할 필요가 있다면 DYNAMIC을 써야함)
	m_IBDesc.BindFlags = BindFlag;		// 이 버퍼의 사용 용도 INDEX_BUFFER
	m_IBDesc.CPUAccessFlags = iCpuAccessFlag; // CPU 액세스 유무
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

	m_VBSubresourceData.pSysMem = pSysMem;					// 정점을 정의한 구조체의 포인터가 들어감

	m_pVertices = new _byte[m_iStride * m_iNumVertices];	// 멤버의 정점배열을 사용 할 크기만큼 선언 (구조체 크기 * 정점 개수)

	memcpy(m_pVertices, pSysMem, m_iStride * m_iNumVertices);// 복사

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
		//Safe_Delete_Array(m_VBSubresourceData.pSysMem);
		//Safe_Delete_Array(m_IBSubresourceData.pSysMem);
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
