#pragma once

#ifndef __POINT_EX_HEALING_H__

#include "Point_Ex_Trail.h"

BEGIN(Client)
typedef struct tagPointHeal_Desc
{
	POINT_TRAIL_EX_DESC Point_Desc;
	_tchar szBuffTarget[MAX_PATH] = L"";
}POINT_EX_HEAL_DESC;

class CPoint_Ex_Healing final : public CPoint_Ex_Trail
{
protected:
	explicit CPoint_Ex_Healing(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Ex_Healing(const CPoint_Ex_Healing& rhs);
	virtual ~CPoint_Ex_Healing() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_LifeTime(_float fTime) { m_PointDesc.fLifeTime = fTime, m_fTime = 0.f; }
	void Set_SpreadDis(_float fDis) { m_PointDesc.fSpreadDis = fDis; }

public:
	void SetUp_Dir_Up(_int iRandNum_Max);
	_tchar m_szBuffTarget[MAX_PATH] = L"";
	_float m_fTime = 0.f;



public:
	static CPoint_Ex_Healing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Ex_Healing* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __POINT_EX_HEALING_H__
#endif // !__POINT_EX_HEALING_H__
