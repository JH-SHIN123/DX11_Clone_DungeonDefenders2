#include "..\public\Collider.h"
#include "Pipeline_Manager.h"


CCollider::CCollider(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{

}

CCollider::CCollider(const CCollider & rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_pBatch(rhs.m_pBatch)
	, m_pAABB(rhs.m_pAABB)
	, m_pOBB(rhs.m_pOBB)
	, m_pSphere(rhs.m_pSphere)
	, m_pInputLayout(rhs.m_pInputLayout)
	, m_eColliderType(rhs.m_eColliderType)	
	, m_vColor(rhs.m_vColor)
{
	Safe_AddRef(m_pInputLayout);

}

HRESULT CCollider::NativeConstruct_Prototype(ECollideType eColliderType)
{
	__super::NativeConstruct_Prototype();

	m_eColliderType = eColliderType;

	m_pEffect = new BasicEffect(m_pDevice);
	if (nullptr == m_pEffect)
		return E_FAIL;

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = nullptr;
	size_t		ShaderByteCodeLength = 0;

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &ShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(DirectX::VertexPositionColor::InputElements, DirectX::VertexPositionColor::InputElementCount, pShaderByteCode, ShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

	m_pBatch = new DirectX::PrimitiveBatch<DirectX::VertexPositionColor>(m_pDevice_Context);
	if (nullptr == m_pBatch)
		return E_FAIL;

	XMStoreFloat4(&m_vColor, DirectX::Colors::Green);

	return S_OK;
}

HRESULT CCollider::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	if (nullptr != pArg)
		memcpy(&m_ColliderDesc, pArg, sizeof(COLLIDER_DESC));


	m_ColliderDesc.vPosition.y += m_ColliderDesc.vScale.y;	

	switch (m_eColliderType)
	{
	case ECollideType::AABB:
		m_pAABB = new BoundingBox(m_ColliderDesc.vPosition, m_ColliderDesc.vScale);
		break;
	case ECollideType::OBB:
		m_pOBB = new BoundingOrientedBox(m_ColliderDesc.vPosition, m_ColliderDesc.vScale, _float4(1.f, 0.f, 0.f, 0.f));
		break;
	case ECollideType::SPHERE:
		m_pSphere = new BoundingSphere(m_ColliderDesc.vPosition, m_ColliderDesc.vScale.x);
		break;
	}

	return S_OK;
}

_int CCollider::Update_Collider(_fmatrix WorldMatrix)
{
	_matrix		TransformMatrix = WorldMatrix;

	_vector		vWorldPos = WorldMatrix.r[3];
	vWorldPos = Fix_Position(vWorldPos);

	//_vector		vPosition;

	switch (m_eColliderType)
	{
	case ECollideType::AABB:
		TransformMatrix = Remove_Rotation(TransformMatrix);
		TransformMatrix.r[3] = vWorldPos - XMLoadFloat3(&m_pAABB->Center);
		m_pAABB->Transform(*m_pAABB, TransformMatrix);
		break;
	case ECollideType::OBB:
		TransformMatrix.r[3] = vWorldPos - XMLoadFloat3(&m_pOBB->Center);
		m_pOBB->Transform(*m_pOBB, TransformMatrix);
		break;
	case ECollideType::SPHERE:
		TransformMatrix = Remove_Rotation(TransformMatrix);
		TransformMatrix.r[3] = vWorldPos - XMLoadFloat3(&m_pSphere->Center);
		m_pSphere->Transform(*m_pSphere, TransformMatrix);
		break;
	}
	return _int();
}

_int CCollider::Update_Collider(_fvector vPosition)
{
	_matrix		TransformMatrix = XMMatrixIdentity();

	//TransformMatrix.r[3] = vPosition;

	switch (m_eColliderType)
	{
	case ECollideType::AABB:
		//TransformMatrix = Remove_Rotation(TransformMatrix);
		TransformMatrix.r[0] = XMVectorSet(m_ColliderDesc.vScale.x, 0.f, 0.f, 0.f);
		TransformMatrix.r[1] = XMVectorSet(0.f, m_ColliderDesc.vScale.y, 0.f, 0.f);
		TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, m_ColliderDesc.vScale.x, 0.f);
		TransformMatrix.r[3] = vPosition - XMLoadFloat3(&m_pAABB->Center);
		m_pAABB->Transform(*m_pAABB, TransformMatrix);
		break;
	case ECollideType::OBB:
		TransformMatrix.r[3] = vPosition - XMLoadFloat3(&m_pOBB->Center);
		m_pOBB->Transform(*m_pOBB, TransformMatrix);
		break;
	case ECollideType::SPHERE:
		TransformMatrix.r[3] = vPosition - XMLoadFloat3(&m_pSphere->Center);
		m_pSphere->Transform(*m_pSphere, TransformMatrix);
		break;
	}
	return _int();
}

HRESULT CCollider::Render_Collider(/*_fmatrix WorldMatrix*/)
{
	if (nullptr == m_pEffect ||
		nullptr == m_pBatch)
		return E_FAIL;

	CPipeline_Manager*	pPipeLine = CPipeline_Manager::GetInstance();
	if (nullptr == pPipeLine)
		return E_FAIL;

	Safe_AddRef(pPipeLine);

	//m_pEffect->SetWorld(Remove_Scale(WorldMatrix));

	m_pEffect->SetView(pPipeLine->Get_Transform(ETransformState::View));
	m_pEffect->SetProjection(pPipeLine->Get_Transform(ETransformState::Proj));

	m_pDevice_Context->IASetInputLayout(m_pInputLayout);

	m_pEffect->Apply(m_pDevice_Context);

	m_pBatch->Begin();

	/* 쉐이더 셋팅에 대한 작업. */

	XMStoreFloat4(&m_vColor, m_IsCollision == true ? DirectX::Colors::Red : DirectX::Colors::Green);

	switch (m_eColliderType)
	{
	case ECollideType::AABB:
		DX::Draw(m_pBatch, *m_pAABB, XMLoadFloat4(&m_vColor));
		break;
	case ECollideType::OBB:
		DX::Draw(m_pBatch, *m_pOBB, XMLoadFloat4(&m_vColor));
		break;
	case ECollideType::SPHERE:
		DX::Draw(m_pBatch, *m_pSphere, XMLoadFloat4(&m_vColor));
		break;
	}

	m_pBatch->End();

	Safe_Release(pPipeLine);

	return S_OK;
}

_matrix CCollider::Remove_Scale(_matrix Transform)
{
	_matrix		Result;

	Result.r[0] = XMVector3Normalize(Transform.r[0]);
	Result.r[1] = XMVector3Normalize(Transform.r[1]);
	Result.r[2] = XMVector3Normalize(Transform.r[2]);
	Result.r[3] = Transform.r[3];

	return Result;
}

_matrix CCollider::Remove_Rotation(_matrix Transform)
{
	_matrix		Result;

	Result.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(Transform.r[0]);
	Result.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(Transform.r[1]);
	Result.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(Transform.r[2]);
	Result.r[3] = Transform.r[3];

	return Result;
}

_fvector CCollider::Fix_Position(_fvector vPosition)
{
	_vector		vResult = vPosition;

	_float		fScaleY = 0.f;

	switch (m_eColliderType)
	{
	case ECollideType::AABB:
		fScaleY = m_pAABB->Extents.y;
		break;
	case ECollideType::OBB:
		fScaleY = m_pOBB->Extents.y;
		break;
	case ECollideType::SPHERE:
		fScaleY = m_pSphere->Radius;
		break;
	}

	vResult.m128_f32[1] += fScaleY;

	return vResult;
}

CCollider * CCollider::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, ECollideType eColliderType)
{
	CCollider*		pInstance = new CCollider(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype(eColliderType)))
	{
		MSG_BOX("Failed to Creating Instance (CCollider) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CCollider::Clone(void* pArg)
{
	CCollider*		pInstance = new CCollider(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCollider) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	if (false == m_IsClone)
	{
		Safe_Delete(m_pEffect);
		Safe_Delete(m_pBatch);
	}

	Safe_Delete(m_pAABB);
	Safe_Delete(m_pOBB);
	Safe_Delete(m_pSphere);
	Safe_Release(m_pInputLayout);
	
}
