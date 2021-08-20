#pragma once

#include "Tool_Define.h"
#include "public\Level.h"

class CMain_Level final : public CLevel
{
public:
	CMain_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CMain_Level() = default;

public:
	virtual HRESULT NativeConstruct() override;
	virtual _int	Tick(_float Timedelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);

public:
	static CMain_Level* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual void Free();
};

