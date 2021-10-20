#include "stdafx.h"
#include "..\public\Point_Spread.h"
#include "EffectDesc_Manager.h"

CPoint_Spread::CPoint_Spread(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CPoint_Spread::CPoint_Spread(const CPoint_Spread & rhs)
	: CGameObject(rhs)
{
}

HRESULT CPoint_Spread::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPoint_Spread::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	SetUp_IndexDir(CEffectDesc_Manager::GetInstance()->Get_PointSpread_DrawCount());
	
	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();

	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	_float fSize = 5.f;

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		pInstance[i].vSize.x = fSize;
		pInstance[i].vSize.y = fSize;

		fSize -= 1.5f;
	}

	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);

	return S_OK;
}

_int CPoint_Spread::Tick(_float TimeDelta)
{
	m_PointDesc.fLifeTime -= TimeDelta;

	VTXMATRIX* pInstance = m_pBufferInstanceCom->Get_InstanceBuffer();


	XMStoreFloat4(&pInstance[m_iInstance_StartIndex].vPosition, m_pMovementCom->Get_State(EState::Position));

	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_vector vPos = m_pMovementCom->Get_State(EState::Position);
		_vector vDir = XMLoadFloat3(&m_pIndexDir[i - m_iInstance_StartIndex]);
		vPos -= vDir * 1.5f;

		XMStoreFloat4(&pInstance[i].vPosition, vPos);
	}



	m_pBufferInstanceCom->Set_InstanceBuffer(pInstance);
	m_pBufferInstanceCom->Update_Instance(TimeDelta);
	return _int();
}

_int CPoint_Spread::Late_Tick(_float TimeDelta)
{
	if (0 >= m_PointDesc.fLifeTime)
		return OBJECT_DEAD;


	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Alpha, this);
}

HRESULT CPoint_Spread::Render()
{
	if (nullptr == m_pBufferInstanceCom)
		return S_OK;

	m_pBufferInstanceCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(0));

	_vector vCamPosition = GET_CAMERA_POSITION;
	m_pBufferInstanceCom->Set_Variable("g_vCamPosition", &XMVectorSet(vCamPosition.m128_f32[0], vCamPosition.m128_f32[1], vCamPosition.m128_f32[2], vCamPosition.m128_f32[3]), sizeof(_vector));
	m_pBufferInstanceCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferInstanceCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	m_pBufferInstanceCom->Render(0, m_iNumInstance, m_iInstance_StartIndex);

	return S_OK;
}

HRESULT CPoint_Spread::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	memcpy(&m_PointDesc, pArg, sizeof(POINT_SPREAD_DESC));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_PointDesc.MoveDesc);

	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_PointDesc.szTextrueName, TEXT("Com_Texture"), (CComponent**)&m_pTexturesCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, m_PointDesc.szPointInstance_PrototypeName, TEXT("Com_Buffer"), (CComponent**)&m_pBufferInstanceCom);

	if (hr != S_OK)
		MSG_BOX("CPoint_Spread::Ready_Component Failed");

	return hr;
}

void CPoint_Spread::Set_Pos(_fvector vPos)
{
	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CPoint_Spread::SetUp_IndexDir(_int iInstanceIndex)
{
	m_iInstance_StartIndex = CEffectDesc_Manager::GetInstance()->Get_PointSpread_StartIndex();
	m_iNumInstance = iInstanceIndex;

	m_pIndexDir = new _float3[m_iNumInstance];

	// 15개라 한다면
	for (_int i = m_iInstance_StartIndex; i < m_iNumInstance + m_iInstance_StartIndex; ++i)
	{
		_vector vDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);

		for (_int j = 0; j < 3; ++j)
		{
			_int iRand = rand() % 10;
			if (rand() % 2 == 0)
				iRand *= -1;

			vDir.m128_f32[j] = _float(iRand);
		}

		XMStoreFloat3(&m_pIndexDir[i - m_iInstance_StartIndex], XMVector3Normalize(vDir));
	}

}

void CPoint_Spread::Check_Instance(_float TimeDelta)
{
	// 0번을 주위로 퍼져나감



}

CPoint_Spread * CPoint_Spread::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPoint_Spread*		pInstance = new CPoint_Spread(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPoint_Spread) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CPoint_Spread * CPoint_Spread::Clone_GameObject(void * pArg)
{
	CPoint_Spread*		pInstance = new CPoint_Spread(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CPoint_Spread) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPoint_Spread::Free()
{
	__super::Free();

	Safe_Delete_Array(m_pIndexDir);

	Safe_Release(m_pBufferInstanceCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pMovementCom);
}
