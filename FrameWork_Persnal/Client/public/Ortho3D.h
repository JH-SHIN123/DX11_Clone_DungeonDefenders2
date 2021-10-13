#pragma once

#ifndef __ORTHO3D_H__

#include "UI_3D.h"

BEGIN(Client)
class COrtho3D final : public CUI_3D
{
private:
	explicit COrtho3D(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit COrtho3D(const COrtho3D& rhs);
	virtual ~COrtho3D() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Pos(_fvector vPos) { m_pMovementCom->Set_State(EState::Position, vPos); }

private:
	_float4x4 Ortho3D_Matrix;

public:
	static COrtho3D* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, void * pArg);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __ORTHO3D_H__
#endif // !__ORTHO3D_H__
