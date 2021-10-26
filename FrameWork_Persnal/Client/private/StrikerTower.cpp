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
	m_pModelCom->Set_AnimationIndex_Start(85.f, 125.f - 86.f);

	Set_Pivot(XMVectorSet(0.035f, 0.035f, 0.035f, 0.f));

	m_eTowerRange = ETowerRange::Quarter;


	return S_OK;
}

_int CStrikerTower::Tick(_float TimeDelta)
{
	_int iReturn = 0;
	if (iReturn = __super::Tick(TimeDelta))
		return iReturn;

	if (true == m_pCollider_Hit->Get_IsCollide())
	{
		_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
		if (60.f > fDis)
		{
			fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;
			_int iRand = rand() % 4;
			CSound_Manager::GetInstance()->StopSound(CHANNEL_ANYTHING_2);
			
			switch (iRand)
			{
			case 0:
				CSound_Manager::GetInstance()->Play_Sound(L"TowerDamage1.ogg", CHANNEL_ANYTHING_2);
				break;
			case 1:
				CSound_Manager::GetInstance()->Play_Sound(L"TowerDamage2.ogg", CHANNEL_ANYTHING_2);
				break;
			case 2:
				CSound_Manager::GetInstance()->Play_Sound(L"TowerDamage3.ogg", CHANNEL_ANYTHING_2);
				break;
			default:
				CSound_Manager::GetInstance()->Play_Sound(L"TowerDamage4.ogg", CHANNEL_ANYTHING_2);
				break;
			}
			CSound_Manager::GetInstance()->Set_Volume(CHANNEL_ANYTHING_2, fDis + 0.2f);
		}


		m_pCollider_Hit->Set_IsCollide(false);
	}
	

	m_pCollider_Hit->Update_Collider(m_pMovementCom->Get_WorldMatrix());

	return iReturn;
}

_int CStrikerTower::Late_Tick(_float TimeDelta)
{
	//Attack_Check(TimeDelta);

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
		m_IsAttack = false;

	_vector vTargetPos;

	if (true == __super::Enemy_Check(TimeDelta, &vTargetPos))
	{
		m_eAnim_Next = CStrikerTower::EStrikerTowerAnim::Fire;

		_float fAnimTime = m_pModelCom->Get_AnimTime();

		if (false == m_IsAttack && 60 == (_uint)fAnimTime)
		{
			m_IsAttack = true;
			_vector vMyPos = m_pMovementCom->Get_State(EState::Position);
			vMyPos += XMVector3Normalize(m_pMovementCom->Get_State(EState::Up)) * 8.f;

			BULLET_DESC Data;
			lstrcpy(Data.szModelName, L"Component_Mesh_StrikerTower_Bullet");
			lstrcpy(Data.szTextureName, L"Component_Texture_Glow_Green");
			Data.vTextureSize = { 5.f, 5.f };
			Data.MoveState_Desc.fRotatePerSec = 50.f;

			vTargetPos.m128_f32[1] += 4.f;
			_vector vDir = XMVector3Normalize(vTargetPos - vMyPos);

			XMStoreFloat3(&Data.vDir, vDir);
			XMStoreFloat4(&Data.MoveState_Desc.vPos, vMyPos);
			Data.MoveState_Desc.vScale = { 1.f, 1.f, 1.f, 0.f };
			Data.MoveState_Desc.fSpeedPerSec = 40.f;
			Data.fLifeTime = 10.f;

			Data.Attack_Collide_Desc.Attack_Desc.eDamageType = EDamageType::Shock;
			Data.Attack_Collide_Desc.Attack_Desc.iDamage = 50;
			Data.Attack_Collide_Desc.Attack_Desc.fHitTime = 0.f;
			Data.Attack_Collide_Desc.vScale = { 2.f, 2.f, 2.f };
			//Data.Attack_Collide_Desc.vPosition = { 0.f, 50.f, 0.f };
			Data.Attack_Collide_Desc.IsCenter = true;

			GET_GAMEINSTANCE->Add_GameObject((_uint)ELevel::Stage1, L"Prototype_StrikerTower_Bullet", (_uint)ELevel::Stage1, L"Layer_Bullet", &Data);


			_float fDis = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));
			if (60.f > fDis)
			{
				fDis = CData_Manager::GetInstance()->Get_SoundVolume_Effect() / fDis;

				CSound_Manager::GetInstance()->StopSound(CHANNEL_TOWER_2);
				CSound_Manager::GetInstance()->Play_Sound(L"Tower_Fireball1.ogg", CHANNEL_TOWER_2);
				CSound_Manager::GetInstance()->Set_Volume(CHANNEL_TOWER_2, fDis + 0.2f);
			}
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

void CStrikerTower::Update_Anim(_float TimeDelta)
{
	m_pModelCom->Update_AnimaionMatrix(TimeDelta);
	m_pModelCom->Update_CombindTransformationMatrix();
}

HRESULT CStrikerTower::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 3.f, 3.f, 3.f };

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pCollider_Hit, &Data);

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
	Safe_Release(m_pCollider_Hit);

}
