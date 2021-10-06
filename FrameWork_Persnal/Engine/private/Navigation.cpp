#include "..\public\Navigation.h"
#include "Cell.h"
#include "ModelLoader.h"

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
	, m_pModelLoader(CModelLoader::Create())
{
}

CNavigation::CNavigation(const CNavigation & rhs)
	: CComponent(rhs)
	, m_pCells(rhs.m_pCells)
	, m_pModelLoader(rhs.m_pModelLoader)
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

void CNavigation::Get_CellOption(_fvector vPos)
{
	_int iIndex = 0;

	for (iIndex; iIndex < m_pCells.size(); ++iIndex)
	{
		if (nullptr == m_pCells[iIndex])
			continue;

		_int iCellOption = -1;
		iCellOption = m_pCells[iIndex]->Check_CellOptoin(vPos);

		if (0 <= iCellOption)
		{
			m_iNowCellOption = iCellOption;
			return;
		}
			
	}

	return ;
}

_fvector CNavigation::Get_Less_NearOption_Pos(_vector* vMyPos)
{
	_int iIndex = m_iNowCellOption - 1;

	_vector vTargetPos = XMVectorZero();

	for (auto& iter : m_pCells)
		iter->Get_CellCenter(iIndex, vMyPos, &vTargetPos);

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

	Safe_Release(m_pModelLoader);
}
