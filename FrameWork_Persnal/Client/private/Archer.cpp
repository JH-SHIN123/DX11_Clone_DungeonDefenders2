#include "stdafx.h"
#include "..\public\Archer.h"
#include "Archer_Arrow.h"

CArcher::CArcher(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CArcher::CArcher(const CArcher & rhs)
	: CMonster(rhs)
{
}

HRESULT CArcher::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CArcher::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	Set_HpBar_OffSet_Position(_float3(0.f, 12.f, 0.f));

	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 40.f);

	//Set_Pivot(XMVectorSet(0.3f, 0.3f, 0.3f, 0.f));

	return S_OK;
}

_int CArcher::Tick(_float TimeDelta)
{
	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EArcherAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
				return OBJECT_DEAD;
		}
	}

	_matrix Matrix = m_pMovementCom->Get_WorldMatrix();
	m_pColliderCom_Hurt->Update_Collider(Matrix);
	m_pStatusCom->Tick(TimeDelta);


	m_IsAttack = true;
	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EArcherAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EArcherAnim::Idle;
		}

	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;


		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Direct:
			m_eAnim_Next = EArcherAnim::Hurt;
			break;
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EArcherAnim::Shock;
			break;
		default:
			break;
		}
	}
	else
	{
		//switch (__super::AI_Check(TimeDelta, &vTargetPos))
		//{
		//case Client::EMonsterAI::Idle:
		//	m_IsAttack = false;
		//	m_eAnim_Next = EArcherAnim::Idle;
		//	break;
		//case Client::EMonsterAI::Attack:
		//	m_IsAttack = true;
		//	m_eAnim_Next = EArcherAnim::Attack;
		//	break;
		//case Client::EMonsterAI::Hurt:

		//	break;
		//case Client::EMonsterAI::Dead:
		//	m_eAnim_Next = EArcherAnim::Death;
		//	break;
		//case Client::EMonsterAI::Shock:
		//	break;
		//case Client::EMonsterAI::Move:
		//	m_IsAttack = false;
		//	m_eAnim_Next = EArcherAnim::Move_Forward;
		//	break;
		//case Client::EMonsterAI::Turn:
		//	m_IsAttack = false;
		//	m_eAnim_Next = EArcherAnim::Turn_Left;
		//	break;
		//default:
		//	break;
		//}
	}







	m_pColliderCom_Attack->Update_Collider(m_pMovementCom->Get_WorldMatrix());
	m_pColliderCom_Hurt->Update_Collider(m_pMovementCom->Get_State(EState::Position));



	__super::Tick(TimeDelta);
	return _int();
}

_int CArcher::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);
	Attack_Check();


	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (true == m_pModelCom->Get_IsFinishedAnimaion())
			return OBJECT_DEAD;
	}


	return __super::Late_Tick(TimeDelta);
}

HRESULT CArcher::Render()
{
	__super::Render();


#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();

#endif // _DEBUG


	return S_OK;
}

void CArcher::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();

	if (true == m_IsBowPulling)
	{
		_matrix Matrix = m_pModelCom->Get_BoneMatrix("b_FingersL");
		_float fX = XMVectorGetX(Matrix.r[3]);
		_float fY = XMVectorGetY(Matrix.r[3]);
		_float fZ = XMVectorGetZ(Matrix.r[3]);

		m_pModelCom->Set_CombindTransformationMatrix("b_BowString", XMMatrixTranslation(fX + 2.f, fY + 1.f, fZ) * Matrix);
	}

	m_eAnim_Cur = m_eAnim_Next;
}

void CArcher::Attack_Check()
{
	_uint iAnimTime = (_uint)m_pModelCom->Get_AnimTime();

	m_IsPickArrow = false;
	m_IsBowPulling = false;

	if (33 <= iAnimTime && iAnimTime <= 50)
		m_IsBowPulling = true;

	//if (5 <= iAnimTime && iAnimTime <= 50)
	//	m_IsBowPulling = true;


	if (true == m_IsAttack && 75 == iAnimTime)
	{
		_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
		vMyPos += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * 8.f;

		BULLET_DESC Data;
		lstrcpy(Data.szModelName, L"Component_Mesh_Archer_Arrow");
		Data.MoveState_Desc.fRotatePerSec = 50.f;

		_vector vDir = XMVector3Normalize(m_pMovementCom->Get_State(EState::Look));

		XMStoreFloat3(&Data.vDir, vDir);
		XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
		Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
		Data.MoveState_Desc.fSpeedPerSec = 10.f;
		Data.fLifeTime = 10.f;

		Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
		Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
		Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
		Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
		//Data.Attack_Collide_Desc.vPosition = { 0.f, 50.f, 0.f };
		Data.Attack_Collide_Desc.IsCenter = true;

		GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_Archer_Arrow", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);

	}
}

HRESULT CArcher::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 7.f, 7.f, 7.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Attack_Collider"), (CComponent**)&m_pColliderCom_Attack, &Data);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_AABB"), TEXT("Com_Hurt_Collider"), (CComponent**)&m_pColliderCom_Hurt, &Data);


	return S_OK;
}

_float CArcher::Anim_Changer(EArcherAnim eAnim)
{
	EArcherAnim eAnim_Term = EArcherAnim::End;

	switch (eAnim)
	{
	case EArcherAnim::Attack:
		eAnim_Term = EArcherAnim::Cough;
		break;
	case EArcherAnim::Cough:
		eAnim_Term = EArcherAnim::Death;
		break;
	case EArcherAnim::Death:
		eAnim_Term = EArcherAnim::Hurt;
		break;
	case EArcherAnim::Hurt:
		eAnim_Term = EArcherAnim::Idle;
		break;
	case EArcherAnim::Idle:
		eAnim_Term = EArcherAnim::Jump;
		break;
	case EArcherAnim::Jump:
		eAnim_Term = EArcherAnim::Move_Backward;
		break;
	case EArcherAnim::Move_Backward:
		eAnim_Term = EArcherAnim::Move_Forward;
		break;
	case EArcherAnim::Move_Forward:
		eAnim_Term = EArcherAnim::Move_Left;
		break;
	case EArcherAnim::Move_Left:
		eAnim_Term = EArcherAnim::Move_Right;
		break;
	case EArcherAnim::Move_Right:
		eAnim_Term = EArcherAnim::Shock;
		break;
	case EArcherAnim::Shock:
		eAnim_Term = EArcherAnim::Turn_Left;
		break;
	case EArcherAnim::Turn_Left:
		eAnim_Term = EArcherAnim::End_Anim;
		break;
	default:
		eAnim_Term = EArcherAnim::Attack;
		break;
	}

	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next - 1.f;

	return fAnim;
}

void CArcher::AI_Check()
{
}

CArcher * CArcher::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CArcher* pInstance = new CArcher(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CArcher) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CArcher::Clone_GameObject(void * pArg)
{
	CArcher* pInstance = new CArcher(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CArcher) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CArcher::Free()
{
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pColliderCom_Hurt);

	__super::Free();
}
