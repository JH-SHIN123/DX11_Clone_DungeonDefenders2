#pragma once

#ifndef __BLOCKADETOWER_H__

#include "DefenceTower.h"

BEGIN(Client)
class CBlockadeTower final : public CDefenceTower
{
private:
	explicit CBlockadeTower(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBlockadeTower(const CBlockadeTower& rhs);
	virtual ~CBlockadeTower() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Update_Anim(_float TimeDelta);

protected:
	HRESULT		Ready_Component(void* pArg);
	_float		m_fIdleAnimation = 80.f;
	CCollider*		m_pCollider_Hit = nullptr;


public:
	static	CBlockadeTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BLOCKADETOWER_H__
#endif // !__BLOCKADETOWER_H__
