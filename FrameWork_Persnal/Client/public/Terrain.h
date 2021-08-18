#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CVIBuffer_Terrain;
class CTextures;
END

BEGIN(Client)

class CTerrain final : public CGameObject
{
public:
	explicit CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain() = default;
public:
	/* 원형객체를 초기화하낟. */
	virtual HRESULT NativeConstruct_Prototype() override;

	/* 복제된 객체를 초기화하낟. */
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_double TimeDelta) override;
	virtual _int Late_Tick(_double TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;

private:
	HRESULT	Ready_Component();

public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END