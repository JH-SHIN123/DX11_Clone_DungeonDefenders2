#include "..\public\Camera.h"
#include "Pipeline_Manager.h"
#include "GameInstance.h"

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
	{
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERA_DESC));
		m_CameraDesc_Second = m_CameraDesc;
	}

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


	Zoom_Check(TimeDelta);


	return _int();
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Set_ZoomIn(_float fFov, _float fZoomSpeed)
{
	m_IsZoomIn = true;
	m_IsZoomOut = false;

	m_fZoomFov = XMConvertToRadians(fFov);
	m_fZoomSpeed = fZoomSpeed;
}

void CCamera::Set_ZoomOut(_float fFov, _float fZoomSpeed)
{
	m_IsZoomIn = false;
	m_IsZoomOut = true;

	m_fZoomFov = XMConvertToRadians(fFov);
	m_fZoomSpeed = fZoomSpeed;
}

void CCamera::Target_Check(_uint iLevel, const _tchar* LayerTag, const _tchar* ComponentTag)
{
	CMovement* pTarget = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject(iLevel, LayerTag))->Get_Component(ComponentTag));

	if (nullptr == pTarget)
		return;

	_vector vTargetPos = pTarget->Get_State(EState::Position);

	_vector vRight = XMVector4Normalize(pTarget->Get_State(EState::Right));
	_vector vUp = XMVector4Normalize(pTarget->Get_State(EState::Up));
	_vector vLook = XMVector4Normalize(pTarget->Get_State(EState::Look));

	_vector vMyPos = vTargetPos + (vRight * m_CameraDesc.vTargetDis.x);
	vMyPos = vTargetPos + (vUp * m_CameraDesc.vTargetDis.y);
	vMyPos = vTargetPos + (vLook * m_CameraDesc.vTargetDis.z);

	m_pMovementCom->Set_State(EState::Position, vMyPos);
}

void CCamera::TargetRotate_Check(_uint iLevel, const _tchar * LayerTag, const _tchar * ComponentTag)
{
	CMovement* pTarget = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject(iLevel, LayerTag))->Get_Component(ComponentTag));

	if (nullptr == pTarget)
		return;

	_vector vTargetPos = pTarget->Get_State(EState::Position);

	_vector vRight = XMVector4Normalize(pTarget->Get_State(EState::Right));
	_vector vUp = XMVector4Normalize(pTarget->Get_State(EState::Up));
	_vector vLook = XMVector4Normalize(pTarget->Get_State(EState::Look));

	_vector vMyPos = vTargetPos + (vRight * m_CameraDesc.vTargetDis.x);
	vMyPos += (vUp * m_CameraDesc.vTargetDis.y);
	vMyPos += (vLook * m_CameraDesc.vTargetDis.z);

	m_pMovementCom->Set_State(EState::Position, vMyPos);

	_vector vNewRight = XMVector4Normalize(m_pMovementCom->Get_State(EState::Right)) * pTarget->Get_Scale(EState::Right);
	_vector vNewLook = XMVector4Normalize(m_pMovementCom->Get_State(EState::Look)) * pTarget->Get_Scale(EState::Look);

	//m_CameraDesc.vAt = _float3(10.f, 2.f, 3.f);
	//m_pMovementCom->Set_RotateAxis_MyRadian(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(10.f));

	pTarget->Set_State(EState::Right, vNewRight);
	pTarget->Set_State(EState::Look, vNewLook);
}

void CCamera::Aim_Check()
{
	_vector vRight = m_pMovementCom->Get_State(EState::Right);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f); // 임의의 축으로 회전을 한다면 카메라가 회전을 해버린당
	_vector vLook = m_pMovementCom->Get_State(EState::Look);

	_matrix RotateMatrix;

	_long dwMouseMove = 0;

	if (dwMouseMove = GET_MOUSE_X)
	{
		RotateMatrix = XMMatrixRotationAxis(vUp, XMConvertToRadians((_float)dwMouseMove * 0.05f));

		vLook = XMVector3TransformNormal(vLook, RotateMatrix);
		m_pMovementCom->Set_State(EState::Look, vLook);

		vRight = XMVector3Cross(vUp, vLook);
		m_pMovementCom->Set_State(EState::Right, vRight);

		vUp = XMVector3Cross(vLook, vRight);
		m_pMovementCom->Set_State(EState::Up, vUp);
	}

	if (dwMouseMove = GET_MOUSE_Y)
	{
		RotateMatrix = XMMatrixRotationAxis(vRight, XMConvertToRadians((_float)dwMouseMove * 0.05f));

		vLook = XMVector3TransformNormal(vLook, RotateMatrix);
		m_pMovementCom->Set_State(EState::Look, vLook);

		vRight = XMVector3Cross(vUp, vLook);
		m_pMovementCom->Set_State(EState::Right, vRight);

		vUp = XMVector3Cross(vLook, vRight);
		m_pMovementCom->Set_State(EState::Up, vUp);
	}

}

void CCamera::Zoom_Check(_float TimeDelta)
{
	if (m_IsZoomIn == true)
	{
		if (m_CameraDesc.fFovy >= m_fZoomFov && false == m_IsZoomReverse)
			m_CameraDesc.fFovy -= TimeDelta * m_fZoomSpeed;

		else
		{
			m_IsZoomReverse = true;
			m_CameraDesc.fFovy += TimeDelta * m_fZoomSpeed;

			if (m_CameraDesc.fFovy >= m_CameraDesc_Second.fFovy)
			{
				m_CameraDesc.fFovy = m_CameraDesc_Second.fFovy;
				m_IsZoomIn = false;
				m_IsZoomReverse = false;
				return;
			}
		}
	}

	if (m_IsZoomOut == true)
	{
		if (m_CameraDesc.fFovy <= m_fZoomFov && false == m_IsZoomReverse)
			m_CameraDesc.fFovy += TimeDelta * m_fZoomSpeed;

		else
		{	
			m_IsZoomReverse = true;
			m_CameraDesc.fFovy -= TimeDelta * m_fZoomSpeed;

			if (m_CameraDesc.fFovy <= m_CameraDesc_Second.fFovy)
			{
				m_CameraDesc.fFovy = m_CameraDesc_Second.fFovy;
				m_IsZoomOut = false;
				m_IsZoomReverse = false;
				return;
			}
		}
	}
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pMovementCom);
	Safe_Release(m_pPipeline_Manager);
}
