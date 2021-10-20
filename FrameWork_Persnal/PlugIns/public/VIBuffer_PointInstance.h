#pragma once

#ifndef __VIBUFFER_POINTINSTANCE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_PointInstance final : public CVIBuffer
{
private:
	explicit CVIBuffer_PointInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_PointInstance(const CVIBuffer_PointInstance& rhs);
	virtual ~CVIBuffer_PointInstance() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex, _int iDrawCount = -1, _int iStartIndex = 0);

public:
	const _uint Get_NumInstance() { return m_iNumInstance; }
	VTXMATRIX*	Get_InstanceBuffer() { return m_pInstanceVtx; }

public:
	void Set_InstanceBuffer(VTXMATRIX* pInstanceBuffer) { memcpy(m_pInstanceVtx, pInstanceBuffer, sizeof(VTXMATRIX) * m_iNumInstance); }

public:
	void Update_Instance(_float TimeDelta);

private:
	D3D11_BUFFER_DESC			m_InstanceBufferDesc;
	D3D11_SUBRESOURCE_DATA		m_InstanceBufferSubresourceData;
	ID3D11Buffer*				m_pVBInstance = nullptr;
	_uint						m_iNumInstance = 0;

	VTXMATRIX*					m_pInstanceVtx = nullptr;

private:
	HRESULT SetUp_VertexBuffer_Desc(D3D11_USAGE Usage, _uint BindFlag, _uint iCpuAccessFlag);
	HRESULT SetUp_VertexSubResourceData(void* pSysMem);

public:
	static CVIBuffer_PointInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __VIBUFFER_POINTINSTANCE_H__
#endif // !__VIBUFFER_POINTINSTANCE_H__

/*
인스턴스 할 때 주목 해야 할 점
D3D11_INPUT_ELEMENT_DESC 의 값 세팅과 Map/Unmap
그리고 Render


























*/