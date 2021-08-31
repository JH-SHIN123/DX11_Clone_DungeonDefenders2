#pragma once

#ifndef __MONSTER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Terrain;
class CTextures;
class CMovement;
END

BEGIN(Client)
class CMonster final : public CGameObject
{
protected:
	explicit CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Cube*			m_pVIBufferCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;

private:
	HRESULT	Ready_Component(void* pArg);

public:
	static CMonster* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __MONSTER_H__
#endif // !__MONSTER_H__
