#pragma once

#ifndef __LIGHTNINGTOWER_BULLET_H__

#include "Bullet.h"

//BEGIN(Engine)
//class CGameObject;
//END

BEGIN(Client)

typedef struct tagLightningBulletDesc
{
	_int iBoundCount = 0;
	BULLET_DESC Bullet_Desc;

}LIGHTNING_BULLET_DESC;

typedef struct tagSortDesc
{
	_float fDis;
	_float3 vPos;
	class CGameObject* pTarget = nullptr;

}SORT_DESC;

class CLightningTower_Bullet final : public CBullet
{
private:
	enum LIGHTNING_TRI
	{
		LIGHTNING_TRI_SPREAD, LIGHTNING_TRI_SPIN, LIGHTNING_TRI_END
	};
private:
	explicit CLightningTower_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CLightningTower_Bullet(const CLightningTower_Bullet& rhs);
	virtual ~CLightningTower_Bullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta)		override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT		Ready_Component(void* pArg);
	void		Spawn_Check(_float TimeDelta);
	void		Create_Effect();
	void		Target_Check(_float TimeDelta);
	void		Move_Check(_float TimeDelta);
	//void		Tri_Position(_float TimeDelta);

//private:
//	void Hit_Check();

	// Âî¸´Âî¸´ Àü±â¸¸
private:
	_int				m_iBoundCount = 0;
	const _int			m_iBoundCount_Max = 3;

	_float3				m_vTargetPos[3];
	_bool				m_IsCopyMonster = false;
	_float				m_fMoveTime = 2.f;


private:
	//vector<class CLightningTower_Bullet_Effect*>	m_vecEffectMesh = { nullptr };
	
public:
	static CLightningTower_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __LIGHTNINGTOWER_BULLET_H__
#endif // !__LIGHTNINGTOWER_BULLET_H__
