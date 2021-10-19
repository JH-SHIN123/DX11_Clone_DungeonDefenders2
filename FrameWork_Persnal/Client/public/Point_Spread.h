#pragma once

#ifndef __POINT_SPREAD_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

typedef struct tagPointSpread_Desc
{
	_int iShaderPass = 0;
	_float3 vDir = {0.f,0.f,0.f};
	_float3 vColor = {1.f, 1.f, 1.f};
	_float fSpreadDis = 5.f;
	_float fLifeTime = 10.f;
	_tchar szTextrueName[MAX_PATH] = L"";
	_tchar szPointInstance_PrototypeName[MAX_PATH] = L"";
	MOVESTATE_DESC MoveDesc;

}POINT_SPREAD_DESC;

class CPoint_Spread final : public CGameObject
{
private:
	explicit CPoint_Spread(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Spread(const CPoint_Spread& rhs);
	virtual ~CPoint_Spread() = default;

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
	CVIBuffer_PointInstance*	m_pBufferInstanceCom	= nullptr;
	CRenderer*					m_pRendererCom = nullptr;
	CTextures*					m_pTexturesCom = nullptr;
	CMovement*					m_pMovementCom = nullptr;

	_float3*					m_pIndexDir	= nullptr;
	_int						m_iNumInstance = 0;
	POINT_SPREAD_DESC			m_PointDesc;

public:
	static CPoint_Spread* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Spread* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __POINT_SPREAD_H__
#endif // !__POINT_SPREAD_H__
