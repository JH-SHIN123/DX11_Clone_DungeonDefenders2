#include "..\public\Navigation.h"
#include "Cell.h"
#include "ModelLoader.h"

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

HRESULT CNavigation::NativeConstruct_Prototype(const char * pNaviFilePath, const char * pNaviFileName)
{
	char			szFullPath[MAX_PATH] = "";

	strcpy(szFullPath, pNaviFilePath);
	strcat(szFullPath, pNaviFileName);

	m_pScene = m_Importer.ReadFile(szFullPath, aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | 
		aiProcess_FlipWindingOrder | aiProcess_JoinIdenticalVertices);

	if (nullptr == m_pScene)
		return E_FAIL;

	_uint		iNumVertices = 0;

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
		iNumVertices += m_pScene->mMeshes[i]->mNumVertices;

	m_pCells.reserve(iNumVertices);
	
	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		for (_uint j = 0; j < m_pScene->mMeshes[i]->mNumVertices; ++j)
		{
			//m_pScene->mMaterials[i]
			aiString		strTexturePath;

			m_pScene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &strTexturePath);

			char			szTextureFileName[MAX_PATH] = "";
			char			szExt[MAX_PATH] = "";

			_splitpath(strTexturePath.data, nullptr, nullptr, szTextureFileName, szExt);
			
			_float3 vPoint[3], vPoint_2[3];
			_float fOffSet_X = 0.05f;
			_float fOffSet_Y = -0.05f;
			_float fOffSet_Z = -0.05f;	
			
			vPoint[0].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint[0].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint[0].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			vPoint_2[0].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint_2[0].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint_2[0].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			++j;

			vPoint[1].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint[1].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint[1].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			++j;

			vPoint[2].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint[2].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint[2].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			vPoint_2[1].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint_2[1].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint_2[1].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			++j;

			vPoint_2[2].x = m_pScene->mMeshes[i]->mVertices[j].x * fOffSet_X;
			vPoint_2[2].y = m_pScene->mMeshes[i]->mVertices[j].z * fOffSet_Y;
			vPoint_2[2].z = m_pScene->mMeshes[i]->mVertices[j].y * fOffSet_Z;
			
			CCell*		pCell = CCell::Create(m_pDevice, m_pDevice_Context, vPoint, atoi(szTextureFileName));
			if (nullptr == pCell)
				return E_FAIL;
			
			pCell->Set_Index((_uint)m_pCells.size());
			
			m_pCells.push_back(pCell);	
			

			CCell*		pCell_2 = CCell::Create(m_pDevice, m_pDevice_Context, vPoint_2, atoi(szTextureFileName));
			if (nullptr == pCell)
				return E_FAIL;

			pCell_2->Set_Index((_uint)m_pCells.size());

			m_pCells.push_back(pCell_2);

		}
	}

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

void CNavigation::SetUp_NavigationIndex(_fvector vPos)
{
	for (auto& iter : m_pCells)
	{
		_int iCellIndex = -1;
		iCellIndex = iter->Check_CellIndex(vPos);

		if (0 <= iCellIndex)
		{
			m_NavigationDesc.iCurrentIndex = iCellIndex;
			return;
		}
	}
}

_fvector CNavigation::Get_CellY(_int iCellIndex, _fvector vPos)
{
	return m_pCells[iCellIndex]->Get_CellPos_Y(vPos);
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

			return false;
		}
		/* 움직일 수 있다. 이웃으로 이동했다. */
		else
		{
			m_NavigationDesc.iCurrentIndex = ResultDesc.iResultIndex;
			m_iNowCellOption = ResultDesc.iCellOption;
			return true;
		}
	}
	else
		return true;



}

_bool CNavigation::Get_CellPos(_fvector vMouseDir, _fvector vMousePos_World, _vector* vOutCellPos)
{
	_vector vCellPos = XMVectorZero();

	_bool IsGetPos = false;
	for (auto& pCell : m_pCells) 
	{
		IsGetPos = pCell->Check_Cell(XMVector3Normalize(vMouseDir), vMousePos_World, vOutCellPos);

		if (true == IsGetPos)
			break;
	}

	return IsGetPos;
}

const _int CNavigation::Get_CellIndex(_fvector vPos)
{
	for (auto& iter : m_pCells)
	{
		_int iCellIndex = -1;
		iCellIndex = iter->Check_CellIndex(vPos);

		if (0 <= iCellIndex)
			return iCellIndex;			
	}

	return -1;
}

