#pragma once

#ifndef __RECT_SPREAD_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagRectSpread_Desc
{
	_int iShaderPass = 0;
	_float3 vDir = { 0.f,0.f,0.f };
	_float3 vColor = { 1.f, 1.f, 1.f };
	_float fSpreadDis = 5.f;
	_float fLifeTime = 10.f;
	_tchar szTextrueName[MAX_PATH] = L"";
	_tchar szPointInstance_PrototypeName[MAX_PATH] = L"";
	MOVESTATE_DESC MoveDesc;

}RECT_SPREAD_DESC;

class CRect_Spread final : public CGameObject
{
private:
	explicit CRect_Spread(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CRect_Spread(const CRect_Spread& rhs);
	virtual ~CRect_Spread() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Component(void* pArg);
	void Set_Pos(_fvector vPos);

private:
	void SetUp_IndexDir(_int iInstanceIndex);
	void Check_Instance(_float TimeDelta);


private:
	CVIBuffer_RectInstance*		m_pBufferInstanceCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTextures*					m_pTexturesCom = nullptr;
	CMovement*					m_pMovementCom = nullptr;

	_float3*					m_pIndexDir = nullptr;
	_int						m_iNumInstance = 0;
	RECT_SPREAD_DESC			m_PointDesc;

public:
	static CRect_Spread* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CRect_Spread* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __RECT_SPREAD_H__
#endif // !__RECT_SPREAD_H__
