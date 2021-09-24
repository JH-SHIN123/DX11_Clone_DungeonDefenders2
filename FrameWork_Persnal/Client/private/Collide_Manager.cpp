#include "stdafx.h"
#include "..\public\Collide_Manager.h"
#include "Collider.h"
#include "Status.h"

IMPLEMENT_SINGLETON(CCollide_Manager)

CCollide_Manager::CCollide_Manager()
{
}


CCollide_Manager::~CCollide_Manager()
{
}

_bool CCollide_Manager::Collide_Check(const _tchar * szDstObjectName, ELevel eDstObjectLevel, const _tchar * szSrcObjectName, ELevel eSrcObjectLevel)
{
	CLayer* pDstLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eDstObjectLevel, szDstObjectName);
	if (nullptr == pDstLayer)
		return false;

	CLayer* pSrcLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eSrcObjectLevel, szSrcObjectName);
	if (nullptr == pSrcLayer)
		return false;

	list<CGameObject*> Dst_Object = pDstLayer->Get_GameObject_List();
	list<CGameObject*> Src_Object = pDstLayer->Get_GameObject_List();

	for (auto& Dst : Dst_Object)
	{
		for (auto& Src : Src_Object)
		{

			return true;
		}
	}


	return false;
}










void CCollide_Manager::Free()
{
}
