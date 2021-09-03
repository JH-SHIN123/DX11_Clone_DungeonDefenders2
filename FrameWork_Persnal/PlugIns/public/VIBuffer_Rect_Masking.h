#pragma once

#ifndef __VIBUFFER_RECT_MASKING_H__

#include "VIBuffer.h"

BEGIN(Engine)
class CVIBuffer_Rect_Masking final : public CVIBuffer
{
private:
	explicit CVIBuffer_Rect_Masking(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_Rect_Masking(const CVIBuffer_Rect_Masking& rhs);
	virtual ~CVIBuffer_Rect_Masking() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Rect_Masking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __VIBUFFER_RECT_MASKING_H__
#endif // !__VIBUFFER_RECT_MASKING_H__
