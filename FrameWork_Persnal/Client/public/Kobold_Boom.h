#pragma once

#ifndef __KOBOLD_BOOM_H__

#include "Bullet.h"

BEGIN(Client)
class CKobold_Boom final : public CBullet
{
private:
	explicit CKobold_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CKobold_Boom(const CKobold_Boom& rhs);
	virtual ~CKobold_Boom() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_WorldMatrix(_fmatrix matWorld) { m_pMovementCom->Set_WorldMatrix(matWorld); }

private:
	HRESULT			Ready_Component(void* pArg);


public:
	static CKobold_Boom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __KOBOLD_BOOM_H__
#endif // !__KOBOLD_BOOM_H__
