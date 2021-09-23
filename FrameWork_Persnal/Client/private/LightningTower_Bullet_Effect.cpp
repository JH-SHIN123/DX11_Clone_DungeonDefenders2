#include "stdafx.h"
#include "..\public\LightningTower_Bullet_Effect.h"

CLightningTower_Bullet_Effect::CLightningTower_Bullet_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CEffect(pDevice, pDevice_Context)
{
}

CLightningTower_Bullet_Effect::CLightningTower_Bullet_Effect(const CLightningTower_Bullet_Effect & rhs)
	: CEffect(rhs)
{
}

HRESULT CLightningTower_Bullet_Effect::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower_Bullet_Effect::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);
	return S_OK;
}

_int CLightningTower_Bullet_Effect::Tick(_float TimeDelta)
{
	m_fTimeColor += TimeDelta * 2.5f;

	if (2.f <= m_fTimeColor)
		m_iShaderPass = 4;

	else if (3.f <= m_fTimeColor)
	{
		m_iShaderPass	= 2;
		m_fTimeColor	= 0.f;
	}

	else
		m_iShaderPass = (_uint)m_fTimeColor + 2;

	return _int();
}

_int CLightningTower_Bullet_Effect::Late_Tick(_float TimeDelta)
{
	//return __super::Late_Tick(TimeDelta);
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CLightningTower_Bullet_Effect::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CLightningTower_Bullet_Effect::Ready_Component(void * pArg)
{
	return S_OK;
}

CLightningTower_Bullet_Effect * CLightningTower_Bullet_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower_Bullet_Effect*		pInstance = new CLightningTower_Bullet_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower_Bullet_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower_Bullet_Effect::Clone_GameObject(void * pArg)
{
	CLightningTower_Bullet_Effect*		pInstance = new CLightningTower_Bullet_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower_Bullet_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower_Bullet_Effect::Free()
{
	__super::Free();
}
