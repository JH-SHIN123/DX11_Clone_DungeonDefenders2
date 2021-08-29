#pragma once

#ifndef __PLAYER_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Terrain;
class CTextures;
class CMovement;
END

BEGIN(Client)

typedef struct tagPlayerStatusDesc
{
	


}PLAYER_DESC;


class CPlayer final : public CGameObject
{
protected:
	explicit CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Key_Check();

private:
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;

private:
	HRESULT	Ready_Component(void* pArg);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free() override;
};

END
#define __PLAYER_H__
#endif // !__PLAYER_H__
