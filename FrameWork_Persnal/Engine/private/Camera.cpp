#include "..\public\Camera.h"
#include "Pipeline_Manager.h"

CCamera::CCamera(ID3D11Device * pDevice, ID3D11DeviceContext * pDeviceContext)
	: CGameObject(pDevice, pDeviceContext)
	, m_pPipeline_Manager(CPipeline_Manager::GetInstance())
{
	Safe_AddRef(m_pPipeline_Manager);
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
	, m_pPipeline_Manager(rhs.m_pPipeline_Manager)
{
	Safe_AddRef(m_pPipeline_Manager);
}

HRESULT CCamera::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();



	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERA_DESC));

	if (nullptr == (m_pMovementCom = CMovement::Create(m_pDevice, m_pDevice_Context)))
		return E_FAIL;

	if (FAILED(m_pMovementCom->NativeConstruct(&m_CameraDesc.StateDesc)))
		return E_FAIL;

	_vector		vEye, vAt, vAxisY;
	vEye	= XMLoadFloat3(&m_CameraDesc.vEye);
	vEye	= XMVectorSetW(vEye, 1.f);

	vAt		= XMLoadFloat3(&m_CameraDesc.vAt);
	vAt		= XMVectorSetW(vAt, 1.f);

	vAxisY	= XMLoadFloat3(&m_CameraDesc.vAxisY);
	vAxisY	= XMVectorSetW(vAxisY, 0.f);

	_vector vLook	= XMVector3Normalize(vAt - vEye);
	m_pMovementCom->Set_State(EState::Look, vLook);

	_vector vRight	= XMVector3Normalize(XMVector3Cross(vAxisY, vLook));
	m_pMovementCom->Set_State(EState::Right, vRight);

	_vector vUp		= XMVector3Normalize(XMVector3Cross(vLook, vRight));
	m_pMovementCom->Set_State(EState::Up, vUp);

	m_pMovementCom->Set_State(EState::Position, vEye);

	return S_OK;
}

_int CCamera::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	_vector vAt = XMLoadFloat3(&m_CameraDesc.vAt);
	vAt = XMVectorSetW(vAt, 1.f);

	_vector vAxisY = XMLoadFloat3(&m_CameraDesc.vAxisY);
	vAxisY = XMVectorSetW(vAxisY, 0.f);

	_matrix ViewMatrix = XMMatrixLookAtLH(m_pMovementCom->Get_State(EState::Position), vAt, vAxisY);
	//m_pMovementCom->Set_WorldMatrix(ViewMatrix);



	m_pPipeline_Manager->Set_Transform(ETransformState::View, m_pMovementCom->Get_WorldMatrix_Inverse());
	m_pPipeline_Manager->Set_Transform(ETransformState::Proj, XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));

	return _int();
}

_int CCamera::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pMovementCom);
	Safe_Release(m_pPipeline_Manager);
}
