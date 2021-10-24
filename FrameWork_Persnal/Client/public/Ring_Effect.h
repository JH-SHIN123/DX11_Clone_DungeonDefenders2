#pragma once

#ifndef __RING_EFFECT_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagRingEffect_Desc
{
	_tchar			szModelName[MAX_PATH] = L"";
	_tchar			szTextureName[MAX_PATH] = L"";
	_float2			vTextureSize = { 1.f, 1.f };
	MOVESTATE_DESC	MoveState_Desc;
	_float			fLifeTime = 5.f;
}RING_EFFECT_DESC;

class CRing_Effect final : public CGameObject
{
private:
	explicit CRing_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CRing_Effect(const CRing_Effect& rhs);
	virtual ~CRing_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_WorldMatrix(_fmatrix WorldMatrix);
	void Set_LifeTime(_float fTime);

private:
	HRESULT			Ready_Component(void* pArg);	

protected:
	CRenderer*				m_pRendererCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;
	CModel*					m_pModelCom = nullptr;

protected:
	_float		m_fLifeTime = 0.f;

public:
	static CRing_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __RING_EFFECT_H__
#endif // !__RING_EFFECT_H__
