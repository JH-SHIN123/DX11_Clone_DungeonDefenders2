#pragma once

// 카메라를 다루기 쉽게 매니저를 만들어봄
// 하지만 카메라가 원래 있어야 할 곳은 GameObject_Manager
// Camera_Manager의 해제는 MainApp의 Free에서 GameObject_Manager보다 앞에서 지우자

// 기존 카메라 > 다른 카메라
// 쓸모 있을까 과연
// 카메라 Tick은 알아서 작동, 


#ifndef __CAMERA_MANAGER_H__

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CCamera;
END

BEGIN(Client)

enum class ECameraMode 
{Free, Target, Move, End};

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public: // Getter
	ECameraMode Get_CameraMode() const { return m_eCameraMode; }

public:
	HRESULT Change_Camera(ECameraMode eCamMode, CCamera* pCamera);

private:
	ECameraMode		m_eCameraMode = ECameraMode::End;

public:
	virtual void Free() override;
};

END
#define __CAMERA_MANAGER_H__
#endif // !__CAMERA_MANAGER_H__
