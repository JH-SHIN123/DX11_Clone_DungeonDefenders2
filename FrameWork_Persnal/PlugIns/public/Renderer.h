#pragma once

#ifndef __RENDERER_H__

#include "Component.h"

BEGIN(Engine)

enum class ENGINE_DLL ERenderGroup
{ Priority, NoneAlpha, Alpha, UI, AlphaUI, FrameUI, AlphaUI_Scecond, SceneChange, Cursor ,End};

class ENGINE_DLL CRenderer final : public CComponent
{
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	CRenderer(const CComponent& rhs);
	virtual ~CRenderer() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	HRESULT Add_GameObjectToRenderer(ERenderGroup eGroup, class CGameObject* pGameObject);
	HRESULT Draw_Renderer();

private:
	HRESULT	Render_Priority();
	HRESULT	Render_NoneAlpha();
	HRESULT	Render_Alpha();
	HRESULT	Render_UI();
	HRESULT	Render_AlphaUI();
	HRESULT	Render_FrameUI();
	HRESULT	Render_AlphaUI_Second();
	HRESULT	Render_SceneChange();
	HRESULT	Render_Cursor();


private:
	typedef list <class CGameObject*> RENDER_OBJECTS;
	RENDER_OBJECTS	m_pRenderObjects[(_uint)ERenderGroup::End]; // Warning C4251. 메모리가 DLL메모리가 아닌 Client의 메모리에 할당이 되기 때문 - 문제는 되지 않는다.

public:
	/* 원형생성. */
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);

	/* 복사본생성. */
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free();

};

END
#define __RENDERER_H__
#endif // !__RENDERER_H__
