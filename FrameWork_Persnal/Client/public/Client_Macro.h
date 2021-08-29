#pragma once

// Easy Get To GameInstance Func In Engine

#define GET_GAMEINSTANCE CGameInstance::GetInstance()

#define GET_VIEW_SPACE CGameInstance::GetInstance()->Get_Transform(ETransformState::View)
#define GET_PROJ_SPACE CGameInstance::GetInstance()->Get_Transform(ETransformState::Proj)

#define GET_INDENTITY_MATRIX XMMatrixIdentity()
#define GET_ORTHO_SPACE XMMatrixOrthographicLH((_float)g_iWinCX, (_float)g_iWinCY, 0.f, 1.f)

#define GET_MOUSE_X CGameInstance::GetInstance()->Get_InputDev()->Get_DIMouseMove(MOUSEMOVESTATE::X)
#define GET_MOUSE_Y CGameInstance::GetInstance()->Get_InputDev()->Get_DIMouseMove(MOUSEMOVESTATE::Y)
#define GET_MOUSE_Z CGameInstance::GetInstance()->Get_InputDev()->Get_DIMouseMove(MOUSEMOVESTATE::Z)
