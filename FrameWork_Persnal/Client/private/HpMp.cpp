#include "stdafx.h"
#include "..\public\HpMp.h"
#include "Player.h"

CHpMp::CHpMp(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CHpMp::CHpMp(const CHpMp & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CHpMp::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CHpMp::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_vInterval = _float2(69.f, 0.f);

	Ready_Component(pArg);

	return S_OK;
}

_int CHpMp::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	m_fTime += TimeDelta * 0.5f;

	HpMp_Check(TimeDelta);

	return _int();
}

_int CHpMp::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_fTime > 1.f)
		m_fTime = 0.f;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CHpMp::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	Render_Hp();
	Render_Mp();

	return S_OK;
}

void CHpMp::HpMp_Check(_float TimeDelta)
{
	CLayer* pLayer = GET_GAMEINSTANCE->Get_Layer((_uint)ELevel::Stage1 , L"Layer_Player");
	if (nullptr == pLayer)
		return;

	CStatus* pStatus = (CStatus*)pLayer->Get_GameObject()->Get_Component(L"Com_Status");

	m_fHp		= (_float)pStatus->Get_Hp();
	m_fHp_Max	= (_float)pStatus->Get_HpMax();
	m_fMp		= (_float)pStatus->Get_Mp();
	m_fMp_Max	= (_float)pStatus->Get_MpMax();


	

	if (true) // 맞았다면
	{
		_vector vMaskPos = m_pMovementCom_Mask->Get_State(EState::Position);
		_float fHpBarMax_Y = XMVectorGetY(m_pMovementCom->Get_State(EState::Position));
		_float fDamageRatio =(1.f - (m_fHp / m_fHp_Max)) * 100.f;
		_float fHpBarRatio = m_fHpScale / 100.f;
		_float fRealHp = fHpBarMax_Y - ( fHpBarRatio * fDamageRatio);
		vMaskPos = XMVectorSetY(vMaskPos, (fRealHp));
		m_pMovementCom_Mask->Set_State(EState::Position, vMaskPos);
	}

	// MP
	_vector vMaskPos = m_pMovementCom_MP_Mask->Get_State(EState::Position);
	_float fMpBarMax_Y = XMVectorGetY(m_pMovementCom_MP->Get_State(EState::Position));
	_float fDamageRatio = (1.f - (m_fMp / m_fMp_Max)) * 100.f;
	_float fMpBarRatio = m_fHpScale / 100.f;
	_float fRealMp = fMpBarMax_Y - (fMpBarRatio * fDamageRatio);
	vMaskPos = XMVectorSetY(vMaskPos, (fRealMp));
	m_pMovementCom_MP_Mask->Set_State(EState::Position, vMaskPos);

}

void CHpMp::Render_Hp()
{
	_float2 fTextureUV = { m_fTime , 0.f };

	_matrix Mat = m_pMovementCom->Get_WorldMatrix() *  GET_INDENTITY_MATRIX * GET_ORTHO_SPACE;

	// Back
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(2);

	// Hp Mask
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Mask->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &fTextureUV, sizeof(_float2));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
	m_pBufferRectCom->Render(4);

	//// 보글보글
	//fTextureUV = _float2(0.f, m_fTime);
	//m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &fTextureUV, sizeof(_float2));
	//m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(4));
	//m_pBufferRectCom->Render(11);


	// Cristal
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(1));
	m_pBufferRectCom->Render(9);
}

void CHpMp::Render_Mp()
{
	_float2 fTextureUV = { m_fTime , 0.f };

	// Mp
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_MP->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// Mp Mask
	fTextureUV.x *= -1;
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_MP_Mask->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &fTextureUV, sizeof(_float2));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(2));
	m_pBufferRectCom->Render(10);

	// Cristal
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_MP->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(1));
	m_pBufferRectCom->Render(8);
}

HRESULT CHpMp::Ready_Component(void* pArg)
{
	UI2D_DESC* pData = new UI2D_DESC;
	memcpy(pData, pArg, sizeof(UI2D_DESC));

	HRESULT hr = S_OK;

	/* For.Transform */
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Mask"), (CComponent**)&m_pMovementCom_Mask);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Mp"), (CComponent**)&m_pMovementCom_MP);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Texture_Mp_Mask"), (CComponent**)&m_pMovementCom_MP_Mask);

	_vector vPosMp = XMVectorSet(pData->Movement_Desc.vPos.x + m_vInterval.x, pData->Movement_Desc.vPos.y + m_vInterval.y, 0.f, 1.f);
	_vector vPosMask = XMVectorSet(pData->Movement_Desc.vPos.x, pData->Movement_Desc.vPos.y, 0.f, 1.f);

	m_fHpScale = pData->Movement_Desc.vPos.y - (-330.f); // 117;

	m_pMovementCom_MP->Set_Scale(XMVectorSet(pData->Movement_Desc.vScale.x, pData->Movement_Desc.vScale.y, 0.f, 0.f));
	m_pMovementCom_MP->Set_State(EState::Position, vPosMp);

	m_pMovementCom_Mask->Set_Scale(XMVectorSet(pData->Movement_Desc.vScale.x, pData->Movement_Desc.vScale.y, 0.f, 0.f));
	m_pMovementCom_Mask->Set_State(EState::Position, vPosMask);

	m_pMovementCom_MP_Mask->Set_Scale(XMVectorSet(pData->Movement_Desc.vScale.x, pData->Movement_Desc.vScale.y, 0.f, 0.f));
	m_pMovementCom_MP_Mask->Set_State(EState::Position, vPosMp);

	Safe_Delete(pData);

	if (hr != S_OK)
		return E_FAIL;

	return S_OK;
}

CHpMp * CHpMp::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CHpMp* pInstance = new CHpMp(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CHpMp) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CHpMp::Clone_GameObject(void * pArg)
{
	CHpMp* pInstance = new CHpMp(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CHpMp) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHpMp::Free()
{
	Safe_Release(m_pMovementCom_Mask);

	Safe_Release(m_pMovementCom_MP);
	Safe_Release(m_pMovementCom_MP_Mask);

	__super::Free();
}
