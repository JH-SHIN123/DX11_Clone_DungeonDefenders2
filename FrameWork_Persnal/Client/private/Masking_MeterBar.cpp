#include "stdafx.h"
#include "Masking_MeterBar.h"

CMasking_MeterBar::CMasking_MeterBar(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CMasking_MeterBar::CMasking_MeterBar(const CMasking_MeterBar & rhs)
	: CUI_2D(rhs)
{
	m_MeterBar_Desc.HasFrameBar = true;
}

HRESULT CMasking_MeterBar::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	//m_MeterBar_Desc.fCount = 0.f;
	//m_MeterBar_Desc.fCount_Max = 0.f;
	//m_MeterBar_Desc.fShaderPass_Index = 0;
	//m_MeterBar_Desc.eFillMode = EMeterBar_FillMode::ZeroToFull;
	//m_MeterBar_Desc.UI_Desc.eLevel = ELevel::Static;
	//lstrcat(m_MeterBar_Desc.UI_Desc.szTextureName, L"NULL");
	//m_MeterBar_Desc.UI_Desc.Movement_Desc.vPos = { 0.f, 0.f, 0.f, 1.f};
	//m_MeterBar_Desc.UI_Desc.Movement_Desc.vScale = { 100.f, 100.f, 0.f, 1.f };

	return S_OK;
}

HRESULT CMasking_MeterBar::NativeConstruct(void * pArg)
{
	memcpy(&m_MeterBar_Desc, pArg, sizeof(MASK_METERBAR_DESC));

	__super::NativeConstruct(&m_MeterBar_Desc.UI_Desc);

	return S_OK;
}

_int CMasking_MeterBar::Tick(_float TimeDelta)
{
	Count_Check(TimeDelta);

	return _int();
}

_int CMasking_MeterBar::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CMasking_MeterBar::Render(_uint MaskShaderPass, _uint UIFramePass )
{
	__super::Render();

	switch (m_MeterBar_Desc.eFrame_Render)
	{
	case ECastingBar_Frame_Render::First:

		break;

	default:
		break;
	}
	if (m_MeterBar_Desc.HasFrameBar)
	{
		if (ECastingBar_Frame_Render::First == m_MeterBar_Desc.eFrame_Render)
		{
			m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
			m_pBufferRectCom->Render(UIFramePass);
		}
	}

	_float2 Time = { -m_fTime, m_fTime * 0.3333f };
	m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &Time, sizeof(_float2));
	m_pBufferRectCom->Set_Variable("g_Textrue_UV", &m_fRatio, sizeof(_float2));
	m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(1));
	m_pBufferRectCom->Render(MaskShaderPass);

	if (m_MeterBar_Desc.HasFrameBar)
	{
		if (ECastingBar_Frame_Render::Second == m_MeterBar_Desc.eFrame_Render)
		{
			m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
			m_pBufferRectCom->Render(UIFramePass);
		}
	}


	return S_OK;
}

void CMasking_MeterBar::Set_Count(_float fCount, _float fCount_Max)
{
	m_MeterBar_Desc.fCount = fCount;
	m_MeterBar_Desc.fCount_Max = fCount_Max;
}

void CMasking_MeterBar::Render_Frame_First(_uint MaskShaderPass, _uint UIFramePass)
{
}

void CMasking_MeterBar::Render_Frame_Second(_uint MaskShaderPass, _uint UIFramePass)
{
}

void CMasking_MeterBar::Count_Check(_float TimeDelta)
{
	// Full To Zero
	switch (m_MeterBar_Desc.eFillMode)
	{
	case EMeterBar_FillMode::ZeroToFull:
		m_fRatio = (m_MeterBar_Desc.fCount / m_MeterBar_Desc.fCount_Max);
		break;
	case EMeterBar_FillMode::FullToZero:
		m_fRatio = (1.f - (m_MeterBar_Desc.fCount / m_MeterBar_Desc.fCount_Max));
		break;
	default:
		break;
	}

	m_fTime += TimeDelta;
	if (m_fTime >= 3.f)
		m_fTime = 0.f;
}

CMasking_MeterBar * CMasking_MeterBar::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMasking_MeterBar* pInstance = new CMasking_MeterBar(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMasking_MeterBar) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMasking_MeterBar::Clone_GameObject(void * pArg)
{
	CMasking_MeterBar* pInstance = new CMasking_MeterBar(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMasking_MeterBar) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMasking_MeterBar::Free()
{
	__super::Free();
}