#pragma once
#ifndef __CELL_H__

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT		{ POINT_A, POINT_B, POINT_C, POINT_END }; // 삼각형을 구성하는 정점 세개!
	enum NEIGHBOR	{ NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE		{ LINE_AB, LINE_BC, LINE_CA, LINE_END };

	typedef struct tagResultDesc
	{
		_bool isIn = false;
		_int iResultIndex = -1;
	}RESULTDESC;

public:
	explicit CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CCell() = default;
public:
	_fvector Get_Point(POINT ePoint) const {
		return XMLoadFloat3(&m_vPoints[ePoint]);
	}

	_uint Get_Index() const {
		return m_iIndex;
	}

public:
	void Set_Neighbor(NEIGHBOR eNeighbor, _uint iIndex);
	void Set_Index(_uint iIndex) {
		m_iIndex = iIndex;
	}
public:
	HRESULT NativeContruct(const _float3* pPoints);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	RESULTDESC isIn(vector<CCell*>& Cells, _fvector vGoalPos);
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;
private:
	_float3			m_vPoints[POINT_END];
	_int			m_Neighbor[NEIGHBOR_END];
	_uint			m_iIndex = 0;

	_float3			m_vLine[LINE_END];
public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _float3* pPoints);
	virtual void Free() override;
};

END
#define __CELL_H__
#endif // !__CELL_H__
