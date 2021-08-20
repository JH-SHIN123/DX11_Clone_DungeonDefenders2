#pragma once

#ifndef __LEVEL_STAGE1_H__


#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
private:
	CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel_Stage1() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_float Timedelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);
	HRESULT Ready_Layer_UI(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera_Free(const _tchar* pLayerTag);

public:
	static CLevel_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual void Free();
};

END
#define __LEVEL_STAGE1_H__
#endif // !__LEVEL_STAGE1_H__