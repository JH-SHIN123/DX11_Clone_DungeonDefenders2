#include "stdafx.h"
#include "..\public\Point_Spread3.h"
#include "EffectDesc_Manager.h"

CPoint_Spread3::CPoint_Spread3(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CPoint_Spread(pDevice, pDevice_Context)
{
}

CPoint_Spread3::CPoint_Spread3(const CPoint_Spread3 & rhs)
	: CPoint_Spread(rhs)
{
}

HRESULT CPoint_Spread3::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Spread3::NativeConstruct(void * pArg)
{
	POINT_SPREAD_DESC_3 Data;

	memcpy(&Data, pArg, sizeof(POINT_SPREAD_DESC_3));

	__super::Ready_Component(&Data.Point_Desc);

	SetUp_Size(Data.vSize);


	m_IsTime = Data.IsTime;

	return S_OK;
}

_int CPoint_Spread3::Tick(_float TimeDelta)
{
	m_PointDesc.fLifeTime -= TimeDelta;
	m_PointDesc.fSpreadDis -= TimeDelta * 5.f;

	if (0.f >= m_PointDesc.fSpreadDis)
		m_PointDesc.fSpreadDis = 0.f;

	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_int iMyIndex = i - m_iInstance_StartIndex;
		_vector vPos = XMLoadFloat4(&m_pIndexPos[iMyIndex]);
		_vector vDir = XMLoadFloat3(&m_pIndexDir[iMyIndex]);
		vPos -= vDir * m_PointDesc.fSpreadDis * TimeDelta;

		if (true == m_IsDown)
		{
			m_fDownTime += TimeDelta * 2.f;
			vPos.m128_f32[1] -= m_fDownTime;
		}

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
		m_pIndexPos[iMyIndex] = pInstance[i].vPosition;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);

	return _int();
}

_int CPoint_Spread3::Late_Tick(_float TimeDelta)
{
	if (0.f >= m_PointDesc.fSpreadDis)
		return OBJECT_DEAD;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPoint_Spread3::Render()
{
	if (true == m_IsTime)
		m_pBufferInstanceCom->Set_Variable("g_fTime", &m_PointDesc.fSpreadDis, sizeof(_float));


	return __super::Render();
}

void CPoint_Spread3::SetUp_Size(_float2 vSize)
{
	SetUp_Dir();

	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize = vSize;

		// 랜덤 float 동적 배열 생성
	}


	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
}

void CPoint_Spread3::SetUp_Dir()
{
	_int iRandNum_Max = 4;
	m_pIndexDir = new _float3[m_iNumInstance];
	m_pIndexPos = new _float4[m_iNumInstance];
	_float4 vPos;
	XMStoreFloat4(&vPos, m_pMovementCom->Get_State(EState::Position));

	// 15개라 한다면
	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_vector vDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		if (0 < iRandNum_Max)
		{
			for (_int j = 0; j < 3; ++j)
			{
				_int iRand = rand() % iRandNum_Max;
				if (rand() % 2 == 0)
					iRand *= -1;

				vDir.m128_f32[j] = _float(iRand);
			}
		}
		vDir.m128_f32[1] = 1.5f;
		m_pIndexPos[i - m_iInstance_StartIndex] = vPos;
		XMStoreFloat3(&m_pIndexDir[i - m_iInstance_StartIndex], XMVector3Normalize(vDir));
	}

}

CPoint_Spread3 * CPoint_Spread3::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Spread3*		pInstance = new CPoint_Spread3(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Spread3) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Spread3 * CPoint_Spread3::Clone_GameObject(void * pArg)
{
	CPoint_Spread3*		pInstance = new CPoint_Spread3(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Spread3) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Spread3::Free()
{
	__super::Free();
}
