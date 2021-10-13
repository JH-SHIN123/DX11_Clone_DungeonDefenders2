#include "stdafx.h"
#include "PhaseMonster_Info.h"

CPhaseMonster_Info::CPhaseMonster_Info(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_3D(pDevice, pDevice_Context)
{
}

CPhaseMonster_Info::CPhaseMonster_Info(const CPhaseMonster_Info & rhs)
	: CUI_3D(rhs)
{
}

HRESULT CPhaseMonster_Info::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CPhaseMonster_Info::NativeConstruct(void * pArg)
{
	__super::NativeConstruct(pArg);

	Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_MonsterIcon"), L"せせ", (CComponent**)&m_pTextureCom_Icon);
	Add_Component((_uint)ELevel::Stage1, TEXT("Component_Texture_MonsterIcon_Num"), L"せせせせ", (CComponent**)&m_pTextureCom_Num);

	return S_OK;
}

_int CPhaseMonster_Info::Tick(_float TimeDelta)
{
	return _int();
}

_int CPhaseMonster_Info::Late_Tick(_float TimeDelta)
{
	//__super::BillBoarding();

	_vector vCamLook = XMVector3Normalize(GET_CAMERA_LOOK);
	_vector vCamPos = GET_CAMERA_POSITION + vCamLook * 5.f;

	_float fCamDis_OffSet = XMVectorGetX(XMVector3Length(vCamPos - m_pMovementCom->Get_State(EState::Position)));
	_float fCamDis_Original = XMVectorGetX(XMVector3Length(GET_CAMERA_POSITION - m_pMovementCom->Get_State(EState::Position)));

	if (fCamDis_OffSet < fCamDis_Original)
		return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Priority_Second, this);

	return 0;
}

HRESULT CPhaseMonster_Info::Render()
{
	if (nullptr == m_pBufferRectCom)
		return E_FAIL;

	_matrix  Matrix_VP = GET_VIEW_SPACE * GET_PROJ_SPACE;
	_vector vPos = m_pMovementCom->Get_State(EState::Position);

	vPos = XMVector3TransformCoord(vPos, Matrix_VP);
	vPos.m128_f32[0] += 1.f;
	vPos.m128_f32[1] += 1.f;

	vPos.m128_f32[0] = (g_iWinCX * vPos.m128_f32[0]) / 2.f;
	vPos.m128_f32[1] = (g_iWinCY * (2.f - vPos.m128_f32[1])) / 2.f;

	vPos.m128_f32[0] = vPos.m128_f32[0] - (g_iWinCX / 2.f);
	vPos.m128_f32[1] = -vPos.m128_f32[1] + (g_iWinCY / 2.f);
	vPos.m128_f32[2] = 0.f;
	vPos.m128_f32[3] = 1.f;

	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();
	WorldMatrix.r[3] = vPos;

	_int iValue = m_iSpawnMonster_Value - 1;
	if (iValue == -1)
		iValue = 0;

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(iValue));
	m_pBufferRectCom->Render(20);

	(WorldMatrix.r[0]).m128_f32[0] = 32.f;
	(WorldMatrix.r[1]).m128_f32[1] = 32.f;
	(WorldMatrix.r[3]).m128_f32[0] -= 32.f;
	(WorldMatrix.r[3]).m128_f32[1] += 36.f;

	for (_int i = 0; i < m_iSpawnMonster_Value; ++i)
	{

		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Icon->Get_ShaderResourceView(m_iMonsterIcon[i]));
		m_pBufferRectCom->Render(20);

		_matrix NumMatrix = WorldMatrix;
		(NumMatrix.r[0]).m128_f32[0] = 28.f;
		(NumMatrix.r[1]).m128_f32[1] = 28.f;
		(NumMatrix.r[3]).m128_f32[0] += 50.f;
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(NumMatrix), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_Num->Get_ShaderResourceView(m_iMonsterCount[i]));
		m_pBufferRectCom->Render(20);

		(WorldMatrix.r[3]).m128_f32[1] -= 33.f;
	}

	return S_OK;
}

void CPhaseMonster_Info::Set_Pos(_fvector vPos)
{
	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CPhaseMonster_Info::Set_PhaseInfo(const PHASEINFO_DESC & PhaseInfo_Desc)
{
	memcpy(&m_Monster_Info, &PhaseInfo_Desc, sizeof(PHASEINFO_DESC));

	m_iSpawnMonster_Value = 0;
	for (_int i = 0; i < (_int)EMonster_List::End; ++i)
	{
		if (true == PhaseInfo_Desc.IsAddMonster[i])
			++m_iSpawnMonster_Value;

		m_iMonsterIcon[i] = 0;
	}

	Monster_Check();
}

void CPhaseMonster_Info::Monster_Check()
{
	_int iMonster = 0;
	for (_int i = 0; i < m_iSpawnMonster_Value; ++i)
	{
		if (true == m_Monster_Info.IsAddMonster[iMonster])
		{
			m_iMonsterIcon[iMonster] = iMonster;
			m_iMonsterCount[iMonster] = m_Monster_Info.iMonsterCount[i];
		}
		else
			--i;
		
		++iMonster;
	}
}

CPhaseMonster_Info * CPhaseMonster_Info::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void * pArg)
{
	CPhaseMonster_Info* pInstance = new CPhaseMonster_Info(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CMasking_MeterBar_3D) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CPhaseMonster_Info::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CPhaseMonster_Info::Free()
{
	__super::Free();
}
