#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Loading.h"

BEGIN(Client)

class CLevel_Loading final : public CLevel
{
public:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual ~CLevel_Loading() = default;
public:
	virtual HRESULT NativeConstruct(ELevel eNextSceneID);
	virtual int Tick(_double Timedelta) override;
	virtual HRESULT Render() override;

private:
	CLoading*		m_pLoading = nullptr;
	ELevel			m_eNextLevelID = ELevel::End;
public:
	static CLevel_Loading* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, ELevel eNextSceneID);
	virtual void Free();

};

END