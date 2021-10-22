#include "stdafx.h"
#include "..\public\Point_Ex_Trail.h"

CPoint_Ex_Trail::CPoint_Ex_Trail(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPoint_Ex_Trail::CPoint_Ex_Trail(const CPoint_Ex_Trail & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPoint_Ex_Trail::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Ex_Trail::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	_float fSize = m_PointDesc.fSize;

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize.x = fSize;
		pInstance[i].vSize.y = fSize;

		//fSize -= 0.5f;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);

	return S_OK;
}

_int CPoint_Ex_Trail::Tick(_float TimeDelta)
{
	m_PointDesc.fLifeTime -= TimeDelta;

	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_int iMyIndex = i - m_iInstance_StartIndex;
		_vector vPos = m_pMovementCom->Get_State(EState::Position);

		m_pTimeBuffer[iMyIndex] -= TimeDelta;
		if (0.f >= m_pTimeBuffer[iMyIndex])
		{
			//m_pTimeBuffer[iMyIndex] = m_PointDesc.fTimeTerm;
			vPos = XMLoadFloat4(&pInstance[i].vPosition);//
			pInstance[i].fTime -= TimeDelta * m_PointDesc.fAlphaTimePower;

			if (0.f >= pInstance[i].fTime)
				pInstance[i].fTime = 0.f;
		}

		_vector vDir = XMLoadFloat3(&m_pIndexDir[iMyIndex]);
		vPos -= vDir * m_PointDesc.fSpreadDis * TimeDelta;

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);
	return _int();
}

_int CPoint_Ex_Trail::Late_Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return OBJECT_DEAD;


	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CPoint_Ex_Trail::Render()
{
	if (nullptr == m_pBufferInstanceCom)
		return S_OK;

	if (true == m_IsColor)
		m_pBufferInstanceCom->Set_Variable("g_vColor", &m_vColor, sizeof(_float4));


	m_pBufferInstanceCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(0));

	_vector vCamPosition = GET_CAMERA_POSITION;
	m_pBufferInstanceCom->Set_Variable("g_vCamPosition", &XMVectorSet(vCamPosition.m128_f32[0], vCamPosition.m128_f32[1], vCamPosition.m128_f32[2], vCamPosition.m128_f32[3]), sizeof(_vector));
	m_pBufferInstanceCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	m_pBufferInstanceCom->Render(m_PointDesc.iShaderPass, m_iNumInstance, m_iInstance_StartIndex);

	return S_OK;
}

void CPoint_Ex_Trail::Move_Spread(_float TimeDelta)
{
	m_PointDesc.fLifeTime -= TimeDelta;

	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_int iMyIndex = i - m_iInstance_StartIndex;
		_vector vPos = XMLoadFloat4(&pInstance[i].vPosition);//m_pMovementCom->Get_State(EState::Position);

		m_pTimeBuffer[iMyIndex] -= TimeDelta;
		if (0.f >= m_pTimeBuffer[iMyIndex])
		{
			m_pTimeBuffer[iMyIndex] = m_PointDesc.fTimeTerm;
			vPos = m_pMovementCom->Get_State(EState::Position);
		}

		_vector vDir = XMLoadFloat3(&m_pIndexDir[iMyIndex]);
		vPos -= vDir * 1.5f * TimeDelta;

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);
}

VTXMATRIX_EXTEND* CPoint_Ex_Trail::Get_InstanceBuffer()
{
	return m_pBufferInstanceCom->Get_InstanceBuffer();
}

void CPoint_Ex_Trail::Set_InstanceBuffer(VTXMATRIX_EXTEND* pInstanceBuffer)
{
	m_pBufferInstanceCom->Set_InstanceBuffer(pInstanceBuffer);
}

HRESULT CPoint_Ex_Trail::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	memcpy(&m_PointDesc, pArg, sizeof(POINT_TRAIL_EX_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_PointDesc.MoveDesc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_PointDesc.szTextrueName, TEXT("Com_Texture"), (CComponent**)&m_pTexturesCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_PointDesc.szPointInstance_PrototypeName, TEXT("Com_Buffer"), (CComponent**)&m_pBufferInstanceCom);

	m_iInstance_StartIndex = CEffectDesc_Manager::GetInstance()->Get_Instance_StartIndex(m_PointDesc.InstanceValue);
	m_iNumInstance = CEffectDesc_Manager::GetInstance()->Get_Instance_DrawCount(m_PointDesc.InstanceValue);


	if (hr != S_OK)
		MSG_BOX("CPoint_Ex_Trail::Ready_Component Failed");

	return hr;
}

void CPoint_Ex_Trail::Set_Pos(_fvector vPos)
{
	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CPoint_Ex_Trail::SetUp_IndexDir(_int iRandNum_Max)
{
	m_pIndexDir = new _float3[m_iNumInstance];
	m_pIndexPos = new _float4[m_iNumInstance];
	m_pTimeBuffer = new _float[m_iNumInstance];

	_float4 vPos;
	XMStoreFloat4(&vPos, m_pMovementCom->Get_State(EState::Position));

	VTXMATRIX_EXTEND* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();


	// 15개라 한다면
	_float fTime = 0.f;
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

		m_pTimeBuffer[i - m_iInstance_StartIndex] = fTime;
		m_pIndexPos[i - m_iInstance_StartIndex] = vPos;
		XMStoreFloat3(&m_pIndexDir[i - m_iInstance_StartIndex], XMVector3Normalize(vDir));
		pInstance[i].vPosition = vPos;
		pInstance[i].fTime = m_PointDesc.fAlphaTime;
		fTime += m_PointDesc.fTimeTerm;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(0.f);

}

CPoint_Ex_Trail * CPoint_Ex_Trail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Ex_Trail*		pInstance = new CPoint_Ex_Trail(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Ex_Trail) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Ex_Trail * CPoint_Ex_Trail::Clone_GameObject(void * pArg)
{
	CPoint_Ex_Trail*		pInstance = new CPoint_Ex_Trail(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Ex_Trail) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Ex_Trail::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pIndexDir);
	Safe_Delete_Array(m_pIndexPos);
	Safe_Delete_Array(m_pTimeBuffer);

	Safe_Release(m_pBufferInstanceCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pMovementCom);
}
