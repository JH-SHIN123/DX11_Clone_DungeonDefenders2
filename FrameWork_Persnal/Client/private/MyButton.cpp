#include "stdafx.h"
#include "..\public\MyButton.h"
#include "Cursor_Manager.h"

CMyButton::CMyButton(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CMyButton::CMyButton(const CMyButton & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CMyButton::NativeConstruct_Prototype(void* pArg)
{
	return S_OK;
}

HRESULT CMyButton::NativeConstruct(void * pArg)
{
	BUTTON_DESC* pData = new BUTTON_DESC;

	memcpy(pData, pArg, sizeof(BUTTON_DESC));

	__super::SetUp_Default_Component(&pData->UI_Desc);

	pData->Text_Desc.MoveDesc = pData->UI_Desc.Movement_Desc;
	
	if (1 < lstrlen(pData->Text_Desc.szText))
	{
		m_pText = CMyText::Create(m_pDevice, m_pDevice_Context, &pData->Text_Desc);
	}

	Safe_Delete(pData);

	return S_OK;
}

_int CMyButton::Tick(_float TimeDelta)
{
	Button_Picking_Check();
	return _int();
}

_int CMyButton::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::FrameUI, this );
}

HRESULT CMyButton::Render(_uint iTextureIndex)
{
	if (nullptr != m_pText)
		m_pText->Render();

	if (nullptr != m_pTextureCom)
	{
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(iTextureIndex));
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pMovementCom->Get_WorldMatrix()), sizeof(_matrix));
		m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
		m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

		m_pBufferRectCom->Render(0);
	}


	return S_OK;
}

_bool CMyButton::Get_IsPick()
{
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (m_IsPick == true)
			return true;
	}
	return false;
}

HRESULT CMyButton::Ready_Component()
{
	return S_OK;
}

void CMyButton::Button_Picking_Check()
{
	POINT pt = GET_CURSOR_XY;
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	m_IsPick = false;

	_float4 vMyPos;
	XMStoreFloat4(&vMyPos, m_pMovementCom->Get_State(EState::Position));
	_float2 vScale = { m_pMovementCom->Get_Scale(EState::Right), m_pMovementCom->Get_Scale(EState::Up) };
	vMyPos.x = (g_iWinCX * 0.5f) + vMyPos.x;
	vMyPos.y = (g_iWinCY * 0.5f) - vMyPos.y;

	m_tPickingBox.left = (LONG)(vMyPos.x);
	m_tPickingBox.top = (LONG)(vMyPos.y - (vScale.y * 0.5f));
	m_tPickingBox.right = (LONG)(vMyPos.x + (vScale.x));
	m_tPickingBox.bottom = (LONG)(vMyPos.y + (vScale.y * 0.5f));


	if (PtInRect(&m_tPickingBox, pt))
		m_IsPick = true;
}

CMyButton * CMyButton::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void* pArg)
{
	CMyButton* pInstance = new CMyButton(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CMyButton) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMyButton::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMyButton::Free()
{
	Safe_Release(m_pText);

	__super::Free();
}
