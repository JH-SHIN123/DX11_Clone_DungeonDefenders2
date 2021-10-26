#include "stdafx.h"
#include "UI_Hit_Effect.h"

CUI_Hit_Effect::CUI_Hit_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CUI_2D(pDevice, pDeviceContext)
{
}

CUI_Hit_Effect::CUI_Hit_Effect(const CUI_Hit_Effect & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CUI_Hit_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CUI_Hit_Effect::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CUI_Hit_Effect::Tick(_float TimeDelta)
{
	return _int();
}

_int CUI_Hit_Effect::Late_Tick(_float TimeDelta)
{
	if (true == m_IsDelete_This)
		return OBJECT_DEAD;

	if (true == m_IsAlphaUp)
	{
		m_fAlphaTime += TimeDelta * 5.f;

		if (1.f <= m_fAlphaTime)
		{
			m_fAlphaTime = 0.f;
			m_IsAlphaUp = false;
		}
	}
	else
	{
		m_fAlphaTime += TimeDelta * 3.f;

		if (0.f >= m_fAlphaTime)
			m_IsDelete_This = true;
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::AlphaUI, this);
}

HRESULT CUI_Hit_Effect::Render()
{
	if (nullptr == m_pBufferRectCom)
		return S_OK;

	_matrix WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet((_float)g_iWinCX, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, (_float)g_iWinCY, 0.f, 0.f);

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Set_Variable("g_fTime", &m_fAlphaTime, sizeof(_float));
	
	m_pBufferRectCom->Render(24);

	return S_OK;
}

CUI_Hit_Effect * CUI_Hit_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CUI_Hit_Effect* pInstance = new CUI_Hit_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CUI_Hit_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CUI_Hit_Effect::Clone_GameObject(void * pArg)
{
	CUI_Hit_Effect* pInstance = new CUI_Hit_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CUI_Hit_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CUI_Hit_Effect::Free()
{
	__super::Free();
}
