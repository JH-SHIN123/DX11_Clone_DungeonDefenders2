#include "stdafx.h"
#include "..\public\Ortho3D.h"

COrtho3D::COrtho3D(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_3D(pDevice, pDevice_Context)
{
}

COrtho3D::COrtho3D(const COrtho3D & rhs)
	: CUI_3D(rhs)
{
}

HRESULT COrtho3D::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT COrtho3D::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int COrtho3D::Tick(_float TimeDelta)
{
	return _int();
}

_int COrtho3D::Late_Tick(_float TimeDelta)
{
	_vector vCamLook = XMVector3Normalize(GET_CAMERA_LOOK);
	_vector vCamPos = GET_CAMERA_POSITION + vCamLook * 5.f;

	_float fCamDis_OffSet = XMVectorGetX(XMVector3Length(vCamPos - m_pMovementCom->Get_State(EState::Position)));
	_float fCamDis_Original = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));

	if (fCamDis_OffSet < fCamDis_Original)
		return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority_Second, this);

	return 0;
}

HRESULT COrtho3D::Render()
{
	if (nullptr == m_pBufferRectCom)
		return E_FAIL;

	_matrix  Matrix_VP = GET_VIEW_SPACE * GET_PROJ_SPACE;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);

	vPos = XMVector3TransformCoord(vPos, Matrix_VP);
	vPos.m128_f32[0] += 1.f;
	vPos.m128_f32[1] += 1.f;

	vPos.m128_f32[0] = (g_iWinCX * vPos.m128_f32[0]) / 2.f;
	vPos.m128_f32[1] = (g_iWinCY * (2.f - vPos.m128_f32[1])) / 2.f;

	vPos.m128_f32[0] = vPos.m128_f32[0] - (g_iWinCX / 2.f);
	vPos.m128_f32[1] = -vPos.m128_f32[1] + (g_iWinCY / 2.f);
	vPos.m128_f32[2] = 0.f;
	vPos.m128_f32[3] = 1.f;

	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();
	WorldMatrix.r[3] = vPos;

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(20);

	return S_OK;
}

COrtho3D * COrtho3D::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void * pArg)
{
	COrtho3D* pInstance = new COrtho3D(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (COrtho3D) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * COrtho3D::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void COrtho3D::Free()
{
	__super::Free();
}
