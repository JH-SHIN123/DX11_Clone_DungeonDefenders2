#pragma once

#ifndef __VIBUFFER_CUBE_H__

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Cube final : public CVIBuffer
{
private:
	explicit CVIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_Cube(const CVIBuffer& rhs);
	virtual ~CVIBuffer_Cube() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pShaderFilePath, const char* pTechniqueName); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	static CVIBuffer_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __VIBUFFER_CUBE_H__
#endif // !__VIBUFFER_CUBE_H__
