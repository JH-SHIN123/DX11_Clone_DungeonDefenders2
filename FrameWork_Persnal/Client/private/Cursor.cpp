#include "stdafx.h"
#include "..\public\Cursor.h"

CCursor::CCursor(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CCursor::CCursor(const CCursor & rhs)
	: CGameObject(rhs)
{
}

HRESULT CCursor::NativeConstruct_Prototype(const _tchar* TextureName)
{
	__super::NativeConstruct_Prototype();
	return Ready_Component(TextureName);
}

HRESULT CCursor::NativeConstruct(void * pArg)
{
	return S_OK;
}

_int CCursor::Tick(_float TimeDelta)
{
	Tracking_Mouse();

	if (EMouseTexture::TPS == m_eMouseTexture)
	{
		POINT	pt{ g_iWinCX >> 1, g_iWinCY >> 1 };
	
		ClientToScreen(g_hWnd, &pt);
		//SetCursorPos(pt.x, pt.y);
	}



	fRotateTime += TimeDelta * 10.f;
	if (fRotateTime >= 360.f)
		fRotateTime = 0.f;

	m_pTransformCom_TPSAim->Set_RotateAxis_MyRadian(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fRotateTime));

	return _int();
}

_int CCursor::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Cursor, this);
}

HRESULT CCursor::Render()
{
	if (false == m_IsShowCursor)
		return S_OK;

	if (EMouseTexture::TPS != m_eMouseTexture)
	{
		m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom->Get_WorldMatrix()), sizeof(_matrix));
		m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
		m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

		m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView((_uint)m_eMouseTexture));
		m_pVIBufferCom->Render(1);
	}

	else
	{
		m_pVIBufferCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(m_pTransformCom_TPSAim->Get_WorldMatrix()), sizeof(_matrix));
		m_pVIBufferCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
		m_pVIBufferCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

		m_pVIBufferCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom_TPSAim->Get_ShaderResourceView(0));
		m_pVIBufferCom->Render(1);
	}

	return S_OK;
}

void CCursor::Set_ShowCursor(_bool IsShowCursor)
{
	m_IsShowCursor = IsShowCursor;
	ShowCursor(IsShowCursor);
}

void CCursor::Tracking_Mouse()
{
	GetCursorPos(&m_tMouse);
	ScreenToClient(g_hWnd, &m_tMouse);

	// 윈도우상의 마우스 좌표를 뷰포트까지
	_float	fScaleX = m_pTransformCom->Get_Scale(EState::Right) * 0.5f;
	_float	fScaleY = m_pTransformCom->Get_Scale(EState::Up) * 0.5f;
	_float	fMouseX, fMouseY;

	switch (m_eMouseMode)
	{
	case Client::EMouseMode::Default:
		fMouseX = m_tMouse.x - (g_iWinCX * 0.5f);
		fMouseY = (g_iWinCY * 0.5f) - m_tMouse.y;
		break;
	case Client::EMouseMode::Reverse_X:
		fMouseX = (g_iWinCX * 0.5f) - m_tMouse.x;
		fMouseY = (g_iWinCY * 0.5f) - m_tMouse.y;
		break;
	case Client::EMouseMode::Reverse_Y:
		fMouseX = m_tMouse.x - (g_iWinCX * 0.5f);
		fMouseY = m_tMouse.y - (g_iWinCY * 0.5f);
		break;
	case Client::EMouseMode::Reverse_XY:
		fMouseX = (g_iWinCX * 0.5f) - m_tMouse.x;
		fMouseY = m_tMouse.y - (g_iWinCY * 0.5f);
		break;
	default:
		m_eMouseMode = EMouseMode::Default;
		break;
	}

	// 마우스 위치 + 오프셋조절
	m_tMouse.x = LONG(fMouseX + fScaleX);
	m_tMouse.y = LONG(fMouseY - fScaleY);
	m_pTransformCom->Set_State(EState::Position, XMVectorSet(fMouseX + fScaleX, fMouseY - fScaleY, 0.f, 1.f));
}

HRESULT CCursor::Ready_Component(const _tchar* TextureName)
{
	HRESULT hr = S_OK;

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TextureName, TEXT("Com_Texture"), (CComponent**)&m_pTextureCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), TEXT("Com_Movement"), (CComponent**)&m_pTransformCom);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_Buffer"), (CComponent**)&m_pVIBufferCom);

	m_pTransformCom->Set_Scale(XMVectorSet(64.f, 64.f, 1.f, 0.f));
	m_pTransformCom->Set_State(EState::Position, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Texture_Cursor_TPSAim"), TEXT("Com_Texture_TPSAim"), (CComponent**)&m_pTextureCom_TPSAim);
	hr = CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Transform"), TEXT("Com_Movement_TPSAim"), (CComponent**)&m_pTransformCom_TPSAim);

	m_pTransformCom_TPSAim->Set_Scale(XMVectorSet(128.f, 128.f, 1.f, 0.f));
	m_pTransformCom_TPSAim->Set_State(EState::Position, XMVectorSet(0.f, 0.f, 0.f, 1.f));


	if (hr != S_OK)
		MSG_BOX("CCursor::Ready_Component Failed");

	return hr;
}

CCursor * CCursor::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, const _tchar* TextureName)
{
	CCursor* pInstance = new CCursor(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype(TextureName)))
	{
		MSG_BOX("Failed to Creating Instance (CCursor) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CCursor::Clone_GameObject(void * pArg)
{
	MSG_BOX("CCursor's Clone Is nullptr, only Use Prototype In CCursor_Manager");
	return nullptr;
}

void CCursor::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom_TPSAim);
	Safe_Release(m_pTransformCom_TPSAim);
}
