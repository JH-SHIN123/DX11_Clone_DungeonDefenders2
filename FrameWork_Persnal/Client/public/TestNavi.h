#pragma once

#ifndef __TESTNAVI_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CTestNavi final : public CGameObject
{
private:
	explicit CTestNavi(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CTestNavi(const CTestNavi& rhs);
	virtual ~CTestNavi() = default;
public:
	/* 원형객체를 초기화하낟. */
	virtual HRESULT NativeConstruct_Prototype() override;

	/* 복제된 객체를 초기화하낟. */
	virtual HRESULT NativeConstruct(void* pArg) override;

	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CRenderer*				m_pRendererCom = nullptr;
	CModel*					m_pModelCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	CTransform*				m_pTransformCom = nullptr;

private:
	CVIBuffer_Terrain*		m_pVIBufferCom = nullptr;

private:
	HRESULT	Ready_Component();

public:
	static CTestNavi* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __TESTNAVI_H__
#endif // !__TESTNAVI_H__
