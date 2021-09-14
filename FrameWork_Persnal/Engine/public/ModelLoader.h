#pragma once

#ifndef __MODELLOADER_H__

#include "Base.h"

BEGIN(Engine)
class CModelLoader final : public CBase
{
	DECLARE_SINGLETON(CModelLoader)
private:
	CModelLoader();
	virtual ~CModelLoader() = default;

public:
	HRESULT Load_ModelFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, const char* pMeshFilePath, const char* pMeshFileName);


private:
	HRESULT Create_MeshContainer(class CModel* pModel, aiMesh* pMesh);
	HRESULT Create_Materials(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, aiMaterial* pMaterial, const char* pMeshFilePath);
	HRESULT Create_HierarchyNode(CModel* pModel, aiNode* pNode, class CHierarcyNode* pParent = nullptr, _uint iDepth = 0);
	HRESULT Create_SkinedInfo(); /* 뼈정보를 캐치해오고 정점의 블렌드 인덱스와 웨이트를 채운다. */

public:
	virtual void Free();
};

END
#define __MODELLOADER_H__
#endif // !__MODELLOADER_H__
