#include "..\public\VIBuffer_Font.h"

CVIBuffer_Font::CVIBuffer_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CVIBuffer(pDevice, pDevice_Context)
{
}

CVIBuffer_Font::CVIBuffer_Font(const CVIBuffer_Font & rhs)
	: CVIBuffer(rhs)
{
}

HRESULT CVIBuffer_Font::NativeConstruct_Prototype(const _tchar * pShaderFilePath, const char * pTechniqueName, void* pArg)
{
	// For.VertexBuffer
	m_iNumVertexBuffers = 1; // 정점 배열을 몇개 할것이니

	if (FAILED(CVIBuffer::SetUp_VertexBuffer_Desc(4, sizeof(VTXTEX), D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0)))
		return E_FAIL;

	_float4 vTexUV; // Left, Top, Right, Bottom
	memcpy(&vTexUV, pArg, sizeof(_float4));

	// 익숙한 정점 세팅
	VTXTEX*		pVertices = new VTXTEX[4];

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexUV = _float2(vTexUV.x, vTexUV.y);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexUV = _float2(vTexUV.z, vTexUV.y);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexUV = _float2(vTexUV.z, vTexUV.w);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexUV = _float2(vTexUV.x, vTexUV.w);

	// 만든 정점들을 가지고 버퍼를 만드는 함수(memcpy를 한다)
	if (FAILED(CVIBuffer::SetUp_VertexSubResourceData(pVertices)))
		return E_FAIL;

	/* For. IndexBuffer */
	// 인덱스 초기화
	if (FAILED(CVIBuffer::SetUp_IndexBuffer_Desc(DXGI_FORMAT_R16_UINT, 2, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0)))
		return E_FAIL;

	// 인덱스 그거
	POLYGONINDICES16*		pPolygonIndices = new POLYGONINDICES16[2];

	pPolygonIndices[0]._0 = 0;
	pPolygonIndices[0]._1 = 1;
	pPolygonIndices[0]._2 = 2;

	pPolygonIndices[1]._0 = 0;
	pPolygonIndices[1]._1 = 2;
	pPolygonIndices[1]._2 = 3;

	// 위에서 만든 인덱스 정보를 대입
	// 인덱스의 서브리소스는 memcpy를 하지않음
	if (FAILED(CVIBuffer::SetUp_IndexSubResourceData(pPolygonIndices)))
		return E_FAIL;

	// 버텍스, 인덱스 세팅 끝, 이제 만들어 주셈
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

	Safe_Delete(pVertices);
	Safe_Delete(pPolygonIndices);
	return S_OK;
}

HRESULT CVIBuffer_Font::NativeConstruct(void * pArg)
{
	return S_OK;
}

CVIBuffer_Font * CVIBuffer_Font::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar * pShaderFilePath, const char * pTechniqueName, void* pArg)
{
	CVIBuffer_Font*		pInstance = new CVIBuffer_Font(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pShaderFilePath, pTechniqueName, pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CVIBuffer_Rect) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Font::Clone(void * pArg)
{
	return nullptr;
}

void CVIBuffer_Font::Free()
{
	__super::Free();
}
