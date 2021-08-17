#pragma once

#ifndef __TEXTURE_H__

#include "Component.h"

enum class ETextureType
{Dds, Tga, Wic, End};

class ENGINE_DLL CTextures final : public CComponent
{
private:
	explicit CTextures(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTextures(const CTextures& rhs);
	virtual ~CTextures() = default;

public: // Getter
	ID3D11ShaderResourceView* Get_ShaderResourceView(_uint iIdx) { return m_Textures[iIdx]; }

public:
	virtual	HRESULT NativeConstruct_Prototype(ETextureType eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual	HRESULT NativeConstruct(void* pArg) override;

private:
	typedef vector<ID3D11ShaderResourceView*> TEXTURES;
	TEXTURES m_Textures;

private:
	_uint m_iNumTextures = 0;

public:
	/* 원형 생성. */
	static CTextures* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ETextureType eType, const _tchar* pTextureFilePath, _int iNumTextures = 1);

	/* 복사본 생성. */
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();

};

#define __TEXTURE_H__
#endif // !__TEXTURE_H__
