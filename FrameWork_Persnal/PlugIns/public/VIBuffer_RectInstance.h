#pragma once

#ifndef __VIBUFFER_RECT_INTANCE_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectInstance final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectInstance(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_RectInstance(const CVIBuffer_RectInstance& rhs);
	virtual ~CVIBuffer_RectInstance() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual HRESULT Render(_uint iPassIndex);
public:
	void Update_Instance(_double TimeDelta);

private:
	ID3D11Buffer*		m_pVBInstance = nullptr;
	_uint				m_iNumInstance = 0;

	VTXMATRIX*			m_pInstanceVertices = nullptr;

public:
	static CVIBuffer_RectInstance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName, _uint iNumInstance);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __VIBUFFER_RECT_INTANCE_H__
#endif //!__VIBUFFER_RECT_INTANCE_H__