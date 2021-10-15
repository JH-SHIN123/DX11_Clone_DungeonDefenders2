#pragma once

#ifndef __EFFECT_SPRITES_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)
class CEffect_Sprites final : public CGameObject
{
protected:
	explicit CEffect_Sprites(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CEffect_Sprites(const CEffect_Sprites& rhs);
	virtual ~CEffect_Sprites() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;	
	virtual HRESULT NativeConstruct(void* pArg) override;	
	virtual _int Tick(_float TimeDelta) override;
	virtual _int Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_RectInstance* m_pBufferInstanceCom = nullptr;

public:
	static CEffect_Sprites* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	virtual CEffect_Sprites* Clone_GameObject(void* pArg = nullptr) override;
	virtual void Free();
};

END
#define __EFFECT_SPRITES_H__
#endif // !__EFFECT_SPRITES_H__
