#pragma once

#ifndef __DUMMY_H__

#include "Tool_Define.h"
#include "GameObject.h"

BEGIN(Engine)

END


BEGIN(Tool)
class CDummy final : public CGameObject
{
public:
	CDummy(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CDummy(const CGameObject& rhs);
	virtual ~CDummy() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;	// Prototype Ready
	virtual HRESULT NativeConstruct(void* pArg) override;	// Clone Ready
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pDevice_Context = nullptr;

protected:
	typedef unordered_map<const _tchar*, class CComponent*>	COMPONENTS;
	COMPONENTS	m_Components;

protected:
	HRESULT Add_Component(_uint iLevelIndex, const _tchar* pPrototypeTag, const _tchar* pComponentTag, CComponent** ppOut, void* pArg = nullptr);

private:
	HRESULT	Ready_Component();

public:
	static CDummy* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

};
END
#define __DUMMY_H__
#endif // !__DUMMY_H__
