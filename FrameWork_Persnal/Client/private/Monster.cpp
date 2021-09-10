#include "stdafx.h"
#include "..\public\Monster.h"
#include "Masking_MeterBar_3D.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMonster::CMonster(const CMonster & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMonster::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMonster::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	return S_OK;
}

_int CMonster::Tick(_float TimeDelta)
{
	m_pMeterBar_Hp->Tick(TimeDelta);


	return _int();
}

_int CMonster::Late_Tick(_float TimeDelta)
{
	m_pMeterBar_Hp->Set_Count((_float)m_pStatusCom->Get_Hp(), (_float)m_pStatusCom->Get_HpMax());



	return _int();
}

HRESULT CMonster::Render()
{

	m_pMeterBar_Hp->Render();

	return S_OK;
}

HRESULT CMonster::Ready_Component(void * pArg)
{
	MONSTER_DESC Data;
	memcpy(&Data, pArg, sizeof(MONSTER_DESC));
	
	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Status"), TEXT("Com_Movement"), (CComponent**)&m_pStatusCom, &Data.Stat_Desc);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Data.Movement_Desc);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect_Model"), TEXT("Com_Buffer"), (CComponent**)&m_pBufferRectCom);
	hr = CGameObject::Add_Component((_uint)Data.eLevel, Data.szTextureName, TEXT("Com_Texture_0"), (CComponent**)&m_pTextureCom);




	if (S_OK != hr)
		MSG_BOX("CMonster::Ready_Component Failed");

	return hr;
}

CGameObject * CMonster::Clone_GameObject(void * pArg)
{
	CMonster* pInstance = new CMonster(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CMonster) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMonster::Free()
{
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pMovementCom);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pBufferRectCom);

	__super::Free();
}
