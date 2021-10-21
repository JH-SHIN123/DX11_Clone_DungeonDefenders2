#pragma once

#ifndef __POINT_SPREAD_3_H__

#include "Point_Spread.h"
#include "EffectDesc_Manager.h"

typedef struct tagPointSpread3_Desc
{
	POINT_SPREAD_DESC Point_Desc;
	_float2 vSize = { 1.f,1.f };
	_bool IsTime = false;;
	_bool IsDown = false;
}POINT_SPREAD_DESC_3;

class CPoint_Spread3 final : public CPoint_Spread
{
protected:
	explicit CPoint_Spread3(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Spread3(const CPoint_Spread3& rhs);
	virtual ~CPoint_Spread3() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	void SetUp_Size(_float2 vSize);
	void SetUp_Dir();

	_bool m_IsTime = false;
	_float m_fTime = 0.f;
	_bool m_IsDown = false;
	_float m_fDownTime = 0.f;


public:
	static CPoint_Spread3* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Spread3* Clone_GameObject(void* pArg = nullptr) override; // POINT_SPREAD_DESC
	virtual void Free();
};

#define __POINT_SPREAD_3_H__
#endif // !__POINT_SPREAD_3_H__
