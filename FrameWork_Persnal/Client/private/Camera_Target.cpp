#include "stdafx.h"
#include "..\public\Camera_Target.h"
#include "Player.h"
#include "Data_Manager.h"

CCamera_Target::CCamera_Target(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CCamera(pDevice, pDevice_Context)
{
}

CCamera_Target::CCamera_Target(const CCamera_Target & rhs)
	: CCamera(rhs)
{
}

HRESULT CCamera_Target::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CCamera_Target::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component();

	Set_CameraView_Mode(Engine::ECameraViewMode::ThirdPerson);
	return S_OK;
}

_int CCamera_Target::Tick(_float TimeDelta)
{
	View_Check(TimeDelta);

	if (GetAsyncKeyState('B') & 0x8000)
		m_eCameraMode_Next = ECameraViewMode::ThirdPerson;

	if (GetAsyncKeyState('N') & 0x8000)
		m_eCameraMode_Next = ECameraViewMode::TopView;

	if (GetAsyncKeyState('M') & 0x8000)
		m_eCameraMode_Next = ECameraViewMode::TopToTPS;


	if(false == CData_Manager::GetInstance()->Get_Tick_Stop())
		TargetRotate_Check((_uint)ELevel::Stage1, L"Layer_Player", L"Com_Movement");

	return __super::Tick(TimeDelta);
}

_int CCamera_Target::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	return _int();
}

HRESULT CCamera_Target::Render()
{
	return S_OK;
}

void CCamera_Target::MouseMove_Check()
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
}

HRESULT CCamera_Target::Ready_Component()
{
	return S_OK;
}

CCamera_Target * CCamera_Target::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CCamera_Target* pInstance = new CCamera_Target(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Target) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCamera_Target::Clone_GameObject(void * pArg)
{
	CCamera_Target* pClone = new CCamera_Target(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCamera_Target) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CCamera_Target::Free()
{
	__super::Free();
}
