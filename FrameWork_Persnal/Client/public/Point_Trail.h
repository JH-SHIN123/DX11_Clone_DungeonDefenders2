#pragma once

#ifndef __POINT_TRAIL_H__

#include "GameObject.h"
#include "Client_Defines.h"
#include "EffectDesc_Manager.h"

BEGIN(Client)
typedef struct tagPointTrail_Desc
{
	_bool IsTime = false;
	_float fSize = 5.f;
	_float fTimeTerm = 0.5f; // Pos갱신 주기
	_int iShaderPass = 0;
	_float3 vDir = { 0.f,0.f,0.f };
	_float3 vColor = { 1.f, 1.f, 1.f };
	_float fSpreadDis = 5.f;
	_float fLifeTime = 10.f;
	_tchar szTextrueName[MAX_PATH] = L"";
	_tchar szPointInstance_PrototypeName[MAX_PATH] = L"";
	EInstanceValue InstanceValue = EInstanceValue::Point_100_10;

	MOVESTATE_DESC MoveDesc;

}POINT_TRAIL_DESC;

class CPoint_Trail final : public CGameObject
{
protected:
	explicit CPoint_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Trail(const CPoint_Trail& rhs);
	virtual ~CPoint_Trail() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Move_Spread(_float TimeDelta);

public:
	VTXMATRIX* Get_InstanceBuffer();
	void Set_InstanceBuffer(VTXMATRIX* pInstanceBuffer);
	_int Get_Instance_StartIndex() { return m_iInstance_StartIndex; }
	_int Get_Instance_Num() { return m_iNumInstance; }

public:
	HRESULT Ready_Component(void* pArg);
	void Set_Pos(_fvector vPos);
	void SetUp_IndexDir(_int iRandNum_Max = 10);
	void SetUp_Color(_float4 vColor) { m_vColor = vColor, m_IsColor = true; }
	void Set_ShaderPass(_uint iShaderPass) { m_PointDesc.iShaderPass = iShaderPass; }

protected:
	CVIBuffer_PointInstance*	m_pBufferInstanceCom = nullptr;
	CTextures*					m_pTexturesCom = nullptr;
	CMovement*					m_pMovementCom = nullptr;
	CRenderer*					m_pRendererCom = nullptr;

protected:
	_float3*					m_pIndexDir = nullptr;
	_int						m_iNumInstance = 0; // 어디 부터 그려
	_int						m_iInstance_StartIndex; // 그릴 개수
	POINT_TRAIL_DESC			m_PointDesc;

	_float4			m_vColor;
	_bool			m_IsColor = false;

protected:
	_float4*		m_pIndexPos = nullptr;
	_float*			m_pTimeBuffer = nullptr; // 일정 시간 주기로 Pos를 갱신하자

public:
	static CPoint_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Trail* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __POINT_TRAIL_H__
#endif // !__POINT_TRAIL_H__
