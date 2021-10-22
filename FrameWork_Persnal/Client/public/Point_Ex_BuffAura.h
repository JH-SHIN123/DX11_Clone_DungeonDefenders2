#pragma once

#ifndef __POINT_EX_BUFFAURA_H__

#include "Client_Defines.h"
#include "Point_Ex_Trail.h"

BEGIN(Client)
class CPoint_Ex_BuffAura final : public CPoint_Ex_Trail
{
protected:
	explicit CPoint_Ex_BuffAura(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CPoint_Ex_BuffAura(const CPoint_Ex_BuffAura& rhs);
	virtual ~CPoint_Ex_BuffAura() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetUp_Dir_Up(_int iRandNum_Max);

	_float m_fTime = 0.f;


public:
	static CPoint_Ex_BuffAura* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CPoint_Ex_BuffAura* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __POINT_EX_BUFFAURA_H__
#endif // !__POINT_EX_BUFFAURA_H__
