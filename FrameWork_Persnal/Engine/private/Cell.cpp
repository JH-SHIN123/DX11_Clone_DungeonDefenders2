#include "..\public\Cell.h"
#include "Pipeline_Manager.h"
#include "GameInstance.h"

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

HRESULT CCell::NativeContruct(const _float3 * pPoints, _int iCellOption)
{
	m_iCellOption = iCellOption;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	memset(m_Neighbor, -1, sizeof(_int) * NEIGHBOR_END);

	XMStoreFloat3(&m_vLine[LINE_AB], XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]));
	XMStoreFloat3(&m_vLine[LINE_BC], XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]));
	XMStoreFloat3(&m_vLine[LINE_CA], XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]));

	if (FAILED(Ready_VertexBuffer()))
		return E_FAIL;

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	//if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	//{
	//	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
	//		return true;
	//	else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
	//		return true;
	//}

	//if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
	//{
	//	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
	//		return true;
	//	else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
	//		return true;
	//}

	//if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
	//{
	//	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint))
	//		return true;
	//	else if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
	//		return true;
	//}

	//return false;

	_float fOffSet = 0.001f;

	//if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
	if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_A]) - vSourPoint)))
	{
		if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_B]) - vDestPoint)))
			return true;
		else if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_C]) - vDestPoint)))
			return true;
	}

	if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_B]) - vSourPoint)))
	{
		if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_A]) - vDestPoint)))
			return true;
		else if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_C]) - vDestPoint)))
			return true;
	}

	if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_C]) - vSourPoint)))
	{
		if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_A]) - vDestPoint)))
			return true;
		else if (true == fOffSet >= XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vPoints[POINT_B]) - vDestPoint)))
			return true;
	}

	return false;

}

