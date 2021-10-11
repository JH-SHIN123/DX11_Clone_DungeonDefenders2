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

	m_pPipeline_Manager->Set_WorldTransform(XMMatrixIdentity());


	return S_OK;
}

HRESULT CCamera::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
	{
		memcpy(&m_CameraDesc, pArg, sizeof(CAMERA_DESC));
		m_CameraDesc_Second = m_CameraDesc;
		m_vTopView_Dir = { 0.f, m_CameraDesc.vTargetAxis.y,  m_CameraDesc.vTargetAxis.z * 0.9f };

		//m_fAxisX_Lenght
		//m_vNoRotate_TargetAxis = m_CameraDesc.vTargetAxis;
		//m_vCalculate_TargetAxis = m_CameraDesc.vTargetAxis;
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

	m_pPipeline_Manager->Set_WorldTransform(XMMatrixIdentity());

	return S_OK;
}

_int CCamera::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);



	SetUp_PipeLine_Matrix();

	return _int();
}

_int CCamera::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	Shake_Check(TimeDelta);
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


}

void CCamera::TargetRotate_Check(_uint iLevel, const _tchar * LayerTag, const _tchar * ComponentTag, _float TimeDelta)
{
	CMovement* pTarget = static_cast<CMovement*>((GET_GAMEINSTANCE->Get_GameObject(iLevel, LayerTag))->Get_Component(ComponentTag));

	if (nullptr == pTarget)
	{
		m_CameraDesc.vAt = { 0.f,0.f,0.f };
		return;
	}

	_float fDis = m_CameraDesc.fDis;
	_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
	if (0.f != m_fAddCameraDis_Time_Max)
	{
		if (false == m_IsOutCameraDis)
		{
			m_fAddCameraDis_Time += TimeDelta;
			vTargetAxis += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * m_fAddCameraDis * m_fAddCameraDis_Time;
			vTargetAxis += -XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * m_fAddCameraDis * m_fAddCameraDis_Time * 1.5f;

			if (m_fAddCameraDis_Time >= m_fAddCameraDis_Time_Max)
				m_IsOutCameraDis = true;
		}
		else
		{
			m_fAddCameraDis_Time -= TimeDelta;
			vTargetAxis += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * m_fAddCameraDis * m_fAddCameraDis_Time;
			vTargetAxis += -XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * m_fAddCameraDis * m_fAddCameraDis_Time * 1.5f;

			if (0 >= m_fAddCameraDis_Time)
				m_fAddCameraDis_Time_Max = 0.f;
		}
	}

	_vector vTargetPos = pTarget->Get_State(EState::Position);

	_vector vMyPos = vTargetPos + XMVector3Normalize(vTargetAxis) * fDis;
	
	if (0 <= m_fShakeTime)
	{
		m_fShakeTime -= TimeDelta;

		_vector vRight = XMVector3Normalize(m_pMovementCom->Get_State(EState::Right));
		_vector vUp = XMVector3Normalize(m_pMovementCom->Get_State(EState::Up));
		_vector vLook = XMVector3Normalize(m_pMovementCom->Get_State(EState::Look));


		_int Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vMyPos += (vRight * (m_vShakePower.x * Shake));
		Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vMyPos += (vUp * (m_vShakePower.y * Shake));
		Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vMyPos += (vLook * (m_CameraDesc.vTargetAxis.z + m_vShakePower.z * Shake));
	}

	m_pMovementCom->Set_State(EState::Position, vMyPos);

	_vector vNewRight = XMVector4Normalize(m_pMovementCom->Get_State(EState::Right)) * pTarget->Get_Scale(EState::Right);
	_vector vRightUp_Cross = XMVector3Cross(vNewRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	_vector vNewLook = XMVector4Normalize(vRightUp_Cross) * pTarget->Get_Scale(EState::Look);

	pTarget->Set_State(EState::Right, vNewRight);
	pTarget->Set_State(EState::Look, vNewLook);

	_vector vAt = m_pMovementCom->Get_State(EState::Position) + (XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * 5.f);

	if (ECameraViewMode::TopView == m_eCameraMode_Next)
	{
		vAt -= (XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * 0.7f);
	}
	else if (ECameraViewMode::TopToTPS == m_eCameraMode_Next)
	{

	}

	if (0 <= m_fShake_At_Time)
	{
		m_fShake_At_Time -= TimeDelta;

		_vector vRight = XMVector3Normalize(m_pMovementCom->Get_State(EState::Right));
		_vector vUp = XMVector3Normalize(m_pMovementCom->Get_State(EState::Up));
		_vector vLook = XMVector3Normalize(m_pMovementCom->Get_State(EState::Look));


		_int Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vAt += (vRight * (m_vShake_At_Power.x * Shake));
		Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vAt += (vUp * (m_vShake_At_Power.y * Shake));
		Shake = (rand()) % 2;
		if (Shake == 0)
			Shake = -1;
		vAt += (vLook * (m_CameraDesc.vTargetAxis.z + m_vShake_At_Power.z * Shake));
	}

	XMStoreFloat3(&m_CameraDesc.vAt, vAt);
}

void CCamera::TargetRotate_Check(CTransform* pTransform)
{
	// 짝퉁
	_vector vTargetPos = pTransform->Get_State(EState::Position);

	_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
	_vector vMyPos = vTargetPos + XMVector3Normalize(vTargetAxis) * m_CameraDesc.fDis;

	m_pMovementCom->Set_State(EState::Position, vMyPos);

	_vector vNewRight = XMVector4Normalize(m_pMovementCom->Get_State(EState::Right)) * pTransform->Get_Scale(EState::Right);
	_vector vRightUp_Cross = XMVector3Cross(vNewRight, XMVectorSet(0.f, 1.f, 0.f, 0.f));
	_vector vNewLook = XMVector4Normalize(vRightUp_Cross) * pTransform->Get_Scale(EState::Look);

	pTransform->Set_State(EState::Right, vNewRight);
	pTransform->Set_State(EState::Look, vNewLook);

	_vector vAt = m_pMovementCom->Get_State(EState::Position) + (XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * 20.f);

	XMStoreFloat3(&m_CameraDesc.vAt, vAt);
}

void CCamera::Cam_Shake(_float3 vPower, _float ShakeTime)
{
	m_fShakeTime = ShakeTime;

	m_vShakePower = vPower;
}

void CCamera::Cam_Shake_At(_float3 vPower, _float ShakeTime)
{
	m_fShake_At_Time = ShakeTime;

	m_vShake_At_Power = vPower;
}

void CCamera::Cam_Add_Dis(_float fAddDis, _float DisTime)
{
	m_IsOutCameraDis = false;
	m_fAddCameraDis = fAddDis;
	m_fAddCameraDis_Time = 0.f;
	m_fAddCameraDis_Time_Max = DisTime;
}

void CCamera::View_Check(_float TimeDelata)
{
	switch (m_eCameraMode_Next)
	{
	case Engine::ECameraViewMode::ThirdPerson:
		View_ThirdPerson(TimeDelata);
		break;
	case Engine::ECameraViewMode::TopView:
		View_TopView(TimeDelata);
		break;
	case Engine::ECameraViewMode::TopToTPS:
		View_Change_Top_ThirdPerson(TimeDelata);
		break;
	case Engine::ECameraViewMode::End:
		break;
	default:
		break;
	}
}

void CCamera::View_ThirdPerson(_float TimeDelata)
{
	_vector vRight = m_pMovementCom->Get_State(EState::Right);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f); // 임의의 축으로 회전을 한다면 카메라가 회전을 해버린당
	_vector vLook = m_pMovementCom->Get_State(EState::Look);

	_matrix RotateMatrix;

	_long dwMouseMove = 0;

	if (dwMouseMove = GET_MOUSE_X)
	{
		RotateMatrix = XMMatrixRotationAxis(vUp, XMConvertToRadians((_float)dwMouseMove * 0.05f));
		XMStoreFloat3(&m_CameraDesc.vTargetAxis,XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc.vTargetAxis), RotateMatrix));
		XMStoreFloat3(&m_CameraDesc_Second.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc_Second.vTargetAxis), RotateMatrix));
		XMStoreFloat3(&m_vTopView_Dir, XMVector3TransformNormal(XMLoadFloat3(&m_vTopView_Dir), RotateMatrix));
		//m_vTopView_Dir

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
		vRight = XMVector3Cross(vUp, vLook);
		vUp = XMVector3Cross(vLook, vRight);

		_vector vNormal_Look = XMVector3Normalize(vLook);
		_float fCetha = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vNormal_Look, XMVectorSet(0.f, 1.f, 0.f, 0.f)))));

		if (m_CameraDesc.fXRotationLock_Min <= fCetha && fCetha <= m_CameraDesc.fXRotationLock_Max)
		{
			XMStoreFloat3(&m_CameraDesc.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc.vTargetAxis), RotateMatrix));
			XMStoreFloat3(&m_CameraDesc_Second.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc_Second.vTargetAxis), RotateMatrix));
			XMStoreFloat3(&m_vTopView_Dir, XMVector3TransformNormal(XMLoadFloat3(&m_vTopView_Dir), RotateMatrix));

			m_pMovementCom->Set_State(EState::Right, vRight);
			m_pMovementCom->Set_State(EState::Look, vLook);
			m_pMovementCom->Set_State(EState::Up, vUp);
		}
	}


	if (m_CameraDesc.fDis > m_CameraDesc_Second.fDis)
	{
		m_CameraDesc.fDis -= m_fDisSecond * TimeDelata; // 8.9

		_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
		vTargetAxis += XMVector3Normalize(vRight) * 0.005f;

		XMStoreFloat3(&m_CameraDesc.vTargetAxis, vTargetAxis);
	}

	else if (m_CameraDesc.fDis < m_CameraDesc_Second.fDis)
	{
		m_CameraDesc.fDis += m_fDisSecond * TimeDelata; // 8.9
	
		_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
		vTargetAxis -= XMVector3Normalize(vRight) * 0.005f;
	
		XMStoreFloat3(&m_CameraDesc.vTargetAxis, vTargetAxis);
	}

	_float fDisCheck = abs(m_CameraDesc.fDis - m_CameraDesc_Second.fDis);
	if (fDisCheck <= 0.5f)
		m_CameraDesc.fDis = m_CameraDesc_Second.fDis;

}

