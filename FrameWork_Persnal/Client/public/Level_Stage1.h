#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Stage1 final : public CLevel
{
public:
	CLevel_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel_Stage1() = default;
public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_double Timedelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Terrain(const _tchar* pLayerTag);

public:
	static CLevel_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual void Free();

};

END