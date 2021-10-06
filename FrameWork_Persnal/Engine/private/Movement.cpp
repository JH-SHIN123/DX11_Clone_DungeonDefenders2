#include "..\public\Movement.h"
#include "Navigation.h"

CMovement::CMovement(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CTransform(pDevice, pDevice_Context)
{
}

CMovement::CMovement(const CMovement & rhs)
	: CTransform(rhs)
{
}

HRESULT CMovement::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMovement::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
	{
		memcpy(&m_MoveStateDesc, pArg, sizeof(MOVESTATE_DESC));

		_vector vScale = XMLoadFloat4(&m_MoveStateDesc.vScale);
		Set_Scale(vScale);

		_vector vPos = XMLoadFloat4(&m_MoveStateDesc.vPos);
		Set_State(EState::Position, vPos);

		if (1.f != m_MoveStateDesc.vRotateLook.z)
		{
			_vector vLook	= XMVector3Normalize(XMLoadFloat4(&m_MoveStateDesc.vRotateLook)) * m_MoveStateDesc.vScale.z;
			_vector vUp		= XMVectorSet(0.f, 1.f, 0.f, 0.f) * m_MoveStateDesc.vScale.y;
			_vector vRight	= XMVector3Normalize(XMVector3Cross(vUp, vLook)) * m_MoveStateDesc.vScale.x;

			Set_State(EState::Right, vRight);
			Set_State(EState::Up, vUp);
			Set_State(EState::Look, vLook);
		}

	}

	return S_OK;
}

_int CMovement::Physics_Tick(_float TimeDelta)
{
	Jumping(TimeDelta);
	HitBack(TimeDelta);
	Gravity(TimeDelta);

	return 0;
}

void CMovement::Set_Scale_Tick(_float TimeDelta, _fvector vScale, _bool IsScaleUp)
{
	if (true == IsScaleUp)
	{
		if(XMVectorGetX(vScale) >= __super::Get_Scale(EState::Right))
			__super::Set_Scale(vScale * TimeDelta);
	}

	else
	{
		if (XMVectorGetX(vScale) <= __super::Get_Scale(EState::Right))
			__super::Set_Scale(vScale * TimeDelta);
	}
}

void CMovement::Go_Straight(_float TimeDelta, CNavigation* pNavigation, _bool IsJump)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vLook		= __super::Get_State(EState::Look);

	vLook = XMVector4Normalize(vLook);

	if (nullptr != pNavigation)
	{
		_float fCellY = 0.f;
		_vector vDir = XMVectorZero();
		if (false == pNavigation->IsMove(vPosition, vLook, &fCellY, &vDir))
			vLook = vDir;

		if (false == IsJump)
			vPosition += XMVectorSetY(vPosition, fCellY);
	}

	vPosition += vLook * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	Set_State(EState::Position, vPosition);
}

