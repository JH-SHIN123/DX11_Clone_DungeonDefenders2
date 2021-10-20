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

	memcpy(&m_Status_Desc, pArg, sizeof(STATUS_DESC));

	m_Status_Desc.iHp = m_Status_Desc.iHp_Max;
	m_Status_Desc.iMp = m_Status_Desc.iMp_Max;

	if (1 == m_Status_Desc.iLevel)
		m_Status_Desc.iExp = 0;

	return S_OK;
}

_uint CStatus::Tick(_float TimeDelta)
{
	Damage_Check(TimeDelta);

	return _uint();
}

_bool CStatus::Level_Check()
{
	// 스탯창은 전투가 끝나고 나옴

	if (m_Status_Desc.iExp >= m_Status_Desc.iExp_Max)
	{
		++m_Status_Desc.iLevel;
		m_Status_Desc.iAtt_Basic	= (_int)(m_Status_Desc.iAtt_Basic * 1.15f);
		m_Status_Desc.iHp_Max		= (_int)(m_Status_Desc.iHp_Max * 1.3f);
		m_Status_Desc.iHp			= m_Status_Desc.iHp_Max;
		m_Status_Desc.iMp_Max		+= m_Status_Desc.iMp_Max >> 1;

		m_Status_Desc.iExp			-= m_Status_Desc.iExp_Max;
		m_Status_Desc.iExp_Max		= (_int)(m_Status_Desc.iExp_Max * 1.2f);
		
		return true;
	}
	return false;
}

void CStatus::Set_Damage(const ATTACK_DESC & pDamage)
{
	m_Hit_Desc = pDamage;
	m_Status_Desc.iHp -= pDamage.iDamage;
}

void CStatus::Set_Damage_Dot(const ATTACK_DESC & pDamage)
{
	m_Hit_Desc = pDamage;
}

void CStatus::Set_Hp(_int iHp)
{
	m_Status_Desc.iHp = iHp;

	if (0 >= m_Status_Desc.iHp)
		m_Status_Desc.iHp = 0;
}

void CStatus::Add_Mp(_int iMp)
{
	m_Status_Desc.iMp += iMp;
	if (m_Status_Desc.iMp > m_Status_Desc.iMp_Max)
		m_Status_Desc.iMp = m_Status_Desc.iMp_Max;
}

void CStatus::Damage_Check(_float TimeDelta)
{
	if (0 >= m_Status_Desc.iHp)
		m_IsDead = true;

	m_Hit_Desc.fHitTime -= TimeDelta;
	m_fDamageTick += TimeDelta;

	if (0.f >= m_Hit_Desc.fHitTime)
		ZeroMemory(&m_Hit_Desc, sizeof(ATTACK_DESC));
	else
	{
		if (0.25f <= m_fDamageTick)
		{
			m_Status_Desc.iHp -= m_Hit_Desc.iDamage;
			m_fDamageTick = 0.f;
		}
	}
}

CStatus * CStatus::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStatus* pInstance = new CStatus(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStatus) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent * CStatus::Clone(void * pArg)
{
	CStatus* pClone = new CStatus(*this);
	if (FAILED(pClone->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Clone (CStatus) ");
		Safe_Release(pClone);
	}
	return pClone;
}

void CStatus::Free()
{
	__super::Free();
}
