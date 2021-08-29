#include "stdafx.h"
#include "Camera_Free.h"

CCamera_Free::CCamera_Free(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CCamera(pDevice, pDevice_Context)
{
}

CCamera_Free::CCamera_Free(const CCamera_Free & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Free::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Free::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CCamera_Free::Tick(_float TimeDelta)
{
	TimeDelta = 0.002f;

	/* TODO. */
	if (nullptr == m_pMovementCom)
		return -1;

	Aim();

	if (GetKeyState(VK_LEFT) & 0x8000)
		m_pMovementCom->Go_Left(TimeDelta);

	if (GetKeyState(VK_RIGHT) & 0x8000)
		m_pMovementCom->Go_Right(TimeDelta);

	if (GetKeyState(VK_UP) & 0x8000)
		m_pMovementCom->Go_Straight(TimeDelta);

	if (GetKeyState(VK_DOWN) & 0x8000)
		m_pMovementCom->Go_Backward(TimeDelta);


	return __super::Tick(TimeDelta); // ÀÌ°Ô »ÇÀÎÆ®
}

_int CCamera_Free::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::Aim()
{
	_vector vRight	= m_pMovementCom->Get_State(EState::Right);
	_vector vUp		= XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector vLook	= m_pMovementCom->Get_State(EState::Look);

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

HRESULT CCamera_Free::Ready_Component()
{
	return S_OK;
}

CCamera_Free * CCamera_Free::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CCamera_Free* pInstance = new CCamera_Free(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Free) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Free::Clone_GameObject(void * pArg)
{
	CCamera_Free* pClone = new CCamera_Free(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Free) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CCamera_Free::Free()
{
	__super::Free();
}
