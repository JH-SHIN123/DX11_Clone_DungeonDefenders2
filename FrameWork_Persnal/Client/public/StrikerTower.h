#pragma once

#ifndef __STRIKERTOWER_H__

#include "DefenceTower.h"

BEGIN(Client)
class CStrikerTower final :	public CDefenceTower
{
private:
	explicit CStrikerTower(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStrikerTower(const CStrikerTower& rhs);
	virtual ~CStrikerTower() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT		Ready_Component(void* pArg);

public:
	static	CStrikerTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;

};

END
#define __STRIKERTOWER_H__
#endif // !__STRIKERTOWER_H__
