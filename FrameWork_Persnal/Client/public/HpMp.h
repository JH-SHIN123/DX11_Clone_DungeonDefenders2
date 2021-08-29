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
	void Damage_Check();


private:
	HRESULT	Ready_Component(void* pArg);

private:
	CMovement*				m_pMovementCom_MP = nullptr;
	CVIBuffer_Rect*			m_pBufferRectCom_MP = nullptr; // 일단은 이거 없어도 잘 씀

	CMovement*				m_pMovementCom_Mask = nullptr;
	//CTextures*				m_pTextureCom_Mask = nullptr;

private:
	_float m_fDamage = 10.f;
	_float2		m_vInterval;


	// CUI_2D을(를) 통해 상속됨
public:
	static CHpMp* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __HPMP_H__
#endif // !__HPMP_H__
