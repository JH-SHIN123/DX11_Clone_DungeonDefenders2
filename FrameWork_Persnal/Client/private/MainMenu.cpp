#include "stdafx.h"
#include "..\public\MainMenu.h"
#include "Level_Loading.h"
#include "Level_Logo.h"
#include "Cursor_Manager.h"

CMainMenu::CMainMenu(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CMainMenu::CMainMenu(const CMainMenu & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CMainMenu::NativeConstruct_Prototype()
{
	__super::NativeConstruct_Prototype();

	return S_OK;
}

HRESULT CMainMenu::NativeConstruct(void * pArg)
{ 
	__super::NativeConstruct(pArg, ELevel::Logo);

	Ready_Component(pArg);

	return S_OK;
}

_int CMainMenu::Tick(_float TimeDelta)
{
	Key_Check();

	if (m_IsClick == true && m_IsMenu == false)
	{
		m_pMovementCom->Go_Up(TimeDelta);

		_float fY = XMVectorGetY(m_pMovementCom->Get_State(EState::Position));
		if (fY >= 220.f)
		{
			m_IsMenu = true;
			m_IsMove = true;
		}
	}

	if (m_IsClick == true)
	{
		m_fAlphaTime += TimeDelta;

		if (m_IsMove == true)
		{
			_vector vPos = XMVectorSet(0.f, 30.f, 0.f, 1.f);
			_float fLenght = XMVectorGetX(XMVector3Length(vPos - m_pMovementCom_UI[0]->Get_State(EState::Position)));
			if (fLenght >= 10.f)
				m_pMovementCom_UI[0]->Go_Dir(TimeDelta, vPos);

			vPos = XMVectorSet(0.f, -130.f, 0.f, 1.f);
			fLenght = XMVectorGetX(XMVector3Length(vPos - m_pMovementCom_UI[1]->Get_State(EState::Position)));
			if (fLenght >= 10.f)
				m_pMovementCom_UI[1]->Go_Dir(TimeDelta, vPos);

			vPos = XMVectorSet(0.f, -285.f, 0.f, 1.f);
			fLenght = XMVectorGetX(XMVector3Length(vPos - m_pMovementCom_UI[2]->Get_State(EState::Position)));
			if (fLenght >= 10.f)
				m_pMovementCom_UI[2]->Go_Dir(TimeDelta, vPos);

			vPos = XMVectorSet(400.f, 285.f, 0.f, 1.f);
			fLenght = XMVectorGetX(XMVector3Length(vPos - m_pMovementCom_UI[2]->Get_State(EState::Position)));
			if (fLenght >= 10.f)
				m_pMovementCom_UI[3]->Go_Dir(TimeDelta, vPos);

		}
	}

	Button_Position_Check(TimeDelta);

	return _int();
}

_int CMainMenu::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (m_IsNextScene == true)
	{
		CGameInstance*		pGameInstance = CGameInstance::GetInstance();

		static_cast<CLevel_Logo*>(pGameInstance->Get_Scene())->Scene_Change(ELevel::Stage1);

		return SCENE_CHANGE;
	}

	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::UI, this);
}

HRESULT CMainMenu::Render()
{
	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	if (m_IsClick == true)
	{
		m_pBufferRectCom->Set_Variable("g_AlphaTime", &m_fAlphaTime, sizeof(_float));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Board->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(0));
		m_pBufferRectCom->Render(6);

		for (_int i = 0; i < 4; ++i)
		{
			m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(2));
			m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI[i]->Get_WorldMatrix()), sizeof(_matrix));

			if (m_IsButtonPick[i] == true)
			{
				m_pBufferRectCom->Render(4);
				Render_Button_Info(i);
			}

			else
				m_pBufferRectCom->Render(1);
		}
	}

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	return S_OK;
}