void CMovement::Go_Backward(_float TimeDelta, CNavigation* pNavigation, _bool IsJump)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vLook		= __super::Get_State(EState::Look);

	vLook = XMVector4Normalize(vLook);

	if (nullptr != pNavigation)
	{
		_float fCellY = 0.f;
		_vector vDir = XMVectorZero();
		if (false == pNavigation->IsMove(vPosition, vLook, &fCellY, &vDir))
			vLook = vDir;

		if (false == IsJump)
			vPosition += XMVectorSetY(vPosition, fCellY);
	}

	vPosition -= vLook * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Right(_float TimeDelta)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vRight		= __super::Get_State(EState::Right);

	vRight = XMVector4Normalize(vRight);

	vPosition += vRight * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Left(_float TimeDelta)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vRight		= __super::Get_State(EState::Right);

	vRight = XMVector4Normalize(vRight);

	vPosition -= vRight * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Dir_Vector(_float TimeDelta, _fvector vDir)
{
	_vector vPosition = __super::Get_State(EState::Position);

	vPosition += vDir * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Dir(_float TimeDelta, _fvector vTargetPos)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vDir		= vTargetPos - vPosition;

	vDir = XMVector4Normalize(vDir);



	vPosition += vDir * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Dir_NoSpeed(_float TimeDelta, _fvector vTargetPos, _float fSpeed)
{
	_vector vPosition = __super::Get_State(EState::Position);
	_vector vDir = vTargetPos - vPosition;

	vDir = vDir * TimeDelta * fSpeed;

	if (1.f > XMVectorGetX(XMVector3Length(vDir)))
		return;

	vPosition += vDir;
	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_Up(_float TimeDelta)
{
	_vector vPosition = __super::Get_State(EState::Position);
	_vector vUp = __super::Get_State(EState::Up);

	vUp = XMVector4Normalize(vUp);

	vPosition += vUp * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::Go_LookDir(_float TimeDelta, CNavigation* pNavigation, _bool IsJump)
{
	_vector vPosition = __super::Get_State(EState::Position);
	_vector vLook = __super::Get_State(EState::Look);

	vLook = XMVector4Normalize(vLook);

	if (nullptr != pNavigation)
	{
		_float fCellY = 0.f;
		_vector vDir = XMVectorZero();

		_bool IsCheck = pNavigation->IsMove(vPosition, vLook, &fCellY, &vDir);

		if (false == IsCheck)
			vLook = vDir;

		else if (false == IsJump && true == IsCheck)
			vPosition = XMVectorSetY(vPosition, fCellY);
	}

	vPosition += vLook * TimeDelta * m_MoveStateDesc.fSpeedPerSec;

	__super::Set_State(EState::Position, vPosition);
}

void CMovement::RotateTick(_float TimeDelta)
{
	RotationY_CCW(TimeDelta);
}

void CMovement::RotateToAxis_Tick(_float TimeDelta, _fvector vAxis)
{
	_vector	vRight	= __super::Get_State(EState::Right);
	_vector	vUp		= __super::Get_State(EState::Up);
	_vector	vLook	= __super::Get_State(EState::Look);

	_matrix	RotateMatrix = XMMatrixRotationAxis(vAxis, m_MoveStateDesc.fRotatePerSec * TimeDelta);

	__super::Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	__super::Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	__super::Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
}

void CMovement::RotateToTarget_Tick(_float TimeDelta, _fvector vTargetPos)
{
}

void CMovement::RotateToTargetOnLand_Tick(_float TimeDelta, _fvector vTargetPos)
{
	_vector vPosition	= __super::Get_State(EState::Position);
	_vector vDir		= XMVector3Normalize(vTargetPos - vPosition);
	_vector vRight		= XMVector3Normalize(__super::Get_State(EState::Right));
	_vector vLeft		= XMVector3Normalize(XMVector3Dot(__super::Get_State(EState::Look), __super::Get_State(EState::Up)));

	_float fRightScala	= XMVectorGetX(XMVector3Dot(vDir, vRight));
	_float fLeftScala	= XMVectorGetX(XMVector3Dot(vDir, vLeft));

	fRightScala > fLeftScala ? RotationY_CW(TimeDelta) : RotationY_CCW(TimeDelta);
}

void CMovement::RotationY_CW(_float TimeDelta)
{
	_vector	vRight = __super::Get_State(EState::Right);
	_vector	vUp = __super::Get_State(EState::Up);
	_vector	vLook = __super::Get_State(EState::Look);

	_matrix	RotateMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_MoveStateDesc.fRotatePerSec * TimeDelta);

	__super::Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	__super::Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	__super::Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
}

void CMovement::RotationY_CCW(_float TimeDelta)
{
	_vector	vRight = __super::Get_State(EState::Right);
	_vector	vUp = __super::Get_State(EState::Up);
	_vector	vLook = __super::Get_State(EState::Look);

	_matrix	RotateMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), -m_MoveStateDesc.fRotatePerSec * TimeDelta);

	__super::Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	__super::Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	__super::Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
}

void CMovement::Jump_Begin()
{
	m_MoveStateDesc.IsJump = true;
	m_MoveStateDesc.IsHitBack = false;
	m_MoveStateDesc.IsGravity = false;
}

void CMovement::Gravity_Begin()
{
	m_MoveStateDesc.IsJump = false;
	m_MoveStateDesc.IsHitBack = true;
	m_MoveStateDesc.IsGravity = false;
}

void CMovement::HitBack_Begin()
{
	m_MoveStateDesc.IsJump = false;
	m_MoveStateDesc.IsHitBack = true;
}

void CMovement::Jumping(_float TimeDelta)
{
}

void CMovement::Gravity(_float TimeDelta)
{
}

void CMovement::HitBack(_float TimeDelta)
{
}

CMovement * CMovement::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMovement* pInstance = new CMovement(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMovement) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CMovement::Clone(void * pArg)
{
	CMovement* pClone = new CMovement(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CMovement) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CMovement::Free()
{
	__super::Free();
}
