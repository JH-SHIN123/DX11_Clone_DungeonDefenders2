#pragma once

#ifndef __PLAYER_BULLET_H__

#include "Bullet.h"

BEGIN(Client)
class CPlayer_Bullet final : public CBullet
{
private:
	explicit CPlayer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPlayer_Bullet(const CPlayer_Bullet& rhs);
	virtual ~CPlayer_Bullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_WorldMatrix(_fmatrix matWorld) { m_pMovementCom->Set_WorldMatrix(matWorld); }
	void Creat_Effect();

private:
	HRESULT			Ready_Component(void* pArg);

	class CPoint_Spread* m_pPointSpread = nullptr;
	class CPoint_Ex_Trail* m_pPoint_Trail = nullptr;

public:
	static CPlayer_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;

};

END
#define __PLAYER_BULLET_H__
#endif // !__PLAYER_BULLET_H__
