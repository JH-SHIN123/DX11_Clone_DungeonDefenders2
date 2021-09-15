#pragma once

#ifndef __CRYSTALCORE_RING_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CCrystalCore_Ring final :	public CGameObject
{
protected:
	explicit CCrystalCore_Ring(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CCrystalCore_Ring(const CCrystalCore_Ring& rhs);
	virtual ~CCrystalCore_Ring() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component(void* pArg);

private:
	CModel*				m_pModelCom = nullptr;
	CStatus*			m_pStatusCom = nullptr;
	CMovement*			m_pMovementCom = nullptr;

public:
	static CCrystalCore_Ring* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void* pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __CRYSTALCORE_RING_H__
#endif // !__CRYSTALCORE_RING_H__
