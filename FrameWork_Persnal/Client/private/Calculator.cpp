#include "stdafx.h"
#include "..\public\Calculator.h"

_vector CCalculator::Convert_LayToAim(_fvector vPos)
{
	POINT		ptMouse;

	ptMouse.x = (LONG)g_iWinCX >> 1;
	ptMouse.y = (LONG)g_iWinCY >> 1;

	ScreenToClient(g_hWnd, &ptMouse);

	D3D11_VIEWPORT ViewPort;
	GET_GAMEINSTANCE->Get_ViewPort(&ViewPort);

	// 윈 > 투영
	_vector vMousePos;
	vMousePos = XMVectorSet(ptMouse.x / (ViewPort.Width * 0.5f) - 1.f, ptMouse.y / -(ViewPort.Height * 0.5f) + 1.f, 0.f, 1.f);

	_matrix ViewMatrix, ProjMatrix;

	ProjMatrix = GET_PROJ_SPACE;
	ViewMatrix = GET_VIEW_SPACE;

	ProjMatrix = XMMatrixInverse(0, ProjMatrix);
	ViewMatrix = XMMatrixInverse(0, ViewMatrix);

	// 투영 > 뷰
	vMousePos = XMVector3TransformCoord(vMousePos, ProjMatrix);

	_vector vRayPos = vPos; // 광선을 발사하는 위치
	_vector vRayDir = vMousePos - vRayPos; // 마우스가 피킹된 지점을 향하는 방향벡터
	vRayDir = XMVectorSetW(vRayDir, 0.f);

	// 뷰 > 월드
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrix);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrix);



	return _vector();
}

void CCalculator::Convert_MouseRay(_fvector vPos, _vector * vOutDir)
{
	POINT		ptMouse;

	ptMouse.x = (LONG)g_iWinCX >> 1;
	ptMouse.y = (LONG)g_iWinCY >> 1;

	ScreenToClient(g_hWnd, &ptMouse);

	//D3D11_VIEWPORT ViewPort;
	//GET_GAMEINSTANCE->Get_ViewPort(&ViewPort);

	// 윈 > 투영
	_vector vMousePos;
	vMousePos = XMVectorSet(ptMouse.x / (g_iWinCX * 0.5f) - 1.f, ptMouse.y / -(g_iWinCY * 0.5f) + 1.f, 0.f, 1.f);

	_matrix ViewMatrix, ProjMatrix;

	ProjMatrix = GET_PROJ_SPACE;
	ViewMatrix = GET_VIEW_SPACE;

	ProjMatrix = XMMatrixInverse(0, ProjMatrix);
	ViewMatrix = XMMatrixInverse(0, ViewMatrix);

	// 투영 > 뷰
	vMousePos = XMVector3TransformCoord(vMousePos, ProjMatrix);

	_vector vRayPos = vPos; // 광선을 발사하는 위치
	_vector vRayDir = vMousePos - vRayPos; // 마우스가 피킹된 지점을 향하는 방향벡터
	vRayDir = XMVectorSetW(vRayDir, 0.f);

	// 뷰 > 월드
	vRayPos = XMVector3TransformCoord(vRayPos, ViewMatrix);
	vRayDir = XMVector3TransformNormal(vRayDir, ViewMatrix);

	*vOutDir = XMVector3Normalize(vRayDir);
}
