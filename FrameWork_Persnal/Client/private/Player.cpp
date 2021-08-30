#include "stdafx.h"
#include "..\public\Player.h"

CPlayer::CPlayer(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPlayer::CPlayer(const CPlayer & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPlayer::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayer::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CPlayer::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	Key_Check(TimeDelta);

	return _int();
}

_int CPlayer::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CPlayer::Render()
{
	if (nullptr == m_pVIBufferCom)
		return UPDATE_ERROR;

	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pVIBufferCom->Render(0);

	return S_OK;
}

void CPlayer::Key_Check(_float TimeDelta)
{
	if (GET_KEY_INPUT(DIK_W))
		m_pMovementCom->Go_Straight(TimeDelta);

	if (GET_KEY_INPUT(DIK_S))
		m_pMovementCom->Go_Backward(TimeDelta);

	if (GET_KEY_INPUT(DIK_A))
		m_pMovementCom->Go_Left(TimeDelta);

	if (GET_KEY_INPUT(DIK_D))
		m_pMovementCom->Go_Right(TimeDelta);

	// 카메라 가져와서 회전 시키기
}

HRESULT CPlayer::Ready_Component(void* pArg)
{
	HRESULT hr = S_OK;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect_Model"), TEXT("Com_Buffer"), (CComponent**)&m_pVIBufferCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, pArg);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_Devil"), TEXT("Com_Texture_0"), (CComponent**)&m_pTextureCom);



	return S_OK;
}

CPlayer * CPlayer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPlayer* pInstance = new CPlayer(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPlayer) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayer::Clone_GameObject(void * pArg)
{
	CPlayer* pInstance = new CPlayer(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CPlayer) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPlayer::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}
