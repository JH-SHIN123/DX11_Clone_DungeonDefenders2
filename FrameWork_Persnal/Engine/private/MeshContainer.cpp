#include "..\public\MeshContainer.h"
#include "HierarchyNode.h"

CMeshContainer::CMeshContainer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(pDevice_Context);
}

void CMeshContainer::Set_LinkedNodePointer(CHierarcyNode * pLinkedNode)
{
	if (nullptr == m_pLinkedNode)
	{
		m_pLinkedNode = pLinkedNode;

		Safe_AddRef(m_pLinkedNode);
	}
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

void CMeshContainer::Free()
{
	Safe_Release(m_pLinkedNode);

	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
