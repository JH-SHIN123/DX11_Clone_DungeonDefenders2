#include "stdafx.h"
#include "..\public\Point_Ex_BuffAura.h"

CPoint_Ex_BuffAura::CPoint_Ex_BuffAura(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CPoint_Ex_Trail(pDevice, pDevice_Context)
{
}

CPoint_Ex_BuffAura::CPoint_Ex_BuffAura(const CPoint_Ex_BuffAura & rhs)
	: CPoint_Ex_Trail(rhs)
{
}

HRESULT CPoint_Ex_BuffAura::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Ex_BuffAura::NativeConstruct(void * pArg)
{
	POINT_EX_BUFF_DESC Data;
	memcpy(&Data, pArg, sizeof(POINT_EX_BUFF_DESC));
	lstrcpy(m_szBuffTarget, Data.szBuffTarget);

	__super::Ready_Component(&Data.Point_Desc);

	SetUp_Dir_Up(m_PointDesc.iRandDir);


	return S_OK;
}

_int CPoint_Ex_BuffAura::Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return 0;

	CGameObject* pTarget = GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, m_szBuffTarget);
	if (nullptr == pTarget)
		return 0;

	_vector vPos = static_cast<CMovement*>(pTarget->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
	//vPos.m128_f32[1] -= m_PointDesc.fSize;// *0.5f;

	m_pMovementCom->Set_State(EState::Position, vPos);

	m_PointDesc.fLifeTime -= TimeDelta;
	m_PointDesc.fSpreadDis -= TimeDelta;
	m_fTime += TimeDelta * 2.f;

	if (1.f >= m_PointDesc.fSpreadDis)
		m_PointDesc.fSpreadDis = 1.f;

	if (1.5f >= m_fTime)
		m_fTime = 1.5f;

	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_int iMyIndex = i - m_iInstance_StartIndex;
		_vector vPos = m_pMovementCom->Get_State(EState::Position);

		if (0.f >= m_pTimeBuffer[iMyIndex])
		{
			//m_pTimeBuffer[iMyIndex] = m_PointDesc.fTimeTerm;
			vPos = XMLoadFloat4(&pInstance[i].vPosition);//
			pInstance[i].fTime -= TimeDelta * m_PointDesc.fAlphaTimePower;

			if (0 < pInstance[i].fTime)
			{
				_float2 vSize;
				//m_pTimeBuffer[iMyIndex] += TimeDelta;
				vSize.x = (m_PointDesc.fSize - pInstance[i].vSize.x) * 0.25f;
				vSize.y = (m_PointDesc.fSize - pInstance[i].vSize.y) * 0.25f;

				pInstance[i].vSize.x += vSize.x;
				pInstance[i].vSize.y += vSize.y;

				_float fCheck = fabs(m_PointDesc.fSize - pInstance[i].vSize.x);

				if (fCheck < 0.5f)
				{
					pInstance[i].vSize.x = m_PointDesc.fSize;
					pInstance[i].vSize.y = m_PointDesc.fSize;
				}

			}
			else
			{
				pInstance[i].vSize.x -= TimeDelta* m_PointDesc.fScalePower;
				pInstance[i].vSize.y -= TimeDelta* m_PointDesc.fScalePower;
			}

			if (0 >= pInstance[i].vSize.x)
			{
				pInstance[i].vSize.x = 0.f;
				pInstance[i].vSize.y = 0.f;
			}

			if (0.f >= pInstance[i].fTime)
				pInstance[i].fTime = 0.f;
		}

		m_pTimeBuffer[iMyIndex] -= TimeDelta;
		//fAnyTime

		_vector vDir = XMLoadFloat3(&m_pIndexDir[iMyIndex]);
		vPos -= vDir * m_PointDesc.fSpreadDis * TimeDelta;
		vPos.m128_f32[1] += m_fTime * TimeDelta;
		// 아우라 처럼 묘한 곡선 ()

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);
	return _int();
}

_int CPoint_Ex_BuffAura::Late_Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return 0;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPoint_Ex_BuffAura::Render()
{
	__super::Render();

	return S_OK;
}

void CPoint_Ex_BuffAura::SetUp_Dir_Up(_int iRandNum_Max)
{
	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();
	if (nullptr == pInstance)
		return;

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	m_pIndexDir		= new _float3[m_iNumInstance];
	m_pIndexPos		= new _float4[m_iNumInstance];
	m_pTimeBuffer	= new _float[m_iNumInstance];

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pMovementCom->Get_State(EState::Position));

	_float fSize = m_PointDesc.fSize;
	_float fTime = 0.f;

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_vector vDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		if (0 < iRandNum_Max)
		{
			for (_int j = 0; j < 3; ++j)
			{
				_int iRand = rand() % iRandNum_Max;
				if (rand() % 2 == 0 && 1 != j)
					iRand *= -1;

				vDir.m128_f32[j] = _float(iRand);
			}
		}
		vDir.m128_f32[1] = 0.f;

		m_pTimeBuffer[i - m_iInstance_StartIndex] = fTime;
		m_pIndexPos[i - m_iInstance_StartIndex] = vPos;
		pInstance[i].vPosition = vPos;
		pInstance[i].fTime = m_PointDesc.fAlphaTime;
		pInstance[i].vSize.x = 0.f;// m_PointDesc.fSize;
		pInstance[i].vSize.y = 0.f;// m_PointDesc.fSize;
		XMStoreFloat3(&m_pIndexDir[i - m_iInstance_StartIndex], XMVector3Normalize(vDir));

		fTime += m_PointDesc.fTimeTerm;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);

	m_pBufferInstanceCom->Update_Instance(0.f); // 일단 한번 세팅

}

CPoint_Ex_BuffAura * CPoint_Ex_BuffAura::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Ex_BuffAura*		pInstance = new CPoint_Ex_BuffAura(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Ex_BuffAura) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Ex_BuffAura * CPoint_Ex_BuffAura::Clone_GameObject(void * pArg)
{
	CPoint_Ex_BuffAura*		pInstance = new CPoint_Ex_BuffAura(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Ex_BuffAura) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Ex_BuffAura::Free()
{
	__super::Free();
}
