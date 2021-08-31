#pragma once
// 다른 스킬도 한번에 몰아 넣자 그냥

#ifndef __PLAYERSKILL_H__

#include "UI_2D.h"

BEGIN(Client)

enum class ECastType
{Buff, Build, Upgrade, End};

enum class EPhase
{ Build, Combat, End};

class CPlayerSkill final : public CUI_2D
{
private:
	explicit CPlayerSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPlayerSkill(const CPlayerSkill& rhs);
	virtual ~CPlayerSkill() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;

	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Setter
	void	Set_Cast(ECastType eCastType, _float fCastTime) { m_eCastType = eCastType, m_fCastTime = fCastTime; }
	_bool	Set_Skill_CoolDown(_uint iIndex, _float fCoolDownTime);

private:
	void CoolDown_Check(_float TimeDelta);

private:
	HRESULT	Ready_Component(void* pArg);
	HRESULT Render_SkillIcon();

private: // Skill Icon
	CMovement*		m_pMovementCom_Skill[10] = { nullptr };
	CTextures*		m_pTextureCom_CoolDown = nullptr;

	_bool			m_IsCoolDown[10] = { false };
	_float			m_fCoolDownTime[10] = { 0.f };
	_float			m_fCoolDownTimeMax[10] = { 0.f };

	_float			m_fUI_Interval = 67.f;
	_int			m_iSkillNum = 10;


private: // Skill Cast
	CTextures*			m_pTextureCom_Cast = nullptr;
	CTextures*			m_pTextureCom_Cast_Mask = nullptr;
	CTransform*			m_pTransformCom_Cast = nullptr;

	ECastType			m_eCastType = ECastType::End;
	_float				m_fCastTime = 3.f;
	_float				m_fCastTime_Now = 0.f;



public:
	static CPlayerSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};
END
#define __PLAYERSKILL_H__
#endif // !__PLAYERSKILL_H__
