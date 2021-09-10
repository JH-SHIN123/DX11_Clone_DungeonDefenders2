#include "..\public\Status.h"

CStatus::CStatus(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CComponent(pDevice, pDevice_Context)
{
}

CStatus::CStatus(const CStatus & rhs)
	: CComponent(rhs)
	, m_Status_Desc(rhs.m_Status_Desc)
{
}

HRESULT CStatus::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	ZeroMemory(&m_Status_Desc, sizeof(STATUS_DESC));

	return S_OK;
}

HRESULT CStatus::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	return S_OK;
}

_uint CStatus::Tick(_float TimeDelta)
{
	Damage_Check(TimeDelta);

	return _uint();
}

void CStatus::Level_Check()
{
	// 레벨업은 전투 끝나야지만 함

	if (m_Status_Desc.iExp >= m_Status_Desc.iExp_Max)
	{
		++m_Status_Desc.iLevel;
		m_Status_Desc.iAtt_Basic	= (_int)(m_Status_Desc.iAtt_Basic * 1.15f);
		m_Status_Desc.iHp_Max		= (_int)(m_Status_Desc.iHp_Max * 1.3f);
		m_Status_Desc.iHp			= m_Status_Desc.iHp_Max;
		m_Status_Desc.iMp_Max		+= m_Status_Desc.iMp_Max >> 1;

		m_Status_Desc.iExp			-= m_Status_Desc.iExp_Max;
		m_Status_Desc.iExp_Max		= (m_Status_Desc.iExp_Max * 1.2f);
	}
}

void CStatus::Set_Damage(const HIT_DESC & pDamage)
{
	m_IsHit = true;
	m_Hit_Desc = pDamage;
}

void CStatus::Damage_Check(_float TimeDelta)
{
	if (true == m_IsHit)
	{
		switch (m_Hit_Desc.eDamageType)
		{
		case EDamageType::Direct:

			break;
		case EDamageType::Dot:
			m_IsHit = false;
			break;
		case EDamageType::Dot_Slow:
			m_IsHit = false;
			break;
		default:
			break;
		}
	}
}

CStatus * CStatus::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CComponent * CStatus::Clone(void * pArg)
{
	return nullptr;
}

void CStatus::Free()
{
}
