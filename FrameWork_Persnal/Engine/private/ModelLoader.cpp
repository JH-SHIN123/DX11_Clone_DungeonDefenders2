#include "..\public\ModelLoader.h"
#include "Model.h"
#include "Textures.h"

IMPLEMENT_SINGLETON(CModelLoader)

CModelLoader::CModelLoader()
{
}

HRESULT CModelLoader::Load_ModelFromFile(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, CModel * pModel, const char * pMeshFilePath, const char * pMeshFileName)
{
	// 모델 만들기중 첫번째로 우선 파일을 읽어오자
	char			szFullPath[MAX_PATH] = "";

	// 경로와 파일이름을 따로 받았는데 이는 모델의 구성에 필요한
	// 머테리얼(텍스처)값이나 뼈대정보 또는 애니메이션따위를 로드하기 위함이다.
	strcpy(szFullPath, pMeshFilePath);
	strcat(szFullPath, pMeshFileName);

	Assimp::Importer			Importer;

	// 메쉬는 파일로부터 읽어진 상황 이제 사용 할 수 있게 변환을 거쳐야 한다.
	const aiScene*		pScene = Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_CalcTangentSpace);
	if (nullptr == pScene)
		return E_FAIL;

	/* 읽어진 메시의 갯수만큼 메시(내가 사용하고자하는 형태로 변환된) 를 생성한다.  */
	for (_int i = 0; i < pScene->mNumMeshes; ++i)
	{
		if (FAILED(Create_MeshContainer(pModel, pScene->mMeshes[i])))
			return E_FAIL;
	}

	/* 머티레얼을 로드하낟. */
	for (_int i = 0; i < pScene->mNumMaterials; ++i)
	{
		if (FAILED(Create_Materials(pDevice, pDevice_Context, pModel, pScene->mMaterials[i], pMeshFilePath)))
			return E_FAIL;
	}

	/* 뼈대정보를 로드한다. */

	/* 애니메이션를 로드한다. */



	return S_OK;
}

HRESULT CModelLoader::Create_MeshContainer(CModel * pModel, aiMesh * pMesh)
{
	if (0 == pMesh->mNumVertices)
		return E_FAIL;

	_uint iStartVertexIndex = pModel->Get_NumVertices(); // 처음에는 0이 들어와야겠지

	/* 현재 메시에 정의된 정점들을 로드한다. (정점하나의 정보가 상당히 많다)*/
	// 아마 내가 더 필요한 정보가 있다라면 mempcy를 더 하면 되것지 머
	for (_int i = 0; i < pMesh->mNumVertices; ++i)
	{
		VTXMESH*		pVertex = new VTXMESH;
		ZeroMemory(pVertex, sizeof(VTXMESH));

		// 정점
		memcpy(&pVertex->vPosition, &pMesh->mVertices[i], sizeof(_float3));

		// 노말맵
		memcpy(&pVertex->vNormal, &pMesh->mNormals[i], sizeof(_float3));

		// 텍스처
		memcpy(&pVertex->vTexUV, &pMesh->mTextureCoords[0][i], sizeof(_float2));

		// 탄젠트 스페이스
		memcpy(&pVertex->vTangent, &pMesh->mTangents[i], sizeof(_float3));

		// 단순히 컨테이너에 담기
		pModel->Add_Vertex(pVertex);
	}

	_uint iStartPolygonIndex = pModel->Get_NumPolygonIndices(); // 처음에는 0이 들어와야겠지

	/* 현재 메시에 정의된 인덱스들을 로드한다. */
	for (_int i = 0; i < pMesh->mNumFaces; ++i)
	{
		/* 인덱스 세개씩 값을 채워나간다. */
		POLYGONINDICES32* pIndices = new POLYGONINDICES32;
		ZeroMemory(pIndices, sizeof(POLYGONINDICES32));

		pIndices->_0 = pMesh->mFaces[i].mIndices[0];
		pIndices->_1 = pMesh->mFaces[i].mIndices[1];
		pIndices->_2 = pMesh->mFaces[i].mIndices[2];

		// 단순 컨테이너 담기
		pModel->Add_PolgygonIndices(pIndices);
	}

	// 메쉬 하나의 정점, 인덱스 정보 세팅이 끝났다
	// 1메쉬 = 1메쉬 컨테이너 객체
	pModel->Add_MeshContainer(pMesh->mName.data, iStartPolygonIndex, iStartVertexIndex, pMesh->mMaterialIndex);

	// 이제 메쉬 하나 만들었다.
	// 메쉬 개수만큼 for문을 돌아서 모델을 완성하자
	return S_OK;
}

HRESULT CModelLoader::Create_Materials(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, CModel * pModel, aiMaterial * pMaterial, const char * pMeshFilePath)
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
		MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szFullPath_w, MAX_PATH);

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

void CModelLoader::Free()
{
}
