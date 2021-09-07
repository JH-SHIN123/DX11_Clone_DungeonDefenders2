#pragma once

#ifndef __STATUS_H__

#include "Component.h"

BEGIN(Engine)

typedef struct tagStatusDesc
{
	_int	iHp;
	_int	iHp_Max;
	_int	iMp;
	_int	iMp_Max;
	_int	iAtt_Basic;	// 기초 뎀
	_int	iAtt_Final;	// 최종 뎀
	_float	fAttSpeed;	// 공속
	_float	fCritical;	// 크리
	_int	iExp;
	_int	iExp_Max;
}STATUS_DESC;

typedef struct tagHitDesc
{
	_int iDamage;
	_int iDamage_2 = 0;
	_float fHitTime;		// 무적시간
	EDamageType eDamageType;
}HIT_DESC;

enum class EDamageType
{Direct, Dot, Dot_Slow, End};

class CStatus final : public CComponent
{
private:
	explicit CStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CStatus(const CStatus& rhs);
	virtual ~CStatus() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	_uint	Tick(_float TimeDelta);

public:


public:
	void Set_Damage(const HIT_DESC& pDamage);

private:
	void Damage_Check(_float TimeDelta);

private:
	STATUS_DESC		m_Status_Desc;
	HIT_DESC		m_Hit_Desc;
	_bool			m_IsDead	= false;
	_bool			m_IsHit		= false;

public:
	static CStatus* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __STATUS_H__
#endif // !__STATUS_H__
