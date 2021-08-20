#pragma once

// 카메라를 다루기 쉽게 매니저를 만들어봄
// 하지만 카메라가 원래 있어야 할 곳은 GameObject_Manager
// Camera_Manager의 해제는 MainApp의 Free에서 GameObject_Manager보다 앞에서 지우자

#ifndef __CAMERA_MANAGER_H__

#include "Base.h"

enum class ECameraMode 
{Free, Target, Move, End};

class CCamera_Manager final : public CBase
{
	DECLARE_SINGLETON(CCamera_Manager)

private:
	CCamera_Manager();
	virtual ~CCamera_Manager() = default;

public:
	virtual void Free() override;
};

#define __CAMERA_MANAGER_H__
#endif // !__CAMERA_MANAGER_H__
