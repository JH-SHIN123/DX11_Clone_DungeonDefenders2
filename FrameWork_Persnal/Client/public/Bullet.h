#pragma once

#ifndef __BULLET_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagBulletDesc
{
	_tchar			szModelName[MAX_PATH] = L"";
	_tchar			szTextureName[MAX_PATH] = L"";
	ELevel			eTextureLevel = ELevel::Stage1;
	STATUS_DESC		Stat_Desc;
	MOVESTATE_DESC	MoveState_Desc;
	_float3			vDir;
	_float			fLifeTime = 5.f;
}BULLET_DESC;

class CBullet abstract : public CGameObject
{
protected:
	explicit CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CBullet(const CBullet& rhs);
	virtual ~CBullet() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);

protected:
	CStatus*				m_pStatusCom		= nullptr;
	CRenderer*				m_pRendererCom		= nullptr;
	CMovement*				m_pMovementCom		= nullptr;
	CModel*					m_pModelCom			= nullptr;
	CVIBuffer_Rect*			m_pBufferRectCom	= nullptr;
	CTextures*				m_pTextureCom		= nullptr;


protected:
	_float		m_fLifeTime = 0.f;
	_float3		m_vGoDir;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __BULLET_H__
#endif // !__BULLET_H__
