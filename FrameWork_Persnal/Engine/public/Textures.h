#pragma once

#ifndef __TEXTURES_H__

#include "Component.h"

BEGIN(Engine)

enum class ENGINE_DLL ETextureType
{ Dds, Tga, Wic, End };

class ENGINE_DLL CTextures final : public CComponent
{	
private:
	explicit CTextures(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTextures(const CTextures& rhs);
	virtual ~CTextures() = default;

public: /* Getter */
	ID3D11ShaderResourceView* Get_ShaderResourceView(_uint iIndex);

public:
	virtual HRESULT NativeConstruct_Prototype(ETextureType eType, const _tchar* pTextureFilePath, _uint iNumTextures);
	virtual HRESULT NativeConstruct(void* pArg) override;

private:
	// 텍스처를 담고 사용가능한 리소스뷰(쉐이더로 그려야함)
	// Render를 할 때는 그릴 영역(Rect)가 필요하다
	vector<ID3D11ShaderResourceView*>			m_Textures;
	typedef vector<ID3D11ShaderResourceView*>	TEXTURES;

private:
	_uint			m_iNumTextures = 0;	// 텍스처 개수

public:
	/* 원형생성. */
	static CTextures* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ETextureType eType, const _tchar* pTextureFilePath, _int iNumTextures = 1);

	/* 복사본생성. */
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();
};
END
#define __TEXTURES_H__
#endif // !__TEXTURES_H__