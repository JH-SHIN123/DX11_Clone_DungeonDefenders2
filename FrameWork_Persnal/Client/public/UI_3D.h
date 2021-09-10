#pragma once

#ifndef __UI_3D_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CMovement;
class CTextures;
class CVIBuffer_Rect;
END

BEGIN(Client)
typedef struct tagUI3DDesc
{
	MOVESTATE_DESC	Movement_Desc;
	_tchar			szTextureName[MAX_PATH] = L"";	// 사용할 텍스처
	ELevel			eLevel = ELevel::Static;		// 텍스처 어따 만들었는지
}UI3D_DESC;

class CUI_3D abstract :	public CGameObject
{
protected:
	explicit CUI_3D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_3D(const CUI_3D& rhs);
	virtual ~CUI_3D() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg);				// Initialize Default Components
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;							// SetUp Default Spaces In Shader

protected:
	HRESULT SetUp_Default_Component(void* pArg);
	void	BillBoarding();

protected:
	CRenderer*				m_pRendererCom		= nullptr;
	CMovement*				m_pMovementCom		= nullptr;
	CTextures*				m_pTextureCom		= nullptr;
	CVIBuffer_Rect*			m_pBufferRectCom	= nullptr;

protected:
	UI3D_DESC		m_UI3D_Desc;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) PURE;
	virtual void Free() override;
};

END
#define __UI_3D_H__
#endif // !__UI_3D_H__
