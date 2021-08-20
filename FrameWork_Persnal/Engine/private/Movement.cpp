#include "..\public\Movement.h"

CMovement::CMovement(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CTransform(pDevice, pDevice_Context)
{
}

CMovement::CMovement(const CMovement & rhs)
	: CTransform(rhs)
{
}

HRESULT CMovement::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CMovement::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CMovement::Tick(_float TimeDelta)
{
	return _int();
}

void CMovement::Go_Straight(_float TimeDelta)
{
}

void CMovement::Go_Back(_float TimeDelta)
{
}

void CMovement::Go_Right(_float TimeDelta)
{
}

void CMovement::Go_Left(_float TimeDelta)
{
}

void CMovement::Go_Dir(_float TimeDelta, _fvector vTargetPos)
{
}

void CMovement::RotateToAxis(_float TimeDelta, _fvector vAxis)
{
}

void CMovement::RotateToTarget(_float TimeDelta, _fvector vTargetPos)
{
}

void CMovement::RotateToTargetOnLand(_float TimeDelta, _fvector vTargetPos)
{
}

void CMovement::Jump_Begin(_float TimeDelta)
{
}

void CMovement::Gravity_Begin()
{
}

void CMovement::HitBack_Begin()
{
}

void CMovement::Jumping(_float TimeDelta)
{
}

void CMovement::Gravity(_float TimeDelta)
{
}

void CMovement::HitBack(_float TimeDelta)
{
}

CMovement * CMovement::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	return nullptr;
}

CComponent * CMovement::Clone(void * pArg)
{
	
	return nullptr;
}

void CMovement::Free()
{
	__super::Free();
}
