#include "stdafx.h"
#include "..\public\Boss_Djinn.h"
#include "Data_Manager.h"
#include "Masking_MeterBar_3D.h"

CBoss_Djinn::CBoss_Djinn(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CMonster(pDevice, pDevice_Context)
{
}

CBoss_Djinn::CBoss_Djinn(const CBoss_Djinn & rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss_Djinn::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Djinn::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);
	Safe_Release(__super::m_pMeterBar_Hp);

	Ready_Component(pArg);

	m_pModelCom->Set_AnimationIndex(0);

	m_pModelCom->Set_AnimationIndex_Start(0.f, 58.f);

	Set_Pivot(XMVectorSet(0.05f, 0.05f, 0.05f, 0.f));

	return S_OK;
}

_int CBoss_Djinn::Tick(_float TimeDelta)
{
	m_pColliderCom_Attack->Set_NotCollide(true);
	Attack_Check();

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EDjinnAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{
				CData_Manager::GetInstance()->Add_MonsterCount();
				return OBJECT_DEAD;
			}
		}
		else
			m_eAnim_Next = EDjinnAnim::Death;
	}

	_vector vTargetPos;

	if (true == m_pModelCom->Get_IsFinishedAnimaion())
	{
		if (EDjinnAnim::Hurt == m_eAnim_Next)
		{
			m_IsHurt = false;
			m_eAnim_Next = EDjinnAnim::Idle;
		}
		m_IsHurt = false;
	}

	if (true == m_pColliderCom_Hurt->Get_IsCollide() || true == m_IsHurt)
	{
		m_IsAttack = false;
		m_pColliderCom_Hurt->Set_IsCollide(false);
		m_IsHurt = true;


		switch (m_pStatusCom->Get_DamageType())
		{
		case Engine::EDamageType::Shock:
			m_eAnim_Next = EDjinnAnim::Hurt;
			break;
		default:
			m_IsHurt = false;
			break;
		}
	}
	else
	{
		switch (__super::AI_Check(TimeDelta, &vTargetPos, m_IsAttack || m_IsHurt))
		{
		case Client::EMonsterAI::Idle:
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EDjinnAnim::Idle;
			break;
		case Client::EMonsterAI::Attack:
		{
			if (EDjinn_Attack::End == m_eAttack_Value)
				m_eAttack_Value = (EDjinn_Attack)(rand() % (_uint)EDjinn_Attack::End);
		
			m_IsAttack = true;
			break;
		}
			//case Client::EMonsterAI::Hurt:
			//	break;
		case Client::EMonsterAI::Dead:
			m_eAnim_Next = EDjinnAnim::Death;
			break;
		case Client::EMonsterAI::Shock:
			m_eAnim_Next = EDjinnAnim::Hurt;
			break;
		case Client::EMonsterAI::Move_Cell:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EDjinnAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Move_Target:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EDjinnAnim::Move_Forward;
			break;
		case Client::EMonsterAI::Turn:
			if (true == m_IsAttack)
				break;
			m_IsAttack = false;
			m_iAttackCount = 0;
			m_eAnim_Next = EDjinnAnim::Move_Forward;
			break;
		default:
			break;
		}
	}

	_matrix Matrix = m_pMovementCom->Get_WorldMatrix();
	m_pColliderCom_Hurt->Update_Collider(Matrix);



	m_pStatusCom->Tick(TimeDelta);

	__super::Tick(TimeDelta);

	return _int();
}

_int CBoss_Djinn::Late_Tick(_float TimeDelta)
{
	Anim_Check(TimeDelta);

	if (0 >= m_pStatusCom->Get_Hp())
	{
		if (EDjinnAnim::Death == m_eAnim_Next)
		{
			if (true == m_pModelCom->Get_IsFinishedAnimaion())
			{
				CData_Manager::GetInstance()->Set_BossDead();
				return OBJECT_DEAD;
			}
		}
		else
			m_eAnim_Next = EDjinnAnim::Death;
	}

	return __super::Late_Tick(TimeDelta);
}

