#pragma once

#ifndef __VIBUFFER_FONT_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Font final : public CVIBuffer
{
private:
	explicit CVIBuffer_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_Font(const CVIBuffer_Font& rhs);
	virtual ~CVIBuffer_Font() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName, void* pArg); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName, void* pArg);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};
END
#define __VIBUFFER_FONT_H__
#endif // !__VIBUFFER_FONT_H__
