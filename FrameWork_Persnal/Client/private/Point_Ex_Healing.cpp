#include "stdafx.h"
#include "..\public\Point_Ex_Healing.h"

CPoint_Ex_Healing::CPoint_Ex_Healing(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CPoint_Ex_Trail(pDevice, pDevice_Context)
{
}

CPoint_Ex_Healing::CPoint_Ex_Healing(const CPoint_Ex_Healing & rhs)
	: CPoint_Ex_Trail(rhs)
{
}

HRESULT CPoint_Ex_Healing::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Ex_Healing::NativeConstruct(void * pArg)
{
	POINT_EX_HEAL_DESC Data;
	memcpy(&Data, pArg, sizeof(POINT_EX_HEAL_DESC));
	lstrcpy(m_szBuffTarget, Data.szBuffTarget);

	__super::Ready_Component(&Data.Point_Desc);

	SetUp_Dir_Up(m_PointDesc.iRandDir);


	return S_OK;
}

_int CPoint_Ex_Healing::Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return 0;

	CGameObject* pTarget = GET_GAMEINSTANCE->Get_GameObject((_uint)ELevel::Stage1, m_szBuffTarget);
	if (nullptr == pTarget)
		return 0;

	_vector vPos = static_cast<CMovement*>(pTarget->Get_Component(L"Com_Movement"))->Get_State(EState::Position);
	vPos.m128_f32[1] += 5.f;

	m_pMovementCom->Set_State(EState::Position, vPos);

	m_PointDesc.fLifeTime -= TimeDelta;


	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < (m_iNumInstance + m_iInstance_StartIndex); ++i)
	{
		_int iMyIndex = i - m_iInstance_StartIndex;
		_vector vPos = XMLoadFloat4(&pInstance[i].vPosition);

		if (0.f >= m_pTimeBuffer[iMyIndex])
		{
			pInstance[i].fTime -= TimeDelta * m_PointDesc.fAlphaTimePower;

			if (0.f >= pInstance[i].fTime)
				pInstance[i].fTime = 0.f;
		}

		m_pTimeBuffer[iMyIndex] -= TimeDelta;

		vPos.m128_f32[1] += m_PointDesc.fScalePower * TimeDelta;

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
	}
	m_fTime -= TimeDelta * m_PointDesc.fAlphaTimePower;
	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);

	if (0 >= m_fTime)
		m_fTime = 0.f;

	return _int();
}

_int CPoint_Ex_Healing::Late_Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return 0;

	return __super::Late_Tick(TimeDelta);
}

HRESULT CPoint_Ex_Healing::Render()
{
	m_pBufferInstanceCom->Set_Variable("g_fTime", &m_fTime, sizeof(_float));

	m_pBufferInstanceCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(0));

	_vector vCamPosition = GET_CAMERA_POSITION;
	m_pBufferInstanceCom->Set_Variable("g_vCamPosition", &XMVectorSet(vCamPosition.m128_f32[0], vCamPosition.m128_f32[1], vCamPosition.m128_f32[2], vCamPosition.m128_f32[3]), sizeof(_vector));
	m_pBufferInstanceCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	m_pBufferInstanceCom->Render(m_PointDesc.iShaderPass, m_iNumInstance, m_iInstance_StartIndex);



	return S_OK;
}

void CPoint_Ex_Healing::SetUp_Dir_Up(_int iRandNum_Max)
{
	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();
	if (nullptr == pInstance)
		return;

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	m_pIndexDir = new _float3[m_iNumInstance];
	m_pIndexPos = new _float4[m_iNumInstance];
	m_pTimeBuffer = new _float[m_iNumInstance];
	m_fTime = 1.f;

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pMovementCom->Get_State(EState::Position));

	_float fSize = m_PointDesc.fSize;
	_float fTime = 0.f;

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_vector vDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
		_vector vPosition = m_pMovementCom->Get_State(EState::Position);

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

		vPosition += XMVector3Normalize(vDir) * m_PointDesc.fSpreadDis;
		XMStoreFloat4(&vPos, vPosition);
		m_pTimeBuffer[i - m_iInstance_StartIndex] = fTime;
		m_pIndexPos[i - m_iInstance_StartIndex] = vPos;
		pInstance[i].vPosition = vPos;
		pInstance[i].fTime = m_PointDesc.fAlphaTime;
		pInstance[i].vSize.x = m_PointDesc.fSize;
		pInstance[i].vSize.y = m_PointDesc.fSize;

		fTime += m_PointDesc.fTimeTerm;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);

	m_pBufferInstanceCom->Update_Instance(0.f); // 일단 한번 세팅

}

CPoint_Ex_Healing * CPoint_Ex_Healing::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Ex_Healing*		pInstance = new CPoint_Ex_Healing(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Ex_Healing) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Ex_Healing * CPoint_Ex_Healing::Clone_GameObject(void * pArg)
{
	CPoint_Ex_Healing*		pInstance = new CPoint_Ex_Healing(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Ex_Healing) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Ex_Healing::Free()
{
	__super::Free();
}
