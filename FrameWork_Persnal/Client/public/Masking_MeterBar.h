#pragma once

#ifndef __MASKING_METERBAR_H__

#include "UI_2D.h"

BEGIN(Client)

enum class EMeterBar_FillMode
{ZeroToFull, FullToZero, End};

typedef struct tagMask_MeterBar_desc
{
	_float				fCount;
	_float				fCount_Max;
	_bool				HasFrameBar;
	_uint				iPassMaskShader_Index;
	UI2D_DESC			UI_Desc;
	EMeterBar_FillMode	eFillMode;

}MASK_METERBAR_DESC;


class CMasking_MeterBar final :	public CUI_2D
{
private:
	explicit CMasking_MeterBar(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMasking_MeterBar(const CMasking_MeterBar& rhs);
	virtual ~CMasking_MeterBar() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Getter
	const _float	Get_Count() const			{ return m_MeterBar_Desc.fCount; }
	const _float	Get_Count_Max() const		{ return m_MeterBar_Desc.fCount_Max; }
	const _int		Get_Count_Int() const		{ return (_int)m_MeterBar_Desc.fCount; }
	const _int		Get_Count_Int_Max() const	{ return (_int)m_MeterBar_Desc.fCount_Max; }
	const _float	Get_Ratio() const			{ return m_fRatio; }

public: // Setter
	void Set_Count(_float fCount, _float fCount_Max);

private:
	void Count_Check(_float TimeDelta);

public:
	static CMasking_MeterBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

private: // 텍스처는 기본 세개: 프레임, 꾸물꾸물, 마스킹
	_float	m_fRatio = 0.f;
	_float	m_fTime = 0.f;
	MASK_METERBAR_DESC m_MeterBar_Desc;

private:
	// class CGameObject*
};

END
#define __MASKING_METERBAR_H__
#endif // !__MASKING_METERBAR_H__
