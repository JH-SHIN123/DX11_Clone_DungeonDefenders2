#include "stdafx.h"
#include "..\public\BlockadeTower.h"

CBlockadeTower::CBlockadeTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CDefenceTower(pDevice, pDevice_Context)
{
}

CBlockadeTower::CBlockadeTower(const CBlockadeTower & rhs)
	: CDefenceTower(rhs)
{
}

HRESULT CBlockadeTower::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CBlockadeTower::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	m_pModelCom->Set_AnimationIndex(0); // 나는 애니메이션 하나에 다 있는 상황 원테이크
	m_pModelCom->Set_AnimationIndex_Start(0.f, m_fIdleAnimation);

	Ready_Component(nullptr);

	Set_Pivot(XMVectorSet(0.035f, 0.035f, 0.035f, 0.f));

	m_eTowerRange = ETowerRange::Half;

	return S_OK;
}

_int CBlockadeTower::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	m_pCollider_Hit->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return iReturn;
}

_int CBlockadeTower::Late_Tick(_float TimeDelta)
{
	m_pModelCom->Update_AnimaionMatrix(TimeDelta);
	m_pModelCom->Update_CombindTransformationMatrix();




	return __super::Late_Tick(TimeDelta);
}

HRESULT CBlockadeTower::Render()
{

	__super::Render();

#ifdef _DEBUG
	m_pCollider_Hit->Render_Collider();
#endif // _DEBUG


	return S_OK;
}

void CBlockadeTower::Update_Anim(_float TimeDelta)
{
	m_pModelCom->Update_AnimaionMatrix(TimeDelta);
	m_pModelCom->Update_CombindTransformationMatrix();
}

HRESULT CBlockadeTower::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 3.f, 3.f, 3.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pCollider_Hit, &Data);



	return S_OK;
}

CBlockadeTower * CBlockadeTower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBlockadeTower*		pInstance = new CBlockadeTower(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBlockadeTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBlockadeTower::Clone_GameObject(void * pArg)
{
	CBlockadeTower*		pInstance = new CBlockadeTower(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CBlockadeTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBlockadeTower::Free()
{
	Safe_Release(m_pCollider_Hit);
	__super::Free();
}
