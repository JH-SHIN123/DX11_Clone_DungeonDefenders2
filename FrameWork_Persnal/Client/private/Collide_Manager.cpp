#include "stdafx.h"
#include "..\public\Collide_Manager.h"
#include "Collider.h"
#include "Status.h"
#include "Monster.h"

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


	for (auto& Src : Src_Object) // Com_Collide_Attack
	{
		CCollider* pSrcCol_Attack = (CCollider*)Src->Get_Component(L"Com_Collide_Attack");

		if (nullptr == pSrcCol_Attack || true == pSrcCol_Attack->Get_NotCollide())
			continue;

		for (auto& Dst : Dst_Object) // Com_Collide_Hit
		{
			CCollider* pDstCol_Hit = (CCollider*)Dst->Get_Component(L"Com_Collide_Hit");

			if (nullptr == pDstCol_Hit)
				continue;

			if (true == pDstCol_Hit->Intersect_Collider(pSrcCol_Attack))
			{
				// ¸Â¾Ò¾î!
				if (false == pSrcCol_Attack->Get_NotCollide())
				{
					CStatus* pDstStat = (CStatus*)Dst->Get_Component(L"Com_Status");

					if (nullptr == pDstStat || 0 >= pDstStat->Get_Hp())
						continue;

					ATTACK_DESC Data;
					Data.eDamageType = pSrcCol_Attack->Get_DamageType();
					Data.iDamage = pSrcCol_Attack->Get_Damage();
					Data.fHitTime = pSrcCol_Attack->Get_HitTime();
					pDstStat->Set_Damage(Data);

					return true;
				}
			}
		}
	}
	return false;
}

void CCollide_Manager::Collide_Check_BrainWash(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
{
	CLayer* pDstLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eDstObjectLevel, szDstObjectLayer);
	if (nullptr == pDstLayer)
		return;

	CLayer* pSrcLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eSrcObjectLevel, szSrcObjectLayer);
	if (nullptr == pSrcLayer)
		return;

	list<CGameObject*> Dst_Object = pDstLayer->Get_GameObject_List();
	list<CGameObject*> Src_Object = pSrcLayer->Get_GameObject_List();

	for (auto& Src : Src_Object) // Com_Collide_Attack
	{
		CCollider* pSrcCol_Attack = (CCollider*)Src->Get_Component(L"Com_Collide_Attack");

		if (nullptr == pSrcCol_Attack || true == pSrcCol_Attack->Get_NotCollide())
			continue;

		for (auto& Dst : Dst_Object) // Com_Collide_Hit
		{
			CCollider* pDstCol_Hit = (CCollider*)Dst->Get_Component(L"Com_Collide_Hit");

			if (nullptr == pDstCol_Hit)
				continue;


			if (true == pDstCol_Hit->Intersect_Collider(pSrcCol_Attack))
				static_cast<CMonster*>(Dst)->Set_IsBrainWash(true);
		}
	}


	return;
}

void CCollide_Manager::Free()
{
}
