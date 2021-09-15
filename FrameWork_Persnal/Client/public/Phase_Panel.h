#pragma once

#ifndef __PHASE_PANEL_H__

#include "UI_2D.h"

BEGIN(Client)

class CPhase_Panel final : public CUI_2D
{
private:
	explicit CPhase_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPhase_Panel(const CPhase_Panel& rhs);
	virtual ~CPhase_Panel() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component();
	void	Move_Check(_float TimeDelta);

private:
	_uint		m_iTextureIndex;
	_float		m_fTime = 1.2f;
	_bool		m_IsDisapeer = false;

public:
	static CPhase_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();

};

END
#define __PHASE_PANEL_H__
#endif // !__PHASE_PANEL_H__
