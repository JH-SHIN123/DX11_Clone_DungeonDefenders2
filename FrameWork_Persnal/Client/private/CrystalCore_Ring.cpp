#include "stdafx.h"
#include "..\public\CrystalCore_Ring.h"

CCrystalCore_Ring::CCrystalCore_Ring(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CCrystalCore_Ring::CCrystalCore_Ring(const CCrystalCore_Ring & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCrystalCore_Ring::NativeConstruct_Prototype()
{
	return E_NOTIMPL;
}

HRESULT CCrystalCore_Ring::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CCrystalCore_Ring::Tick(_float TimeDelta)
{
	m_pMovementCom->RotateToAxis_Tick(-TimeDelta, XMVectorSet(0.f, 1.f, 0.f, 0.f));


	return _int();
}

_int CCrystalCore_Ring::Late_Tick(_float TimeDelta)
{
	return _int();
}

HRESULT CCrystalCore_Ring::Render()
{
	return S_OK;
}

HRESULT CCrystalCore_Ring::Ready_Component(void * pArg)
{
	return S_OK;
}

CCrystalCore_Ring * CCrystalCore_Ring::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void* pArg)
{
	CCrystalCore_Ring*		pInstance = new CCrystalCore_Ring(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CCrystalCore_Ring) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCrystalCore_Ring::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CCrystalCore_Ring::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pStatusCom);
	Safe_Release(m_pMovementCom);
}
