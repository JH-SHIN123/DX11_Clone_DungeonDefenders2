#pragma once

#ifndef __STAFF_BASIC_H__

#include "Weapon.h"

BEGIN(Client)
class CStaff_Basic final : public CWeapon
{
private:
	explicit CStaff_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStaff_Basic(const CStaff_Basic& rhs);
	virtual ~CStaff_Basic() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);


public:
	static CStaff_Basic* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __STAFF_BASIC_H__
#endif // !__STAFF_BASIC_H__
