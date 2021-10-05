#pragma once

#ifndef __MODELLOADER_H__

#include "Base.h"

BEGIN(Engine)
class CModelLoader final : public CBase
{
private:
	CModelLoader();
	virtual ~CModelLoader() = default;

public:
	const aiScene* Get_aiScene() const { return m_pScene; }

public:
	HRESULT Load_ModelFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, const char* pMeshFilePath, const char* pMeshFileName);
	HRESULT Ready_HierarchyNodes(class CModel* pModel);

private:
	const aiScene*			m_pScene = nullptr;
	Assimp::Importer		m_Importer;

private:
	HRESULT Create_MeshContainer(class CModel* pModel, aiMesh* pMesh);
	HRESULT Create_Materials(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, aiMaterial* pMaterial, const char* pMeshFilePath);
	HRESULT Create_HierarchyNode(CModel* pModel, aiNode* pNode, class CHierarchyNode* pParent = nullptr, _uint iDepth = 0);
	HRESULT Clone_HierarchyNode(CModel* pModel, CHierarchyNode* pHierarchyNode, class CHierarchyNode* pParent = nullptr, _uint iDepth = 0);


public:
	static CModelLoader* Create();
	virtual void Free();
};

END
#define __MODELLOADER_H__
#endif // !__MODELLOADER_H__
