#pragma once

#ifndef __HPMP_H__

#include "UI_2D.h"

BEGIN(Client)
class CHpMp final :	public CUI_2D
{
private:
	explicit CHpMp(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CHpMp(const CHpMp& rhs);
	virtual ~CHpMp() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void HpMp_Check(_float TimeDelta);

private:
	void Render_Hp();
	void Render_Mp();

private:
	HRESULT	Ready_Component(void* pArg);

private:
	CMovement*				m_pMovementCom_Mask = nullptr; // hp
	CMovement*				m_pMovementCom_MP = nullptr;
	CMovement*				m_pMovementCom_MP_Mask = nullptr; //mp

private:
	class CNumber_Font* m_pHpFont = nullptr;
	class CNumber_Font* m_pMpFont = nullptr;

private:
	_float		m_fDamage = 10.f;
	_float2		m_vInterval;
	_float		m_fTime = 0.f;

	_float		m_fHp = 80.f;
	_float		m_fHp_Max = 100.f;

	_float		m_fMp = 80.f;
	_float		m_fMp_Max = 100.f;

	_float		m_fHpScale = 117.f;


	// CUI_2D을(를) 통해 상속됨
public:
	static CHpMp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __HPMP_H__
#endif // !__HPMP_H__
