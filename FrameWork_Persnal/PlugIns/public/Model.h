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
	_uint Get_NumVertices() const { return (_uint)m_Vertices.size(); }
	_uint Get_NumPolygonIndices() const { return (_uint)m_PolygonIndices.size(); }
	_uint Get_NumMaterials() const { return (_uint)m_Materials.size(); }
	class CMeshContainer* Get_MeshContainer(_uint iMeshIndex);
	_bool Get_IsFinishedAnimaion();

public:
	void Set_AnimationIndex(_uint iAnimationIndex);
	void Set_AnimationIndex_Start(_float fAnimationStart, _float fAnimationStart_Term, _uint iAnimationIndex = 0);
	void Set_AnimationIndex_Start_Second(class CHierarchyNode* pNode, _float TimeDelta, _float fAnimationStart, _float fAnimationStart_Term, _float fFrameSpeed, _uint iAnimationIndex = 0);


public:
	virtual HRESULT NativeConstruct_Prototype(const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Ready_VIBuffer(const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix);
	HRESULT Add_Vertex(const VTXMESH& Vertex);
	HRESULT Add_PolgygonIndices(const POLYGONINDICES32& PolygonIndices);
	HRESULT Add_MeshContainer(const char* pMeshName, _uint iStartPolygonIndex, _uint iStartVertexIndex, _uint iMaterialIndex);
	HRESULT Add_Materials(MESHTEXTURE* pMeshMaterialTexture);
	HRESULT Add_HierarchyNode(class CHierarchyNode* pHierarchyNode);
	void	Add_AnimChannelToHierarchyNode(_uint iAnimIndex, class CAnimChannel* pChannel);
	HRESULT Reserve_Vertices(_uint iNumVertices);
	HRESULT Reserve_VIBuffer(_uint iNumVertices, _uint iNumFace);


	HRESULT Set_Variable(const char* pConstanceName, void* pData, _int iByteSize);
	HRESULT Set_ShaderResourceView(const char * pConstanceName, _uint iMaterialIndex, aiTextureType eMaterialType, _uint iTextureIndex = 0);

	HRESULT SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC * pInputElementDesc, _uint iNumElement, const _tchar * pShaderFilePath, const char * pTechniqueName);
	HRESULT SetUp_Animation(const aiScene* pScene);
	HRESULT Sort_MeshesByMaterial();
	HRESULT SetUp_SkinnedInfo(const aiScene* pScene);
	HRESULT Update_CombindTransformationMatrix(_float TimeDelta, _float fFrameSpeed = 1.f);
	HRESULT Bind_VIBuffer();

	HRESULT Render_Model(_uint iMaterialIndex, _uint iPassIndex);



private:
	class CModelLoader*		m_pModelLoader = nullptr;

private:
	vector<VTXMESH>						m_Vertices;
	typedef vector<VTXMESH>				VERTICES;

	vector<POLYGONINDICES32>			m_PolygonIndices;
	typedef vector<POLYGONINDICES32>	POLYGONINDICES;

private:
	vector<class CMeshContainer*>			m_Meshes;
	typedef vector<class CMeshContainer*>	MESHES;

	vector<vector<CMeshContainer*>>			m_SortMeshesByMaterial;

	vector<MESHTEXTURE*>				m_Materials;
	typedef vector<MESHTEXTURE*>		MATERIALS;

	vector<class CHierarchyNode*>		m_HierarchyNodes;

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
	class CHierarchyNode* Find_HierarchyNode(const char* pNodeName);




public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const char* pMeshFilePath, const char* pMeshFileName, const _tchar* pShaderFilePath, const char* pTechniqueName, _fmatrix PivotMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __MODEL_H__
#endif // !__MODEL_H__
