#pragma once

#ifndef __LEVEL_LOGO_H__


#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Logo final : public CLevel
{
private:
	CLevel_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel_Logo() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_float Timedelta) override;
	virtual HRESULT Render() override;

public:
	void Scene_Change(ELevel eLevel);

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_MainMenu(const _tchar* pLayerTag);

private:
	_bool m_IsChange = false;
	ELevel m_eNextLevel = ELevel::End;

public:
	static CLevel_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual void Free();

};

END
#define __LEVEL_LOGO_H__
#endif // !__LEVEL_LOGO_H__