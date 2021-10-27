#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_RectRHW final : public CVIBuffer
{
private:
	explicit CVIBuffer_RectRHW(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_RectRHW(const CVIBuffer_RectRHW& rhs);
	virtual ~CVIBuffer_RectRHW() = default;
public:
	virtual HRESULT NativeConstruct_Prototype(_float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath, const char* pTechniqueName); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_RectRHW* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, _float fX, _float fY, _float fSizeX, _float fSizeY, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END