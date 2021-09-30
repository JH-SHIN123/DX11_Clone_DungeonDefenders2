#include "stdafx.h"
#include "..\public\Fade.h"
#include "Level_Loading.h"
#include "Level_Logo.h"
#include "Level_Stage1.h"

CFade::CFade(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CFade::CFade(const CFade & rhs)
	: CGameObject(rhs)
{
}

HRESULT CFade::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CFade::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CFade::Tick(_float TimeDelta)
{
	if (false == m_IsFadeIn)
	{
		m_fTime -= TimeDelta;

	}
	else
	{
		m_fTime += TimeDelta;

	}


	return _int();
}

_int CFade::Late_Tick(_float TimeDelta)
{
	if (false == m_IsFadeIn)
	{
		if (-0.25f >= m_fTime)
			return OBJECT_DEAD;

	}
	else
	{
		if (1.25f <= m_fTime)
		{
			CGameInstance*		pGameInstance = CGameInstance::GetInstance();
			switch (m_eNowLevel)
			{
			case Client::ELevel::Logo:
				static_cast<CLevel_Logo*>(pGameInstance->Get_Scene())->Scene_Change(m_eNextLevel);
				break;
			case Client::ELevel::Stage1:
				static_cast<CLevel_Stage1*>(pGameInstance->Get_Scene())->Scene_Change(m_eNextLevel);
				break;
			case ELevel::Loading:
				static_cast<CLevel_Loading*>(pGameInstance->Get_Scene())->Set_RealFinish(true);
				break;
			default:
				break;
			}

			return OBJECT_DEAD;
		}
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::SceneChange, this);
}

HRESULT CFade::Render()
{
	m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	//g_MaskTexture g_TextureTime_UV
	m_pVIBufferCom->Set_Variable("g_AlphaTime", &m_fTime, sizeof(_float));
	m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTexturesCom->Get_ShaderResourceView(0));
	m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pVIBufferCom->Render(5);


	return S_OK;
}

HRESULT CFade::Ready_Component(void * pArg)
{
	HRESULT hr = S_OK;

	MOVESTATE_DESC Data;

	Data.vPos = { 0.f, 0.f, 0.f, 1.f };
	Data.fRotatePerSec = 1.5f;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Black"), TEXT("Com_Texture"), (CComponent**)&m_pTexturesCom);

	MOVESTATE_DESC Move;
	Move.vPos = { 0.f, 0.f,0.f,1.f };
	Move.vScale = { 1280.f, 720.f, 0.f, 0.f };
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &Move);

	if (nullptr != pArg)
	{
		FADE_DESC Data;
		memcpy(&Data, pArg, sizeof(FADE_DESC));

		m_eNextLevel = Data.eNextLevel;
		m_IsFadeIn = Data.IsFadeIn;
		m_eNowLevel = Data.eNowLevel;

		if (true == m_IsFadeIn)
			m_fTime = -0.25f;
	}

	return S_OK;
}

CFade * CFade::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CFade* pInstance = new CFade(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CFade) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CFade::Clone_GameObject(void * pArg)
{
	CFade* pInstance = new CFade(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CFade) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CFade::Free()
{
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTexturesCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pMovementCom);

	__super::Free();
}
