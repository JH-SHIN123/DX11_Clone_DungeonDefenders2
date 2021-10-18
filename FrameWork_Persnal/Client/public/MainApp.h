#pragma once

#ifndef __MAINAPP_H__

#include "Client_Defines.h"
#include "Base.h"
#include "Loading.h"

BEGIN(Engine)
class CRenderer;
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Ready_MainApp();
	_int	Update_MainApp(_double TimeDelta);
	HRESULT Render_MainApp();

private:
	CRenderer* m_pRenderer = nullptr;

private:
	 CGameInstance*			m_pGameInstance = nullptr;

private:
	ID3D11Device*					m_pDevice = nullptr;
	ID3D11DeviceContext*			m_pDevice_Context = nullptr;

private:
	HRESULT Ready_DefaultLevel(ELevel eLevelID);
	HRESULT	Ready_Component_PrototypeForStatic();
	HRESULT	Ready_UI_Texture();
	HRESULT Ready_Cursor();
	HRESULT Ready_Loading();

private:
	_tchar			m_szFPS[MAX_PATH] = TEXT("");
	_uint			m_iNumDraw = 0;
	_float			m_TimeAcc = 0.f;


public:
	static CMainApp* Create();
	virtual void Free() override;
};

END
#define __MAINAPP_H__
#endif // !__MAINAPP_H__