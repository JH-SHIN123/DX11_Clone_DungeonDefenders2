#include "..\public\Model.h"
#include "Textures.h"
#include "Animation.h"
#include "AnimChannel.h"
#include "ModelLoader.h"
#include "HierarchyNode.h"
#include "MeshContainer.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
	, m_pModelLoader(CModelLoader::Create())
{

}

CModel::CModel(const CModel & rhs)
	: CComponent(rhs)
	, m_pModelLoader(rhs.m_pModelLoader)
	, m_pVB(rhs.m_pVB)
	, m_VBDesc(rhs.m_VBDesc)
	, m_VBSubresourceData(rhs.m_VBSubresourceData)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iStride(rhs.m_iStride)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_pIB(rhs.m_pIB)
	, m_IBDesc(rhs.m_IBDesc)
	, m_IBSubresourceData(rhs.m_IBSubresourceData)
	, m_iNumPolygons(rhs.m_iNumPolygons)
	, m_iIndicesStride(rhs.m_iIndicesStride)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, m_pEffect(rhs.m_pEffect)
	, m_InputLayouts(rhs.m_InputLayouts)
	, m_Vertices(rhs.m_Vertices)
	, m_PolygonIndices(rhs.m_PolygonIndices)
	/*, m_Meshes(rhs.m_Meshes)*/
	/*, m_SortMeshesByMaterial(rhs.m_SortMeshesByMaterial)*/
	, m_Materials(rhs.m_Materials)
	/*, m_HierarchyNodes(rhs.m_HierarchyNodes)*/
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_iCurrentAnimationIndex(rhs.m_iCurrentAnimationIndex)
	/*, m_Animations(rhs.m_Animations)*/
{
	Safe_AddRef(m_pModelLoader);

	Safe_AddRef(m_pEffect);
	Safe_AddRef(m_pIB);
	Safe_AddRef(m_pVB);

	for (auto& InputLayout : m_InputLayouts)
	{
		Safe_AddRef(InputLayout.pPass);
		Safe_AddRef(InputLayout.pLayout);
	}

	/* 새롭게 복제하여 생성한다. */
	for (auto& pOriginalMeshContainer : rhs.m_Meshes)
	{
		CMeshContainer*	pMeshContainer = pOriginalMeshContainer->Clone();
		m_Meshes.push_back(pMeshContainer);
	}
}

CMeshContainer * CModel::Get_MeshContainer(_uint iMeshIndex)
{
	if (iMeshIndex >= m_Meshes.size())
		return nullptr;

	return m_Meshes[iMeshIndex];
}

_bool CModel::Get_IsFinishedAnimaion()
{
	return m_Animations[m_iCurrentAnimationIndex]->Get_IsEnd();
}

_bool CModel::Get_IsFinishedAnimaion_Second()
{
	return m_Animations[m_iCurrentAnimationIndex]->Get_IsEnd_Second();
}

_float CModel::Get_AnimTime(_uint iAniIndex)
{
	return m_Animations[iAniIndex]->Get_AnimTime();
}

_fmatrix CModel::Get_BoneMatrix(const char * szBoneName)
{
	CHierarchyNode* pNode = Find_HierarchyNode(szBoneName);

	if (nullptr == pNode)
		return XMMatrixIdentity();

	return pNode->Get_CombindTransformationMatrix() * pNode->Get_OffSetMatrix();
}

void CModel::Set_AnimationIndex(_uint iAnimationIndex)
{
	if (iAnimationIndex >= m_iNumAnimations)
		return;

	m_iCurrentAnimationIndex = iAnimationIndex;
}

void CModel::Set_AnimationIndex_Start(_float fAnimationStart, _float fAnimationStart_Term, _uint iAnimationIndex)
{
	if (iAnimationIndex >= m_iNumAnimations)
		return;

	m_iCurrentAnimationIndex = iAnimationIndex;

	m_Animations[m_iCurrentAnimationIndex]->Set_AnimationIndex_Start(fAnimationStart, fAnimationStart_Term);
}

void CModel::Set_AnimationIndex_Start_SecondNode(_float fAnimationStart, _float fAnimationStart_Term, _uint iAnimationIndex)
{
	if (iAnimationIndex >= m_iNumAnimations)
		return;
	
	m_Animations[iAnimationIndex]->Set_AnimationIndex_Start_Second(fAnimationStart, fAnimationStart_Term);
}

