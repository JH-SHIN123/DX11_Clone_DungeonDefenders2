#pragma once

#ifndef __VIBUFFER_TERRAIN_H__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final :	public CVIBuffer
{
private:
	explicit CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const _tchar* pHeightMapPath, const _tchar* pShaderFilePath, const char* pTechniqueName); /* 버퍼를 생성한다.*/
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	HANDLE				m_hFile = 0;
	BITMAPFILEHEADER	m_fh;
	BITMAPINFOHEADER	m_ih;

	_ulong*				m_pPixels = nullptr;
	_ulong				m_dwNumVerticesX = 0;
	_ulong				m_dwNumVerticesZ = 0;


public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _tchar* pHeightMapPath, const _tchar* pShaderFilePath, const char* pTechniqueName);
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, _uint iNumVerticesX, _uint iNumVerticesZ, const _tchar* pShaderFilePath, const char* pTechniqueName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __VIBUFFER_TERRAIN_H__
#endif // !__VIBUFFER_RECT_H__
