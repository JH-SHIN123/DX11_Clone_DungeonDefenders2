#include "stdafx.h"
#include "..\public\PhaseInfo.h"
#include "Data_Manager.h"

CPhaseInfo::CPhaseInfo(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CPhaseInfo::CPhaseInfo(const CPhaseInfo & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CPhaseInfo::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPhaseInfo::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CPhaseInfo::Tick(_float TimeDelta)
{
	m_fBarAlphaTime += TimeDelta;
	if (m_fBarAlphaTime >= 3.f)
		m_fBarAlphaTime = 0.f;

	Phase_Check(TimeDelta);
	Enemy_Check();

	return _int();
}

_int CPhaseInfo::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CPhaseInfo::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	// PlayerInfo
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);


	Phase_Render();
	Text_Render();

	return S_OK;
}

void CPhaseInfo::Phase_Check(_float TimeDelta)
{
	m_ePhaseState = CData_Manager::GetInstance()->Get_NowPhase();
	
	switch (m_ePhaseState)
	{
	case Client::EPhaseState::Clear:
		Phase_Check_Clear(TimeDelta);
		m_iNowPhase = 0;
		break;
	case Client::EPhaseState::Build:
		Phase_Check_Build(TimeDelta);
		m_iNowPhase = 0;
		break;
	case Client::EPhaseState::Combat:
		Phase_Check_Combat(TimeDelta);
		Enemy_Check();
		m_iNowPhase = 1;
		break;
	case Client::EPhaseState::Boss:
		Phase_Check_Boss(TimeDelta);
		Enemy_Check();
		m_iNowPhase = 1;
		break;
	default:
		break;
	}
	
	_vector vMeterPos = m_pMovementCom_Enemy->Get_State(EState::Position);
}

void CPhaseInfo::Phase_Check_Clear(_float TimeDelta)
{
}

void CPhaseInfo::Phase_Check_Build(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos <= 100.f)
		m_pMovementCom_Enemy->Go_Up(TimeDelta);
}

void CPhaseInfo::Phase_Check_Combat(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos >= 5.f)
		m_pMovementCom_Enemy->Go_Up(-TimeDelta);
}

void CPhaseInfo::Phase_Check_Boss(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos >= 5.f)
		m_pMovementCom_Enemy->Go_Up(-TimeDelta);
}

void CPhaseInfo::Enemy_Check()
{
	if (EPhaseState::Combat != m_ePhaseState &&
		EPhaseState::Boss != m_ePhaseState) return;

	m_iEnemyCount_Max = CData_Manager::GetInstance()->Get_MonsterCount_Max();
	m_iEnemyCount = CData_Manager::GetInstance()->Get_MonsterCount();	

	m_fMeter_Ratio = (1.f - ((_float)m_iEnemyCount / m_iEnemyCount_Max));

	if (true == CData_Manager::GetInstance()->Get_BossPhase())
		m_fMeter_Ratio = (((_float)m_iEnemyCount / m_iEnemyCount_Max));
}

void CPhaseInfo::Phase_Render()
{
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_PhaseInfo->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_PhaseInfo->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// °°Àº À§Ä¡¿¡ »Ç°ñ»Ç°ñÀ» ³Ö°í ¸¶½ºÅ·À¸·Î ÀÚ¸¥´Ù
	_float2 Time = { -m_fBarAlphaTime, m_fBarAlphaTime * 0.3f };
	m_pBufferRectCom->Set_Variable("g_TextureTime_UV", &Time, sizeof(_float2));
	m_pBufferRectCom->Set_Variable("g_Textrue_UV", &m_fMeter_Ratio, sizeof(_float2));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Enemy->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Enemy_MeterTile->Get_ShaderResourceView(0));
	m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom_Enemy_Mask->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(12);
	
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Enemy->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);
}

void CPhaseInfo::Text_Render()
{
	// Phase
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[0]->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// Phase
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[1]->Get_ShaderResourceView(m_iNowPhase));
	m_pBufferRectCom->Render(1);

	// DefenseUnits
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[2]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[2]->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// Score
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[3]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[3]->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

}

