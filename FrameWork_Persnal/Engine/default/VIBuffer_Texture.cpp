#include "VIBuffer_Texture.h"

CVIBuffer_Texture::CVIBuffer_Texture(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
}

CVIBuffer_Texture::CVIBuffer_Texture(const CVIBuffer & rhs)
{
}

HRESULT CVIBuffer_Texture::NativeConstruct(const _tchar * pShaderFilePath, const char * pTechniqueName, const _tchar * pTexturePath)
{
	return S_OK;
}

HRESULT CVIBuffer_Texture::NativeConstruct(void * pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Texture::SetUp_Texture(const _tchar * pTexturePath)
{
	return S_OK;
}

CVIBuffer_Texture * CVIBuffer_Texture::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CComponent * CVIBuffer_Texture::Clone(void * pArg)
{
	return nullptr;
}

void CVIBuffer_Texture::Free()
{
}
