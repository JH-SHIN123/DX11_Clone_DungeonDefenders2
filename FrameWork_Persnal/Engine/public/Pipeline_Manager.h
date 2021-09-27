#pragma once

#ifndef __PIPELINE_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

enum class ETransformState
{ View, Proj, End};

class ENGINE_DLL CPipeline_Manager final :	public CBase
{
	DECLARE_SINGLETON(CPipeline_Manager)
public:
	CPipeline_Manager();
	virtual ~CPipeline_Manager() = default;

public:
	_uint Tick();

public:	// Getter
	_matrix Get_Transform(ETransformState eState) const { return XMLoadFloat4x4(&m_StateMatrices[(_uint)eState]); }
	_fvector Get_CamRight_Dir() const	{ return XMLoadFloat4((_float4*)&m_CameraWorldMatrix.m[0][0]); }
	_fvector Get_CamUp_Dir()	const	{ return XMLoadFloat4((_float4*)&m_CameraWorldMatrix.m[1][0]); }
	_fvector Get_CamLook_Dir()	const	{ return XMLoadFloat4((_float4*)&m_CameraWorldMatrix.m[2][0]); }
	_fvector Get_CamPosition()	const	{ return XMLoadFloat4((_float4*)&m_CameraWorldMatrix.m[3][0]); }


public:	// Setter
	void Set_Transform(ETransformState eState, _fmatrix TransformStateMatrix) { XMStoreFloat4x4(&m_StateMatrices[(_uint)eState], TransformStateMatrix); }
	void Set_WorldTransform(_fmatrix TransformStateMatrix) { XMStoreFloat4x4(&m_CameraWorldMatrix, TransformStateMatrix); }

private:
	_float4x4				m_StateMatrices[(_uint)ETransformState::End];
	_float4x4				m_CameraWorldMatrix;

public:
	virtual void Free() override;
};

END
#define __PIPELINE_MANAGER_H__
#endif // !__PIPELINE_MANAGER_H__
