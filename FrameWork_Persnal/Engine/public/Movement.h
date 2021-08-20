#pragma once

/* For. Dynamic State*/
/* what if you want to Static State. used Transform Component*/

#ifndef __MOVEMENT_H__

#include "Transform.h"

BEGIN(Engine)

typedef struct tagMoveStateDesc
{
	tagMoveStateDesc() {}

	_float	fSpeedPerSec = 0.f;
	_float	fRotatePerSec = 0.f;

	// JumpUp ~ JumpDown, Gravity(false)
	_bool	IsJump = false;
	_float	fJumpTime = 0.f;
	_float	fJumpForce = 0.f;

	// FreeFall, Jump(false)
	_bool	IsGravity = false;
	_float	fGravityTime = 0.f;
	
	_bool	IsHitBack = false;
	_float	fHitBackTime = 0.f;
	_float	fHitBackForce = 0.f;

	tagMoveStateDesc(_float InSpeedPerSec, _float InRotationPerSec)
		: fSpeedPerSec(InSpeedPerSec), fRotatePerSec(InRotationPerSec) {}

	tagMoveStateDesc(_float InSpeedPerSec, _float InRotationPerSec
		, _float InJumpTime, _float InJumpForce)
		: fSpeedPerSec(InSpeedPerSec), fRotatePerSec(InRotationPerSec)
		, fJumpTime(InJumpTime), fJumpForce(InJumpForce){}

}MOVESTATE_DESC;

class ENGINE_DLL CMovement final : public CTransform
{
private:
	explicit CMovement(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMovement(const CMovement& rhs);
	virtual ~CMovement() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public: 
	_int Tick(_float TimeDelta);

public: // Getter

public:	// Setter

public: // Move
	void Go_Straight(_float TimeDelta);
	void Go_Back(_float TimeDelta);
	void Go_Right(_float TimeDelta);
	void Go_Left(_float TimeDelta);
	void Go_Dir(_float TimeDelta, _fvector vTargetPos);

public: // Rotate
	void RotateToAxis(_float TimeDelta, _fvector vAxis);
	void RotateToTarget(_float TimeDelta, _fvector vTargetPos);
	void RotateToTargetOnLand(_float TimeDelta, _fvector vTargetPos);

public: // physics
	void Jump_Begin(_float TimeDelta);
	void Gravity_Begin();
	void HitBack_Begin();
	
private: // Tick
	void Jumping(_float TimeDelta);
	void Gravity(_float TimeDelta);
	void HitBack(_float TimeDelta);

private:
	//CTransform*		m_pTransform_Com = nullptr;
	MOVESTATE_DESC	m_MoveStateDesc;
	
public:
	static CMovement* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END
#define __MOVEMENT_H__
#endif // !__MOVEMENT_H__
