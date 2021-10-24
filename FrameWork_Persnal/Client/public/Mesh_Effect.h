#pragma once

#ifndef __MESH_EFFECT_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagMeshEffect_Desc
{
	_tchar			szModelName[MAX_PATH] = L"";
	_tchar			szTextureName[MAX_PATH] = L"";
	_float2			vTextureSize = { 1.f, 1.f };
	MOVESTATE_DESC	MoveState_Desc;
	_float3			vDir = { 0.f ,0.f,0.f};
	_float			fLifeTime = 5.f;
}MESH_EFFECT_DESC;

class CMesh_Effect final : public CGameObject
{
protected:
	explicit CMesh_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMesh_Effect(const CMesh_Effect& rhs);
	virtual ~CMesh_Effect() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT			Ready_Component(void* pArg);

protected:
	CRenderer*				m_pRendererCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;

	CModel*					m_pModelCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;


protected:
	_float		m_fLifeTime = 0.f;
	_float3		m_vGoDir;
	_bool		m_IsSetResource = false;
	_float2		m_vTextureSize = { 1.f, 1.f };

public:
	static CMesh_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MESH_EFFECT_H__
#endif // !__MESH_EFFECT_H__
