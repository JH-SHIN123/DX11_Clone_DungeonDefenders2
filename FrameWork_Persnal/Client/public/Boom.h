#pragma once

#ifndef __BOOM_H__

#include "Bullet.h"

BEGIN(Client)
class CBoom final : public CBullet
{
private:
	explicit CBoom(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBoom(const CBoom& rhs);
	virtual ~CBoom() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);

private:
	_float	m_fExplosionTime = 0.f;
	


public:
	static CBoom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BOOM_H__
#endif // !__BOOM_H__
