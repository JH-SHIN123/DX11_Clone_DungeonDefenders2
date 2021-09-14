#include "stdafx.h"
#include "..\public\Phase_Panel.h"

CPhase_Panel::CPhase_Panel(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CPhase_Panel::CPhase_Panel(const CPhase_Panel & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CPhase_Panel::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPhase_Panel::NativeConstruct(void * pArg)
{
	Ready_Component();

	memcpy(&m_iTextureIndex, pArg, sizeof(_uint));

	return S_OK;
}

_int CPhase_Panel::Tick(_float TimeDelta)
{
	Move_Check(TimeDelta);

	return _int();
}

_int CPhase_Panel::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Option_UI_1, this);
}

HRESULT CPhase_Panel::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(m_iTextureIndex));
	m_pBufferRectCom->Render(1);

	return S_OK;
}

HRESULT CPhase_Panel::Ready_Component()
{
	HRESULT hr = S_OK;

	MOVESTATE_DESC Data;
	ZeroMemory(&Data, sizeof(MOVESTATE_DESC));

	Data.fSpeedPerSec = 100.f;
	Data.vPos = { 0.f, g_iWinCY >> 1, 0.f, 1.f };
	Data.vScale = { 512.f, 70.f, 0.f, 0.f }; //512, 128

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_Buffer"), (CComponent**)&m_pBufferRectCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Phase_Text"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);

	return S_OK;
}

void CPhase_Panel::Move_Check(_float TimeDelta)
{
	// 회전은 필요없이 스케일만 써서 연출 가능

	m_pMovementCom->Go_Up(-TimeDelta);
	_float fScale = m_pMovementCom->Get_Scale(EState::Up);

	fScale += 1.f;

	if (128.f <= fScale)
		fScale = 128.f;

	m_pMovementCom->Set_Scale(XMVectorSet(512.f, fScale, 0.f, 0.f));
}

CPhase_Panel * CPhase_Panel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPhase_Panel* pInstance = new CPhase_Panel(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPhase_Panel) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPhase_Panel::Clone_GameObject(void * pArg)
{
	CPhase_Panel* pClone = new CPhase_Panel(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CPhase_Panel) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CPhase_Panel::Free()
{
	__super::Free();
}
