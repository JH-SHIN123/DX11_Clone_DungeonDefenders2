#pragma once

#ifndef __GAMEOBJECT_MANAGER_H__

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class CGameObject_Manager final : public CBase
{
	DECLARE_SINGLETON(CGameObject_Manager)
public:
	CGameObject_Manager();
	virtual ~CGameObject_Manager() = default;
public:
	HRESULT Reserve_Container(_uint	iNumLevels);
	/* 원형객체를 컨테이너에 추가한다. */
	HRESULT Add_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag, class CGameObject* pPrototype);

	/* 원형객체를 검색하여 복제한 후, 레이어에 추가하나다,. */
	HRESULT Add_GameObject(_uint iPrototypeLevelIndex, const _tchar* pPrototypeTag, _uint iLevelIndex, const _tchar* pLayerTag, void* pArg = nullptr);

	// 자식 용도로 쓸 게임 오브젝트를 만들어 준다.
	CGameObject* Add_Create_Clone(_uint iPrototypeLevelIndex, const _tchar* pPrototypeTag, _uint iLevelIndex, void* pArg);

	// Getter
	CGameObject* Get_GameObject(_uint iLevelIndex, const _tchar* pLayerTag);
	CLayer* Get_Layer(_uint iLevelIndex, const _tchar* pLayerTag);

	_int Tick_GameObject(_float TimeDelta);
	void Clear(_uint iLevelIndex);

private:
	typedef unordered_map<const _tchar*, CGameObject*>	PROTOTYPES;
	PROTOTYPES*		m_pPrototypes = nullptr;

private:
	typedef unordered_map<const _tchar*, CLayer*>	LAYERS;
	LAYERS*			m_pLayers = nullptr;

private:
	_uint			m_iNumLevels = 0;

private:
	CGameObject* Find_Prototype(_uint iLevelIndex, const _tchar* pPrototypeTag);
	CLayer* Find_Layer(_uint iLevelIndex, const _tchar* pLayerTag);
	CLayer* Find_Layer_List(_uint iLevelIndex, const _tchar* pLayerTag);

public:
	virtual void Free() override;
};

END
#define __GAMEOBJECT_MANAGER_H__
#endif // !__GAMEOBJECT_MANAGER_H__