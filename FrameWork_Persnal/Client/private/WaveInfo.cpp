#include "stdafx.h"
#include "..\public\WaveInfo.h"

CWaveInfo::CWaveInfo(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CWaveInfo::CWaveInfo(const CWaveInfo & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CWaveInfo::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CWaveInfo::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CWaveInfo::Tick(_float TimeDelta)
{
	m_fBarAlphaTime += TimeDelta;
	if (m_fBarAlphaTime >= 3.f)
		m_fBarAlphaTime = 0.f;


	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_eWaveState = EWaveState::Combat;
	}

	if (GetAsyncKeyState('R') & 0x8000)
	{
		m_eWaveState = EWaveState::Build;
	}

	Wave_Check(TimeDelta);


	return _int();
}

_int CWaveInfo::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CWaveInfo::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	// PlayerInfo
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);


	Wave_Render();
	Text_Render();

	return S_OK;
}

void CWaveInfo::Wave_Check(_float TimeDelta)
{
	switch (m_eWaveState)
	{
	case Client::EWaveState::Clear:
		Wave_Check_Clear(TimeDelta);
		break;
	case Client::EWaveState::Build:
		Wave_Check_Build(TimeDelta);
		break;
	case Client::EWaveState::Combat:
		Wave_Check_Combat(TimeDelta);
		Enemy_Check();
		break;
	case Client::EWaveState::Boss:
		Wave_Check_Boss(TimeDelta);
		Enemy_Check();
		break;
	default:
		break;
	}
	_vector vMeterPos = m_pMovementCom_Enemy->Get_State(EState::Position);
}

void CWaveInfo::Wave_Check_Clear(_float TimeDelta)
{
}

void CWaveInfo::Wave_Check_Build(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos <= 100.f)
		m_pMovementCom_Enemy->Go_Up(TimeDelta);
}

void CWaveInfo::Wave_Check_Combat(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos >= 5.f)
		m_pMovementCom_Enemy->Go_Up(-TimeDelta);
}

void CWaveInfo::Wave_Check_Boss(_float TimeDelta)
{
	_float fMeterPos = XMVectorGetY(m_pMovementCom_Enemy->Get_State(EState::Position));
	_float fStopPos = abs(m_vKillBar_Pos.y - fMeterPos);

	if (fStopPos >= 5.f)
		m_pMovementCom_Enemy->Go_Up(-TimeDelta);
}

void CWaveInfo::Enemy_Check()
{
	// 게임인스턴스에서 몬스터 현재개수, 최대 개수 받아옴
	// Hp했던것 처럼 비율 만들면 됨!!!!!!!!@!!!!!@!@!@!@!@!@!@!@!@$@#%ㅋ^7ㅕ

	m_fMeter_Ratio = (1.f - ((_float)m_iEnemyCount / m_iEnemyCount_Max));
}

void CWaveInfo::Wave_Render()
{
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_WaveInfo->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_WaveInfo->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// 같은 위치에 뽀골뽀골을 넣고 마스킹으로 자른다
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

void CWaveInfo::Text_Render()
{
	// Wave
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[0]->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	// Phase
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Text[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Text[1]->Get_ShaderResourceView(1));
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

HRESULT CWaveInfo::Ready_Component(void * pArg)
{
	UI2D_DESC* pData = new UI2D_DESC;
	memcpy(pData, pArg, sizeof(UI2D_DESC));

	HRESULT hr = S_OK;

	pData->Movement_Desc.vPos = _float4(540.f, 255.f, 0.f, 1.f);
	pData->Movement_Desc.vScale = _float4(192.f, 192.f, 0.f, 0.f);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_WaveInfo"), (CComponent**)&m_pMovementCom_WaveInfo, &pData->Movement_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Wave"), TEXT("Com_Texture_Panel_Wave"), (CComponent**)&m_pTextureCom_WaveInfo);

	// Enemy Bar
	MOVESTATE_DESC Data;
	Data.fSpeedPerSec = 100.f;
	Data.vScale = _float4(512.f, 64.f, 0.f, 0.f);
	Data.vPos = _float4(m_vKillBar_Pos.x, m_vKillBar_Pos.y, 0.f, 1.f);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Enemy"), (CComponent**)&m_pMovementCom_Enemy, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBar"), TEXT("Com_Texture_Enemy"), (CComponent**)&m_pTextureCom_Enemy);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBarMask"), TEXT("Com_Texture_Enemy_Mask"), (CComponent**)&m_pTextureCom_Enemy_Mask);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_MeterTile"), TEXT("Com_Texture_MeterTile"), (CComponent**)&m_pTextureCom_Enemy_MeterTile);

	// Text Wave
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Wave"), (CComponent**)&m_pMovementCom_Text[0]);
	m_pMovementCom_Text[0]->Set_Scale(XMVectorSet(64.f, 32.f, 0.f, 0.f));
	m_pMovementCom_Text[0]->Set_State(EState::Position, XMVectorSet(540.f, 320.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Wave"), TEXT("Com_Texture_Text_Wave"), (CComponent**)&m_pTextureCom_Text[0]);

	// Text Phase
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Phase"), (CComponent**)&m_pMovementCom_Text[1]);
	m_pMovementCom_Text[1]->Set_Scale(XMVectorSet(96.f, 24.f, 0.f, 0.f)); //256,64
	m_pMovementCom_Text[1]->Set_State(EState::Position, XMVectorSet(540.f, 255.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Phase"), TEXT("Com_Texture_Text_Phase"), (CComponent**)&m_pTextureCom_Text[1]);

	// Text DefenseUnits
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_DefenseUnits"), (CComponent**)&m_pMovementCom_Text[2]);
	m_pMovementCom_Text[2]->Set_Scale(XMVectorSet(160.f, 20.f, 0.f, 0.f)); //512,64 8:1
	m_pMovementCom_Text[2]->Set_State(EState::Position, XMVectorSet(540.f, 205.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_DefenseUnits"), TEXT("Com_Texture_Text_DefenseUnits"), (CComponent**)&m_pTextureCom_Text[2]);

	// Text Score
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Text_Score"), (CComponent**)&m_pMovementCom_Text[3]);
	m_pMovementCom_Text[3]->Set_Scale(XMVectorSet(200.f, 25.f, 0.f, 0.f)); //256,32 8:1
	m_pMovementCom_Text[3]->Set_State(EState::Position, XMVectorSet(460.f, -325.f, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_WaveInfo_Text_Score"), TEXT("Com_Texture_Text_Score"), (CComponent**)&m_pTextureCom_Text[3]);


	if (hr != S_OK)
		MSG_BOX("Ready_Component Failed (CWaveInfo)");

	Safe_Delete(pData);
	return S_OK;
}

CWaveInfo * CWaveInfo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CWaveInfo* pInstance = new CWaveInfo(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CWaveInfo) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CWaveInfo::Clone_GameObject(void * pArg)
{
	CWaveInfo* pInstance = new CWaveInfo(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CWaveInfo) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CWaveInfo::Free()
{
	Safe_Release(m_pMovementCom_Enemy);
	Safe_Release(m_pTextureCom_Enemy);
	Safe_Release(m_pTextureCom_Enemy_Mask);
	Safe_Release(m_pTextureCom_Enemy_MeterTile);

	Safe_Release(m_pMovementCom_WaveInfo);
	Safe_Release(m_pTextureCom_WaveInfo);


	for (_int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pTextureCom_Text[i]);
		Safe_Release(m_pMovementCom_Text[i]);
	}

	__super::Free();
}