HRESULT CModel::NativeConstruct_Prototype(const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix)
{
	if (nullptr == m_pModelLoader)
		return E_FAIL;

	__super::NativeConstruct_Prototype();

	if (FAILED(m_pModelLoader->Load_ModelFromFile(m_pDevice, m_pDevice_Context, this, pMeshFilePath, pMeshFileName)))
		return E_FAIL;

	/* 렌더링의 편의를 위해 재질이 같은 메시들을 모아둔다. */
	if (FAILED(Sort_MeshesByMaterial()))
		return E_FAIL;

	/* 부모 노드의 상태행렬들을 순차적으로 자식에게 누적시키기위해. */
	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pDest, CHierarchyNode* pSour)
	{
		return pDest->Get_Depth() < pSour->Get_Depth();
	});

	/* 뼈대정보를 로드한다. */
	if (FAILED(SetUp_SkinnedInfo(m_pModelLoader->Get_aiScene())))
		return E_FAIL;

	if (FAILED(Ready_VIBuffer(pShaderFilePath, pTechniqueName, PivotMatrix)))
		return E_FAIL;

	/* 애니메이션를 로드한다. */
	if (FAILED(SetUp_Animation(m_pModelLoader->Get_aiScene())))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	/* 새롭게 복제하여 생성한 메시를 정렬해준다.  */
	if (FAILED(Sort_MeshesByMaterial()))
		return E_FAIL;

	if (FAILED(m_pModelLoader->Ready_HierarchyNodes(this)))
		return E_FAIL;

	sort(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [](CHierarchyNode* pDest, CHierarchyNode* pSour)
	{
		return pDest->Get_Depth() < pSour->Get_Depth();
	});

	if (FAILED(SetUp_SkinnedInfo(m_pModelLoader->Get_aiScene())))
		return E_FAIL;

	if (FAILED(SetUp_Animation(m_pModelLoader->Get_aiScene())))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Ready_VIBuffer(const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix)
{
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

	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_IBDesc, &m_IBSubresourceData, &m_pIB)))
		return E_FAIL;

	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 60, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(SetUp_InputLayOuts(ElementDesc, 6, pShaderFilePath, pTechniqueName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Add_Vertex(const VTXMESH& Vertex)
{
	m_Vertices.push_back(Vertex);

	return S_OK;
}

HRESULT CModel::Add_PolgygonIndices(const POLYGONINDICES32& PolygonIndices)
{
	m_PolygonIndices.push_back(PolygonIndices);

	return S_OK;
}

HRESULT CModel::Add_MeshContainer(const char* pMeshName, _uint iStartPolygonIndex, _uint iStartVertexIndex, _uint iMaterialIndex)
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

HRESULT CModel::Add_HierarchyNode(CHierarchyNode * pHierarchyNode)
{
	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	return S_OK;
}

void CModel::Add_AnimChannelToHierarchyNode(_uint iAnimIndex, CAnimChannel * pChannel)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
	{
		return !strcmp(pChannel->Get_Name(), pNode->Get_Name());
	});

	(*iter)->Set_AnimChannelPointer(iAnimIndex, pChannel);
}

HRESULT CModel::Reserve_Vertices(_uint iNumVertices)
{
	m_Vertices.reserve(iNumVertices);

	return S_OK;
}

HRESULT CModel::Reserve_VIBuffer(_uint iNumVertices, _uint iNumFace)
{
	m_Vertices.reserve(iNumVertices);

	m_PolygonIndices.reserve(iNumFace);

	return S_OK;
}

HRESULT CModel::Set_Variable(const char * pConstanceName, void * pData, _int iByteSize)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectVariable*	pVariable = m_pEffect->GetVariableByName(pConstanceName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iByteSize);
}

HRESULT CModel::Set_ShaderResourceView(const char * pConstanceName, _uint iMaterialIndex, aiTextureType eMaterialType, _uint iTextureIndex)
{
	if (nullptr == m_pEffect)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable*	pVariable = m_pEffect->GetVariableByName(pConstanceName)->AsShaderResource();
	if (nullptr == pVariable)
		return E_FAIL;

	CTextures*		pTexture = m_Materials[iMaterialIndex]->pMaterialTexture[eMaterialType];
	if (nullptr == pTexture)
		return S_OK;

	ID3D11ShaderResourceView*		pShaderResourceView = pTexture->Get_ShaderResourceView(iTextureIndex);
	if (nullptr == pShaderResourceView)
		return E_FAIL;

	return pVariable->SetResource(pShaderResourceView);
}

HRESULT CModel::SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, _uint iNumElement, const _tchar* pShaderFilePath, const char* pTechniqueName)
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

