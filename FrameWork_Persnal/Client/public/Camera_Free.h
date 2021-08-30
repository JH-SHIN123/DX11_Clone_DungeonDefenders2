#pragma once

#ifndef __CAMERA_FREE_H__

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Free final : public CCamera
{
private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CCamera_Free(const CCamera_Free& rhs);
	virtual ~CCamera_Free() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Component();

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};
END
#define __CAMERA_FREE_H__
#endif // __CAMERA_FREE_H__
