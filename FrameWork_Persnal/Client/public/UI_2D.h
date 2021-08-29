#pragma once

#ifndef __UI_2D_H__

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CRenderer;
class CMovement;
class CTextures;
class CVIBuffer_Rect;
END

BEGIN(Client)
typedef struct tagUI2DDesc
{
	_float2 vPos;		// 최초 위치
	_float2 vScale;		// 최초 이미지 크기
	_bool	IsMove;		// 움직임?
	_float2 vMovePos;	// 움직인다면 어디까지 움직이게
	_tchar  szTextureName[MAX_PATH]; // 사용할 텍스처
}UI2D_DESC;

class CUI_2D abstract : public CGameObject
{
protected:
	explicit CUI_2D(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	explicit CUI_2D(const CUI_2D& rhs);
	virtual ~CUI_2D() = default;

public:
	virtual HRESULT NativeConstruct_Prototype() override;
	virtual HRESULT NativeConstruct(void* pArg, ELevel eLevel = ELevel::Static) ;
	virtual _int	Tick(_float TimeDelta) override;
	virtual	_int	Late_Tick(_float TimeDelta) override;
	virtual HRESULT Render() override;

public: // Setter
	void Set_IsRender(_bool IsRender) { m_IsRender = IsRender; }
	void Set_IsMove(_bool IsMove) { m_UI2D_Desc.IsMove = IsMove; }

protected: // 스무스 이동
	HRESULT SetUp_Default_Component(void* pArg, ELevel eLevel);

protected:
	CRenderer*				m_pRendererCom = nullptr;
	CMovement*				m_pMovementCom = nullptr;
	CTextures*				m_pTextureCom = nullptr;
	CVIBuffer_Rect*			m_pBufferRectCom = nullptr;

protected:
	UI2D_DESC		m_UI2D_Desc;
	_bool			m_IsRender = true;

public:
	virtual CGameObject* Clone_GameObject(void* pArg = nullptr) PURE; // pArg == CAMERA_DESC
	virtual void Free() override;

};

END
#define __UI_2D_H__
#endif // !__UI_2D_H__
