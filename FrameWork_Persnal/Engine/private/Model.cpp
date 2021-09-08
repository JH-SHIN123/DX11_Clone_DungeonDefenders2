#include "..\public\Model.h"
#include "ModelLoader.h"
#include "MeshContainer.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
{
}

HRESULT CModel::NativeConstruct_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	if (nullptr == m_pModelLoader)
		return E_FAIL;

	__super::NativeConstruct_Prototype();

	// 모델로더 싱글톤을 이용해 자신(메쉬)에게 필요한 정보를 받아온다. 
	// 진짜 다받아온다
	if (FAILED(m_pModelLoader->Load_ModelFromFile(m_pDevice, m_pDevice_Context, this, pMeshFilePath, pMeshFileName)))
		return E_FAIL;

	// 
	if (FAILED(Ready_VIBuffer(pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

HRESULT CModel::Ready_VIBuffer(const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	// VIBuffer의 세팅과 비슷하다 크게 달라지는건 아직 없다.

	/* For. VertexBuffer */
	m_iNumVertexBuffers = 1;

	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = (_uint)m_Vertices.size();
	m_iStride = sizeof(VTXMESH);

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	ZeroMemory(&m_VBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_VBSubresourceData.pSysMem = &m_Vertices[0];
	m_VBSubresourceData.SysMemPitch = m_iStride * m_iNumVertices;

	/* For. IndexBuffer */

	ZeroMemory(&m_IBDesc, sizeof(D3D11_BUFFER_DESC));

	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_iIndicesStride = sizeof(POLYGONINDICES32);
	m_iNumPolygons = (_uint)m_PolygonIndices.size();
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_IBDesc.ByteWidth = m_iIndicesStride * m_iNumPolygons;
	m_IBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_IBDesc.CPUAccessFlags = 0;
	m_IBDesc.MiscFlags = 0;
	m_IBDesc.StructureByteStride = 0;

	ZeroMemory(&m_IBSubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	m_IBSubresourceData.pSysMem = &m_PolygonIndices[0];
	m_IBSubresourceData.SysMemPitch = m_iIndicesStride * m_iNumPolygons;

	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;


	if (FAILED(m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubresourceData, &m_pIB)))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",		 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",	 0, DXGI_FORMAT_R32G32B32_FLOAT,	0,	24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	 0, DXGI_FORMAT_R32G32_FLOAT,		0,	36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX",	 0, DXGI_FORMAT_R32G32B32A32_UINT,	0,	44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(SetUp_InputLayOuts(ElementDesc, 6, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Add_Vertex(VTXMESH * pVertex)
{
	if (nullptr == pVertex)
		return E_FAIL;

	m_Vertices.push_back(pVertex);

	return S_OK;
}

HRESULT CModel::Add_PolgygonIndices(POLYGONINDICES32 * pPolygonIndices)
{
	if (nullptr == pPolygonIndices)
		return E_FAIL;

	m_PolygonIndices.push_back(pPolygonIndices);

	return S_OK;
}

HRESULT CModel::Add_MeshContainer(const char * pMeshName, _uint iStartPolygonIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	CMeshContainer*		pMeshContainer = CMeshContainer::Create(m_pDevice, m_pDevice_Context, pMeshName, iStartPolygonIndex, (_uint)m_PolygonIndices.size() - iStartPolygonIndex, iStartVertexIndex, iMaterialIndex);
	if (nullptr == pMeshContainer)
		return E_FAIL;

	m_Meshes.push_back(pMeshContainer);

	return S_OK;
}

HRESULT CModel::Add_Materials(MESHTEXTURE * pMeshMaterialTexture)
{
	if (nullptr == pMeshMaterialTexture)
		return E_FAIL;

	m_Materials.push_back(pMeshMaterialTexture);

	return S_OK;
}

HRESULT CModel::Reserve_VIBuffer(_uint iNumVertices, _uint iNumFace)
{
	m_Vertices.reserve(iNumVertices);

	m_PolygonIndices.reserve(iNumFace);

	return S_OK;
}

HRESULT CModel::SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC * pInputElementDesc, _uint iNumElement, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	ID3DBlob*		pCompileShader = nullptr;
	ID3DBlob*		pCompileShaderErrorMsg = nullptr;

	if (FAILED(D3DCompileFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileShaderErrorMsg)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByName(pTechniqueName);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;

	//D3D11_RASTERIZER_DESC

	// D3D11_DEPTH_STENCIL_DESC

	// D3D11_BLEND_DESC		

	m_InputLayouts.reserve(TechniqueDesc.Passes);

	for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
	{
		INPUTLAYOUTDESC			InputLayoutDesc;

		InputLayoutDesc.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		PassDesc;
		InputLayoutDesc.pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(pInputElementDesc, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &InputLayoutDesc.pLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(InputLayoutDesc);
	}
	Safe_Release(pTechnique);
	Safe_Release(pCompileShaderErrorMsg);
	Safe_Release(pCompileShader);

	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const char * pMeshFilePath, const char * pMeshFileName, const _tchar * pShaderFilePath, const char * pTechniqueName)
{
	CModel*		pInstance = new CModel(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype(pMeshFilePath, pMeshFileName, pShaderFilePath, pTechniqueName)))
	{
		MSG_BOX("Failed to Creating Instance (CModel) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pClone = new CModel(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CModel) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CModel::Free()
{
	__super::Free();

	Safe_Release(m_pModelLoader);

}
