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
	if (m_eWaveState == EWaveState::Combat ||
		m_eWaveState == EWaveState::Boss)
		m_fBarAlphaTime += 0.03f;

	if (m_fBarAlphaTime >= 1.f)
		m_fBarAlphaTime = 1.f;

	if (m_eWaveState == EWaveState::Build)
	{
		if(m_fBarAlphaTime >= 0.f)
		m_fBarAlphaTime -= 0.03f;
	}

	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_eWaveState = EWaveState::Combat;
	}

	if (GetAsyncKeyState('R') & 0x8000)
	{
		m_eWaveState = EWaveState::Build;
	}


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

void CWaveInfo::Wave_Check()
{
}

void CWaveInfo::Wave_Render()
{
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_WaveInfo->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_WaveInfo->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	m_pBufferRectCom->Set_Variable("g_AlphaTime", &m_fBarAlphaTime, sizeof(_float));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Enemy->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Enemy->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(6);
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

	pData->vPos = _float2(540.f, 255.f);
	pData->vScale = _float2(192.f, 192.f);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_WaveInfo"), (CComponent**)&m_pMovementCom_WaveInfo);
	m_pMovementCom_WaveInfo->Set_Scale(XMVectorSet(pData->vScale.x, pData->vScale.y, 0.f, 0.f));
	m_pMovementCom_WaveInfo->Set_State(EState::Position, XMVectorSet(pData->vPos.x, pData->vPos.y, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Panel_Wave"), TEXT("Com_Texture_Panel_Wave"), (CComponent**)&m_pTextureCom_WaveInfo);

	// Enemy Bar
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Enemy"), (CComponent**)&m_pMovementCom_Enemy);
	m_pMovementCom_Enemy->Set_Scale(XMVectorSet(512.f, 64.f, 0.f, 0.f));
	m_pMovementCom_Enemy->Set_State(EState::Position, XMVectorSet(m_vKillBar_Pos.x, m_vKillBar_Pos.y, 0.f, 1.f));
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_EnemyKillBar"), TEXT("Com_Texture_Enemy"), (CComponent**)&m_pTextureCom_Enemy);

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

	Safe_Release(m_pMovementCom_Enemy_Mask);
	Safe_Release(m_pTextureCom_Enemy_Mask);

	Safe_Release(m_pMovementCom_WaveInfo);
	Safe_Release(m_pTextureCom_WaveInfo);


	for (_int i = 0; i < 5; ++i)
	{
		Safe_Release(m_pTextureCom_Text[i]);
		Safe_Release(m_pMovementCom_Text[i]);
	}

	__super::Free();
}
