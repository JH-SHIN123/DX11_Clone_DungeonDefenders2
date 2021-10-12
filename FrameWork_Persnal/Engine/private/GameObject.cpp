#include "..\public\GameObject.h"
#include "GameInstance.h"
#include "Component.h"

CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: m_pDevice(pDevice)
	, m_pDevice_Context(pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);
}

CGameObject::CGameObject(const CGameObject & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pDevice_Context(rhs.m_pDevice_Context)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDevice_Context);

	Set_Pivot(XMVectorSet(1.f, 1.f, 1.f, 0.f));
}

HRESULT CGameObject::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CGameObject::NativeConstruct(void* pArg)
{
	return S_OK;
}

_int CGameObject::Tick(_float TimeDelta)
{
	return _int();
}

_int CGameObject::Late_Tick(_float TimeDelta)
{
	// 자식의 Late_Tick에서 렌더러에 넣는데 이걸
	// 절두체 컬링으로 판단하게 된다면?
	// 일단은 간단하게 뷰 행렬 기준으로 카메라의 Z보다 작으면 그리지 말자

	return _int();
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

CComponent * CGameObject::Get_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Set_Pivot(_fvector vScale)
{
	_matrix PivotMatrix = XMMatrixIdentity();

	PivotMatrix = XMMatrixScalingFromVector(vScale);

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);
}

void CGameObject::Set_Pivot_Rotate(_fvector RotateXYZW)
{
	XMStoreFloat4x4(&m_PivotMatrix,XMLoadFloat4x4(&m_PivotMatrix) * XMMatrixRotationRollPitchYaw(
		XMConvertToDegrees(XMVectorGetX(RotateXYZW)), XMConvertToDegrees(XMVectorGetY(RotateXYZW)), XMConvertToDegrees(XMVectorGetZ(RotateXYZW))));
}

void CGameObject::Set_Pivot_Rotate_Radian(_fvector RadianXYZW)
{
	XMStoreFloat4x4(&m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix) * XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(RadianXYZW)), XMConvertToRadians(XMVectorGetY(RadianXYZW)), XMConvertToRadians(XMVectorGetZ(RadianXYZW))));
}

void CGameObject::Set_Pivot_Rotate_Radian_New(_fvector RadianXYZW)
{
	XMStoreFloat4x4(&m_PivotMatrix, XMMatrixRotationRollPitchYaw(
		XMConvertToRadians(XMVectorGetX(RadianXYZW)), XMConvertToRadians(XMVectorGetY(RadianXYZW)), XMConvertToRadians(XMVectorGetZ(RadianXYZW))));
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent ** ppOut, void * pArg)
{
	// 레벨인덱스 예외처리 없는게 아쉽네
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	CComponent*		pComponent = Find_Component(pComponentTag);
	if (nullptr != pComponent)
		return E_FAIL;

	pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));

	*ppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

HRESULT CGameObject::Add_Component_Array(_uint iLevelIndex, const _tchar * pPrototypeTag, const _tchar * pComponentTag, CComponent *** pppOut, void * pArg)
{
	CGameInstance*	pGameInstance = CGameInstance::GetInstance();
	if (nullptr == pGameInstance)
		return E_FAIL;

	CComponent*		pComponent = Find_Component(pComponentTag);
	if (nullptr != pComponent)
		return E_FAIL;

	pComponent = pGameInstance->Clone_Component(iLevelIndex, pPrototypeTag, pArg);
	if (nullptr == pComponent)
		return E_FAIL;

	m_Components.insert(COMPONENTS::value_type(pComponentTag, pComponent));

	**pppOut = pComponent;

	Safe_AddRef(pComponent);

	return S_OK;
}

CComponent * CGameObject::Find_Component(const _tchar * pComponentTag)
{
	auto iter = find_if(m_Components.begin(), m_Components.end(), CTagFinder(pComponentTag));

	if (iter == m_Components.end())
		return nullptr;

	return iter->second;
}

void CGameObject::Free()
{
	for (auto Pair : m_Components)
		Safe_Release(Pair.second);

	m_Components.clear();

	Safe_Release(m_pDevice_Context);
	Safe_Release(m_pDevice);
}
