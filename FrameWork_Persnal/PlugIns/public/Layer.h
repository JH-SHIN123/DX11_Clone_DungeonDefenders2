#pragma once

#include "GameObject.h"

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;
public:
	HRESULT NativeConstruct();
	HRESULT Add_GameObject(CGameObject* pGameObject);

	_int Tick(_double TimeDelta);
	_int Late_Tick(_double TimeDelta);

private:
	list<class CGameObject*>			m_Objects;
public:
	static CLayer* Create();
	virtual void Free() override;
};

END