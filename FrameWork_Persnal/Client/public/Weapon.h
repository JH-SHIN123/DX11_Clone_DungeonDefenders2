#pragma once

#ifndef __WEAPON_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagWeaponDesc
{
	_tchar			szModelName[MAX_PATH] = L"";
	_tchar			szTextureName[MAX_PATH] = L"";
	ELevel			eTextureLevel = ELevel::Stage1;
	MOVESTATE_DESC	MoveState_Desc;
}WEAPON_DESC;

class CWeapon abstract : public CGameObject
{
protected:
	explicit CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Weapon_Equip(_fmatrix BoneMatrix, _fmatrix ParentMatrix);
	void Set_RotateMatrix(_fmatrix RotateMatrix);
	void Set_OffSetPosition(_fmatrix vPosition);

public:
	void Create_Bullet();

private:
	HRESULT			Ready_Component(void* pArg);

protected:
	CRenderer*				m_pRendererCom	= nullptr;
	CMovement*				m_pMovementCom	= nullptr;
	CModel*					m_pModelCom		= nullptr;
	CTextures*				m_pTextureCom	= nullptr;

	_float4x4			m_RotateMatrix;
	_float4x4			m_OffSetMatrix;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __WEAPON_H__
#endif // !__WEAPON_H__
