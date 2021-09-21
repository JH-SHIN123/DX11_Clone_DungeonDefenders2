#pragma once

#ifndef __LAYER_H__

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	class CGameObject* Get_GameObject() const { return m_Objects.front(); }
	list<class CGameObject*> Get_GameObject_List() const { return m_Objects; }

public:
	HRESULT NativeConstruct();
	HRESULT Add_GameObject(CGameObject* pGameObject);

	_int Tick(_float TimeDelta);
	_int Late_Tick(_float TimeDelta);

private:
	list<class CGameObject*>			m_Objects;
public:
	static CLayer* Create();
	virtual void Free() override;
};

END
#define __LAYER_H__
#endif // !__LAYER_H__