CCell::RESULTDESC CCell::isIn(vector<CCell*>& Cells, _fvector vGoalPos, _float* Cell_Y, _int iCount)
{
	RESULTDESC		ResultDesc;

	//ResultDesc.iCount = iCount;

	ResultDesc.isIn = true;

	for (_uint i = 0; i < LINE_END; ++i)
	{
		_vector	vSour = XMVector3Normalize(vGoalPos - XMLoadFloat3(&m_vPoints[i]));
		_vector vDest = XMVector3Normalize(XMVectorSet(m_vLine[i].z * -1.f, 0.f, m_vLine[i].x, 0.f));


		_vector vTri_A, vTri_B, vTri_C;
		vTri_A = XMLoadFloat3(&m_vPoints[0]);
		vTri_B = XMLoadFloat3(&m_vPoints[1]);
		vTri_C = XMLoadFloat3(&m_vPoints[2]);
		_vector vOut = XMPlaneFromPoints(vTri_A, vTri_B, vTri_C);
		*Cell_Y = -(XMVectorGetX(vOut) * XMVectorGetX(vGoalPos) + XMVectorGetZ(vOut) * XMVectorGetZ(vGoalPos) + XMVectorGetW(vOut)) / XMVectorGetY(vOut);

		/* 현재 셀을 나갔다. */
		if (0 < XMVectorGetX(XMVector3Dot(vSour, vDest)))
		{
			ResultDesc.isIn = false;
			ResultDesc.vDstPoints = m_vLine[i];

			if (0 <= m_Neighbor[i] && 5 >= ResultDesc.iCount)
			{
				++ResultDesc.iCount;
				ResultDesc = Cells[m_Neighbor[i]]->isIn(Cells, vGoalPos, Cell_Y, ResultDesc.iCount);
			}
			//else if (0 <= m_Neighbor[1])
			//	ResultDesc = Cells[m_Neighbor[1]]->isIn(Cells, vGoalPos, Cell_Y);
			//else if (0 <= m_Neighbor[2])
			//	ResultDesc = Cells[m_Neighbor[2]]->isIn(Cells, vGoalPos, Cell_Y);

			// 꼭짓점 예외처리...

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

_bool CCell::Check_Cell(_fvector vMouseDir, _fvector vMousePos_World, _vector* vOutPos)
{
	_bool IsIn = false;
	_float fDis = 0.f;

	IsIn = TriangleTests::Intersects(vMousePos_World, vMouseDir, XMLoadFloat3(&m_vPoints[0]), XMLoadFloat3(&m_vPoints[1]), XMLoadFloat3(&m_vPoints[2]), fDis);

	if (true == IsIn)
	{
		_vector vWorldMouseDir = XMVectorZero();
		_vector vWorldMousePos = XMVectorZero();

		vWorldMouseDir = vMouseDir * fDis;
		vWorldMousePos = vMousePos_World + vWorldMouseDir;

		_vector	vSour = XMVector3Normalize(vWorldMousePos - XMLoadFloat3(&m_vPoints[0]));
		_vector vDest = XMVector3Normalize(XMVectorSet(m_vLine[0].z * -1.f, 0.f, m_vLine[0].x, 0.f));


		_vector vTri_A, vTri_B, vTri_C;
		vTri_A = XMLoadFloat3(&m_vPoints[0]);
		vTri_B = XMLoadFloat3(&m_vPoints[1]);
		vTri_C = XMLoadFloat3(&m_vPoints[2]);
		_vector vOut = XMPlaneFromPoints(vTri_A, vTri_B, vTri_C);
		
		// y는 구한듯?
		vWorldMousePos = XMVectorSetY(vWorldMousePos, -(XMVectorGetX(vOut) * XMVectorGetX(vWorldMousePos) + XMVectorGetZ(vOut) * XMVectorGetZ(vWorldMousePos) + XMVectorGetW(vOut)) / XMVectorGetY(vOut));

		vWorldMousePos = XMVectorSetW(vWorldMousePos, 1.f);
		*vOutPos = vWorldMousePos;
	}

	return IsIn;
}

#ifdef _DEBUG

HRESULT CCell::Render_Cell(_uint iCurrentIndex)
{
	_uint		iOffSet = 0;

	m_pDevice_Context->IASetVertexBuffers(0, 1, &m_pVB, &m_iStride, &iOffSet);

	if (m_iIndex == iCurrentIndex)
	{
		m_pDevice_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pEffect->GetVariableByName("g_vCellColor")->SetRawValue(&XMVectorSet(1.f, 0.7f, 0.7f, 1.f), 0, sizeof(_vector));
	}
	else
	{
		m_pDevice_Context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		m_pEffect->GetVariableByName("g_vCellColor")->SetRawValue(&XMVectorSet(1.f, 1.f, 1.f, 1.f), 0, sizeof(_vector));
	}

	m_pDevice_Context->IASetInputLayout(m_InputLayouts[0].pLayout);

	CPipeline_Manager*	pPipeLine = CPipeline_Manager::GetInstance();
	if (nullptr == pPipeLine)
		return E_FAIL;

	m_pEffect->GetVariableByName("g_WorldMatrix")->SetRawValue(&XMMatrixIdentity(), 0, sizeof(_matrix));
	m_pEffect->GetVariableByName("g_ViewMatrix")->SetRawValue(&XMMatrixTranspose(GET_VIEW_SPACE), 0, sizeof(_matrix));
	m_pEffect->GetVariableByName("g_ProjMatrix")->SetRawValue(&XMMatrixTranspose(GET_PROJ_SPACE), 0, sizeof(_matrix));

	m_InputLayouts[0].pPass->Apply(0, m_pDevice_Context);

	m_pDevice_Context->Draw(4, 0);

	return S_OK;
}



HRESULT CCell::Ready_VertexBuffer()
{
	ZeroMemory(&m_VBDesc, sizeof(D3D11_BUFFER_DESC));

	m_iNumVertices = 4;
	m_iStride = sizeof(VTXCOL);

	m_VBDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_VBDesc.Usage = D3D11_USAGE_IMMUTABLE;
	m_VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_VBDesc.CPUAccessFlags = 0;
	m_VBDesc.MiscFlags = 0;
	m_VBDesc.StructureByteStride = m_iStride;

	VTXCOL*		pVertices = new VTXCOL[4];

	pVertices[0].vPosition = m_vPoints[POINT_A];
	pVertices[0].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	pVertices[1].vPosition = m_vPoints[POINT_B];
	pVertices[1].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	pVertices[2].vPosition = m_vPoints[POINT_C];
	pVertices[2].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	pVertices[3].vPosition = m_vPoints[POINT_A];
	pVertices[3].vColor = _float4(1.f, 1.f, 1.f, 1.f);

	ZeroMemory(&m_VBSubresourceData, sizeof(m_VBSubresourceData));
	m_VBSubresourceData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&m_VBDesc, &m_VBSubresourceData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);


	D3D11_INPUT_ELEMENT_DESC			ElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	_uint		iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

	ID3DBlob*		pCompileShader = nullptr;
	ID3DBlob*		pCompileShaderErrorMsg = nullptr;

	HRESULT hr = S_OK;
	if (FAILED(hr = D3DCompileFromFile(L"../Bin/Shader/Shader_Cell.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, nullptr, "fx_5_0", iFlag, 0, &pCompileShader, &pCompileShaderErrorMsg)))
		return E_FAIL;

	if (FAILED(D3DX11CreateEffectFromMemory(pCompileShader->GetBufferPointer(), pCompileShader->GetBufferSize(), 0, m_pDevice, &m_pEffect)))
		return E_FAIL;

	ID3DX11EffectTechnique*		pTechnique = m_pEffect->GetTechniqueByName("DefaultTechnique");
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC		TechniqueDesc;
	if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
		return E_FAIL;



	m_InputLayouts.reserve(TechniqueDesc.Passes);

	for (uint32_t i = 0; i < TechniqueDesc.Passes; ++i)
	{
		INPUTLAYOUTDESC			InputLayoutDesc;

		InputLayoutDesc.pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC		PassDesc;
		InputLayoutDesc.pPass->GetDesc(&PassDesc);

		if (FAILED(m_pDevice->CreateInputLayout(ElementDesc, 2, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &InputLayoutDesc.pLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(InputLayoutDesc);
	}
	Safe_Release(pTechnique);
	Safe_Release(pCompileShaderErrorMsg);
	Safe_Release(pCompileShader);


	return S_OK;
}

#endif

CCell * CCell::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _float3 * pPoints,_int iCellOption)
{
	CCell*		pInstance = new CCell(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeContruct(pPoints, iCellOption)))
	{
		MSG_BOX("Failed to Creating Instance (CCell) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CCell::Free()
{
	for (auto& InputLayout : m_InputLayouts)
	{
		Safe_Release(InputLayout.pPass);
		Safe_Release(InputLayout.pLayout);
	}
	m_InputLayouts.clear();

	Safe_Release(m_pEffect);
	Safe_Release(m_pVB);

	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}

/*
_float fPointDis[POINT_END];
_vector vPointPos[POINT_END];
vPointPos[POINT_A] = XMLoadFloat3(&m_vPoints[POINT_A]) - vGoalPos;
vPointPos[POINT_B] = XMLoadFloat3(&m_vPoints[POINT_B]) - vGoalPos;
vPointPos[POINT_C] = XMLoadFloat3(&m_vPoints[POINT_C]) - vGoalPos;

fPointDis[POINT_A] = XMVectorGetX(XMVector3Length(XMVectorSetY(vPointPos[POINT_A], 0.f)));
fPointDis[POINT_B] = XMVectorGetX(XMVector3Length(XMVectorSetY(vPointPos[POINT_B], 0.f)));
fPointDis[POINT_C] = XMVectorGetX(XMVector3Length(XMVectorSetY(vPointPos[POINT_C], 0.f)));

if (fPointDis[POINT_A] < fPointDis[POINT_B] &&
fPointDis[POINT_A] < fPointDis[POINT_C])
{
if (fPointDis[POINT_B] <= fPointDis[POINT_C])
{
ResultDesc.vDstPoints = m_vPoints[POINT_A];
ResultDesc.vSrcPoints = m_vPoints[POINT_B];
}

else if (fPointDis[POINT_B] > fPointDis[POINT_C])
{
ResultDesc.vDstPoints = m_vPoints[POINT_A];
ResultDesc.vSrcPoints = m_vPoints[POINT_C];
}
}
else if (fPointDis[POINT_B] < fPointDis[POINT_C] &&
fPointDis[POINT_B] < fPointDis[POINT_A])
{
if (fPointDis[POINT_A] <= fPointDis[POINT_C])
{
ResultDesc.vDstPoints = m_vPoints[POINT_A];
ResultDesc.vSrcPoints = m_vPoints[POINT_B];
}

else if (fPointDis[POINT_A] > fPointDis[POINT_C])
{
ResultDesc.vDstPoints = m_vPoints[POINT_B];
ResultDesc.vSrcPoints = m_vPoints[POINT_C];
}
}

else if (fPointDis[POINT_C] < fPointDis[POINT_A] &&
fPointDis[POINT_C] < fPointDis[POINT_B])
{
if (fPointDis[POINT_A] <= fPointDis[POINT_B])
{
ResultDesc.vDstPoints = m_vPoints[POINT_C];
ResultDesc.vSrcPoints = m_vPoints[POINT_A];
}

else if (fPointDis[POINT_A] > fPointDis[POINT_B])
{
ResultDesc.vDstPoints = m_vPoints[POINT_B];
ResultDesc.vSrcPoints = m_vPoints[POINT_C];
}
}




*/