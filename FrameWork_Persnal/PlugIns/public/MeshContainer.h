#pragma once

#ifndef __MESHCONTAINER_H__

#include "Base.h"

BEGIN(Engine)
class CMeshContainer final : public CBase
{
private:
	explicit CMeshContainer(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CMeshContainer() = default;

public:
	HRESULT NativeConstruct(const char* pMeshName, _uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex);

private:
	ID3D11Device*				m_pDevice = nullptr;
	ID3D11DeviceContext*		m_pDevice_Context = nullptr;

public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const char* pMeshName,
		_uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex);
	virtual void Free() override;
};

END
#define __MESHCONTAINER_H__
#endif // !__MESHCONTAINER_H__
