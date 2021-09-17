#include "..\public\ModelLoader.h"
#include "Model.h"
#include "Textures.h"
#include "HierarchyNode.h"
#include "MeshContainer.h"

CModelLoader::CModelLoader()
{

}

HRESULT CModelLoader::Load_ModelFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, CModel * pModel, const char* pMeshFilePath, const char* pMeshFileName)
{
	char			szFullPath[MAX_PATH] = "";

	strcpy(szFullPath, pMeshFilePath);
	strcat(szFullPath, pMeshFileName);

	m_pScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (nullptr == m_pScene)
		return E_FAIL;

	_uint		iNumVertices = 0;

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
		iNumVertices += m_pScene->mMeshes[i]->mNumVertices;

	pModel->Reserve_Vertices(iNumVertices);


	/* 읽어진 메시의 갯수만큼 메시(내가 사용ㅇ하고자하는 형태로 변환된) 를 생성한다.  */
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		if (FAILED(Create_MeshContainer(pModel, m_pScene->mMeshes[i])))
			return E_FAIL;
	}

	/* 머티레얼을 로드하낟. */
	for (_uint i = 0; i < m_pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(pDevice, pDevice_Context, pModel, m_pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	/* 노드들의 계층구조를 생성한다. */
	if (FAILED(Ready_HierarchyNodes(pModel)))
		return E_FAIL;


	return S_OK;
}

HRESULT CModelLoader::Ready_HierarchyNodes(CModel* pModel)
{
	if (nullptr != m_pScene->mRootNode)
	{
		if (FAILED(Create_HierarchyNode(pModel, m_pScene->mRootNode)))
			return E_FAIL;
	}
	return S_OK;
}



HRESULT CModelLoader::Create_MeshContainer(CModel* pModel, aiMesh * pMesh)
{
	if (0 == pMesh->mNumVertices)
		return E_FAIL;

	_uint iStartVertexIndex = pModel->Get_NumVertices();

	/* 현재 메시에 정의된 정점들을 로드한다. */
	for (_uint i = 0; i < pMesh->mNumVertices; ++i)
	{
		VTXMESH			Vertex;
		ZeroMemory(&Vertex, sizeof(VTXMESH));

		memcpy(&Vertex.vPosition, &pMesh->mVertices[i], sizeof(_float3));

		memcpy(&Vertex.vNormal, &pMesh->mNormals[i], sizeof(_float3));

		memcpy(&Vertex.vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		memcpy(&Vertex.vTangent, &pMesh->mTangents[i], sizeof(_float3));

		pModel->Add_Vertex(Vertex);

	}

	_uint iStartPolygonIndex = pModel->Get_NumPolygonIndices();

	/* 현재 메시에 정의된 인덱스들을 로드한다. */
	for (_uint i = 0; i < pMesh->mNumFaces; ++i)
	{
		/* 인덱스 세개씩 값을 채워나간다. */
		POLYGONINDICES32 Indices;
		ZeroMemory(&Indices, sizeof(POLYGONINDICES32));

		Indices._0 = pMesh->mFaces[i].mIndices[0];
		Indices._1 = pMesh->mFaces[i].mIndices[1];
		Indices._2 = pMesh->mFaces[i].mIndices[2];

		pModel->Add_PolgygonIndices(Indices);
	}

	pModel->Add_MeshContainer(pMesh->mName.data, iStartPolygonIndex, iStartVertexIndex, pMesh->mMaterialIndex);

	return S_OK;
}

HRESULT CModelLoader::Create_Materials(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, CModel * pModel, aiMaterial * pMaterial, const char* pMeshFilePath)
{
	MESHTEXTURE*		pMaterialTexture = new MESHTEXTURE;
	ZeroMemory(pMaterialTexture, sizeof(MESHTEXTURE));

	for (_int i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		aiString		strTexturePath;

		if (FAILED(pMaterial->GetTexture(aiTextureType(i), 0, &strTexturePath)))
			continue;

		char			szTextureFileName[MAX_PATH] = "";
		char			szExt[MAX_PATH] = "";

		_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);

		strcat(szTextureFileName, szExt);

		char			szFullPath[MAX_PATH] = "";

		strcpy(szFullPath, pMeshFilePath);
		strcat(szFullPath, szTextureFileName);

		_tchar			szFullPath_w[MAX_PATH] = TEXT("");
		MultiByteToWideChar(CP_ACP, 0, szFullPath, (_uint)strlen(szFullPath), szFullPath_w, MAX_PATH);


		if (0 == strcmp(szExt, ".dds"))
			pMaterialTexture->pMaterialTexture[i] = CTextures::Create(pDevice, pDevice_Context, ETextureType::Dds, szFullPath_w);
		else if (0 == strcmp(szExt, ".tga"))
			pMaterialTexture->pMaterialTexture[i] = CTextures::Create(pDevice, pDevice_Context, ETextureType::Tga, szFullPath_w);
		else
			pMaterialTexture->pMaterialTexture[i] = CTextures::Create(pDevice, pDevice_Context, ETextureType::Wic, szFullPath_w);
	}

	pModel->Add_Materials(pMaterialTexture);

	return S_OK;
}

HRESULT CModelLoader::Create_HierarchyNode(CModel* pModel, aiNode * pNode, CHierarchyNode* pParent, _uint iDepth)
{
	_matrix		TransformationMatrix;

	memcpy(&TransformationMatrix, &pNode->mTransformation, sizeof(_matrix));

	CHierarchyNode*		pHierarchyNode = CHierarchyNode::Create(pNode->mName.data, TransformationMatrix, pParent, iDepth);
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	pModel->Add_HierarchyNode(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Create_HierarchyNode(pModel, pNode->mChildren[i], pHierarchyNode, iDepth + 1);

	return S_OK;
}

HRESULT CModelLoader::Clone_HierarchyNode(CModel * pModel, CHierarchyNode * pNode, CHierarchyNode * pParent, _uint iDepth)
{


	return E_NOTIMPL;
}



CModelLoader * CModelLoader::Create()
{
	return new CModelLoader;
}

void CModelLoader::Free()
{

}
