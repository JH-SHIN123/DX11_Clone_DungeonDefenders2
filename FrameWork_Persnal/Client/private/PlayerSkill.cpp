#include "stdafx.h"
#include "..\public\PlayerSkill.h"

CPlayerSkill::CPlayerSkill(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CPlayerSkill::CPlayerSkill(const CPlayerSkill & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CPlayerSkill::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CPlayerSkill::NativeConstruct(void * pArg)
{
	if (FAILED(__super::NativeConstruct(pArg)))
		return E_FAIL;

	Ready_Component(pArg);

	return S_OK;
}

_int CPlayerSkill::Tick(_float TimeDelta)
{
	// 플레이어 받아와서 무언가를 하자

	m_fCastTime += TimeDelta;

	if (m_fCastTime >= 1.5f)
	{
		m_fCastTime = 0.f;
	}

	CoolDown_Check(TimeDelta);

	return _int();
}

_int CPlayerSkill::Late_Tick(_float TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return UPDATE_ERROR;


	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CPlayerSkill::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	// 캐스팅 바
	m_pBufferRectCom->Set_Variable("g_AlphaTime", &m_fCastTime, sizeof(_float));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom_Cast->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Cast->Get_ShaderResourceView(0));
	//m_pVIBufferCom->Set_ShaderResourceView("g_MaskTexture", nullptr);
	m_pBufferRectCom->Render(5);


	Render_SkillIcon();

	return S_OK;
}

_bool CPlayerSkill::Set_Skill_CoolDown(_uint iIndex, _float fCoolDownTime)
{
	if (false == m_IsCoolDown[iIndex])
	{
		m_IsCoolDown[iIndex] = true;
		m_fCoolDownTime[iIndex] = 0.f;
		m_fCoolDownTimeMax[iIndex] = fCoolDownTime * 2.f;
		return true;
	}

	else
		return false;
}

void CPlayerSkill::CoolDown_Check(_float TimeDelta)
{
	for (_int i = 0; i < m_iSkillNum; ++i)
	{
		if (true == m_IsCoolDown[i])
		{
			m_fCoolDownTime[i] += TimeDelta;

			if (m_fCoolDownTime[i] >= m_fCoolDownTimeMax[i] * 0.5f)
			{
				m_IsCoolDown[i] = false;
				m_fCoolDownTime[i] = 0.f;
				m_fCoolDownTimeMax[i] = 0.f;
			}
		}
	}
}

void CPlayerSkill::Phase_Check()
{
}

HRESULT CPlayerSkill::Ready_Component(void* pArg)
{
	UI2D_DESC* pData = new UI2D_DESC;
	memcpy(pData, pArg, sizeof(UI2D_DESC));

	HRESULT hr = S_OK;

	// Skill Icon
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_0"), (CComponent**)&m_pMovementCom_Skill[0]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_1"), (CComponent**)&m_pMovementCom_Skill[1]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_2"), (CComponent**)&m_pMovementCom_Skill[2]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_3"), (CComponent**)&m_pMovementCom_Skill[3]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_4"), (CComponent**)&m_pMovementCom_Skill[4]);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_5"), (CComponent**)&m_pMovementCom_Skill[5]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_6"), (CComponent**)&m_pMovementCom_Skill[6]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_7"), (CComponent**)&m_pMovementCom_Skill[7]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_8"), (CComponent**)&m_pMovementCom_Skill[8]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Skill_9"), (CComponent**)&m_pMovementCom_Skill[9]);

	_vector vPos = XMVectorSet(pData->Movement_Desc.vPos.x, pData->Movement_Desc.vPos.y, 0.f, 1.f);
	_vector vIntarval = XMVectorSet(67.3f, 0.f, 0.f, 0.f);

	for (_int i = 0; i < m_iSkillNum; ++i)
	{
		if (5 == i)
			vPos = XMVectorSet(pData->Movement_Desc.vPos.x, pData->Movement_Desc.vPos.y - 67.f, 0.f, 1.f);

		m_pMovementCom_Skill[i]->Set_Scale(XMVectorSet(pData->Movement_Desc.vScale.x, pData->Movement_Desc.vScale.y, 0.f, 0.f));
		m_pMovementCom_Skill[i]->Set_State(EState::Position, vPos);
		vPos += vIntarval;
	}

	if (hr != S_OK)
		MSG_BOX("Ready_Component Failed (CPlayerSkill)");

	// Skill Cast
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Texture_Panel_newBar"), TEXT("Com_Texture_Panel_newBar"), (CComponent**)&m_pTextureCom_Cast)))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Texture_newBarMask"), TEXT("Com_Texture_newBarMask"), (CComponent**)&m_pTextureCom_Cast_Mask)))
		return E_FAIL;
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Transform"), TEXT("Com_Transform_Cast"), (CComponent**)&m_pTransformCom_Cast)))
		return E_FAIL;

	m_pTransformCom_Cast->Set_Scale(XMVectorSet(256.f, 64.f, 0.f, 0.f));
	m_pTransformCom_Cast->Set_State(EState::Position, XMVectorSet(-40.f, -245.f, 0.f, 1.f));

	// CoolDown
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static
		, TEXT("Component_Texture_CoolDown"), TEXT("Com_Texture_CoolDown"), (CComponent**)&m_pTextureCom_CoolDown)))
		return E_FAIL;

	Safe_Delete(pData);
	return hr;
}

HRESULT CPlayerSkill::Render_SkillIcon()
{
	_int PassNumber = 1;

	m_pBufferRectCom->Set_ShaderResourceView("g_MaskTexture", m_pTextureCom_CoolDown->Get_ShaderResourceView(0));

	for (_int i = 0; i < m_iSkillNum; ++i)
	{
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Skill[i]->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(i));

		if (true == m_IsCoolDown[i])
		{
			_float TimeDelta = m_fCoolDownTime[i] / m_fCoolDownTimeMax[i] + 0.5f;
			m_pBufferRectCom->Set_Variable("g_AlphaTime", &TimeDelta, sizeof(_float));
			PassNumber = 7;
		}

		m_pBufferRectCom->Render(PassNumber);
		PassNumber = 1;
	}

	return S_OK;
}

CPlayerSkill * CPlayerSkill::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CPlayerSkill* pInstance = new CPlayerSkill(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CPlayerSkill) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPlayerSkill::Clone_GameObject(void * pArg)
{
	CPlayerSkill* pClone = new CPlayerSkill(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CGameObject) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CPlayerSkill::Free()
{
	for (_int i = 0; i < m_iSkillNum; ++i)
	{
		Safe_Release(m_pMovementCom_Skill[i]);
	}

	Safe_Release(m_pTextureCom_Cast);
	Safe_Release(m_pTextureCom_CoolDown);
	Safe_Release(m_pTextureCom_Cast_Mask);
	Safe_Release(m_pTransformCom_Cast);

	__super::Free();
}
