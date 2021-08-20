#pragma once

#ifndef __GAMEINSTANCE_H__

#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "GameObject_Manager.h"
#include "Component_Manager.h"

/* 싱글톤. */
/* 엔진프로젝트 하나에 딱 한번만 할당된다. */
/* 게임내에서 사용할 대부분의 데이터에 접근할 수 있는 루트가 된다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
public:
	CGameInstance();
	virtual ~CGameInstance() = default;
public:
	/* 게임 초기화 과정을 거치낟. */
	HRESULT Initialize(HWND hWnd, CGraphic_Device::WINMODE eWinMode, _uint iSizeX, _uint iSizeY, ID3D11Device** ppDevice = nullptr, ID3D11DeviceContext** ppDevice_Context = nullptr);
	HRESULT Reserve_Container(_uint	iNumLevels);
	_int Tick(_double TimeDelta);
	void Clear_This_Level(_uint iLevelIndex);

public:

#pragma region Graphic_Device
public:
	HRESULT Clear_Back_Buffer(_float4 vClearColor);
	HRESULT Clear_Depth_Stencil_Buffer();
	HRESULT Present();
#pragma endregion


#pragma region Level_Manager
public:
	HRESULT SetUp_CurrentLevel(class CLevel* pCurrentLevel);
	HRESULT Render_Level();

#pragma endregion


#pragma region GameObject_Manager	
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CGameObject* pPrototype);
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);
#pragma endregion


#pragma region Component_Manager
public:
	HRESULT Add_Prototype(_uint	iNumLevels, const _tchar* pComponentTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint	iNumLevels, const _tchar* pComponentTag, void* pArg = nullptr);
#pragma endregion


private:
	CGraphic_Device*				m_pGraphic_Device = nullptr;
	CLevel_Manager*					m_pLevel_Manager = nullptr;
	CGameObject_Manager*			m_pGameObject_Manager = nullptr;
	CComponent_Manager*				m_pComponent_Manager = nullptr;

public:
	static _ulong	Release_Engine();
	virtual void Free() override;
};

END
#define __GAMEINSTANCE_H__
#endif // !__GAMEINSTANCE_H__