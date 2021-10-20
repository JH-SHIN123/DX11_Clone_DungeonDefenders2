#include "stdafx.h"
#include "..\public\Point_Spread2.h"
#include "EffectDesc_Manager.h"

CPoint_Spread2::CPoint_Spread2(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CPoint_Spread(pDevice, pDevice_Context)
{
}

CPoint_Spread2::CPoint_Spread2(const CPoint_Spread2 & rhs)
	: CPoint_Spread(rhs)
{
}

HRESULT CPoint_Spread2::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Spread2::NativeConstruct(void * pArg)
{
	POINT_SPREAD_DESC_2 Data;

	memcpy(&Data, pArg, sizeof(POINT_SPREAD_DESC_2));

	__super::Ready_Component(&Data.Point_Desc);

	SetUp_Size(Data.vSize);



	return S_OK;
}

_int CPoint_Spread2::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CPoint_Spread2::Late_Tick(_float TimeDelta)
{

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPoint_Spread2::Render()
{



	return __super::Render();
}

void CPoint_Spread2::SetUp_Size(_float2 vSize)
{
	SetUp_IndexDir(CEffectDesc_Manager::GetInstance()->Get_PointSpread_DrawCount());

	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize = vSize;

		// 랜덤 float 동적 배열 생성
	}


	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
}

CPoint_Spread2 * CPoint_Spread2::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Spread2*		pInstance = new CPoint_Spread2(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Spread2) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Spread2 * CPoint_Spread2::Clone_GameObject(void * pArg)
{
	CPoint_Spread2*		pInstance = new CPoint_Spread2(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Spread2) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Spread2::Free()
{
	__super::Free();
}
