#include "stdafx.h"
#include "..\public\LightningTower.h"
#include "LightningTower_Bullet.h"

CLightningTower::CLightningTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CDefenceTower(pDevice, pDevice_Context)
{
}

CLightningTower::CLightningTower(const CLightningTower & rhs)
	: CDefenceTower(rhs)
{
}

HRESULT CLightningTower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CLightningTower::NativeConstruct(void * pArg)
{
	__super::Set_TowerRangeAngle(180.f,-180.f);

	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	m_pModelCom->Set_AnimationIndex(0); // 나는 애니메이션 하나에 다 있는 상황 원테이크
	m_pModelCom->Set_AnimationIndex_Start(17.f, 62.f - 18.f);

	Set_Pivot(XMVectorSet(0.04f, 0.04f, 0.04f, 0.f));

	return S_OK;
}

_int CLightningTower::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	m_pCollider_Hit->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return iReturn;
}

_int CLightningTower::Late_Tick(_float TimeDelta)
{
	Attack_Check(TimeDelta);

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	//if (true == m_pModelCom->Get_IsFinishedAnimaion())
	//	m_IsAttack = false;

	_vector vTargetPos;

	if (true == __super::Enemy_Check(TimeDelta, &vTargetPos))
	{
		m_eAnim_Next = CLightningTower::EAnim::FireLoop;

		_float fAnimTime = m_pModelCom->Get_AnimTime();


		if (m_fAttStartTime > m_fAttCharging && false == m_IsAttack)
		{
			m_fAttCharging += TimeDelta;
			


		}
		else if (true == m_IsAttack)
		{
			m_fAttCharging += TimeDelta;

			if (5.f <= m_fAttCharging)
			{
				m_fAttCharging = 0.f;

				_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
				vMyPos += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * 8.f;

				_vector vDir = vTargetPos;

				BULLET_DESC Data;
				lstrcpy(Data.szModelName, L"Component_Mesh_LightningTower_Bullet");
				Data.MoveState_Desc.fRotatePerSec = 10.f;
				XMStoreFloat3(&Data.vDir, vDir);
				XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
				Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
				Data.MoveState_Desc.fSpeedPerSec = 7.f;
				Data.fLifeTime = 10.f;
				
				Data.Attack_Collide_Desc.IsCenter = true;
				Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
				Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
				Data.Attack_Collide_Desc.Attack_Desc.iDamage = 10;

				LIGHTNING_BULLET_DESC Dat;
				Dat.Bullet_Desc = Data;
				Dat.iBoundCount = 0;

				GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_LightningTower_Bullet", (_uint)ELevel::Stage1, L"Layer_Bullet", &Dat);
			}


		}
		else
		{
			m_IsAttack = true;
			m_fAttCharging = 0.f;
		}
		
	}
	else
	{
		m_eAnim_Next = EAnim::Idle;
		m_IsAttack = false;
	}

	if (m_eAnim_Cur != m_eAnim_Next)
	{
		m_eAnim_Cur = m_eAnim_Next;
		EAnim eAnim;

		switch (m_eAnim_Next)
		{
		case EAnim::FireLoop:
			eAnim = EAnim::Idle;
			break;
		case EAnim::Idle:
			eAnim = EAnim::End_Anim;
			break;
		default:
			break;
		}
		_float fAnimTerm = ((_float)eAnim) - (_float)m_eAnim_Next - 1.f;
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, fAnimTerm);
	}


	m_pModelCom->Update_CombindTransformationMatrix();


	return __super::Late_Tick(TimeDelta);
}

HRESULT CLightningTower::Render()
{
	__super::Render();

	return S_OK;
}

void CLightningTower::Update_Anim(_float TimeDelta)
{
	m_pModelCom->Update_AnimaionMatrix(TimeDelta);
	m_pModelCom->Update_CombindTransformationMatrix();
}

HRESULT CLightningTower::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 3.f, 3.f, 3.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pCollider_Hit, &Data);

	if (S_OK != hr)
		MSG_BOX("CLightningTower::Ready_Component Failed!");

	return hr;
}

void CLightningTower::Attack_Check(_float TimeDelta)
{
}

void CLightningTower::Anim_Check()
{
}

CLightningTower * CLightningTower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CLightningTower*		pInstance = new CLightningTower(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CLightningTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CLightningTower::Clone_GameObject(void * pArg)
{
	CLightningTower*		pInstance = new CLightningTower(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CLightningTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CLightningTower::Free()
{
	Safe_Release(m_pCollider_Hit);

	__super::Free();
}