HRESULT CBoss_Djinn::Render()
{
	if (nullptr == m_pModelCom)
		return S_OK;

	m_pModelCom->Bind_VIBuffer();

	m_pModelCom->Set_Variable("g_PivotMatrix", &XMMatrixTranspose(XMLoadFloat4x4(&m_PivotMatrix)), sizeof(_matrix));
	m_pModelCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pModelCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_VIEW_SPACE), sizeof(_matrix));
	m_pModelCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_PROJ_SPACE), sizeof(_matrix));

	LIGHT_DESC*		LightDesc = GET_GAMEINSTANCE->Get_LightDesc(0);

	m_pModelCom->Set_Variable("vLightPosition", &LightDesc->vPosition, sizeof(_float3));
	m_pModelCom->Set_Variable("fRange", &LightDesc->fRadius, sizeof(_float));

	m_pModelCom->Set_Variable("vLightDiffuse", &LightDesc->vDiffuse, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightAmbient", &LightDesc->vAmbient, sizeof(_float4));
	m_pModelCom->Set_Variable("vLightSpecular", &LightDesc->vSpecular, sizeof(_float4));

	m_pModelCom->Set_Variable("vCameraPosition", &GET_GAMEINSTANCE->Get_CamPosition(), sizeof(_vector));

	m_pModelCom->Set_ShaderResourceView_Direct("g_MaskingTexture", m_pTextureCom_Specular->Get_ShaderResourceView(0));
	m_pModelCom->Set_Variable("g_vColor", &m_vGemColor_Now, sizeof(_float4));

	_uint iNumMaterials = m_pModelCom->Get_NumMaterials();

	for (_uint i = 0; i < iNumMaterials; ++i)
	{
		if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_DIFFUSE)))
			return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_NORMALS)))
		//	return E_FAIL;
		//if (FAILED(m_pModelCom->Set_ShaderResourceView("g_DiffuseTexture", i, aiTextureType::aiTextureType_SPECULAR)))
		//	return E_FAIL;

		m_pModelCom->Render_Model(i, 6);
	}

#ifdef _DEBUG
	m_pColliderCom_Attack->Render_Collider();
	m_pColliderCom_Hurt->Render_Collider();
	m_pColliderCom_LeftHand->Render_Collider();
#endif // _DEBUG

	return S_OK;
}

void CBoss_Djinn::Anim_Check(_float TimeDelta)
{
	if (nullptr == m_pModelCom)
		return;

	if (m_eAnim_Cur != m_eAnim_Next)
		m_pModelCom->Set_AnimationIndex_Start((_float)m_eAnim_Next, Anim_Changer(m_eAnim_Next));

	m_pModelCom->Update_AnimaionMatrix(TimeDelta);

	m_pModelCom->Update_CombindTransformationMatrix();

	_matrix OffSet = XMMatrixTranslation(-100.f, 100.f, 0.f);
	_matrix BoneMatrix = OffSet * m_pModelCom->Get_BoneMatrix("joint24");
	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();
	WorldMatrix.r[3] += BoneMatrix.r[3] * 0.05f;
	m_pColliderCom_Attack->Update_Collider(WorldMatrix);



	// ¿Þ¼Õ
	OffSet = XMMatrixTranslation(65.f, 100.f, 0.f);
	BoneMatrix = OffSet * m_pModelCom->Get_BoneMatrix("HandL");
	WorldMatrix = m_pMovementCom->Get_WorldMatrix();
	WorldMatrix.r[3] += BoneMatrix.r[3] * 0.05f;

	m_pColliderCom_LeftHand->Update_Collider(WorldMatrix);

	m_eAnim_Cur = m_eAnim_Next;

	SetUp_GemColor();
}

void CBoss_Djinn::Attack_Check()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();

	if (true == m_IsAttack)
	{
		switch (m_eAttack_Value)
		{
		case Client::EDjinn_Attack::Attack:
			Attack_Default();
			break;
		case Client::EDjinn_Attack::EnergyBall:
			Attack_EnergyBall();
			break;
		case Client::EDjinn_Attack::WideRange:
			Attack_WideRange();
			break;
		case Client::EDjinn_Attack::TrapBall:
			Attack_TrapBall();
			break;
		case Client::EDjinn_Attack::RepeatBall:
			Attack_RepeatBall();
			break;
		default:
			m_eAttack_Value = EDjinn_Attack::End;
			m_iAttackCount = 0;
			m_IsAttack = false;
			break;
		}
	}
	else
	{
		m_eAttack_Value = EDjinn_Attack::End;
		m_iAttackCount = 0;
	}
}