void CCamera::View_TopView(_float TimeDelata)
{
	_vector vRight = m_pMovementCom->Get_State(EState::Right);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f); // 임의의 축으로 회전을 한다면 카메라가 회전을 해버린당
	_vector vLook = m_pMovementCom->Get_State(EState::Look);

	_matrix RotateMatrix;

	RotateMatrix = XMMatrixRotationAxis(vRight, XMConvertToRadians((_float)10 * 0.05f));
	vLook = XMVector3TransformNormal(vLook, RotateMatrix);
	vRight = XMVector3Cross(vUp, vLook);
	vUp = XMVector3Cross(vLook, vRight);

	_vector vNormal_Look = XMVector3Normalize(vLook);
	_float fCetha = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vNormal_Look, XMVectorSet(0.f, 1.f, 0.f, 0.f)))));

	if (fCetha <= 170.f) // 움직여야 할 때
	{
		XMStoreFloat3(&m_CameraDesc.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc.vTargetAxis), RotateMatrix));
		XMStoreFloat3(&m_CameraDesc_Second.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc_Second.vTargetAxis), RotateMatrix));
		XMStoreFloat3(&m_vTopView_Dir, XMVector3TransformNormal(XMLoadFloat3(&m_vTopView_Dir), RotateMatrix));

		m_pMovementCom->Set_State(EState::Right, vRight);
		m_pMovementCom->Set_State(EState::Look, vLook);
		m_pMovementCom->Set_State(EState::Up, vUp);
	}


	if (m_CameraDesc.fDis <= 17.f)
	{
		m_CameraDesc.fDis += m_fDisSecond * TimeDelata; // 8.9
	}

	_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
	_float fFirstLength = XMVectorGetX(XMVector3Length(vTargetAxis));
	_float fSecondLength = XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vTopView_Dir)));

	if (fFirstLength >= fSecondLength)
	{
		vTargetAxis -= XMVector3Normalize(vRight) * 0.005f;
		XMStoreFloat3(&m_CameraDesc.vTargetAxis, vTargetAxis);
	}
	else
	{
		//m_CameraDesc.vTargetAxis = m_vTopView_Dir;
	}

}

