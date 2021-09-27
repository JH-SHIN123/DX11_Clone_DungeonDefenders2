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

_bool CCollide_Manager::Collide_Check(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
{
	CLayer* pDstLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eDstObjectLevel, szDstObjectLayer);
	if (nullptr == pDstLayer)
		return false;

	CLayer* pSrcLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eSrcObjectLevel, szSrcObjectLayer);
	if (nullptr == pSrcLayer)
		return false;

	list<CGameObject*> Dst_Object = pDstLayer->Get_GameObject_List();
	list<CGameObject*> Src_Object = pSrcLayer->Get_GameObject_List();

	for (auto& Dst : Dst_Object) // Com_Collide_Hit
	{
		CCollider* pDstCol_Hit = (CCollider*)Dst->Get_Component(L"Com_Collide_Hit");

		if (nullptr == pDstCol_Hit)
			continue;

		for (auto& Src : Src_Object) // Com_Collide_Attack
		{
			CCollider* pSrcCol_Attack = (CCollider*)Src->Get_Component(L"Com_Collide_Attack");

			if (nullptr == pSrcCol_Attack)
				continue;

			if (true == pDstCol_Hit->Intersect_Collider(pSrcCol_Attack))
			{
				// 맞았어!
				CStatus* pDstStat = (CStatus*)Dst->Get_Component(L"Com_Status");
				if (nullptr == pDstStat)
					return true;
				
				ATTACK_DESC Data;
				Data.eDamageType = pSrcCol_Attack->Get_DamageType();
				Data.iDamage = pSrcCol_Attack->Get_Damage();
				Data.fHitTime = pSrcCol_Attack->Get_HitTime();

				pDstStat->Set_Damage(Data);
				//pDstCol_Hit->
				// 데미지 계산은 스탯이 하지만 데미지의 정보는 콜라이더가 가지는 형식이다.


				//return true;
			}
		}
	}


	return false;
}










void CCollide_Manager::Free()
{
}
