#include "stdafx.h"
#include "..\public\Cursor_Manager.h"

IMPLEMENT_SINGLETON(CCursor_Manager)

CCursor_Manager::CCursor_Manager()
{
}

HRESULT CCursor_Manager::Set_Cursor(CCursor * pCursor)
{
	m_Cursor = pCursor; //원본 자체를 여기에 담을건디... 레퍼증가? 필요없겠다

	return S_OK;
}

_int CCursor_Manager::Tick(_float TimeDelta)
{
	m_Cursor->Tick(TimeDelta);

	m_Cursor->Late_Tick(TimeDelta);
	return _int();
}

HRESULT CCursor_Manager::Render()
{

	return S_OK;
}

void CCursor_Manager::Free()
{
	if (m_Cursor)
		Safe_Release(m_Cursor);
}
