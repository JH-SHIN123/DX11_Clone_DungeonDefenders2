#pragma once

#ifndef __ARCHER_BULLET_H__

#include "Bullet.h"

BEGIN(Client)
class CArcher_Arrow final : public CBullet
{
private:
	explicit CArcher_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CArcher_Arrow(const CArcher_Arrow& rhs);
	virtual ~CArcher_Arrow() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

private:
	_bool	m_IsMove = false;
	_float	m_fCastTime = 0.5f;

public:
	static CArcher_Arrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __ARCHER_BULLET_H__
#endif // !__ARCHER_BULLET_H__
