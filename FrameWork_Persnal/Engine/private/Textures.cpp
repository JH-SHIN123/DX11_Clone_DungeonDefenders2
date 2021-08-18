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
	// 프로토 타입에서 만들었으니 대입하고 레퍼런스 카운팅
	for (auto& pShaderResourceView : m_Textures)
		Safe_AddRef(pShaderResourceView);
}

ID3D11ShaderResourceView * CTextures::Get_ShaderResourceView(_uint iIndex)
{
	if (m_iNumTextures < iIndex || true == m_Textures.empty())
		return nullptr;

	return m_Textures[iIndex];
}

HRESULT CTextures::NativeConstruct_Prototype(ETextureType eType, const _tchar * pTextureFilePath, _uint iNumTextures)
{
	// 이미지 파일을 세팅하는 그런 부분
	CComponent::NativeConstruct_Prototype();

	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED)))	// 윈 10 이슈 
		return E_FAIL;

	m_iNumTextures = iNumTextures;

	_tchar			szTextureFileName[MAX_PATH] = L"";

	// 이미지 개수만큼
	for (_uint i = 0; i < iNumTextures; ++i)
	{
		// 텍스처 Load
		DirectX::ScratchImage		Image;

		// Load한 텍스처를 리소스 화
		ID3D11Resource*				pResource = nullptr;

		// 리소스를 최종적으로 사용 가능한 형태로 다루게하는 쉐이더리소스뷰
		ID3D11ShaderResourceView*	pShaderResourceView = nullptr;

		wsprintf(szTextureFileName, pTextureFilePath, i);

		HRESULT hr = 0;

		// 확장자별로 Load함수가 다르다.
		switch (eType)
		{
		case ETextureType::Dds:
			hr = DirectX::LoadFromDDSFile(szTextureFileName, DirectX::DDS_FLAGS_NONE, nullptr, Image);
			break;
		case ETextureType::Tga:
			// 얘는 인자가 살짝 다르다
			hr = DirectX::LoadFromTGAFile(szTextureFileName, nullptr, Image);
			break;
		case ETextureType::Wic:
			hr = DirectX::LoadFromWICFile(szTextureFileName, DirectX::WIC_FLAGS_NONE, nullptr, Image);
			break;
		}

		if (FAILED(hr))
			return E_FAIL;

		// 텍스처 정보를 토대로 리소스를 만들고있다.
		if (FAILED(DirectX::CreateTexture(m_pDevice, Image.GetImages(), Image.GetImageCount(), Image.GetMetadata(), &pResource)))
			return E_FAIL;

		// 실질적으로 사용 가능한 형태의 쉐이더리소스뷰로 만드는 모습
		if (FAILED(m_pDevice->CreateShaderResourceView(pResource, nullptr, &pShaderResourceView)))
			return E_FAIL;

		// pResource는 거쳐가는 단계이고 포인터니 지운다.
		Safe_Release(pResource);

		m_Textures.push_back(pShaderResourceView);
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
	CComponent::Free();

	for (auto& pShaderResourceView : m_Textures)
		Safe_Release(pShaderResourceView);

	m_Textures.clear();
}