void CCamera::View_Change_Top_ThirdPerson(_float TimeDelata)
{
	_vector vRight = m_pMovementCom->Get_State(EState::Right);
	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f); // 임의의 축으로 회전을 한다면 카메라가 회전을 해버린당
	_vector vLook = m_pMovementCom->Get_State(EState::Look);

	_matrix RotateMatrix;

	_long dwMouseMove = 0;

	RotateMatrix = XMMatrixRotationAxis(vRight, XMConvertToRadians((_float)-10 * 0.05f));
	vLook = XMVector3TransformNormal(vLook, RotateMatrix);
	vRight = XMVector3Cross(vUp, vLook);
	vUp = XMVector3Cross(vLook, vRight);

	_vector vNormal_Look = XMVector3Normalize(vLook);
	_float fCetha = XMConvertToDegrees(acosf(XMVectorGetX(XMVector3Dot(vNormal_Look, XMVectorSet(0.f, 1.f, 0.f, 0.f)))));

	XMStoreFloat3(&m_CameraDesc.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc.vTargetAxis), RotateMatrix));
	XMStoreFloat3(&m_CameraDesc_Second.vTargetAxis, XMVector3TransformNormal(XMLoadFloat3(&m_CameraDesc_Second.vTargetAxis), RotateMatrix));
	XMStoreFloat3(&m_vTopView_Dir, XMVector3TransformNormal(XMLoadFloat3(&m_vTopView_Dir), RotateMatrix));

	m_pMovementCom->Set_State(EState::Right, vRight);
	m_pMovementCom->Set_State(EState::Look, vLook);
	m_pMovementCom->Set_State(EState::Up, vUp);

	_float fDegree = (m_CameraDesc.fXRotationLock_Max + m_CameraDesc.fXRotationLock_Min) * 0.5f;
	m_CameraDesc.fDis -= m_fDisSecond * TimeDelata;



	if (m_CameraDesc.fDis >= m_CameraDesc_Second.fDis)
	{
		m_CameraDesc.fDis -= m_fDisSecond * TimeDelata; // 8.9

	}

	if (abs(m_CameraDesc.vTargetAxis.x) < abs(m_CameraDesc_Second.vTargetAxis.x))
	{
		_vector vTargetAxis = XMLoadFloat3(&m_CameraDesc.vTargetAxis);
		vTargetAxis += XMVector3Normalize(vRight) * 0.005f;

		XMStoreFloat3(&m_CameraDesc.vTargetAxis, vTargetAxis);
	}
	else
		m_CameraDesc.vTargetAxis = m_CameraDesc_Second.vTargetAxis;

	if (fCetha <= fDegree)
		m_eCameraMode_Next = ECameraViewMode::ThirdPerson;
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

void CCamera::Shake_Check(_float TimeDelta)
{
	if (0.f >= m_fShakeTime)
		return;

	m_fShakeTime -= TimeDelta;



}

void CCamera::SetUp_PipeLine_Matrix()
{
	_vector vAt = XMLoadFloat3(&m_CameraDesc.vAt);

	//if (ECameraViewMode::TopView == m_eCameraMode_Next)
	//	vAt = XMVectorZero();


	_vector vAxisY = XMLoadFloat3(&m_CameraDesc.vAxisY);
	vAxisY = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	_matrix ViewMatrix = XMMatrixLookAtLH(m_pMovementCom->Get_State(EState::Position), vAt, vAxisY);

	m_pPipeline_Manager->Set_WorldTransform(m_pMovementCom->Get_WorldMatrix());
	m_pPipeline_Manager->Set_Transform(ETransformState::View, ViewMatrix);
	m_pPipeline_Manager->Set_Transform(ETransformState::Proj, XMMatrixPerspectiveFovLH(m_CameraDesc.fFovy, m_CameraDesc.fAspect, m_CameraDesc.fNear, m_CameraDesc.fFar));
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pMovementCom);
	Safe_Release(m_pPipeline_Manager);
}
