#include "..\public\Textures.h"



CTextures::CTextures(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CTextures::CTextures(const CTextures & rhs)
	: CComponent(rhs)
	, m_Textures(rhs.m_Textures)
	, m_iNumTextures(rhs.m_iNumTextures)
{
	for (auto& pShaderResourceView : m_Textures)
		Safe_AddRef(pShaderResourceView);
}

HRESULT CTextures::NativeConstruct_Prototype(ETextureType eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	CComponent::NativeConstruct_Prototype();

	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED))) // ¿©10 ¿ÃΩ¥
		return E_FAIL;

	m_iNumTextures = iNumTextures;

	_tchar	szTextureFilePath[MAX_PATH] = L"";

	for (_uint i = 0; i < m_iNumTextures; ++i)
	{
		ID3D11ShaderResourceView* pShaderResourceView = nullptr;

		wsprintf(szTextureFilePath, pTextureFilePath, i);

		DirectX::ScratchImage Image;

		HRESULT hr = S_OK;

		switch (eType)
		{
		case ETextureType::Dds:
			hr = DirectX::LoadFromDDSFile(szTextureFilePath, DirectX::DDS_FLAGS_NONE, nullptr, Image);
			break;
		case ETextureType::Tga:
			hr = DirectX::LoadFromTGAFile(szTextureFilePath, nullptr, Image);
			break;
		case ETextureType::Wic:
			hr = DirectX::LoadFromWICFile(szTextureFilePath, DirectX::WIC_FLAGS_NONE, nullptr, Image);
			break;
		default:
			hr = E_FAIL;
			break;
		}

		if (FAILED(hr))
		{
			MSG_BOX("ERROR! Can't Load Texture File");
			return E_FAIL;
		}

		ID3D11Resource* pResource = nullptr;

		if (FAILED(DirectX::CreateTexture(m_pDevice, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &pResource)))
		{
			MSG_BOX("ERROR! Can't Create Texture File");
			return E_FAIL;
		}

		if (FAILED(m_pDevice->CreateShaderResourceView(pResource, nullptr, &pShaderResourceView)))
		{
			MSG_BOX("ERROR! Can't Create ShaderResourceView");
			return E_FAIL;
		}

		Safe_Release(pResource);

		m_Textures.emplace_back(pShaderResourceView);

	}

	return S_OK;
}

HRESULT CTextures::NativeConstruct(void * pArg)
{
	CComponent::NativeConstruct(pArg);

	return S_OK;
}

CTextures * CTextures::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, ETextureType eType, const _tchar * pTextureFilePath, _int iNumTextures)
{
	CTextures*		pInstance = new CTextures(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(eType, pTextureFilePath, iNumTextures)))
	{
		MSG_BOX("Failed to Creating Instance (CTextures) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTextures::Clone(void * pArg)
{
	CTextures*		pInstance = new CTextures(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CTextures) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTextures::Free()
{
	for (auto& pShaderResourceView : m_Textures)
		Safe_Release(pShaderResourceView);
	m_Textures.clear();

	CComponent::Free();
}