_fvector CNavigation::Check_NeighborCell_Center(const _int & iCellIndex, _bool IsGreater)
{
	//_int iNeighbor_Option[CCell::NEIGHBOR_END];
	//_int iNeighbor_Index[CCell::NEIGHBOR_END];
	//_vector vNeighbor_Center[CCell::NEIGHBOR_END];
	//
	//for (_int i = 0; i < CCell::LINE_END; ++i)
	//{
	//	iNeighbor_Option[i] = m_pCells[m_pCells[iCellIndex]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_CellOption();
	//	iNeighbor_Index[i] = m_pCells[m_pCells[iCellIndex]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_Index();
	//	vNeighbor_Center[i] = m_pCells[m_pCells[iCellIndex]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_CellCenter();
	//}
	//
	//_int iLess_Option = iNeighbor_Option[0];
	//_int iLess_Index = iNeighbor_Index[0];
	//_vector vLess_Center = vNeighbor_Center[0];
	//
	//
	//if (iLess_Option > iNeighbor_Option[1])
	//{
	//	iLess_Option = iNeighbor_Option[1];
	//	iLess_Index = iNeighbor_Index[1];
	//	vLess_Center = vNeighbor_Center[1];
	//}
	//else if(iLess_Option ==  iNeighbor_Option[1])
	//{
	//	// 이웃의 옵션값이 같으면 그 이웃의 가장 작은 옵션 값을 구하자
	//	// Dest 원래 작은 값 
	//	// Sour 새로운 작은 값
	//	//_int iDest_Option[CCell::NEIGHBOR_END];
	//	//_vector vDest_Center[CCell::NEIGHBOR_END];
	//
	//	_int iDest_Index_Less;
	//	_int iDest_Option_Less = 999;//iDest_Option[0];
	//	_int iSour_Index_Less;
	//	_int iSour_Option_Less = 999;//iSour_Option[0];
	//
	//	for (_int i = 0; i < CCell::LINE_END; ++i)
	//	{
	//		_int iOption = m_pCells[m_pCells[iLess_Index]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_CellOption();
	//		if (iOption <= iDest_Option_Less)
	//		{
	//			iDest_Option_Less = iOption;
	//			iDest_Index_Less = m_pCells[m_pCells[iLess_Index]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_Index();
	//		}
	//
	//		iOption = m_pCells[m_pCells[iNeighbor_Index[1]]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_CellOption();
	//		if (iOption <= iSour_Option_Less)
	//		{
	//			iSour_Option_Less = iOption;
	//			iSour_Index_Less = m_pCells[m_pCells[iNeighbor_Index[1]]->Get_Neighbor_Index((CCell::NEIGHBOR)i)]->Get_Index();
	//		}
	//	}
	//
	//	// 옵셔 ㄴ비교
	//
	//
	//}
	
	return XMVectorSet(0.f, 0.f, 0.f, 0.f);
}

_vector CNavigation::Get_CellCenter_Pos(const _int& iCellIndex)
{
	return m_pCells[iCellIndex]->Get_CellCenter();
}

_bool CNavigation::Get_Collision(_float * fCellY, _fvector vPos)
{
	_vector vNoYPos = XMVectorSetY(vPos, 0.f);

	_int iIndex = Get_CellIndex(vNoYPos);

	if (-1 == iIndex)
		return false;

	vNoYPos = m_pCells[iIndex]->Get_CellPos_Y(vNoYPos);

	*fCellY = vNoYPos.m128_f32[1];

	return true;
}

_bool CNavigation::IsChanged_CellOption(_fvector vPos)
{
	for (auto& iter : m_pCells)
	{
		_int iCellOption = -1;
		iCellOption = iter->Check_CellOptoin(vPos);

		if (0 <= iCellOption)
		{
			m_iNowCellOption = iCellOption;
			return true;
		}
	}

	return false;
}

_fvector CNavigation::Get_Less_NearOption_Pos(_fvector vMyPos)
{
	_int iCellOption = m_iNowCellOption - 1;

	_vector vTargetPos = XMVectorZero();

	for (auto& iter : m_pCells)
		iter->Get_CellCenter(iCellOption, vMyPos, &vTargetPos);

	return vTargetPos;
}

_fvector CNavigation::Get_MyCell_Pos(_fvector vMyPos)
{
	_int iCellOption = m_iNowCellOption;

	_vector vTargetPos = XMVectorZero();

	for (auto& iter : m_pCells)
		iter->Get_CellCenter(iCellOption, vMyPos, &vTargetPos);

	return vTargetPos;
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

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const char * pNavigationDataFile, const char * pNaviFileName)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype(pNavigationDataFile, pNaviFileName)))
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
