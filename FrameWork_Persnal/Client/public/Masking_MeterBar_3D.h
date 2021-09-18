#pragma once

#ifndef __MASKING_METERBAR_3D_H__

#include "UI_3D.h"
#include "Masking_MeterBar.h"

BEGIN(Client)
typedef struct tagMask_MeterBar_desc3D
{
	_float						fCount;
	_float						fCount_Max;
	_bool						HasFrameBar;
	UI3D_DESC					UI_Desc;
	EMeterBar_FillMode			eFillMode;
	ECastingBar_Frame_Render	eFrame_Render;
}MASK_METERBAR_DESC_3D;

class CMasking_MeterBar_3D final : public CUI_3D
{
private:
	explicit CMasking_MeterBar_3D(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMasking_MeterBar_3D(const CMasking_MeterBar_3D& rhs);
	virtual ~CMasking_MeterBar_3D() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);				
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render(_uint MaskShaderPass = 12, _uint UIFramePass = 1);		

public: // Getter
	const _float	Get_Count() const { return m_MeterBar_Desc.fCount; }
	const _float	Get_Count_Max() const { return m_MeterBar_Desc.fCount_Max; }
	const _int		Get_Count_Int() const { return (_int)m_MeterBar_Desc.fCount; }
	const _int		Get_Count_Int_Max() const { return (_int)m_MeterBar_Desc.fCount_Max; }
	const _float	Get_Ratio() const { return m_fRatio; }

public: // Setter
	void Set_Count(_float fCount, _float fCount_Max);

public:
	void Render_Frame_First(_uint MaskShaderPass, _uint UIFramePass);
	void Render_Frame_Second(_uint MaskShaderPass, _uint UIFramePass);

private:
	void Count_Check(_float TimeDelta);


private: // 텍스처는 기본 세개: 프레임, 꾸물꾸물, 마스킹
	_float	m_fRatio = 0.f;
	_float	m_fTime = 0.f;
	MASK_METERBAR_DESC_3D m_MeterBar_Desc;

public:
	static CMasking_MeterBar_3D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void * pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MASKING_METERBAR_3D_H__
#endif // !__MASKING_METERBAR_3D_H__
