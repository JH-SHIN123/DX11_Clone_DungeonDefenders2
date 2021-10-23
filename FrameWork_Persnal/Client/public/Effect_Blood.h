#pragma once
#ifndef __EFFECT_BLOOD_H__

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CEffect_Blood final : public CGameObject
{
	explicit CEffect_Blood(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context);
	explicit CEffect_Blood(const CEffect_Blood& rhs);
	virtual ~CEffect_Blood() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);
	void BillBorad();

protected:
	CRenderer*		m_pRendererCom = nullptr;
	CMovement*		m_pMovementCom = nullptr;
	CTextures*		m_pTexturesCom = nullptr;
	CVIBuffer_Rect*	m_pVIBufferCom = nullptr;

protected:
	_uint			m_iShaderPass = 0;

public:
	static CEffect_Blood* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __EFFECT_BLOOD_H__
#endif // !__EFFECT_BLOOD_H__
