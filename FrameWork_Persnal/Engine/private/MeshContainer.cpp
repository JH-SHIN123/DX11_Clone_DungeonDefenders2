#include "..\public\MeshContainer.h"
#include "HierarchyNode.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(pDevice_Context);
}

CMeshContainer::CMeshContainer(const CMeshContainer & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDevice_Context(rhs.m_pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);

	strcpy(m_szMeshName, rhs.m_szMeshName);
	m_iStartPolygonIndex	= rhs.m_iStartPolygonIndex;
	m_iNumPolgygons			= rhs.m_iNumPolgygons;
	m_iStartVertexIndex		= rhs.m_iStartVertexIndex;
	m_iMaterialIndex		= rhs.m_iMaterialIndex;
}

HRESULT CMeshContainer::NativeConstruct(const char * pMeshName, _uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	strcpy(m_szMeshName, pMeshName);
	m_iStartPolygonIndex	= iStartPolygonIndex;
	m_iNumPolgygons			= iNumPolgygons;
	m_iStartVertexIndex		= iStartVertexIndex;
	m_iMaterialIndex		= iMaterialIndex;

	return S_OK;
}

HRESULT CMeshContainer::Add_BoneDesc(BONEDESC * pBoneDesc)
{
	if (nullptr == pBoneDesc)
		return E_FAIL;

	m_Bones.push_back(pBoneDesc);

	return S_OK;
}

void CMeshContainer::Compute_BoneMatrices(_matrix * pBoneMatrices)
{
	_uint		iIndex = 0;

	for (auto& pBoneDesc : m_Bones)
		pBoneMatrices[iIndex++] = XMMatrixTranspose(XMLoadFloat4x4(&pBoneDesc->OffsetMatrix) * pBoneDesc->pHierarchyNode->Get_CombindTransformationMatrix());
}

void CMeshContainer::Clone_Bones()
{
	vector<BONEDESC*>	Bones;

	for (auto& pBoneDesc : m_Bones)
		Bones.push_back(new BONEDESC(*pBoneDesc));

	m_Bones.clear();

	m_Bones = Bones;
}

CMeshContainer * CMeshContainer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const char * pMeshName, _uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex)
{
	CMeshContainer*		pInstance = new CMeshContainer(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pMeshName, iStartPolygonIndex, iNumPolgygons, iStartVertexIndex, iMaterialIndex)))
	{
		MSG_BOX("Failed to Creating Instance (CMeshContainer) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CMeshContainer* CMeshContainer::Clone()
{
	CMeshContainer*		pInstance = new CMeshContainer(*this);

	return pInstance;
}

void CMeshContainer::Free()
{
	for (auto& pBone : m_Bones)
	{
		Safe_Release(pBone->pHierarchyNode);
		Safe_Delete(pBone);
	}
	m_Bones.clear();

	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}