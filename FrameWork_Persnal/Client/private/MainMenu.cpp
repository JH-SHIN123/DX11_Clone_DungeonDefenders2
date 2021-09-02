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
		}
	}

	Button_Position_Check(TimeDelta);

	Button_Select_Check(TimeDelta);

	return _int();
}

_int CMainMenu::Late_Tick(_float TimeDelta)
{
	__super::Late_Tick(TimeDelta);

	if (GetAsyncKeyState('R') & 0x8000)
		m_IsNextScene = true;

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

	if (m_IsClick == true) // 최초 클릭
	{
		m_pBufferRectCom->Set_Variable("g_AlphaTime", &m_fAlphaTime, sizeof(_float));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_Board->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(0));
		m_pBufferRectCom->Render(6);

		Render_Button();
	}

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(0));
	m_pBufferRectCom->Render(1);

	if (m_IsClick == false)
		return S_OK;

	Render_Button_Select(m_eButtonSelect);

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
	m_pMovementCom_UI[3]->Set_Scale(XMVectorSet(64.f, 64.f, 0.f, 0.f));
	m_pMovementCom_UI[3]->Set_State(EState::Position, XMVectorSet(600.f, 317.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_0"), (CComponent**)&m_pMovementCom_UI_Info[0]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_1"), (CComponent**)&m_pMovementCom_UI_Info[1]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_2"), (CComponent**)&m_pMovementCom_UI_Info[2]);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Info_3"), (CComponent**)&m_pMovementCom_UI_Info[3]);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Select_0"), (CComponent**)&m_pMovementCom_UI_Select[0]);
	m_pMovementCom_UI_Select[0]->Set_Scale(XMVectorSet(256.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI_Select[0]->Set_State(EState::Position, XMVectorSet(-340.f, -280.f, 0.f, 1.f));

	Ready_Button_Stage();

	return S_OK;
}

HRESULT CMainMenu::Ready_Button_Stage()
{
	HRESULT hr = S_OK;
	
	hr = CGameObject::Add_Component((_uint)ELevel::Logo, TEXT("Component_Texture_UI_TabMenu"), TEXT("Com_Texture_UI_TabMenu"), (CComponent**)&m_pTextureCom_UI_Stage_Button);
	hr = CGameObject::Add_Component((_uint)ELevel::Logo, TEXT("Component_Texture_UI_Stage_Select"), TEXT("Com_Texture_UI_Stage_PreView_0"), (CComponent**)&m_pTextureCom_UI_Stage_PreView[0]);
	hr = CGameObject::Add_Component((_uint)ELevel::Logo, TEXT("Component_Texture_UI_Stage_Select_Difficult"), TEXT("Com_Texture_UI_Stage_PreView_1"), (CComponent**)&m_pTextureCom_UI_Stage_PreView[1]);

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_0"), (CComponent**)&m_pMovementCom_UI_Stage_Button[0]);
	m_pMovementCom_UI_Stage_Button[0]->Set_Scale(XMVectorSet(256.f, 256.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_Button[0]->Set_State(EState::Position, XMVectorSet(-400.f, -40.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_1"), (CComponent**)&m_pMovementCom_UI_Stage_Button[1]);
	m_pMovementCom_UI_Stage_Button[1]->Set_Scale(XMVectorSet(256.f, 256.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_Button[1]->Set_State(EState::Position, XMVectorSet(-200.f, -40.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_2"), (CComponent**)&m_pMovementCom_UI_Stage_Button[2]);
	m_pMovementCom_UI_Stage_Button[2]->Set_Scale(XMVectorSet(256.f, 256.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_Button[2]->Set_State(EState::Position, XMVectorSet(0.f, -40.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_3"), (CComponent**)&m_pMovementCom_UI_Stage_Button[3]);
	m_pMovementCom_UI_Stage_Button[3]->Set_Scale(XMVectorSet(128.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_Button[3]->Set_State(EState::Position, XMVectorSet(200.f, 100.f, 0.f, 1.f));

	// preview
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_PreView_0"), (CComponent**)&m_pMovementCom_UI_Stage_PreView[0]);
	m_pMovementCom_UI_Stage_PreView[0]->Set_Scale(XMVectorSet(128.f, 128.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_PreView[0]->Set_State(EState::Position, XMVectorSet(200.f, 0.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_PreView_1"), (CComponent**)&m_pMovementCom_UI_Stage_PreView[1]);
	m_pMovementCom_UI_Stage_PreView[1]->Set_Scale(XMVectorSet(32.f, 32.f, 0.f, 0.f));
	m_pMovementCom_UI_Stage_PreView[1]->Set_State(EState::Position, XMVectorSet(250.f, 10.f, 0.f, 1.f));

//	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement_UI_Stage_PreView_3"), (CComponent**)&m_pMovementCom_UI_Stage_PreView[2]);
//	m_pMovementCom_UI_Stage_PreView[2]->Set_Scale(XMVectorSet(128.f, 128.f, 0.f, 0.f));
//	m_pMovementCom_UI_Stage_PreView[2]->Set_State(EState::Position, XMVectorSet(200.f, 0.f, 0.f, 1.f));



	return S_OK;
}

void CMainMenu::Key_Check()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_IsClick = true;
	}

	for (_int i = 0; i < 4; ++i)
	{
		//m_eButtonSelect = EButtonSelect::End;

		if (m_IsButtonPick[i] == true)
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				switch (i)
				{
				case 0:
					m_eButtonSelect = EButtonSelect::Stage;
					return;
				case 1:
					m_eButtonSelect = EButtonSelect::Option;
					return;
				case 2:
					m_eButtonSelect = EButtonSelect::HighScore;
					return;
				case 3:
					m_eButtonSelect = EButtonSelect::Exit;
					return;
				default:
					return;
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

	for (_int i = 0; i < 4; ++i) // 이거 잘 만져야 함
	{
		_float4 vMyPos;
		XMStoreFloat4(&vMyPos, m_pMovementCom_UI[i]->Get_State(EState::Position));
		_float2 vScale = { m_pMovementCom_UI[i]->Get_Scale(EState::Right), m_pMovementCom_UI[i]->Get_Scale(EState::Up) };
		vMyPos.x = (g_iWinCX * 0.5f) + vMyPos.x;
		vMyPos.y = (g_iWinCY * 0.5f) - vMyPos.y;

		m_tButtonRect[i].left = (LONG)(vMyPos.x - (vScale.x * 0.5f));
		m_tButtonRect[i].top = (LONG)(vMyPos.y - (vScale.y * 0.5f));
		m_tButtonRect[i].right = (LONG)(vMyPos.x + (vScale.x * 0.5f));
		m_tButtonRect[i].bottom = (LONG)(vMyPos.y + (vScale.y * 0.5f));

		if (i < 3)
		{
			if (PtInRect(&m_tButtonRect[i], pt))
			{
				m_IsButtonPick[i] = true;
				m_fButtonScaleTime[i] += TimeDelta * 2.f;
				if (1.15f <= m_fButtonScaleTime[i])
					m_fButtonScaleTime[i] = 1.15f;
				m_pMovementCom_UI[i]->Set_Scale_Tick(m_fButtonScaleTime[i], XMVectorSet(520.f, 130.f, 1.f, 0.f), true);
			}
			else
			{
				m_IsButtonPick[i] = false;
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

		else
		{
			if (PtInRect(&m_tButtonRect[3], pt)) // Exit Button
			{
				m_IsButtonPick[3] = true;
				m_fButtonScaleTime[3] -= TimeDelta * 2.f;
				if (1.f >= m_fButtonScaleTime[3])
					m_fButtonScaleTime[3] = 1.f;
				m_pMovementCom_UI[3]->Set_Scale_Tick(m_fButtonScaleTime[3], XMVectorSet(70.f, 70.f, 1.f, 0.f), true);
			}
			else
			{
				m_IsButtonPick[3] = false;
				m_fButtonScaleTime[3] -= TimeDelta * 2.f;
				if (1.f >= m_fButtonScaleTime[3])
					m_fButtonScaleTime[3] = 1.f;
				m_pMovementCom_UI[3]->Set_Scale_Tick(m_fButtonScaleTime[3], XMVectorSet(64.f, 64.f, 1.f, 0.f), false);
			}
		}
	}

	for (_int i = 0; i < 1; ++i) // 버튼 선택시
	{
		_float4 vMyPos;
		XMStoreFloat4(&vMyPos, m_pMovementCom_UI_Select[i]->Get_State(EState::Position));
		_float2 vScale = { m_pMovementCom_UI_Select[i]->Get_Scale(EState::Right), m_pMovementCom_UI_Select[i]->Get_Scale(EState::Up) };
		vMyPos.x = (g_iWinCX * 0.5f) + vMyPos.x;
		vMyPos.y = (g_iWinCY * 0.5f) - vMyPos.y;

		m_tButton_Select[i].left = (LONG)(vMyPos.x - (vScale.x * 0.5f));
		m_tButton_Select[i].top = (LONG)(vMyPos.y - (vScale.y * 0.5f));
		m_tButton_Select[i].right = (LONG)(vMyPos.x + (vScale.x * 0.5f));
		m_tButton_Select[i].bottom = (LONG)(vMyPos.y + (vScale.y * 0.5f));

		if (PtInRect(&m_tButton_Select[i], pt))
		{
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				m_eButtonSelect = EButtonSelect::End;
		}
		else
		{

		}
	}
}

void CMainMenu::Button_Select_Check(_float TimeDelta)
{
	switch (m_eButtonSelect)
	{
	case Client::EButtonSelect::Stage:
		Button_Stage_Check(TimeDelta);
		break;
	case Client::EButtonSelect::Option:
		break;
	case Client::EButtonSelect::HighScore:
		break;
	case Client::EButtonSelect::Exit:
		break;
	case Client::EButtonSelect::End:
		break;
	}
}

void CMainMenu::Button_Stage_Check(_float TimeDelta)
{
	// RECT 만들기
	POINT pt = GET_CURSOR_XY;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	for (_int i = 0; i < 4; ++i)
	{
		_float4 vMyPos;
		XMStoreFloat4(&vMyPos, m_pMovementCom_UI_Stage_Button[i]->Get_State(EState::Position));
		_float2 vScale = { m_pMovementCom_UI_Stage_Button[i]->Get_Scale(EState::Right), m_pMovementCom_UI_Stage_Button[i]->Get_Scale(EState::Up) };
		vMyPos.x = (g_iWinCX * 0.5f) + vMyPos.x;
		vMyPos.y = (g_iWinCY * 0.5f) - vMyPos.y;

		m_tSelect_Stage_Button[i].left = (LONG)(vMyPos.x - (vScale.x * 0.5f));
		m_tSelect_Stage_Button[i].top = (LONG)(vMyPos.y - (vScale.y * 0.5f));
		m_tSelect_Stage_Button[i].right = (LONG)(vMyPos.x + (vScale.x * 0.5f));
		m_tSelect_Stage_Button[i].bottom = (LONG)(vMyPos.y + (vScale.y * 0.5f));
	}

}

void CMainMenu::Render_Button()
{

	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI[3]->Get_WorldMatrix()), sizeof(_matrix));
	if (true == m_IsButtonPick[3])
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(4));
	else
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(5));

	m_pBufferRectCom->Render(6);

	if (EButtonSelect::End != m_eButtonSelect) // 새로운 창을 띄운 상태
		return;

	// 기본버튼
	for (_int i = 0; i < 3; ++i)
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

void CMainMenu::Render_Button_Info(_int iIndex)
{
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(3));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Info[iIndex]->Get_WorldMatrix()), sizeof(_matrix));

	m_pBufferRectCom->Render(1);
}

void CMainMenu::Render_Button_Select(EButtonSelect eSelect)
{
	if (EButtonSelect::Exit > eSelect) 
	{
		// 뒤로가기 버튼임
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI->Get_ShaderResourceView(1));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Select[0]->Get_WorldMatrix()), sizeof(_matrix));
		
		m_pBufferRectCom->Render(6);

		// 진짜 버튼
		switch (eSelect)
		{
		case Client::EButtonSelect::Stage:
			Render_Button_Stage();
			break;
		case Client::EButtonSelect::Option:
			break;
		case Client::EButtonSelect::HighScore:
			break;
		}
	}
}

void CMainMenu::Render_Button_Stage()
{
	// 스테이지 선택 버튼
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_Button->Get_ShaderResourceView(1));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_Button[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(6);

	// 난이도 선택
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_Button->Get_ShaderResourceView(2));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_Button[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(6);

	// 캐릭터 선택
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_Button->Get_ShaderResourceView(1));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_Button[2]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(6);

	// 프리뷰 렌더
	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_PreView[0]->Get_ShaderResourceView(m_StageMakeInfo.iStage));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_PreView[0]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(6);

	m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_PreView[1]->Get_ShaderResourceView(m_StageMakeInfo.iDifficult));
	m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_PreView[1]->Get_WorldMatrix()), sizeof(_matrix));
	m_pBufferRectCom->Render(6);

	//m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_UI_Stage_PreView[2]->Get_ShaderResourceView(m_StageMakeInfo.iClass));
	//m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom_UI_Stage_PreView[2]->Get_WorldMatrix()), sizeof(_matrix));
	//m_pBufferRectCom->Render(6);
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
	for (_int i = 0; i < 4; ++i)
	{
		Safe_Release(m_pMovementCom_UI[i]);
		Safe_Release(m_pMovementCom_UI_Info[i]);
		Safe_Release(m_pMovementCom_UI_Select[i]);
		Safe_Release(m_pMovementCom_UI_Stage_Button[i]);

		if (i < 2)
		{
			Safe_Release(m_pTextureCom_UI_Stage_PreView[i]);
			Safe_Release(m_pMovementCom_UI_Stage_PreView[i]);
		}
	}
	Safe_Release(m_pMovementCom_Board);
	Safe_Release(m_pTextureCom_UI);
	Safe_Release(m_pTextureCom_UI_Stage_Button);

	__super::Free();
}