HRESULT CBoss_Djinn::Ready_Component(void * pArg)
{
	HRESULT  hr = S_OK;

	COLLIDER_DESC Data;
	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 4.f, 4.f, 4.f };
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Hit"), (CComponent**)&m_pColliderCom_Hurt, &Data);

	ZeroMemory(&Data, sizeof(COLLIDER_DESC));
	Data.vScale = { 4.f, 4.f, 4.f };
	
	Data.Attack_Desc.eDamageType = EDamageType::Direct;
	Data.Attack_Desc.iDamage = 100;
	Data.IsCenter = true;
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack"), (CComponent**)&m_pColliderCom_Attack, &Data);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Collider_Sphere"), TEXT("Com_Collide_Attack_LeftHand"), (CComponent**)&m_pColliderCom_LeftHand, &Data);


	hr = CGameObject::Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_Boss_Djinn_Sepcualr"), TEXT("¹»¹ÙÂ¥»þ"), (CComponent**)&m_pTextureCom_Specular);

	if (hr != S_OK)
		MSG_BOX("CBoss_Djinn::Ready_Component");

	return hr;
}

_float CBoss_Djinn::Anim_Changer(EDjinnAnim eAnim)
{
	EDjinnAnim eAnim_Term = EDjinnAnim::End;

	switch (eAnim)
	{
	case Client::EDjinnAnim::Attack_1:
		eAnim_Term = EDjinnAnim::Charge;
		break;
	case Client::EDjinnAnim::Charge:
		eAnim_Term = EDjinnAnim::Coughing;
		break;
	case Client::EDjinnAnim::Coughing:
		eAnim_Term = EDjinnAnim::Death;
		break;
	case Client::EDjinnAnim::Death:
		eAnim_Term = EDjinnAnim::Hurt;
		break;
	case Client::EDjinnAnim::Hurt:
		eAnim_Term = EDjinnAnim::Idle;
		break;
	case Client::EDjinnAnim::Idle:
		eAnim_Term = EDjinnAnim::Interrupt;
		break;
	case Client::EDjinnAnim::Interrupt:
		eAnim_Term = EDjinnAnim::Move_Backward;
		break;
	case Client::EDjinnAnim::Move_Backward:
		eAnim_Term = EDjinnAnim::Move_Forward;
		break;
	case Client::EDjinnAnim::Move_Forward:
		eAnim_Term = EDjinnAnim::Move_Left;
		break;
	case Client::EDjinnAnim::Move_Left:
		eAnim_Term = EDjinnAnim::Move_Right;
		break;
	case Client::EDjinnAnim::Move_Right:
		eAnim_Term = EDjinnAnim::Move_Up;
		break;
	case Client::EDjinnAnim::Move_Up:
		eAnim_Term = EDjinnAnim::Shock;
		break;
	case Client::EDjinnAnim::Shock:
		eAnim_Term = EDjinnAnim::Spell_1_Enter;
		break;
	case Client::EDjinnAnim::Spell_1_Enter:
		eAnim_Term = EDjinnAnim::Spell_1_Enter_Alt;
		break;
	case Client::EDjinnAnim::Spell_1_Enter_Alt:
		eAnim_Term = EDjinnAnim::Spell_1_Loop;
		break;
	case Client::EDjinnAnim::Spell_1_Loop:
		eAnim_Term = EDjinnAnim::Spell_1_Loop_Alt;
		break;
	case Client::EDjinnAnim::Spell_1_Loop_Alt:
		eAnim_Term = EDjinnAnim::Spell_1_Start;
		break;
	case Client::EDjinnAnim::Spell_1_Start:
		eAnim_Term = EDjinnAnim::Spell_1_Stop;
		break;
	case Client::EDjinnAnim::Spell_1_Stop:
		eAnim_Term = EDjinnAnim::Spell_2_Loop;
		break;
	case Client::EDjinnAnim::Spell_2_Loop:
		eAnim_Term = EDjinnAnim::Spell_2_Start;
		break;
	case Client::EDjinnAnim::Spell_2_Start:
		eAnim_Term = EDjinnAnim::Spell_2_Stop;
		break;
	case Client::EDjinnAnim::Spell_2_Stop:
		eAnim_Term = EDjinnAnim::Turn_Left;
		break;
	case Client::EDjinnAnim::Turn_Left:
		eAnim_Term = EDjinnAnim::Turn_Right;
		break;
	case Client::EDjinnAnim::Turn_Right:
		eAnim_Term = EDjinnAnim::End_Anim;
		break;
	default:
		break;
	}

	_float fAnim = (_float)eAnim_Term - (_float)m_eAnim_Next - 1.f;

	return fAnim;
}

