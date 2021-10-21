#pragma once

#ifndef __POINT_SPREAD_2_H__

#include "Point_Spread.h"
#include "EffectDesc_Manager.h"

BEGIN(Client)
// 이건 동적으로 퍼져 나가는거

typedef struct tagPointSpread2_Desc
{
	POINT_SPREAD_DESC Point_Desc;
	_float2 vSize = { 1.f,1.f };
	_bool IsTime = false;;
	_bool IsDown = false;
}POINT_SPREAD_DESC_2;

class CPoint_Spread2 final : public CPoint_Spread
{
protected:
	explicit CPoint_Spread2(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Spread2(const CPoint_Spread2& rhs);
	virtual ~CPoint_Spread2() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetUp_Size(_float2 vSize);

	_bool m_IsTime = false;
	_float m_fTime = 0.f;
	_bool m_IsDown = false;
	_float m_fDownTime = 0.f;


public:
	static CPoint_Spread2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Spread2* Clone_GameObject(void* pArg = nullptr) override; // POINT_SPREAD_DESC
	virtual void Free();
};

END
#define __POINT_SPREAD_2_H__
#endif // !__POINT_SPREAD_2_H__
