#pragma once

#ifndef __CRYSTALCORE_H__

#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)
class CCrystalCore final : public CGameObject
{
protected:
	explicit CCrystalCore(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CCrystalCore(const CCrystalCore& rhs);
	virtual ~CCrystalCore() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;
	
private:
	HRESULT	Ready_Component(void* pArg);

private:
	void Button_Dis_Check();

private:
	CModel*				m_pModelCom			= nullptr;
	CStatus*			m_pStatusCom		= nullptr;
	CRenderer*			m_pRendererCom		= nullptr;
	CMovement*			m_pMovementCom		= nullptr;
	CCollider*			m_pColliderCom_Hit	= nullptr;

private: // Activate Button
	class COrtho3D*		m_pOrtho3D_Text = nullptr;

private:
	class CCrystalCore_Ring*		m_pRing_Up = nullptr;
	class CCrystalCore_Ring*		m_pRing_Down = nullptr;

public:
	static CCrystalCore* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __CRYSTALCORE_H__
#endif // !__CRYSTALCORE_H__
