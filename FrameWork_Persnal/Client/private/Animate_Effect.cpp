#include "stdafx.h"
#include "..\public\Animate_Effect.h"

CAnimate_Effect::CAnimate_Effect(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CAnimate_Effect::CAnimate_Effect(const CAnimate_Effect & rhs)
	: CGameObject(rhs)
{
}

HRESULT CAnimate_Effect::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CAnimate_Effect::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CAnimate_Effect::Tick(_float TimeDelta)
{
	if (m_IsDelete_This)
		return OBJECT_DEAD;
	TimeDelta = 0.5f;
	Dead_Check(TimeDelta);


	Texture_Scale_Check();



	return 0;
}

_int CAnimate_Effect::Late_Tick(_float TimeDelta)
{
	if (m_IsDelete_This)
		return OBJECT_DEAD;


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::SceneChange, this);;
}

HRESULT CAnimate_Effect::Render(_uint iShaderPass)
{
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureExCom->Get_ShaderResourceView((_uint)m_fTime));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
 	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
 	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	m_pBufferRectCom->Render(iShaderPass);

	return S_OK;
}

HRESULT CAnimate_Effect::Ready_Component(void* pArg)
{
	HRESULT hr = S_OK;

	ANIEFFECT_DESC Data;
	memcpy(&m_AnimateEffect_Desc, pArg, sizeof(ANIEFFECT_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pBufferRectCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom);
	hr = CGameObject::Add_Component((_uint)m_AnimateEffect_Desc.ePrototypeTextureLevel, m_AnimateEffect_Desc.szTextureEX_Name, TEXT("Com_Texture"), (CComponent**)&m_pTextureExCom);
	
	m_pMovementCom->Set_State(EState::Position, XMLoadFloat4(&m_AnimateEffect_Desc.vPos));
	m_iTextureNum = m_pTextureExCom->Get_TexturesNum();

	if (hr != S_OK)
		MSG_BOX("CAnimate_Effect::Ready_Component() Failed");

	return hr;
}

void CAnimate_Effect::Dead_Check(_float TimeDelta)
{
	if (false == m_AnimateEffect_Desc.IsLoop)
	{
		m_fTime += TimeDelta * m_AnimateEffect_Desc.fAnimateSpeed;
		if ((_uint)m_fTime >= m_iTextureNum)
		{
			m_fTime = (_float)m_iTextureNum;
			m_IsDelete_This = true;
		}
	}
	else
	{
		m_fTime += TimeDelta * m_AnimateEffect_Desc.fAnimateSpeed;
		if ((_uint)m_fTime >= m_iTextureNum)
			m_fTime = 0.f;

		if (m_fTime >= m_AnimateEffect_Desc.fLoopTime)
			m_IsDelete_This = true;
	}
}

void CAnimate_Effect::Texture_Scale_Check()
{
	_float2 vScale;
	vScale = m_pTextureExCom->Get_TextureScale((_uint)m_fTime);

	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	_vector vRealScale;

	switch (m_AnimateEffect_Desc.eTexOffSet)
	{
	case eTextureOffSet::None:
		vRealScale = XMVectorSet(vScale.x * m_AnimateEffect_Desc.vOffSet.x, vScale.y * m_AnimateEffect_Desc.vOffSet.y, 0.f, 0.f);
		m_pMovementCom->Set_Scale(vRealScale);
		break;
	case eTextureOffSet::LeftBottom:
		break;
	case eTextureOffSet::LeftTop:
		break;
	case eTextureOffSet::RightBottom:
		break;
	case eTextureOffSet::RightTop:
		break;
	default:
		return;
	}
}

CAnimate_Effect * CAnimate_Effect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CAnimate_Effect* pInstance = new CAnimate_Effect(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CAnimate_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CAnimate_Effect::Clone_GameObject(void * pArg)
{
	CAnimate_Effect* pInstance = new CAnimate_Effect(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CAnimate_Effect) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CAnimate_Effect::Free()
{
	Safe_Release(m_pTextureExCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pBufferRectCom);

	__super::Free();
}
