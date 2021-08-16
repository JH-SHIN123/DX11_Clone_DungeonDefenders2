#pragma once

#ifndef __VIBUFFER_RECT_H__ 

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Texture final : public CVIBuffer
{
private:
	explicit CVIBuffer_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_Texture(const CVIBuffer& rhs);
	virtual ~CVIBuffer_Texture() = default;

public:
	virtual HRESULT	NativeConstruct(const _tchar* pShaderFilePath, const char* pTechniqueName, const _tchar* pTexturePath);
	virtual HRESULT NativeConstruct(void* pArg) override;
	HRESULT	SetUp_Texture(const _tchar* pTexturePath);

public:
	static CVIBuffer_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

private:


};

END
#define __VIBUFFER_RECT_H__
#endif // !__VIBUFFER_RECT_H__