void CBoss_Djinn::AI_Check()
{
}

void CBoss_Djinn::SetUp_GemColor()
{
	m_vGemColor[(_uint)EDjinn_Attack::Attack]		= { 1.0f, 1.0f, 1.0f, 1.f };
	m_vGemColor[(_uint)EDjinn_Attack::EnergyBall]	= { 2.0f, 1.8f, 0.0f, 1.f };
	m_vGemColor[(_uint)EDjinn_Attack::WideRange]	= { 1.5f, 0.2f, 0.2f, 0.f };
	m_vGemColor[(_uint)EDjinn_Attack::TrapBall]		= { 0.8f, 2.0f, 0.8f, 0.f };
	m_vGemColor[(_uint)EDjinn_Attack::RepeatBall]	= { 0.8f, 0.0f, 1.7f ,0.f };
}

void CBoss_Djinn::Attack_Default()
{
	_bool IsFinished	= m_pModelCom->Get_IsFinishedAnimaion();
	_uint iAnimTime		= (_uint)m_pModelCom->Get_AnimTime();

	switch (m_iAttackCount)
	{
	case 0:
	{
		m_eAnim_Next = EDjinnAnim::Attack_1;

		if (EDjinnAnim::Attack_1 == m_eAnim_Next && 40 == iAnimTime)
			m_pColliderCom_Attack->Set_NotCollide(false);

		if (true == IsFinished)
			++m_iAttackCount;
	}
		break;
	default:
		m_iAttackCount = 0;
		m_IsAttack = false;
		m_eAttack_Value = EDjinn_Attack::End;
		break;
	}
}

void CBoss_Djinn::Attack_EnergyBall()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	_uint iAnimTime = (_uint)m_pModelCom->Get_AnimTime();

	switch (m_iAttackCount)
	{
	case 0:
		m_eAnim_Next = EDjinnAnim::Spell_2_Start;
		
		if (true == IsFinished)
			++m_iAttackCount;

		break;
	case 1:
	{
		
	}
		break;
	default:
		m_iAttackCount = 0;
		m_IsAttack = false;
		m_eAttack_Value = EDjinn_Attack::End;
		break;
	}
}

void CBoss_Djinn::Attack_WideRange()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	_uint iAnimTime = (_uint)m_pModelCom->Get_AnimTime();

	switch (m_iAttackCount)
	{
	case 0:
		break;
	default:
		m_iAttackCount = 0;
		m_IsAttack = false;
		m_eAttack_Value = EDjinn_Attack::End;
		break;
	}
}

void CBoss_Djinn::Attack_TrapBall()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	_uint iAnimTime = (_uint)m_pModelCom->Get_AnimTime();

	switch (m_iAttackCount)
	{
	case 0:
		break;
	default:
		m_iAttackCount = 0;
		m_IsAttack = false;
		m_eAttack_Value = EDjinn_Attack::End;
		break;
	}
}

void CBoss_Djinn::Attack_RepeatBall()
{
	_bool IsFinished = m_pModelCom->Get_IsFinishedAnimaion();
	_uint iAnimTime = (_uint)m_pModelCom->Get_AnimTime();

	switch (m_iAttackCount)
	{
	case 0:
		break;
	default:
		m_iAttackCount = 0;
		m_IsAttack = false;
		m_eAttack_Value = EDjinn_Attack::End;
		break;
	}
}

CBoss_Djinn * CBoss_Djinn::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CBoss_Djinn* pInstance = new CBoss_Djinn(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CBoss_Djinn) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CBoss_Djinn::Clone_GameObject(void * pArg)
{
	CBoss_Djinn* pInstance = new CBoss_Djinn(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CBoss_Djinn) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CBoss_Djinn::Free()
{
	Safe_Release(m_pColliderCom_Hurt);
	Safe_Release(m_pColliderCom_Attack);
	Safe_Release(m_pTextureCom_Specular);

	Safe_Release(m_pColliderCom_LeftHand);

	__super::Free();
}
