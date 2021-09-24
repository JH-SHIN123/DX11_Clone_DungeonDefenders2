#pragma once

#ifndef __CURSOR_MANAGER_H__

#include "Client_Defines.h"
#include "Cursor.h"


BEGIN(Client)
class CCursor_Manager final : public CBase
{
	DECLARE_SINGLETON(CCursor_Manager)

private:
	CCursor_Manager();
	virtual ~CCursor_Manager() = default;

public: // Getter
	POINT		Get_Mouse() const { return m_Cursor->Get_Mouse(); }
	_bool		Get_IsShowCursor(_bool IsShow) { return m_Cursor->Get_IsShowCursor(); }
	void	Get_MousePos_WorldSpace(ID3D11DeviceContext* pDevice_Constext, _vector* vMouseWorldPos, _vector* vMouseWorldDir);

public: // Setter
	HRESULT Set_Cursor(CCursor* pCursor);
	void	Set_ShowCursor(_bool IsShowCursor) { m_Cursor->Set_ShowCursor(IsShowCursor); }
	void	Set_MouseMode(EMouseMode eMode) { m_Cursor->Set_MouseMode(eMode); }
	void	Set_MouseTexture(EMouseTexture eMode) { m_Cursor->Set_MouseTexture(eMode); }

	

public:
	_int		Tick(_float TimeDelta);
	HRESULT		Render();

private:
	CCursor*	m_Cursor = nullptr;

public:
	virtual void Free() override;
};

END
#define __CURSOR_MANAGER_H__
#endif // !__CURSOR_MANAGER_H__
