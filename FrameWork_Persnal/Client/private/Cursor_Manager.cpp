#include "stdafx.h"
#include "..\public\Cursor_Manager.h"

IMPLEMENT_SINGLETON(CCursor_Manager)

CCursor_Manager::CCursor_Manager()
{
}

void CCursor_Manager::Get_MousePos_WorldSpace( _vector* vMouseWorldPos, _vector* vMouseWorldDir)
{
	POINT ptMouse = Get_Mouse();

	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	// 윈도우상의 마우스 좌표를 뷰포트까지
	_float	fMouseX, fMouseY;

	fMouseX = (ptMouse.x / (g_iWinCX * 0.5f)) - 1.f;
	fMouseY = ((g_iWinCY * 0.5f) / ptMouse.y) - 1.f;

	//_uint iNum = 0;
	//D3D11_VIEWPORT ViewPort;
	//pDevice_Constext->RSGetViewports(&iNum, &ViewPort);

	_vector vMousePos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	vMousePos = XMVectorSetX(vMousePos, fMouseX);// / (g_iWinCX  * 0.25f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, fMouseY);// / -(g_iWinCY * 0.25f) + 1.f);

	_matrix Proj = XMMatrixInverse(0, GET_PROJ_SPACE);
	vMousePos = XMVector3TransformCoord(vMousePos, Proj);
	_vector vRayPos = XMVectorSet(0.f,0.f,0.f,1.f);
	_vector vRayDir = vMousePos - vRayPos;


	_matrix View = XMMatrixInverse(0, GET_VIEW_SPACE);
	vRayPos = XMVector3TransformCoord(vRayPos, View);
	vRayDir = XMVector3TransformNormal(vRayDir, View);

	*vMouseWorldPos = vRayPos;
	*vMouseWorldDir = XMVector3Normalize(vRayDir);
	return;
}

HRESULT CCursor_Manager::Set_Cursor(CCursor * pCursor)
{
	m_Cursor = pCursor;

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
