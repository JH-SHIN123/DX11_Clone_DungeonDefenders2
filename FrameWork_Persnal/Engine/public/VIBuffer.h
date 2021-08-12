#pragma once

#ifndef __VIBUFFER_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer abstract :
	public CComponent
{
protected:
	explicit CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIdx);

protected: // For.Vertices
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubresourceData;
	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0;
	_uint						m_iNumVertexBuffers = 0;

	void*						m_pVertices = nullptr;

protected: // For.Indices
	ID3D11Buffer*				m_pIB = nullptr;
	D3D11_BUFFER_DESC			m_IBDesc;
	D3D11_SUBRESOURCE_DATA		m_IBSubresourceData;
	_uint						m_iNumPolygons = 0;
	_uint						m_iIndicesStride = 0;
	DXGI_FORMAT					m_eIndexFormat;
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology;

protected: /* For.Shader */
	ID3DX11Effect*				m_pEffect = nullptr;

	vector<INPUTLAYOUTDESC>		m_InputLayouts;

protected:
	HRESULT SetUp_VertexBuffer_Desc(_uint iNumVertices, _uint iStride, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag);
	HRESULT SetUp_IndexBuffer_Desc(DXGI_FORMAT eFormat, _uint iNumPolygons, D3D11_PRIMITIVE_TOPOLOGY eTopology, D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag);
	HRESULT SetUp_InputLayOuts(D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, _uint iNumElement, const _tchar* pShaderFilePath, const char* pTechniqueName);// 쉐이더를 한번 초기화 하고 쭉 쓰자
	HRESULT SetUp_VertexSubResourceData(void* pSysMem);
	HRESULT SetUp_IndexSubResourceData(void* pSysMem);

public:
	virtual CComponent* Clone(void* pArg) PURE;
	virtual void Free() override;
};

END
#define __VIBUFFER_H__
#endif // !__VIBUFFER_H__
