#pragma once
#ifndef __CAMERA_TARGHET_H__


#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CCamera_Target final : public CCamera
{
private:
	CCamera_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CCamera_Target(const CCamera_Target& rhs);
	virtual ~CCamera_Target() = default;
public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;
	virtual _int	Tick(_float TimeDelta) override;
	virtual _int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Getter
	void MouseMove_Check();

private:
	HRESULT	Ready_Component();

private:

public:
	static CCamera_Target* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __CAMERA_TARGHET_H__
#endif // !__CAMERA_TARGHET_H__
