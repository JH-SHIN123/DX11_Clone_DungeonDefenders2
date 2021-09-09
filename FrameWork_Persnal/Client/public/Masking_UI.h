#pragma once

#ifndef __MASKING_UI_H__

#include "UI_2D.h"

BEGIN(Client)

typedef struct tagMasking_UI_Desc
{
	UI2D_DESC		UI_Desc;
	_bool			HasFrame = false;
	_bool			IsFrameFirst = false;
	_float3			vFrameScale;
	_float3			vFrameOffSetPos = { 0.f, 0.f, 0.f };

}MASK_UI_DESC;

class CMasking_UI final : public CUI_2D
{
private:
	explicit CMasking_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMasking_UI(const CMasking_UI& rhs);
	virtual ~CMasking_UI() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render(_uint MaskShaderPass = 12, _uint UIFramePass = 1);

public:
	static CMasking_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

private: // 텍스처는 기본 세개: 마스킹, 텍스처, 프레임
	MASK_UI_DESC		m_Mask_Desc;
	_float4x4			m_FrameMatrix;
};

END
#define __MASKING_UI_H__
#endif // !__MASKING_UI_H__
