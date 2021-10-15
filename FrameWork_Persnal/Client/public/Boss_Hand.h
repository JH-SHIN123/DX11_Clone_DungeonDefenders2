#pragma once
#ifndef __BOSS_HAND_H__

#include "Monster.h"

BEGIN(Client)
class CBoss_Hand final : public CMonster
{
private:
	explicit CBoss_Hand(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoss_Hand(const CBoss_Hand& rhs);
	virtual ~CBoss_Hand() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	static CBoss_Hand* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOSS_HAND_H__
#endif // !__BOSS_HAND_H__
