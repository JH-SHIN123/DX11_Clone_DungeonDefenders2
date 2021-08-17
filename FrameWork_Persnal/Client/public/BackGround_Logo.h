#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CTextures;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBackGround_Logo final : public CGameObject
{
public:
	CBackGround_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CBackGround_Logo(const CBackGround_Logo& rhs);
	virtual ~CBackGround_Logo() = default;
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
	CTextures*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pBuffer_Rect = nullptr;

private:
	_bool		m_IsReverse = false;

private:
	HRESULT	Ready_Component();


public:
	/* 원형객체를 생성. */
	static CBackGround_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);

	/* 복제하여 객체를 생성. */
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;

	virtual void Free();
};

END