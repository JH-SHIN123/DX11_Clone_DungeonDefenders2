#pragma once

#ifndef __STRIKERTOWER_H__

#include "DefenceTower.h"

BEGIN(Client)
class CStrikerTower final :	public CDefenceTower
{
	enum class EStrikerTowerAnim
	{
		Fire = 0,
		Idle = 85,
		End_Anim = 125,
		End = -1
	};
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

public:
	void Update_Anim(_float TimeDelta);

protected:
	HRESULT		Ready_Component(void* pArg);

private:
	void Attack_Check(_float TimeDelta);
	void Anim_Check();

private:
	EStrikerTowerAnim	m_eAnim_Cur = EStrikerTowerAnim::End;
	EStrikerTowerAnim	m_eAnim_Next = EStrikerTowerAnim::Idle;
	_bool				m_IsAttack = false;
	_float				m_fCoolTime = 0.f;
	const _float		m_fAttCoolTime = 4.f;

public:
	static	CStrikerTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;

};

END
#define __STRIKERTOWER_H__
#endif // !__STRIKERTOWER_H__
