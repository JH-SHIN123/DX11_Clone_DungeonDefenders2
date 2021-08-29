#include "stdafx.h"
#include "..\public\Terrain.h"
#include "Loading.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{

}

CTerrain::CTerrain(const CTerrain & rhs)
	: CGameObject(rhs)
{

}

HRESULT CTerrain::NativeConstruct_Prototype()
{
	CGameObject::NativeConstruct_Prototype();



	return S_OK;
}

HRESULT CTerrain::NativeConstruct(void * pArg)
{
	CGameObject::NativeConstruct(pArg);

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

_int CTerrain::Tick(_float TimeDelta)
{
	return _int();
}

_int CTerrain::Late_Tick(_float TimeDelta)
{
	if (nullptr == m_pRendererCom)
		return -1;

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::NoneAlpha, this);
}

HRESULT CTerrain::Render()
{
	if (nullptr == m_pVIBufferCom)
		return -1;
/*
	_float3			vTest(0.f, 0.f, 0.f);



	_vector			vResult = XMLoadFloat3(&vTest);
	XMVectorSetW(vResult, 1.f);*/
	


	_matrix			WorldMatrix, ViewMatrix, ProjMatrix, OrthMatrix;

	WorldMatrix = XMMatrixIdentity();
	ProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(60.0f), (_float)g_iWinCX / g_iWinCY, 0.2f, 300.f);
	OrthMatrix = XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.2f, 300.f);

	// 행렬이 들어갈때 전치 행렬로 바뀌기 때문에 전치행렬로 바뀌기 전에 직접 전치 행렬로 바꿔 전치전치 행렬이 된다.
	// 당연히 전치전치행렬이라는 개소리는 없으며 일반 평범한 행렬이다. 360도 회전과 같음
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	// 이미지 세팅 g_DiffuseTexture에다가 m_pTextureCom의 0번째의 리소스를 넣어주시오.
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));


	// 0번째 쉐이더를 이용해 그릴게
	// 쉐이더 세팅은 Prototype을 생성하면서 세팅을 함
	m_pVIBufferCom->Render(0);

	return S_OK;
}

HRESULT CTerrain::Ready_Component()
{
	CGameInstance* pGameInstance = GET_GAMEINSTANCE;

	if (nullptr == pGameInstance)
		return E_FAIL;

	Safe_AddRef(pGameInstance);

	/* 렌더러컴포넌트를 추가하자.*/
	/* For.Renderer*/
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.VIBuffer */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_VIBuffer_Terrain"), TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	/* For.Textures */
	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_Devil"), TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;




	/* For.Transform */

	Safe_Release(pGameInstance);

	return S_OK;
}


CTerrain * CTerrain::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CTerrain*		pInstance = new CTerrain(pDevice, pDevice_Context);

	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CTerrain) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CTerrain::Clone_GameObject(void * pArg)
{
	CTerrain*		pInstance = new CTerrain(*this);

	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CTerrain) ");
		Safe_Release(pInstance);
	}

	return pInstance;
}
void CTerrain::Free()
{
	CGameObject::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
}
