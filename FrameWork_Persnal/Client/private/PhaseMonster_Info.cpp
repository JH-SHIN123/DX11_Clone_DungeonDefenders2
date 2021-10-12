#include "stdafx.h"
#include "PhaseMonster_Info.h"

CPhaseMonster_Info::CPhaseMonster_Info(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_3D(pDevice, pDevice_Context)
{
}

CPhaseMonster_Info::CPhaseMonster_Info(const CPhaseMonster_Info & rhs)
	: CUI_3D(rhs)
{
}

HRESULT CPhaseMonster_Info::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPhaseMonster_Info::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_int CPhaseMonster_Info::Tick(_float TimeDelta)
{
	return _int();
}

_int CPhaseMonster_Info::Late_Tick(_float TimeDelta)
{
	__super::BillBoarding();


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority_Second, this);
}

HRESULT CPhaseMonster_Info::Render()
{
	if (nullptr == m_pBufferRectCom)
		return E_FAIL;

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(m_iSpawnMonster_Value));
	m_pBufferRectCom->Render(20);

	//for (_int i = 0; i < m_iSpawnMonster_Value; ++i)
	//{
	//	_vector vPos = XMLoadFloat3(&m_vPos[i]);
	//	vPos = XMVectorSetW(vPos, 1.f);
	//	_vector vRight = XMVector3Normalize(m_pMovementCom->Get_State(EState::Right)) * m_vScale[i].x;
	//	_vector vUp = XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * m_vScale[i].y;
	//	_vector vLook = XMVector3Normalize(m_pMovementCom->Get_State(EState::Look)) * m_vScale[i].z;
	//
	//	_matrix WorldMatrix = XMMatrixIdentity();
	//	WorldMatrix.r[0] = vRight;
	//	WorldMatrix.r[1] = vUp;
	//	WorldMatrix.r[2] = vLook;
	//	WorldMatrix.r[3] = vPos;
	//
	//	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	//	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(m_iMonsterIcon[i]));
	//	m_pBufferRectCom->Render(1);
	//}
	return S_OK;
}

void CPhaseMonster_Info::Set_Pos(_fvector vPos)
{
	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CPhaseMonster_Info::Set_PhaseInfo(const PHASEINFO_DESC & PhaseInfo_Desc)
{
	memcpy(&m_Monster_Info, &PhaseInfo_Desc, sizeof(PHASEINFO_DESC));

	m_iSpawnMonster_Value = 0;
	for (_int i = 0; i < (_int)EMonster_List::End; ++i)
	{
		if (true == PhaseInfo_Desc.IsAddMonster[i])
			++m_iSpawnMonster_Value;

		m_iMonsterIcon[i] = 0;
	}
}

void CPhaseMonster_Info::Monster_Check()
{
	_int iMonster = 0;
	for (_int i = 0; i < m_iSpawnMonster_Value; ++i)
	{
		if (true == m_Monster_Info.IsAddMonster[iMonster])
		{
			m_iMonsterIcon[iMonster] = iMonster;
		}
		else
			--i;
		
		++iMonster;
	}
}

CPhaseMonster_Info * CPhaseMonster_Info::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void * pArg)
{
	CPhaseMonster_Info* pInstance = new CPhaseMonster_Info(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CMasking_MeterBar_3D) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPhaseMonster_Info::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CPhaseMonster_Info::Free()
{
	__super::Free();
}
