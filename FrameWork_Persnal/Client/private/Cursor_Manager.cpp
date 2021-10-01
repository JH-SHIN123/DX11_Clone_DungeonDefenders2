#include "stdafx.h"
#include "..\public\Cursor_Manager.h"

IMPLEMENT_SINGLETON(CCursor_Manager)

CCursor_Manager::CCursor_Manager()
{
}

void CCursor_Manager::Get_MousePos_WorldSpace(ID3D11DeviceContext* pDevice_Constext, _vector* vMouseWorldPos, _vector* vMouseWorldDir)
{
	POINT ptMouse = Get_Mouse();


	_uint iNum = 0;
	D3D11_VIEWPORT ViewPort;
	pDevice_Constext->RSGetViewports(&iNum, &ViewPort);
	_vector vMousePos = XMVectorZero();
	_float fViewX = ptMouse.x / (g_iWinCX * 0.5f) - 1.f;
	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (g_iWinCX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(g_iWinCY * 0.5f) + 1.f);


	_matrix Proj = XMMatrixInverse(nullptr, GET_PROJ_SPACE);
	vMousePos = XMVector3TransformCoord(vMousePos, Proj);
	_vector vRayPos = XMVectorZero();
	_vector vRayDir = vMousePos - vRayPos;


	_matrix View = XMMatrixInverse(nullptr, GET_VIEW_SPACE);
	XMVector3TransformCoord(vRayPos, View);
	XMVector3TransformNormal(vRayDir, View);

	*vMouseWorldPos = vRayPos;
	*vMouseWorldDir = vRayDir;
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
