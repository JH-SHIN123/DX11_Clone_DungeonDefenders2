#include "..\public\Transform.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
	, m_WorldMatrix(rhs.m_WorldMatrix)
{
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	CComponent::NativeConstruct_Prototype();

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	CComponent::NativeConstruct(pArg);

	TRANSFORM_DESC Data;
	if (nullptr != pArg)
	{
		memcpy(&Data, pArg, sizeof(TRANSFORM_DESC));
		Set_Scale(XMLoadFloat4(&Data.vScale));
		Set_State(EState::Position, XMLoadFloat4(&Data.vPos));
	}


	return S_OK;
}

_vector CTransform::Get_State(EState eState) const
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[(_uint)eState][0]);
}

_float CTransform::Get_Scale(EState eState) const
{
	if (EState::Position == eState)
		return 0.f;

	return XMVectorGetX(XMVector3Length(XMLoadFloat3((_float3*)(&m_WorldMatrix.m[(_uint)eState][0]))));
}

void CTransform::Set_State(EState eState, _fvector vState)
{
	memcpy(&m_WorldMatrix.m[(_uint)eState][0], &vState, sizeof(_vector));
}

void CTransform::Set_Scale(_fvector vScale)
{
	// Ω∫¿⁄¿Ã
	_vector vPosition = Get_State(EState::Position);
	_matrix ScaleMatrix = XMMatrixScalingFromVector(vScale);
	_float4x4 Scale4x4;
	XMStoreFloat4x4(&Scale4x4, ScaleMatrix);
	memcpy(&Scale4x4.m[(_uint)EState::Position][0], &vPosition, sizeof(_vector));
	m_WorldMatrix = Scale4x4;
	//memcpy(&ScaleMatrix.m[(_uint)EState::Position][0], &vPosition, sizeof(_vector));
}

void CTransform::Set_Scale_LinearRotate(_fvector vScale)
{
	_vector vRight	= XMVector3Normalize(Get_State(EState::Right))	* XMVectorGetX(vScale);
	_vector vUp		= XMVector3Normalize(Get_State(EState::Up))		* XMVectorGetY(vScale);
	_vector vLook	= XMVector3Normalize(Get_State(EState::Look))	* XMVectorGetZ(vScale);

	memcpy(&m_WorldMatrix.m[(_uint)EState::Right][0], &vRight, sizeof(_vector));
	memcpy(&m_WorldMatrix.m[(_uint)EState::Up][0], &vUp, sizeof(_vector));
	memcpy(&m_WorldMatrix.m[(_uint)EState::Look][0], &vLook, sizeof(_vector));
}

void CTransform::Set_Rotate(_fmatrix fMatrix)
{
	for (_int i = 0; i < 3; ++i)
	{
		_vector vRotate = XMVector3Normalize(fMatrix.r[i]) * Get_Scale((EState)i);

		memcpy(&m_WorldMatrix.m[i][0], &vRotate, sizeof(_vector));
	}
}

void CTransform::Set_RotateAxis(_fvector vAxis, _float fRadian)
{
	_vector	vRight	= XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(EState::Right);
	_vector	vUp		= XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(EState::Up);
	_vector	vLook	= XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(EState::Look);

	_matrix	RotateMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
}

void CTransform::Set_RotateAxis_MyRadian(_fvector vAxis, _float fRadian)
{
	_vector	vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * Get_Scale(EState::Right);
	_vector	vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * Get_Scale(EState::Up);
	_vector	vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * Get_Scale(EState::Look);

	_matrix	RotateMatrix = XMMatrixRotationAxis(vAxis, m_vRadian.y + fRadian);

	Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
}

void CTransform::RotateToTarget(_fvector vTargetPos)
{
	_vector	vRight, vUp, vLook;
	_vector	vPosition;

	vPosition = Get_State(EState::Position);

	_vector		vDirection = vTargetPos - vPosition;

	vLook	= XMVector3Normalize(vDirection) * Get_Scale(EState::Look);
	vRight	= XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * Get_Scale(EState::Right);
	vUp		= XMVector3Normalize(XMVector3Cross(vLook, vRight)) * Get_Scale(EState::Up);

	Set_State(EState::Right, vRight);
	Set_State(EState::Up, vUp);
	Set_State(EState::Look, vLook);
}

void CTransform::RotateToTargetOnLand(_fvector vTargetPos)
{
	_vector	vRight, vUp, vLook;
	_vector	vPosition;

	vPosition = Get_State(EState::Position);

	_vector		vDirection = vTargetPos - vPosition;

	vLook	= XMVector3Normalize(vDirection) * Get_Scale(EState::Look);
	vRight	= XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)) * Get_Scale(EState::Right);
	vUp		= Get_State(EState::Up);
	vLook	= XMVector3Normalize(XMVector3Cross(vRight, vUp)) * Get_Scale(EState::Look);

	Set_State(EState::Right, vRight);
	Set_State(EState::Up, vUp);
	Set_State(EState::Look, vLook);
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CTransform* pInstance = new CTransform(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CTransform) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	CTransform* pClone = new CTransform(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CTransform) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CTransform::Free()
{
	CComponent::Free();
}
