#pragma once

#ifndef __GAMEOBJECT_H__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
protected:
	explicit CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT NativeConstruct_Prototype();	// Prototype Ready
	virtual HRESULT NativeConstruct(void* pArg);	// Clone Ready
	virtual _int Tick(_float TimeDelta);
	virtual _int Late_Tick(_float TimeDelta);
	virtual HRESULT Render();

public:
	class CComponent* Get_Component(const _tchar* pComponentTag);

public:
	void	Set_IsDelete_This(_bool IsDeleteThis) { m_IsDelete_This = IsDeleteThis; }
	_bool	Get_IsDelete_This() const { return m_IsDelete_This; }

protected:
	_bool		m_IsDelete_This = false;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

protected:
	typedef unordered_map<const _tchar*, class CComponent*>	COMPONENTS;
	COMPONENTS	m_Components;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Add_Component_Array(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent*** pppOut, void* pArg = nullptr);

private:
	class CComponent* Find_Component(const _tchar* pComponentTag);

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) = 0;
	virtual void Free();
};

END
#define __GAMEOBJECT_H__
#endif // !__GAMEOBJECT_H__