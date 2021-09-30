#include "..\public\Cell.h"

CCell::CCell(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

void CCell::Set_Neighbor(NEIGHBOR eNeighbor, _uint iIndex)
{
	m_Neighbor[eNeighbor] = iIndex;
}

HRESULT CCell::NativeContruct(const _float3 * pPoints)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	memset(m_Neighbor, -1, sizeof(_int) * NEIGHBOR_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
		else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
			return true;
		else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
			return true;
	}

	return false;
}

CCell::RESULTDESC CCell::isIn(vector<CCell*>& Cells, _fvector vGoalPos)
{
	RESULTDESC		ResultDesc;

	ResultDesc.isIn = true;

	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vSour = XMVector3Normalize(vGoalPos - XMLoadFloat3(&m_vPoints[i]));
		_vector vDest = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0.f, m_vLine[i].x, 0.f));

		/* 현재 셀을 나갔다. */
		if (0 < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			ResultDesc.isIn = false;

			if (0 <= m_Neighbor[i])
				ResultDesc = Cells[m_Neighbor[i]]->isIn(Cells, vGoalPos);


			if (true == ResultDesc.isIn)
			{
				/* 현재 이웃을 탐색한 부모기준의 정보로 채운다. */
				ResultDesc.isIn = false;
				ResultDesc.iResultIndex = Cells[m_Neighbor[i]]->Get_Index();
				break;
			}
			break;
		}
	}

	return ResultDesc;
}

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _float3 * pPoints)
{
	CCell*		pInstance = new CCell(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeContruct(pPoints)))
	{
		MSG_BOX("Failed to Creating Instance (CCell) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
