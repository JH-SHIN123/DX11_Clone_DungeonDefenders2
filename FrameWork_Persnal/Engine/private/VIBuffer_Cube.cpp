#include "VIBuffer_Cube.h"

CVIBuffer_Cube::CVIBuffer_Cube(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{
}

CVIBuffer_Cube::CVIBuffer_Cube(const CVIBuffer & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Cube::NativeConstruct_Prototype(const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	m_iNumVertexBuffers = 1; // 정점 배열을 몇개 할것이니

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(8, sizeof(VTXTEX), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	VTXTEX_CUBE*		pVertices = new VTXTEX_CUBE[4];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, -0.5f);
	pVertices[0].vTexUV = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, -0.5f);
	pVertices[1].vTexUV = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, -0.5f);
	pVertices[2].vTexUV = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, -0.5f);
	pVertices[3].vTexUV = _float3(-0.5f, 0.5f, -0.5f);

	pVertices[4].vPosition = _float3(-0.5f, 0.5f, 0.5f);
	pVertices[4].vTexUV = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[5].vPosition = _float3(0.5f, 0.5f, 0.5f);
	pVertices[5].vTexUV = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[6].vPosition = _float3(0.5f, -0.5f, 0.5f);
	pVertices[6].vTexUV = _float3(-0.5f, 0.5f, 0.5f);

	pVertices[7].vPosition = _float3(-0.5f, -0.5f, 0.5f);
	pVertices[7].vTexUV = _float3(-0.5f, 0.5f, 0.5f);

	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	if (FAILED(CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT_R16_UINT, 12, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0)))
		return E_FAIL;

	POLYGONINDICES16*		pPolygonIndices = new POLYGONINDICES16[12];

	// 전면
	pPolygonIndices[0]._0 = 0;
	pPolygonIndices[0]._1 = 1;
	pPolygonIndices[0]._2 = 2;
	pPolygonIndices[1]._0 = 0;
	pPolygonIndices[1]._1 = 2;
	pPolygonIndices[1]._2 = 3;

	// 우측
	pPolygonIndices[2]._0 = 1;
	pPolygonIndices[2]._1 = 5;
	pPolygonIndices[2]._2 = 6;
	pPolygonIndices[3]._0 = 1;
	pPolygonIndices[3]._1 = 6;
	pPolygonIndices[3]._2 = 2;

	// 후면
	pPolygonIndices[4]._0 = 5;
	pPolygonIndices[4]._1 = 4;
	pPolygonIndices[4]._2 = 7;
	pPolygonIndices[5]._0 = 5;
	pPolygonIndices[5]._1 = 7;
	pPolygonIndices[5]._2 = 6;

	// 좌측
	pPolygonIndices[6]._0 = 4;
	pPolygonIndices[6]._1 = 0;
	pPolygonIndices[6]._2 = 3;
	pPolygonIndices[7]._0 = 4;
	pPolygonIndices[7]._1 = 7;
	pPolygonIndices[7]._2 = 3;

	// 머리
	pPolygonIndices[8]._0 = 4;
	pPolygonIndices[8]._1 = 5;
	pPolygonIndices[8]._2 = 1;
	pPolygonIndices[9]._0 = 4;
	pPolygonIndices[9]._1 = 1;
	pPolygonIndices[9]._2 = 0;

	// 바닥
	pPolygonIndices[10]._0 = 3;
	pPolygonIndices[10]._1 = 2;
	pPolygonIndices[10]._2 = 6;
	pPolygonIndices[11]._0 = 3;
	pPolygonIndices[11]._1 = 6;
	pPolygonIndices[11]._2 = 7;

	if (FAILED(CVIBuffer::SetUp_IndexSubResourceData(pPolygonIndices)))
		return E_FAIL;

	if (FAILED(CVIBuffer::NativeConstruct_Prototype()))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC	ElementDesc[] = {
		{ "POSITION"						// 시멘틱스(이런 용도~ 라고 알려줌)
		, 0									// 시멘틱스가 곂치는 애들 중에서 몇번째냐 (텍스처는 8개까지 지원이 가능하다, 인덱스)	
		, DXGI_FORMAT_R32G32B32_FLOAT		// 데이터는 어떤 형식이니 32비트 4개짜리 float (32bits = 4bytes)
		, 0									// 입력 어셈블러를 설정하는 정수 값 0~15라는데 잘 모르겠음
		, 0									// 정점의 해당 정보의 시작 부분 (byte 단위이다) 1빠니까 0
		, D3D11_INPUT_PER_VERTEX_DATA		// 정점이냐 인스턴스냐 인데 인스턴스를 잘 몰겠음
		, 0 },								// 인스턴스 수 라는데 인스턴스가 아니니 0

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 } // 위에있는걸 똑같이 침
	};

	// 레이아웃 설정 함수
	if (FAILED(CVIBuffer::SetUp_InputLayOuts(ElementDesc, 2, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Cube::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

CVIBuffer_Cube * CVIBuffer_Cube::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pTechniqueName)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_Cube) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Cube::Clone(void * pArg)
{
	CVIBuffer_Cube*		pInstance = new CVIBuffer_Cube(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CVIBuffer_Cube) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CVIBuffer_Cube::Free()
{
	__super::Free();
}
