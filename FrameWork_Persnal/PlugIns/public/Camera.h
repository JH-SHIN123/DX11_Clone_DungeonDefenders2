#pragma once

#ifndef __CAMERA_H__

#include "GameObject.h"
#include "Movement.h"

BEGIN(Engine)

class CPipeline_Manager;

typedef struct tagCameraDesc
{
	/* 카메라 복제본에게 필요한 정보들. */
	_float3		vEye;
	_float3		vAt;
	_float3		vAxisY;

	/* 투영관련 정보. */
	_float		fAspect;
	_float		fFovy;
	_float		fNear;
	_float		fFar;

	/* 카메라의 트랜스폼에게 필요한 정보. */
	MOVESTATE_DESC		StateDesc;

}CAMERA_DESC;

class ENGINE_DLL CCamera abstract :	public CGameObject
{
protected:
	explicit CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CCamera(const CCamera& rhs);
	virtual ~CCamera() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	 CMovement*				m_pMovementCom = nullptr;
	 CPipeline_Manager*		m_pPipeline_Manager = nullptr;

protected:
	CAMERA_DESC				m_CameraDesc;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) PURE; // pArg == CAMERA_DESC
	virtual void Free() override;
};
END
#define __CAMERA_H__
#endif //!__CAMERA_H__
