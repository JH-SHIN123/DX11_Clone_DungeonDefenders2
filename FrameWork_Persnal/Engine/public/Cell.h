#pragma once
#ifndef __CELL_H__

#include "Base.h"

BEGIN(Engine)
enum class EPoint { A, B, C, End }; // 삼각형을 구성하는 정점 세개!

class CCell final : public CBase
{
public:
	explicit CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CCell() = default;

public:
	HRESULT NativeContruct(const _float3* pPoints);

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

private:
	_float3			m_vPoints[(_uint)EPoint::End];

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _float3* pPoints);
	virtual void Free() override;
};

END
#define __CELL_H__
#endif // !__CELL_H__
