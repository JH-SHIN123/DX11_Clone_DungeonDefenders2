#pragma once
#ifndef __CELL_H__

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum NEIGHBOR { NEIGHBOR_AB, NEIGHBOR_BC, NEIGHBOR_CA, NEIGHBOR_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

	typedef struct tagResultDesc
	{
		_bool isIn = false;
		_int iResultIndex = -1;
		_float3 vDstPoints;
		_int iCount = 0;
		_uint iCellOption = -1;
	}RESULTDESC;

public:
	explicit CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CCell() = default;

public:
	_fvector Get_Point(POINT ePoint) const { return XMLoadFloat3(&m_vPoints[ePoint]); }
	_uint Get_Index() const { return m_iIndex; }
	_uint Check_CellOptoin(_fvector vPos);

public:
	void Set_Neighbor(NEIGHBOR eNeighbor, _uint iIndex);
	void Set_Index(_uint iIndex) { m_iIndex = iIndex; }

public:
	HRESULT NativeContruct(const _float3* pPoints, _int iCellOption);
	_bool Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	RESULTDESC isIn(vector<CCell*>& Cells, _fvector vGoalPos, _float* Cell_Y, _int iCount = 0);

	_bool Check_Cell(_fvector vMouseDir, _fvector vMousePos_World, _vector* vOutPos);
	_fvector Get_CellCenter(_uint iOption, _vector* vMyPos, _vector* vTargetPos);

#ifdef _DEBUG
public:
	HRESULT Render_Cell(_uint iCurrentIndex);
#endif
private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;
private:
	_float3			m_vPoints[POINT_END];
	_int			m_Neighbor[NEIGHBOR_END];
	_uint			m_iIndex = 0;
	_int			m_iCellOption = 0;

	_float3			m_vLine[LINE_END];

#ifdef _DEBUG

private:
	ID3D11Buffer*				m_pVB = nullptr;
	D3D11_BUFFER_DESC			m_VBDesc;
	D3D11_SUBRESOURCE_DATA		m_VBSubresourceData;
	_uint						m_iNumVertices = 0;
	_uint						m_iStride = 0;
	_uint						m_iNumVertexBuffers = 0;
	void*						m_pVertices = nullptr;

private:
	ID3DX11Effect*				m_pEffect = nullptr;
	vector<INPUTLAYOUTDESC>		m_InputLayouts;

private:
	HRESULT Ready_VertexBuffer();
#endif

public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const _float3* pPoints, _int iCellOption );
	virtual void Free() override;
};

END
#define __CELL_H__
#endif // !__CELL_H__
