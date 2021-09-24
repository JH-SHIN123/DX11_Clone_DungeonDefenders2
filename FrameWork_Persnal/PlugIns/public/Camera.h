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
	_float3		vTargetAxis;
	_float		fDis;
	_float		fXRotationLock_Min = 0.f;
	_float		fXRotationLock_Max = 360.f;


	/* 투영관련 정보. */
	_float		fAspect;
	_float		fFovy;
	_float		fNear;
	_float		fFar;

	/* 카메라의 트랜스폼에게 필요한 정보. */
	MOVESTATE_DESC		StateDesc;

}CAMERA_DESC;

enum class ECameraViewMode
{ThirdPerson, TopView, TopToTPS ,End};

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

public: // Setter
	//void Set_CameraDesc_Second(const CAMERA_DESC& CamDesc) { m_CameraDesc_Second = CamDesc; }

public:
	void Set_ZoomIn(_float fFov, _float fZoomSpeed = 1.f);
	void Set_ZoomOut(_float fFov, _float fZoomSpeed = 1.f);
	void Set_CameraView_Mode(ECameraViewMode eViewMode) { m_eCameraMode_Next = eViewMode; }

public: // Tick
	void	Target_Check(_uint iLevel, const _tchar* LayerTag, const _tchar* ComponentTag);
	void	TargetRotate_Check(_uint iLevel, const _tchar* LayerTag, const _tchar* ComponentTag);
	void	TargetRotate_Check(CTransform* pTransform);

protected:
	void	View_Check(_float TimeDelata);


private: //
	void	View_ThirdPerson(_float TimeDelata);	// main camera view
	void	View_TopView(_float TimeDelata);		//sub camera view
	void	View_Change_Top_ThirdPerson(_float TimeDelata);

private:
	void Zoom_Check(_float TimeDelta);

private:
	void SetUp_PipeLine_Matrix();

protected:
	 CMovement*				m_pMovementCom = nullptr;
	 CPipeline_Manager*		m_pPipeline_Manager = nullptr;

protected:
	CAMERA_DESC				m_CameraDesc;
	CAMERA_DESC				m_CameraDesc_Second;
	_float3					m_vTopView_Dir;
	_bool					m_IsZoomIn = false;
	_bool					m_IsZoomOut = false;
	_bool					m_IsZoomReverse = false;
	_float					m_fZoomFov = 0.f;
	_float					m_fZoomSpeed = 0.f;
	_float					m_fDisSecond = 2.f;
	//_float					m_fAxisX_Lenght = 0.f;
	//_float3					m_vNoRotate_TargetAxis;
	//_float3					m_vCalculate_TargetAxis;

protected:
	ECameraViewMode			m_eCameraMode_Now = ECameraViewMode::ThirdPerson;
	ECameraViewMode			m_eCameraMode_Next = ECameraViewMode::ThirdPerson;


protected:
	//_float4  

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) PURE; // pArg == CAMERA_DESC
	virtual void Free() override;
};
END
#define __CAMERA_H__
#endif //!__CAMERA_H__
