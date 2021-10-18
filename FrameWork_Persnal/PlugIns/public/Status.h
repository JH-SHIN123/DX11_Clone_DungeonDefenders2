#pragma once

#ifndef __STATUS_H__

#include "Component.h"
#include "Collider.h"

BEGIN(Engine)

typedef struct tagStatusDesc
{
	_int	iHp;
	_int	iHp_Max;
	_int	iMp;
	_int	iMp_Max;
	_int	iAtt_Basic;	// 기초 뎀
	_float	fAttSpeed;	// 공속
	_float	fCritical;	// 크리
	_int	iLevel;
	_int	iExp;
	_int	iExp_Max;
}STATUS_DESC;

class ENGINE_DLL CStatus final : public CComponent
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
	_bool Level_Check();

public:
	//_int		Make_Final_Attack(_int iAtt_Basic); // 최종 데미지가 나온다
	//HIT_DESC	Make_Final_Damage(_int iFinalDamage, EDamageType eDamageType, _float fHitTime);

public: // Getter
	const _int Get_Hp()					const { return m_Status_Desc.iHp; }
	const _int Get_HpMax()				const { return m_Status_Desc.iHp_Max; }
	const _int Get_Mp()					const { return m_Status_Desc.iMp; }
	const _int Get_MpMax()				const { return m_Status_Desc.iMp_Max; }
	const _int Get_AttBasic()			const { return m_Status_Desc.iAtt_Basic; }
	const _int Get_Level()				const { return m_Status_Desc.iLevel; }
	const _int Get_Exp()				const { return m_Status_Desc.iExp; }
	const _int Get_ExpMax()				const { return m_Status_Desc.iExp_Max; }
	const STATUS_DESC Get_Stat()		const { return m_Status_Desc; }
	const EDamageType Get_DamageType()	const { return m_Hit_Desc.eDamageType; }

public: // Setter
	void Set_Damage(const ATTACK_DESC& pDamage);
	void Set_Damage_Dot(const ATTACK_DESC& pDamage);

	void Set_Hp(_int iHp);
	void Set_AttBasic(_int iAtt) { m_Status_Desc.iAtt_Basic = iAtt; }
	void Set_Stat(const STATUS_DESC& Status) { m_Status_Desc = Status; }

public: // Add
	void Add_Exp(_int iExp) { m_Status_Desc.iExp += iExp; }

private:
	void Damage_Check(_float TimeDelta);

private:
	STATUS_DESC		m_Status_Desc;
	ATTACK_DESC		m_Hit_Desc;
	_float			m_fDamageTick = 0.f;
	_bool			m_IsDead	= false;

public:
	static CStatus* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __STATUS_H__
#endif // !__STATUS_H__
