#include "stdafx.h"
#include "..\public\Staff_Basic.h"

CStaff_Basic::CStaff_Basic(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CWeapon(pDevice, pDevice_Context)
{
}

CStaff_Basic::CStaff_Basic(const CStaff_Basic & rhs)
	: CWeapon(rhs)
{
}

HRESULT CStaff_Basic::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStaff_Basic::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	//Set_Pivot(XMVectorSet(0.75f, 0.75f, 0.75f, 0.f));
	_matrix RotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToDegrees(-50.f), XMConvertToDegrees(-5.f), 0.f);// * );
	Set_RotateMatrix(RotateMatrix);

	m_pModelCom->Set_AnimationIndex(0);
	m_pModelCom->Set_AnimationIndex_Start(0.f, 179.f);

	return S_OK;
}

_int CStaff_Basic::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);


	return _int();
}

_int CStaff_Basic::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	_matrix RotateMatrix = XMMatrixRotationRollPitchYaw(XMConvertToDegrees(-50.f), XMConvertToDegrees(-210.f), 0.f);// * );
	Set_RotateMatrix(RotateMatrix);


	m_pModelCom->Update_AnimaionMatrix(TimeDelta);
	m_pModelCom->Update_CombindTransformationMatrix();

	return _int();
}

HRESULT CStaff_Basic::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStaff_Basic::Ready_Component(void * pArg)
{
	return S_OK;
}

CStaff_Basic * CStaff_Basic::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStaff_Basic*		pInstance = new CStaff_Basic(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStaff_Basic) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStaff_Basic::Clone_GameObject(void * pArg)
{
	CStaff_Basic*		pInstance = new CStaff_Basic(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStaff_Basic) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStaff_Basic::Free()
{
	__super::Free();
}
