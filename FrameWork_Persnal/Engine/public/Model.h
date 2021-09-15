#pragma once

#ifndef __MODEL_H__

#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CModel final : public CComponent
{
private:
	explicit CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	_uint Get_NumVertices()			const { return (_uint)m_Vertices.size(); }
	_uint Get_NumPolygonIndices()	const { return (_uint)m_PolygonIndices.size(); }
	_uint Get_NumMaterials()		const { return (_uint)m_Materials.size(); }

	class CMeshContainer* Get_MeshContainer(_uint iMeshIndex);


public:
	virtual HRESULT NativeConstruct_Prototype(const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Ready_VIBuffer(const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix);
	HRESULT Add_Vertex(VTXMESH* pVertex);
	HRESULT Add_PolgygonIndices(POLYGONINDICES32* pPolygonIndices);
	HRESULT Add_MeshContainer(const char* pMeshName, _uint iStartPolygonIndex, _uint iStartVertexIndex, _uint iMaterialIndex);
	HRESULT Add_Materials(MESHTEXTURE* pMeshMaterialTexture);
	HRESULT Add_HierarchyNode(class CHierarcyNode* pHierarchyNode);
	void	Add_AnimChannelToHierarchyNode(class CAnimChannel* pChannel);
	HRESULT Reserve_VIBuffer(_uint iNumVertices, _uint iNumFace);

	HRESULT Set_Variable(const char* pConstanceName, void* pData, _int iByteSize);
	HRESULT Set_ShaderResourceView(const char * pConstanceName, _uint iMaterialIndex, aiTextureType eMaterialType, _uint iTextureIndex = 0);

	HRESULT SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC * pInputElementDesc, _uint iNumElement, const _tchar * pShaderFilePath, const char * pTechniqueName);
	HRESULT SetUp_Animation(const aiScene* pScene);
	HRESULT Sort_MeshesByMaterial();
	HRESULT SetUp_SkinnedInfo(const aiScene* pScene);
	HRESULT Update_CombindTransformationMatrix(_float TimeDelta);
	HRESULT Bind_VIBuffer();

	HRESULT Render_Model(_uint iMaterialIndex, _uint iPassIndex);




private:
	class CModelLoader*		m_pModelLoader = nullptr;

private:
	typedef vector<VTXMESH*>		VERTICES;		// 모델의 모든 정점을 깡그리 다 담고있음
	VERTICES		m_Vertices;
	VTXMESH*		m_pVertices = nullptr;


	typedef vector<POLYGONINDICES32*>		POLYGONINDICES;	// 모델의 모든 인덱스를 깡그리 다 담고있음
	POLYGONINDICES			m_PolygonIndices;
	POLYGONINDICES32*		m_pPolygonIndices = nullptr;


private:
	typedef vector<class CMeshContainer*>	MESHES;			// 매쉬 하나가 구성되는데에 필요한 정점, 인덱스를 가지고있다.
	MESHES			m_Meshes;								//

	vector<vector<CMeshContainer*>>			m_SortMeshesByMaterial;

	typedef vector<MESHTEXTURE*>			MATERIALS;
	MATERIALS		m_Materials;

	vector<class CHierarcyNode*>			m_HierarchyNodes;

protected:
	_uint						m_iNumAnimations;
	_uint						m_iCurrentAnimationIndex = 0;
	vector<class CAnimation*>	m_Animations;

protected: /* For.Vertices */
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubresourceData;
	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0;
	_uint						m_iNumVertexBuffers = 0;

protected: /* For.Indices */
	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_IBDesc;
	D3D11_SUBRESOURCE_DATA		m_IBSubresourceData;
	_uint						m_iNumPolygons;
	_uint						m_iIndicesStride;
	DXGI_FORMAT					m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

protected: /* For.Shader */
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<INPUTLAYOUTDESC>		m_InputLayouts;

private:
	class CHierarcyNode* Find_HierarchyNode(const char* pNodeName);

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __MODEL_H__
#endif // !__MODEL_H__
