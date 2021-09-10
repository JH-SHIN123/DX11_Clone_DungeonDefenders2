#pragma once

#ifndef __TEXTURES_EXTEND_H__

#include "Textures.h"

BEGIN(Engine)
class ENGINE_DLL CTextures_Extend final : public CComponent
{
private:
	explicit CTextures_Extend(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTextures_Extend(const CTextures_Extend& rhs);
	virtual ~CTextures_Extend() = default;

public: /* Getter */
	ID3D11ShaderResourceView* Get_ShaderResourceView(_uint iIndex);

public:
	virtual HRESULT NativeConstruct_Prototype(ETextureType eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_float2 Get_TextureScale(_uint iTextureIndex) const;
	_uint Get_TexturesNum() const { return m_iNumTextures; }

private:
	// 텍스처를 담고 사용가능한 리소스뷰(쉐이더로 그려야함)
	// Render를 할 때는 그릴 영역(Rect)가 필요하다
	vector<ID3D11ShaderResourceView*>			m_Textures;
	typedef vector<ID3D11ShaderResourceView*>	TEXTURES;

	vector<_float2>	m_vecTextureScale;

private:
	_uint			m_iNumTextures = 0;	// 텍스처 개수

public:
	static CTextures_Extend* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ETextureType eType, const _tchar* pTextureFilePath, _int iNumTextures = 1);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};

END
#define __TEXTURES_EXTEND_H__
#endif // !__TEXTURES_EXTEND_H__
