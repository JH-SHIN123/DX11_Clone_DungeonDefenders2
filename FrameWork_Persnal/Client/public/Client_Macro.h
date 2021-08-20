#pragma once

// Easy Get To GameInstance Func In Engine

#define GET_GAMEINSTANCE CGameInstance::GetInstance()

#define GET_VIEW_SPACE CGameInstance::GetInstance()->Get_Transform(ETransformState::View)
#define GET_PROJ_SPACE CGameInstance::GetInstance()->Get_Transform(ETransformState::Proj)