HRESULT CMainMenu::Ready_Component(void * pArg)
{
	MOVESTATE_DESC Data;
	m_pMovementCom->Set_SpeedPerSec(300.f);

	if (FAILED(CGameObject::Add_Component((_uint)ELevel::Logo
		, TEXT("Component_Texture_Generic_Panel"), TEXT("Com_Texture_Generic_Panel"), (CComponent**)&m_pTextureCom_UI)))
		return E_FAIL;

	HRESULT hr = S_OK;

	// 백보드
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_Board"), (CComponent**)&m_pMovementCom_Board);
	m_pMovementCom_Board->Set_Scale(XMVectorSet(1024.f, 512.f, 0.f, 0.f));
	m_pMovementCom_Board->Set_State(EState::Position, XMVectorSet(0.f, -50.f, 0.f, 1.f));

	// 버튼들
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_0"), (CComponent**)&m_pMovementCom_UI[0]);
	m_pMovementCom_UI[0]->Set_Scale(XMVectorSet(512.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI[0]->Set_State(EState::Position, XMVectorSet(-200.f, -500.f, 0.f, 1.f));
	m_pMovementCom_UI[0]->Set_SpeedPerSec(670.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_1"), (CComponent**)&m_pMovementCom_UI[1]);
	m_pMovementCom_UI[1]->Set_Scale(XMVectorSet(512.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI[1]->Set_State(EState::Position, XMVectorSet(0.f, -800.f, 0.f, 1.f));
	m_pMovementCom_UI[1]->Set_SpeedPerSec(700.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_2"), (CComponent**)&m_pMovementCom_UI[2]);
	m_pMovementCom_UI[2]->Set_Scale(XMVectorSet(512.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI[2]->Set_State(EState::Position, XMVectorSet(0.f, -700.f, 0.f, 1.f));
	m_pMovementCom_UI[2]->Set_SpeedPerSec(400.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_3"), (CComponent**)&m_pMovementCom_UI[3]);
	m_pMovementCom_UI[3]->Set_Scale(XMVectorSet(512.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI[3]->Set_State(EState::Position, XMVectorSet(0.f, 600.f, 0.f, 1.f));
	m_pMovementCom_UI[3]->Set_SpeedPerSec(400.f);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_0"), (CComponent**)&m_pMovementCom_UI_Info[0]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_1"), (CComponent**)&m_pMovementCom_UI_Info[1]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_2"), (CComponent**)&m_pMovementCom_UI_Info[2]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_3"), (CComponent**)&m_pMovementCom_UI_Info[3]);


	return S_OK;
}

void CMainMenu::Key_Check()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_IsClick = true;
	}

	for (_int i = 0; i < 3; ++i)
	{
		if (m_IsButtonPick[i] == true)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				switch (i)
				{
				case 0: // 첫번째
					m_IsNextScene = true;
					break;
				default:
					break;
				}
			}
		}
	}
}

void CMainMenu::Button_Position_Check(_float TimeDelta)
{
	POINT pt = GET_CURSOR_XY;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (_int i = 0; i < 3; ++i) // 이거 잘 만져야 함
	{
		_float4 vMyPos;
		XMStoreFloat4(&vMyPos, m_pMovementCom_UI[i]->Get_State(EState::Position));
		_float2 vScale = { m_pMovementCom_UI[i]->Get_Scale(EState::Right), m_pMovementCom_UI[i]->Get_Scale(EState::Up) };
		vMyPos.x = (g_iWinCX * 0.5f) + vMyPos.x;
		vMyPos.y = (g_iWinCY * 0.5f) - vMyPos.y;

		m_tButtonRect[i].left	= (LONG)(vMyPos.x - (vScale.x * 0.5f));
		m_tButtonRect[i].top	= (LONG)(vMyPos.y - (vScale.y * 0.5f));
		m_tButtonRect[i].right	= (LONG)(vMyPos.x + (vScale.x * 0.5f));
		m_tButtonRect[i].bottom = (LONG)(vMyPos.y + (vScale.y * 0.5f));

		if (PtInRect(&m_tButtonRect[i], pt))
			m_IsButtonPick[i] = true;
		else
			m_IsButtonPick[i] = false;

		if (true == m_IsButtonPick[i])
		{
			m_fButtonScaleTime[i] += TimeDelta * 2.f;
			if (1.15f <= m_fButtonScaleTime[i])
				m_fButtonScaleTime[i] = 1.15f;
			m_pMovementCom_UI[i]->Set_Scale_Tick(m_fButtonScaleTime[i], XMVectorSet(520.f, 130.f, 1.f, 0.f), true);
		}
		else
		{
			m_fButtonScaleTime[i] -= TimeDelta * 2.f;
			if (1.f >= m_fButtonScaleTime[i])
				m_fButtonScaleTime[i] = 1.f;
			m_pMovementCom_UI[i]->Set_Scale_Tick(m_fButtonScaleTime[i], XMVectorSet(512.f, 128.f, 1.f, 0.f), false);
		}

		m_pMovementCom_UI_Info[i]->Set_Scale(XMVectorSet(256.f, 128.f, 0.f, 0.f));
		_vector vPos = m_pMovementCom_UI[i]->Get_State(EState::Position);
		_float fX = XMVectorGetX(vPos) + 420.f;
		vPos = XMVectorSetX(vPos, fX);
		m_pMovementCom_UI_Info[i]->Set_State(EState::Position, vPos);
	}
}

void CMainMenu::Render_Button_Info(_int iIndex)
{
	if (iIndex == 3)
		return;

	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(3));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Info[iIndex]->Get_WorldMatrix()), sizeof(_matrix));

	m_pBufferRectCom->Render(1);
}

CMainMenu * CMainMenu::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CMainMenu* pInstance = new CMainMenu(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CMainMenu) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMainMenu::Clone_GameObject(void * pArg)
{
	CMainMenu* pInstance = new CMainMenu(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CGameObject) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CMainMenu::Free()
{
	for (_int i = 0; i < 7; ++i)
	{
		Safe_Release(m_pMovementCom_UI[i]);
		Safe_Release(m_pMovementCom_UI_Info[i]);
	}
	Safe_Release(m_pMovementCom_Board);
	Safe_Release(m_pTextureCom_UI);

	__super::Free();
}
