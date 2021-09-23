#pragma once

/* For. Static State*/
/* what if you want to Dynamic State. may used Movement Component*/

#ifndef __TRANSFORM_H__

#include "Component.h"

BEGIN(Engine)

enum class EState
{Right, Up, Look, Position, End};

enum class ERotateEuler
{Right, Up, Look, End};

typedef struct tagTransformDesc
{
	_float4 vPos;
	_float4 vScale;

}TRANSFORM_DESC;

class ENGINE_DLL CTransform : public CComponent
{
protected:
	explicit CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTransform(const CTransform& rhs);
	virtual ~CTransform() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public: // Getter
	_vector Get_State(EState eState) const;
	_float	Get_Scale(EState eState) const;
	_matrix	Get_WorldMatrix() const { return XMLoadFloat4x4(&m_WorldMatrix); }
	_matrix	Get_WorldMatrix_Inverse() const { return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)); }

public:	// Setter
	void	Set_State(EState eState, _fvector vState);
	void	Set_Scale(_fvector vScale); // only scale
	void	Set_Scale_LinearRotate(_fvector vScale);
	void	Set_Rotate(_fmatrix fMatrix);
	void	Set_RotateAxis(_fvector vAxis, _float fRadian);
	void	Set_RotateAxis_MyRadian(_fvector vAxis, _float fRadian);
	void	Set_WorldMatrix(const _fmatrix& WorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); }
	void	Set_WorldMatrix(const _float4x4& WorldMatrix) { m_WorldMatrix = WorldMatrix; }

public:	// Rotate
	void	RotateToTarget(_fvector vTargetPos);
	void	RotateToTargetOnLand(_fvector vTargetPos);
	
private:
	_float4x4		m_WorldMatrix;
	_float3			m_vRadian = _float3(0.f, 0.f, 0.f);

public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __TRANSFORM_H__
#endif // !__TRANSFORM_H__