HRESULT CModel::SetUp_Animation(const aiScene * pScene)
{
	if (nullptr == pScene)
		return E_FAIL;

	m_iNumAnimations = pScene->mNumAnimations;
	if (0 == m_iNumAnimations)
		return S_OK;


	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Reserve_AnimChannel(m_iNumAnimations);
	}

	for (_uint i = 0; i < m_iNumAnimations; ++i)
	{
		aiAnimation*		pAnim = pScene->mAnimations[i];
		if (nullptr == pAnim)
			return E_FAIL;

		CAnimation*		pAnimation = CAnimation::Create(pAnim->mName.data, (_float)pAnim->mDuration, (_float)pAnim->mTicksPerSecond);
		if (nullptr == pAnimation)
			return E_FAIL;

		_double		LastTime = 0.0;

		_uint		iNumChannel = pAnim->mNumChannels;

		_vector		vScale, vRotation, vPosition;
		vScale = XMVectorZero();
		vRotation = XMVectorZero();
		vPosition = XMVectorZero();;

		for (_uint j = 0; j < iNumChannel; ++j)
		{
			aiNodeAnim*			pNodeAnim = pAnim->mChannels[j];

			CAnimChannel*		pChannel = CAnimChannel::Create(pNodeAnim->mNodeName.data);
			if (nullptr == pChannel)
				return E_FAIL;

			_uint		iNumKeyFrames = pNodeAnim->mNumPositionKeys;
			iNumKeyFrames = max(iNumKeyFrames, pNodeAnim->mNumScalingKeys);
			iNumKeyFrames = max(iNumKeyFrames, pNodeAnim->mNumRotationKeys);

			for (_uint k = 0; k < iNumKeyFrames; ++k)
			{
				KEYFRAME*		pKeyFrame = new KEYFRAME;
				ZeroMemory(pKeyFrame, sizeof(KEYFRAME));

				if (pNodeAnim->mNumScalingKeys > k)
				{
					memcpy(&vScale, &pNodeAnim->mScalingKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pNodeAnim->mScalingKeys[k].mTime;
				}

				if (pNodeAnim->mNumRotationKeys > k)
				{
					vRotation = XMVectorSetX(vRotation, pNodeAnim->mRotationKeys[k].mValue.x);
					vRotation = XMVectorSetY(vRotation, pNodeAnim->mRotationKeys[k].mValue.y);
					vRotation = XMVectorSetZ(vRotation, pNodeAnim->mRotationKeys[k].mValue.z);
					vRotation = XMVectorSetW(vRotation, pNodeAnim->mRotationKeys[k].mValue.w);
					pKeyFrame->Time = pNodeAnim->mRotationKeys[k].mTime;
				}

				if (pNodeAnim->mNumPositionKeys > k)
				{
					memcpy(&vPosition, &pNodeAnim->mPositionKeys[k].mValue, sizeof(_float3));
					pKeyFrame->Time = pNodeAnim->mPositionKeys[k].mTime;
				}

				LastTime = max(LastTime, pKeyFrame->Time);

				memcpy(&pKeyFrame->vScale, &vScale, sizeof(_float3));
				memcpy(&pKeyFrame->vRotation, &vRotation, sizeof(_float4));
				memcpy(&pKeyFrame->vPosition, &vPosition, sizeof(_float3));

				pChannel->Add_KeyFrame(pKeyFrame);
			}

			Add_AnimChannelToHierarchyNode(i, pChannel);


			pAnimation->Add_Channel(pChannel);
		}
		pAnimation->Set_LastTime((_float)LastTime);
		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

HRESULT CModel::Sort_MeshesByMaterial()
{
	_uint		iNumMaterials = (_uint)m_Materials.size();

	m_SortMeshesByMaterial.resize(iNumMaterials);

	for (auto& pMeshContainer : m_Meshes)
	{
		_uint	iMaterialIndex = pMeshContainer->Get_MaterialIndex();
		if (iMaterialIndex >= iNumMaterials)
			return E_FAIL;

		m_SortMeshesByMaterial[iMaterialIndex].push_back(pMeshContainer);
		Safe_AddRef(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::SetUp_SkinnedInfo(const aiScene * pScene)
{
	for (_uint i = 0; i < pScene->mNumMeshes; ++i)
	{
		aiMesh*	pMesh = pScene->mMeshes[i];
		CMeshContainer*	pMeshContainer = m_Meshes[i];

		for (_uint j = 0; j < pMesh->mNumBones; ++j)
		{
			aiBone*		pBone = pMesh->mBones[j];
			if (nullptr == pBone)
				return E_FAIL;

			BONEDESC*		pBoneDesc = new BONEDESC;
			ZeroMemory(pBoneDesc, sizeof(BONEDESC));

			CHierarchyNode*	pHierarchyNode = Find_HierarchyNode(pBone->mName.data);
			pBoneDesc->pHierarchyNode = pHierarchyNode;
			Safe_AddRef(pHierarchyNode);

			memcpy(&pBoneDesc->OffsetMatrix, &XMMatrixTranspose(XMMATRIX(pBone->mOffsetMatrix[0])), sizeof(_matrix));
			pHierarchyNode->Set_OffsetMatrix(XMLoadFloat4x4(&pBoneDesc->OffsetMatrix));

			if (false == m_IsClone)
			{
				/* 정점의 블렌드인덱스와 블렌드 웨이트 값을 채운다. */
				for (_uint k = 0; k < pBone->mNumWeights; ++k)
				{
					_uint	iVertexIndex = pBone->mWeights[k].mVertexId + pMeshContainer->Get_StartVertexIndex();

					if (0.f == m_Vertices[iVertexIndex].vBlendWeight.x)
					{
						m_Vertices[iVertexIndex].vBlendIndex.x = j;
						m_Vertices[iVertexIndex].vBlendWeight.x = pBone->mWeights[k].mWeight;
					}
					else if (0.f == m_Vertices[iVertexIndex].vBlendWeight.y)
					{
						m_Vertices[iVertexIndex].vBlendIndex.y = j;
						m_Vertices[iVertexIndex].vBlendWeight.y = pBone->mWeights[k].mWeight;
					}
					else if (0.f == m_Vertices[iVertexIndex].vBlendWeight.z)
					{
						m_Vertices[iVertexIndex].vBlendIndex.z = j;
						m_Vertices[iVertexIndex].vBlendWeight.z = pBone->mWeights[k].mWeight;
					}
					else
					{
						m_Vertices[iVertexIndex].vBlendIndex.w = j;
						m_Vertices[iVertexIndex].vBlendWeight.w = pBone->mWeights[k].mWeight;
					}
				}
			}
			if (FAILED(pMeshContainer->Add_BoneDesc(pBoneDesc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel::Update_AnimaionMatrix(_float TimeDelta, _float fFrameSpeed)
{
/* 애니메이션의 재생이 있다면. 매프레임마다 호출. */
	if (m_iNumAnimations > 0)
	{
		/* 현재 애니메이션 채널들이 시간에 맞는 상태 변환값을 가지게 한다. */
		m_Animations[m_iCurrentAnimationIndex]->Update_Transform(TimeDelta, fFrameSpeed);
	}

	return S_OK;
}

HRESULT CModel::Update_AnimaionMatrix_Second(const char* szNodeName, _float TimeDelta, _uint iAnimationIndex, _float fFrameSpeed)
{
	/* 현재 애니메이션 채널들이 시간에 맞는 상태 변환값을 가지게 한다. */
	CHierarchyNode* pNode = Find_HierarchyNode(szNodeName);

	if (nullptr == pNode)
		return E_FAIL;

	vector<string> vecNodeNames;
	vecNodeNames.reserve(m_iNodesCount);

	vecNodeNames.emplace_back(pNode->Get_Name());


	m_Animations[iAnimationIndex]->Update_Transform_Node(pNode->Get_Name(), TimeDelta, fFrameSpeed);

	Update_Animation_Node(pNode, &vecNodeNames, TimeDelta, fFrameSpeed, iAnimationIndex, true);
	
	return S_OK;
}

HRESULT CModel::Update_CombindTransformationMatrix()
{
	for (auto& pHierarchyNode : m_HierarchyNodes)
	{
		pHierarchyNode->Update_CombindTransformationMatrix(m_iCurrentAnimationIndex);
	}
	return S_OK;
}

HRESULT CModel::Bind_VIBuffer()
{
	if (nullptr == m_pDevice ||
		nullptr == m_pDevice_Context)
		return E_FAIL;

	_uint		iOffset = 0;

	m_pDevice_Context->IASetVertexBuffers(0, m_iNumVertexBuffers, &m_pVB, &m_iStride, &iOffset);
	m_pDevice_Context->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pDevice_Context->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CModel::Set_Depth(const _uint & iDepthCount)
{
	m_iNodesDepth = iDepthCount;
}

HRESULT CModel::Render_Model(_uint iMaterialIndex, _uint iPassIndex)
{
	m_pDevice_Context->IASetInputLayout(m_InputLayouts[iPassIndex].pLayout);

	_matrix		BoneMatrices[128];

	for (auto& pMeshContainer : m_SortMeshesByMaterial[iMaterialIndex])
	{
		ZeroMemory(BoneMatrices, sizeof(_matrix) * 128);

		pMeshContainer->Compute_BoneMatrices(BoneMatrices);

		Set_Variable("g_BoneMatrices", BoneMatrices, sizeof(_matrix) * 128);

		if (FAILED(m_InputLayouts[iPassIndex].pPass->Apply(0, m_pDevice_Context)))
			return E_FAIL;

		m_pDevice_Context->DrawIndexed(3 * pMeshContainer->Get_NumPolygons(), 3 * pMeshContainer->Get_StartPolygonIndex(), pMeshContainer->Get_StartVertexIndex());
	}
	return S_OK;
}

CHierarchyNode* CModel::Find_HierarchyNode(const char * pNodeName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pHierarchyNode)->bool
	{
		return !strcmp(pNodeName, pHierarchyNode->Get_Name());
	});

	if (iter == m_HierarchyNodes.end())
		return nullptr;
	else
		return *iter;
}

CHierarchyNode * CModel::Find_HierarchyNode_Parent(const char * pNodeName, _uint iDepth, vector<string>* vecNodeNames)
{
	_bool IsReturn = true;

	for (_int i = 0; i < m_HierarchyNodes.size(); ++i)
	{
		if (nullptr != m_HierarchyNodes[i]->Get_ParentNode())
		{
			// 부모 노드 이름중에 내가 지명한 이름이랑 곂침?
			if (!strcmp(m_HierarchyNodes[i]->Get_Name_Parent(), pNodeName))
			{
				IsReturn = true;
				//목록에서 이름 곂치는지 검사 해봐
				for (auto& NameIter : *vecNodeNames)
				{
					if (!strcmp(m_HierarchyNodes[i]->Get_Name(), NameIter.data()))
					{
						IsReturn = false;
					}
				}

				if (true == IsReturn)
					return m_HierarchyNodes[i];
			}
		}
	}

	return nullptr;
}

HRESULT CModel::Update_Animation_Node(CHierarchyNode * pNode, vector <string>* vecNodeNames, _float TimeDelta, _float fFrameSpeed, _uint iAnimationIndex, _bool IsStartNode)
{
	// 필요한거 부모노드의 이름, 이름이 담긴 벡터
	// 노드의 부모노드 이름 == 부모노드의 이름
	// 내 이름 != 이름벡터
	
	// 재귀 이거 어떻게 탈출함 시발 ㅋㅋㅋ




	// 노드 하나의 바닥까지는 도달 가능
	CHierarchyNode* pChildNode = Find_HierarchyNode_Parent(pNode->Get_Name(), pNode->Get_Depth(), vecNodeNames);

	if (nullptr != pChildNode)
	{
		vecNodeNames->emplace_back(pChildNode->Get_Name());
		m_Animations[iAnimationIndex]->Update_Transform_Node(pChildNode->Get_Name(), TimeDelta, fFrameSpeed);
		Update_Animation_Node(pChildNode, vecNodeNames, TimeDelta, fFrameSpeed, iAnimationIndex, false);
	}
	else
	{
		if (false == IsStartNode)
		{
			auto& iter = vecNodeNames->front();

			if (!strcmp(iter.data(), pNode->Get_Name()))
				return S_OK;

			Update_Animation_Node(pNode->Get_ParentNode(), vecNodeNames, TimeDelta, fFrameSpeed, iAnimationIndex, false);
		}
		
	}


	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const char * pMeshFilePath, const char * pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pMeshFilePath, pMeshFileName, pShaderFilePath, pTechniqueName, PivotMatrix)))
	{
		MSG_BOX("Failed to Creating Instance (CModel) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CModel) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	if (false == m_IsClone)
	{
		m_Vertices.clear();

		for (auto& pMaterial : m_Materials)
		{
			for (auto& pTexture : pMaterial->pMaterialTexture)
				Safe_Release(pTexture);
			Safe_Delete(pMaterial);
		}

		m_Materials.clear();
	}

	for (auto& pMeshContainers : m_SortMeshesByMaterial)
	{
		for (auto& pMeshContainer : pMeshContainers)
			Safe_Release(pMeshContainer);
		pMeshContainers.clear();
	}
	m_SortMeshesByMaterial.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();

	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();

	for (auto& InputLayout : m_InputLayouts)
	{
		Safe_Release(InputLayout.pPass);
		Safe_Release(InputLayout.pLayout);
	}
	m_InputLayouts.clear();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pIB);
	Safe_Release(m_pVB);





	Safe_Release(m_pModelLoader);
}
