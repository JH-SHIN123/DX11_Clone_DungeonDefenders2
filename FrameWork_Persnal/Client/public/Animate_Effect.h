#pragma once

#ifndef __ANIMATE_EFFECT_H__

#include "UI_2D.h"

enum class eTextureOffSet
{ None, LeftTop, LeftBottom, RightTop, RightBottom, End };

typedef struct tagAnimateEffect_Desc
{
	_float4			vPos;
	_float2			vOffSet = { 0.f, 0.f };
	_bool			IsLoop = false;
	_float			fLoopTime = 0.f;
	_float			fAnimateSpeed = 2.f;
	_tchar			szTextureEX_Name[MAX_PATH] = L"";
	ELevel			ePrototypeTextureLevel = ELevel::Static;
	eTextureOffSet	eTexOffSet = eTextureOffSet::None;
}ANIEFFECT_DESC;

class CAnimate_Effect final : public CGameObject
{
private:
	explicit CAnimate_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CAnimate_Effect(const CAnimate_Effect& rhs);
	virtual ~CAnimate_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta)		override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render(_uint iShaderPass = 1);

private:
	HRESULT	Ready_Component(void* pArg);
	void	Dead_Check(_float TimeDelta);
	void	Texture_Scale_Check();
	
private:
	ANIEFFECT_DESC		m_AnimateEffect_Desc;
	_float				m_fTime = 0.f;
	_uint				m_iTextureNum = 1;
	CTextures_Extend*	m_pTextureExCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CMovement*			m_pMovementCom		= nullptr;
	CVIBuffer_Rect*		m_pBufferRectCom	= nullptr;


public:
	static CAnimate_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context); // pArg = BUTTON_DESC
	virtual CGameObject* Clone_GameObject(void* pArg) override;
	virtual void Free();
};

#define __ANIMATE_EFFECT_H__
#endif // !__ANIMATE_EFFECT_H__
