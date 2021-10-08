#include "..\public\VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_hFile(rhs.m_hFile)
	, m_fh(rhs.m_fh)
	, m_ih(rhs.m_ih)
	, m_pPixels(rhs.m_pPixels)
	, m_dwNumVerticesX(rhs.m_dwNumVerticesX)
	, m_dwNumVerticesZ(rhs.m_dwNumVerticesZ)
{
}

HRESULT CVIBuffer_Terrain::NativeConstruct_Prototype(const _tchar* pHeightMapPath, const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	// For.VertexBuffer
	m_iNumVertexBuffers = 1; // 정점 배열을 몇개 할것이니

	// 읽을 파일 만들고
	m_hFile = CreateFile(pHeightMapPath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == m_hFile)
		return E_FAIL;

	_ulong	dwByte = 0;

	// bmp 파일은 세번 읽어야 제맛

	ReadFile(m_hFile, &m_fh, sizeof(m_fh), &dwByte, nullptr);
	if (0 == m_hFile)
		return E_FAIL;

	ReadFile(m_hFile, &m_ih, sizeof(m_ih), &dwByte, nullptr);
	if (0 == m_hFile)
		return E_FAIL;

	// 두번째로 읽은 Info Header에는 해당 bmp이미지의 길이(픽셀 수)를 가지고 있다
	// 바로 대입하고 있는데 이게 인터벌값을 정의 하지 않았지만 인터벌을 1.f로 사용 하려고 그런거
	m_dwNumVerticesX = m_ih.biWidth;
	m_dwNumVerticesZ = m_ih.biHeight;

	_ulong dwNumVertices = m_dwNumVerticesX * m_dwNumVerticesZ;

	// 픽셀 수 만큼 할당
	m_pPixels = new _ulong[dwNumVertices];

	ReadFile(m_hFile, m_pPixels, sizeof(_ulong) * dwNumVertices, &dwByte, nullptr);


	if (0 == dwByte)
		return E_FAIL;

	CloseHandle(m_hFile);

	// 정점의 개수 = dwNumVertices
	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(dwNumVertices, sizeof(VTXNORTEX), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	_float			fVertexInterval = 1.f;

	VTXNORTEX*		pVertices = new VTXNORTEX[dwNumVertices];

	for (_uint i = 0; i < m_dwNumVerticesZ; ++i)
	{
		for (_uint j = 0; j < m_dwNumVerticesX; ++j)
		{
			_uint		iIndex = i * m_dwNumVerticesX + j;

			pVertices[iIndex].vPosition = _float3(j * fVertexInterval, (m_pPixels[iIndex] & 0x000000ff) / 15.0f, i * fVertexInterval);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexUV = _float2((_float)j / (m_dwNumVerticesX - 1), (_float)i / (m_dwNumVerticesZ - 1));
		}
	}

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	/* For. IndexBuffer */
	if (FAILED(CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT_R32_UINT, (m_dwNumVerticesX - 1) * (m_dwNumVerticesZ - 1) * 2, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0)))
		return E_FAIL;

	POLYGONINDICES32*		pPolygonIndices = new POLYGONINDICES32[m_iNumPolygons];

	_uint		iNumPolygons = 0;

	for (_uint i = 0; i < m_dwNumVerticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_dwNumVerticesX - 1; ++j)
		{
			_uint		iIndex = i * m_dwNumVerticesX + j;
			_vector		vDest, vSour, vNormal;

			pPolygonIndices[iNumPolygons]._0 = iIndex + m_dwNumVerticesX;
			pPolygonIndices[iNumPolygons]._1 = iIndex + m_dwNumVerticesX + 1;
			pPolygonIndices[iNumPolygons]._2 = iIndex + 1;

			// 윗 삼각형 하나를 둘러싸는 정점의 노말값을 채워주고 있다.
			// 노말 += 외적((정규화 내적(정점1 - 정점2)) (정규화 내적(정점2 - 정점3)))
			// 노말값을 정점을 세팅하면서 000으로 채웠기에 누적이 되는 구조를 취해도 무방하다.
			vDest = XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vPosition) - XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vPosition);
			vSour = XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vPosition) - XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vNormal) + vNormal);

			++iNumPolygons;

			pPolygonIndices[iNumPolygons]._0 = iIndex + m_dwNumVerticesX;
			pPolygonIndices[iNumPolygons]._1 = iIndex + 1;
			pPolygonIndices[iNumPolygons]._2 = iIndex;

			// 아래 삼각형
			vDest = XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vPosition) - XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vPosition);
			vSour = XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vPosition) - XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vNormal, XMLoadFloat3(&pVertices[pPolygonIndices[iNumPolygons]._2].vNormal) + vNormal);

			++iNumPolygons;
		}
	}

	for (_uint i = 0; i < dwNumVertices; ++i)
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));

	if (FAILED(CVIBuffer::SetUp_IndexSubResourceData(pPolygonIndices)))
		return E_FAIL;

	if (FAILED(CVIBuffer::NativeConstruct_Prototype()))
		return E_FAIL;

	// 엇 Rect와 다른데 왜 되는거냐, 쉐이더파일이 별개다
	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	// 근데 쉐이더 구조체의 이름을 직접 명시한 적이 없는데 어떻게 알아 먹을까
	// 위에서 정해준 시멘틱스와 데이터 크기를 가지고 판단을 하나?

	if (FAILED(CVIBuffer::SetUp_InputLayOuts(ElementDesc, 3, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);
	Safe_Delete_Array(pPolygonIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::NativeConstruct(void * pArg)
{
	CVIBuffer::NativeConstruct(pArg);

	return S_OK;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pHeightMapPath, const _tchar* pShaderFilePath, const char* pTechniqueName)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pHeightMapPath, pShaderFilePath, pTechniqueName)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_Terrain) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain * CVIBuffer_Terrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, _uint iNumVerticesX, _uint iNumVerticesZ, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	// 직접 높이를 제어할 때
	return nullptr;
}

CComponent * CVIBuffer_Terrain::Clone(void * pArg)
{
	CVIBuffer_Terrain*		pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_Terrain) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	CVIBuffer::Free();
	
	if (false == m_IsClone)
	{
		Safe_Delete_Array(m_pPixels);
	}

}
