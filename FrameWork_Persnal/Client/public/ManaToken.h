#pragma once

#ifndef __MANATOKEN_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

enum class EManaType
{Default, Large, End};

typedef struct tagManaToken_Desc
{
	EManaType  eManaType = EManaType::Default;
	MOVESTATE_DESC Move_Desc;
	COLLIDER_DESC Collider_Desc;
	_float3 vDir = { 0.f,0.f,0.f };
	_int iShaderPass = 1;
	
}MANATOKEN_DESC;

class CManaToken final : public CGameObject
{
private:
	explicit CManaToken(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CManaToken(const CManaToken& rhs);
	virtual ~CManaToken() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);

private:
	CRenderer*				m_pRendererCom			= nullptr;
	CMovement*				m_pMovementCom			= nullptr;
	CModel*					m_pModelCom				= nullptr;
	CCollider*				m_pColliderCom_Attack	= nullptr;

	CVIBuffer_Rect*			m_pBufferRectCom		= nullptr;
	CTextures*				m_pTextureCom			= nullptr;

private:
	_float3		vDir;
	EManaType	m_eManaType;
	_int		m_iShaderPass;

private:
	_float m_fDropTime = 1.5f;
	_float m_fMoveRatio = 0.f;
	_int m_iMp = 0;

public:
	static CManaToken* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MANATOKEN_H__
#endif // !__MANATOKEN_H__