HRESULT CPhaseInfo::Ready_Component(void * pArg)
{
	UI2D_DESC* pData = new UI2D_DESC;
	memcpy(pData, pArg, sizeof(UI2D_DESC));

	HRESULT hr = S_OK;

	pData->Movement_Desc.vPos = _float4(540.f, 255.f, 0.f, 1.f);
	pData->Movement_Desc.vScale = _float4(192.f, 192.f, 0.f, 0.f);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_PhaseInfo"), (CComponent**)&m_pMovementCom_PhaseInfo, &pData->Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Phase"), TEXT("Com_Texture_Panel_Phase"), (CComponent**)&m_pTextureCom_PhaseInfo);

	// Enemy Bar
	MOVESTATE_DESC Data;
	Data.fSpeedPerSec = 100.f;
	Data.vScale = _float4(512.f, 64.f, 0.f, 0.f);
	Data.vPos = _float4(m_vKillBar_Pos.x, m_vKillBar_Pos.y, 0.f, 1.f);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Enemy"), (CComponent**)&m_pMovementCom_Enemy, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBar"), TEXT("Com_Texture_Enemy"), (CComponent**)&m_pTextureCom_Enemy);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBarMask"), TEXT("Com_Texture_Enemy_Mask"), (CComponent**)&m_pTextureCom_Enemy_Mask);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_MeterTile"), TEXT("Com_Texture_MeterTile"), (CComponent**)&m_pTextureCom_Enemy_MeterTile);

	// Text Phase
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Wave"), (CComponent**)&m_pMovementCom_Text[0]);
	m_pMovementCom_Text[0]->Set_Scale(XMVectorSet(64.f, 32.f, 0.f, 0.f));
	m_pMovementCom_Text[0]->Set_State(EState::Position, XMVectorSet(540.f, 320.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_PhaseInfo_Text_Wave"), TEXT("Com_Texture_Text_Wave"), (CComponent**)&m_pTextureCom_Text[0]);

	// Text Phase
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Phase"), (CComponent**)&m_pMovementCom_Text[1]);
	m_pMovementCom_Text[1]->Set_Scale(XMVectorSet(96.f, 24.f, 0.f, 0.f)); //256,64
	m_pMovementCom_Text[1]->Set_State(EState::Position, XMVectorSet(540.f, 255.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_PhaseInfo_Text_Phase"), TEXT("Com_Texture_Text_Phase"), (CComponent**)&m_pTextureCom_Text[1]);

	// Text DefenseUnits
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_DefenseUnits"), (CComponent**)&m_pMovementCom_Text[2]);
	m_pMovementCom_Text[2]->Set_Scale(XMVectorSet(160.f, 20.f, 0.f, 0.f)); //512,64 8:1
	m_pMovementCom_Text[2]->Set_State(EState::Position, XMVectorSet(540.f, 205.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_PhaseInfo_Text_DefenseUnits"), TEXT("Com_Texture_Text_DefenseUnits"), (CComponent**)&m_pTextureCom_Text[2]);

	// Text Score
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Score"), (CComponent**)&m_pMovementCom_Text[3]);
	m_pMovementCom_Text[3]->Set_Scale(XMVectorSet(200.f, 25.f, 0.f, 0.f)); //256,32 8:1
	m_pMovementCom_Text[3]->Set_State(EState::Position, XMVectorSet(460.f, -325.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_PhaseInfo_Text_Score"), TEXT("Com_Texture_Text_Score"), (CComponent**)&m_pTextureCom_Text[3]);


	if (hr != S_OK)
		MSG_BOX("Ready_Component Failed (CPhaseInfo)");

	Safe_Delete(pData);
	return S_OK;
}

CPhaseInfo * CPhaseInfo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPhaseInfo* pInstance = new CPhaseInfo(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPhaseInfo) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPhaseInfo::Clone_GameObject(void * pArg)
{
	CPhaseInfo* pInstance = new CPhaseInfo(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CPhaseInfo) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPhaseInfo::Free()
{
	Safe_Release(m_pMovementCom_Enemy);
	Safe_Release(m_pTextureCom_Enemy);
	Safe_Release(m_pTextureCom_Enemy_Mask);
	Safe_Release(m_pTextureCom_Enemy_MeterTile);

	Safe_Release(m_pMovementCom_PhaseInfo);
	Safe_Release(m_pTextureCom_PhaseInfo);


	for (_int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pTextureCom_Text[i]);
		Safe_Release(m_pMovementCom_Text[i]);
	}

	__super::Free();
}
