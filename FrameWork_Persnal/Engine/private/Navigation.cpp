#include "..\public\Navigation.h"
#include "Cell.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_pCells(rhs.m_pCells)
{
	for (auto& pCell : m_pCells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::NativeConstruct_Prototype(const _tchar * pNavigationDataFile)
{
	_ulong			dwByte = 0;
	HANDLE			hFile = CreateFile(pNavigationDataFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoint[3];

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*		pCell = CCell::Create(m_pDevice, m_pDevice_Context, vPoint);
		if (nullptr == pCell)
			return E_FAIL;

		pCell->Set_Index((_uint)m_pCells.size());

		m_pCells.push_back(pCell);
	}

	CloseHandle(hFile);

	if (FAILED(Ready_CellNeighbor()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNavigation::NativeConstruct(void * pArg)
{
	if (nullptr != pArg)
		m_NavigationDesc = *(NAVI_DESC*)pArg;

	return S_OK;
}

_bool CNavigation::IsMove(_fvector vOriginalPos, _fvector vDirection, _float* Cell_Y, _vector* vOutSlidingDir)
{
	_vector			vGoalPos = vOriginalPos + vDirection;

	CCell::RESULTDESC ResultDesc = m_pCells[m_NavigationDesc.iCurrentIndex]->isIn(m_pCells, vGoalPos, Cell_Y);

	if (false == ResultDesc.isIn)
	{
		/* 아에 움직일 수 없다. */
		if (0 > ResultDesc.iResultIndex)
		{
			// 여기서 슬라이딩
			_vector vLineNormal = XMVector3Normalize(XMLoadFloat3(&ResultDesc.vDstPoints));
			vLineNormal = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLineNormal);

			*vOutSlidingDir = vDirection - (vLineNormal * (XMVector3Dot(vDirection, vLineNormal)));

			//vGoalPos = vOriginalPos + *vOutSlidingDir;
			//ResultDesc = m_pCells[m_NavigationDesc.iCurrentIndex]->isIn(m_pCells, vGoalPos, Cell_Y);
			return true;
		}
		/* 움직일 수 있다. 이웃으로 이동했다. */
		else
		{
			m_NavigationDesc.iCurrentIndex = ResultDesc.iResultIndex;
			return true;
		}
	}
	else
		return true;



}

#ifdef _DEBUG

HRESULT CNavigation::Render_Navigation()
{
	for (auto& pCell : m_pCells)
		pCell->Render_Cell(m_NavigationDesc.iCurrentIndex);

	return S_OK;
}

#endif

HRESULT CNavigation::Ready_CellNeighbor()
{
	for (auto& pSourCell : m_pCells)
	{
		for (auto& pDestCell : m_pCells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_AB, pDestCell->Get_Index());

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_BC, pDestCell->Get_Index());

			else if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::NEIGHBOR_CA, pDestCell->Get_Index());
		}
	}
	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar * pNavigationDataFile)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNavigationDataFile)))
	{
		MSG_BOX("Failed to Creating Instance (CNavigation) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CNavigation) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_pCells)
		Safe_Release(pCell);

	m_pCells.clear();

}
