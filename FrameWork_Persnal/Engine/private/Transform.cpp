#include "..\public\Transform.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CTransform::CTransform(const CTransform & rhs)
	: CComponent(rhs)
{
}

HRESULT CTransform::NativeConstruct_Prototype()
{
	CComponent::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CTransform::NativeConstruct(void * pArg)
{
	CComponent::NativeConstruct(pArg);

	return S_OK;
}

_vector CTransform::Get_State(EState eState) const
{
	return XMLoadFloat4((_float4*)&m_WorldMatrix.m[(_uint)eState][0]);
}

_float CTransform::Get_Scale(EState eState) const
{
	return XMVectorGetIntX(XMVector3Length(XMLoadFloat3((_float3*)(&m_WorldMatrix.m[(_uint)eState][0]))));
}

void CTransform::Set_State(EState eState, _fvector vState)
{
	memcpy(&m_WorldMatrix.m[(_uint)eState][0], &vState, sizeof(_vector));
}

void CTransform::Set_RotateAxis(_fvector vAxis, _float fRadian)
{
	_vector	vRight	= XMVectorSet( 1.f, 0.f, 0.f, 0.f ) * XMVectorGetIntX(XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[(_uint)EState::Right][0])));
	_vector	vUp		= XMVectorSet( 0.f, 1.f, 0.f, 0.f ) * XMVectorGetIntX(XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[(_uint)EState::Up][0])));
	_vector	vLook	= XMVectorSet( 0.f, 0.f, 1.f, 0.f ) * XMVectorGetIntX(XMVector3Length(XMLoadFloat3((_float3*)&m_WorldMatrix.m[(_uint)EState::Look][0])));

	_matrix	RotateMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	Set_State(EState::Right, XMVector3TransformNormal(vRight, RotateMatrix));
	Set_State(EState::Up, XMVector3TransformNormal(vUp, RotateMatrix));
	Set_State(EState::Look, XMVector3TransformNormal(vLook, RotateMatrix));
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
