#pragma once

#ifndef __PIPELINE_MANAGER_H__

#include "Base.h"

BEGIN(Engine)

enum class ETransformState
{View, Proj, End};

class ENGINE_DLL CPipeline_Manager final :	public CBase
{
	DECLARE_SINGLETON(CPipeline_Manager)
public:
	CPipeline_Manager();
	virtual ~CPipeline_Manager() = default;

public:	// Getter
	_matrix Get_Transform(ETransformState eState) const { return XMLoadFloat4x4(&m_StateMatrices[(_uint)eState]); }

public:	// Setter
	void Set_Transform(ETransformState eState, _fmatrix TransformStateMatrix) { XMStoreFloat4x4(&m_StateMatrices[(_uint)eState], TransformStateMatrix); }

private:
	_float4x4				m_StateMatrices[(_uint)ETransformState::End];

public:
	virtual void Free() override;
};

END
#define __PIPELINE_MANAGER_H__
#endif // !__PIPELINE_MANAGER_H__
