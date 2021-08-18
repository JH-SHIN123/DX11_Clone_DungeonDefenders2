#pragma once

#ifndef __LAYER_H__

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
#define __LAYER_H__
#endif // !__LAYER_H__