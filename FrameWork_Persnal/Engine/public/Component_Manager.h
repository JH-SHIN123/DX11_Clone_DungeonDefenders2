#pragma once

#ifndef __CCOMPONENT_MANAGER_H__

#include "Component.h"
#include "Renderer.h"
#include "Textures.h"
#include "VIBuffer.h"
#include "Transform.h"
#include "Movement.h"
#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"

class CComponent_Manager final : public CBase
{
	DECLARE_SINGLETON(CComponent_Manager)

private:
	CComponent_Manager();
	virtual ~CComponent_Manager() = default;

public:
	HRESULT Reserve_Container(_uint iNumLevels);
	HRESULT Add_Prototype(_uint iNumLevels, const _tchar* pComponentTag, CComponent* pPrototype);
	CComponent* Clone_Component(_uint iNumLevels, const _tchar* pComponentTag, void* pArg);
	void Clear(_uint iLevelIndex);

private:
	typedef unordered_map<const _tchar*, CComponent*> COMPONENTS;
	COMPONENTS*	m_pComponents = nullptr; // 배열로 접근 할 것이다.

private:
	_uint	m_iNumLevels = 0;

private:
	CComponent* Find_Component(_uint iNumLevels, const _tchar* pComponentTag);

public:
	virtual void Free() override;
};

#define __CCOMPONENT_MANAGER_H__
#endif // !__CCOMPONENT_MANAGER_H__
