#include "stdafx.h"
#include "..\public\StrikerTower.h"
#include "StrikerTower_Bullet.h"

CStrikerTower::CStrikerTower(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CDefenceTower(pDevice, pDevice_Context)
{
}

CStrikerTower::CStrikerTower(const CStrikerTower & rhs)
	: CDefenceTower(rhs)
{
}

HRESULT CStrikerTower::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CStrikerTower::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Ready_Component(pArg);

	m_pModelCom->Set_AnimationIndex(0); // 나는 애니메이션 하나에 다 있는 상황 원테이크
	m_pModelCom->Set_AnimationIndex_Start(0.f, (_float)m_eAnim_Next - 1.f);

	return S_OK;
}

_int CStrikerTower::Tick(_float TimeDelta)
{
	__super::Tick(TimeDelta);

	return _int();
}

_int CStrikerTower::Late_Tick(_float TimeDelta)
{
	Attack_Check(TimeDelta);

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
		m_IsAttack = false;

	if (true == __super::Enemy_Check(TimeDelta))
	{
		m_eAnim_Next = CStrikerTower::EStrikerTowerAnim::Fire;

		_float fAnimTime = m_pModelCom->Get_AnimTime();

		if (false == m_IsAttack && 60 == (_uint)fAnimTime)
		{
			m_IsAttack = true;
			_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
			vMyPos += m_pMovementCom->Get_State(EState::Up) * 250.f;

			BULLET_DESC Data;
			lstrcpy(Data.szModelName, L"Component_Mesh_BlockcadeTower_Bullet");
			Data.MoveState_Desc.fRotatePerSec = 50.f;
			XMStoreFloat3(&Data.vDir, m_pMovementCom->Get_State(EState::Look));
			XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
			Data.MoveState_Desc.vScale = { 0.01f, 0.01f, 0.01f, 0.f };
			Data.MoveState_Desc.fSpeedPerSec = 200.f;
			Data.fLifeTime = 10.f;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_StrikerTower_Bullet", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);
		}
	}
	else
	{
		m_eAnim_Next = CStrikerTower::EStrikerTowerAnim::Idle;
	}

	if (m_eAnim_Cur != m_eAnim_Next)
	{
		m_eAnim_Cur = m_eAnim_Next;
		EStrikerTowerAnim eAnim;

		switch (m_eAnim_Next)
		{
		case Client::CStrikerTower::EStrikerTowerAnim::Fire:
			eAnim = CStrikerTower::EStrikerTowerAnim::Idle;
			break;
		case Client::CStrikerTower::EStrikerTowerAnim::Idle:
			eAnim = CStrikerTower::EStrikerTowerAnim::End_Anim;
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

HRESULT CStrikerTower::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CStrikerTower::Ready_Component(void * pArg)
{
	return S_OK;
}

void CStrikerTower::Attack_Check(_float TimeDelta)
{
	if (m_fCoolTime == 0.f)
	{
		// 몬스터가 반경 안에 있다.

	}
}

void CStrikerTower::Anim_Check()
{
}

CStrikerTower * CStrikerTower::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CStrikerTower*		pInstance = new CStrikerTower(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CStrikerTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CStrikerTower::Clone_GameObject(void * pArg)
{
	CStrikerTower*		pInstance = new CStrikerTower(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Cloned Instance (CStrikerTower) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CStrikerTower::Free()
{
	__super::Free();
}
