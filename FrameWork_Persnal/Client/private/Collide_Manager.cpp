#include "stdafx.h"
#include "..\public\Collide_Manager.h"
#include "Collider.h"
#include "Status.h"
#include "Monster.h"
#include "Movement.h"

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
				// 맞았어!
				if (false == pSrcCol_Attack->Get_NotCollide())
				{
					CStatus* pDstStat = (CStatus*)Dst->Get_Component(L"Com_Status");

					if (nullptr == pDstStat)// || 0 >= pDstStat->Get_Hp())
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

void CCollide_Manager::Collide_Check_Poison(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
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
				static_cast<CMonster*>(Dst)->Set_Poison(true);
		}
	}
	return;
}

_bool CCollide_Manager::Collide_Check_Always(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
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

		if (nullptr == pSrcCol_Attack)
			continue;

		for (auto& Dst : Dst_Object) // Com_Collide_Hit
		{
			CCollider* pDstCol_Hit = (CCollider*)Dst->Get_Component(L"Com_Collide_Hit");

			if (nullptr == pDstCol_Hit)
				continue;

			if (true == pDstCol_Hit->Intersect_Collider(pSrcCol_Attack))
			{
				// 맞았어!
				if (false == pSrcCol_Attack->Get_NotCollide())
				{

					return true;
				}
			}
		}
	}
	return false;
}

void CCollide_Manager::Collide_Push(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
{
	CLayer* pDstLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eDstObjectLevel, szDstObjectLayer);
	if (nullptr == pDstLayer)
		return;

	CLayer* pSrcLayer = GET_GAMEINSTANCE->Get_Layer((_uint)eSrcObjectLevel, szSrcObjectLayer);
	if (nullptr == pSrcLayer)
		return;

	list<CGameObject*> Dst_Object = pDstLayer->Get_GameObject_List();
	list<CGameObject*> Src_Object = pSrcLayer->Get_GameObject_List();

	for (auto& Src : Src_Object)
	{
		CCollider* pSrcCol_Hit = (CCollider*)Src->Get_Component(L"Com_Collide_Push");

		if (true == Src->Get_Pushing() || nullptr == pSrcCol_Hit)
			continue;

		for (auto& Dst : Dst_Object)
		{
			if(Src == Dst)
				continue;

			CCollider* pDstCol_Hit = (CCollider*)Dst->Get_Component(L"Com_Collide_Push");

			if (true == Dst->Get_Pushing() || nullptr == pDstCol_Hit)
				continue;

			if (true == pDstCol_Hit->Intersect_Collider(pSrcCol_Hit))
			{
				CMovement* pMovmentCom_Src = (CMovement*)Src->Get_Component(L"Com_Movement");
				CMovement* pMovmentCom_Dst = (CMovement*)Dst->Get_Component(L"Com_Movement");

				_vector vPos_Src = pMovmentCom_Src->Get_State(EState::Position);
				_vector vPos_Dst = pMovmentCom_Dst->Get_State(EState::Position);

				// 밀어내기
				_vector vDir_Src = XMVector3Normalize(vPos_Dst - vPos_Src);
				_vector vPos_Src_Col = vPos_Src - (vDir_Src * pSrcCol_Hit->Get_Radius());
				_vector vPos_Dst_Col = (vDir_Src * pDstCol_Hit->Get_Radius()) + vPos_Dst;

				_vector vDir = (vPos_Dst_Col - vPos_Src_Col);
				_float fDis = XMVectorGetX(XMVector3Length(vDir)) * 0.5f;
				vDir = (XMVector3Normalize(vDir) /*+ XMVector3Normalize(pMovmentCom_Src->Get_State(EState::Look))*/) * 0.1f;
				vDir = XMVectorSetY(vDir, 0.f);
				vDir = XMVectorSetW(vDir, 0.f);

				vPos_Dst += vDir;
				vPos_Src -= vDir;

				pMovmentCom_Src->Set_State(EState::Position, vPos_Src);
				pMovmentCom_Dst->Set_State(EState::Position, vPos_Dst);
				_int i = 0;
				
			}
		}
	}

	return;
}

void CCollide_Manager::Collide_Push_Monster(const _tchar * szDstObjectLayer, ELevel eDstObjectLevel, const _tchar * szSrcObjectLayer, ELevel eSrcObjectLevel)
{
}

void CCollide_Manager::Free()
{
}
