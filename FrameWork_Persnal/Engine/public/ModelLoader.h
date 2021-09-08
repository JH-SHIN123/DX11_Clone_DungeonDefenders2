#pragma once

#ifndef __MODELLOADER_H__

#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CModelLoader  final : public CBase
{
	DECLARE_SINGLETON(CModelLoader)
public:
	CModelLoader();
	virtual ~CModelLoader() = default;

public:
	HRESULT Load_ModelFromFile(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, const char* pMeshFilePath, const char* pMeshFileName);

private:
	HRESULT Create_MeshContainer(class CModel* pModel, aiMesh* pMesh);
	HRESULT Create_Materials(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, class CModel* pModel, aiMaterial* pMaterial, const char* pMeshFilePath);


public:
	virtual void Free() override;
};

END
#define __MODELLOADER_H__
#endif // !__MODELLOADER_H__
