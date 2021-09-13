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
	_uint Get_MaterialIndex()		const	{ return m_iMaterialIndex; }
	_uint Get_NumPolygons()			const	{ return m_iNumPolgygons; }
	_uint Get_StartPolygonIndex()	const	{ return m_iStartPolygonIndex; }
	_uint Get_StartVertexIndex()	const	{ return m_iStartVertexIndex; }

public:
	void Set_LinkedNodePointer(class CHierarcyNode* pLinkedNode); // 노드가 가지고 있는 행렬에 쉽게 접근이 가능하도록

public:
	HRESULT NativeConstruct(const char* pMeshName, _uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex);
	HRESULT Add_BoneDesc(BONEDESC* pBoneDesc);
	void	Compute_BoneMatrices(_matrix* pBoneMatrices);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

private:
	class CHierarcyNode*	m_pLinkedNode = nullptr;

private:
	char					m_szMeshName[MAX_PATH] = "";
	_uint					m_iStartPolygonIndex = 0;
	_uint					m_iNumPolgygons = 0;
	_uint					m_iStartVertexIndex = 0;
	_uint					m_iMaterialIndex = 0;

private:
	vector<BONEDESC*>		m_Bones;


public:
	static CMeshContainer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const char* pMeshName,
		_uint iStartPolygonIndex, _uint iNumPolgygons, _uint iStartVertexIndex, _uint iMaterialIndex);
	virtual void Free() override;
};

END
#define __MESHCONTAINER_H__
#endif // !__MESHCONTAINER_H__
