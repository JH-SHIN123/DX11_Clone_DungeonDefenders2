#pragma once

#ifndef __LIGHTNINGTOWER_H__

#include "DefenceTower.h"

BEGIN(Client)
class CLightningTower final : public CDefenceTower
{
	enum class EAnim
	{
		FireLoop = 0,
		Idle = 17,
		End_Anim = 63,
		End = -1
	};
private:
	explicit CLightningTower(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CLightningTower(const CLightningTower& rhs);
	virtual ~CLightningTower() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT		Ready_Component(void* pArg);

private:
	void Attack_Check(_float TimeDelta);
	void Anim_Check();

private:
	EAnim			m_eAnim_Cur = EAnim::End;
	EAnim			m_eAnim_Next = EAnim::Idle;
	_bool			m_IsAttack = false;
	_bool			m_IsSpawnBall = false;
	_float			m_fAttCharging = 0.f;
	const _float	m_fAttStartTime = 1.5f;
	const _float	m_fAttEndTime = 1.f;

private:
	CModel*			m_pModelCom_LightingBall = nullptr;
	CMovement*		m_pMovmentCom_LightingBall = nullptr;

public:
	static	CLightningTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __LIGHTNINGTOWER_H__
#endif // !__LIGHTNINGTOWER_H__
