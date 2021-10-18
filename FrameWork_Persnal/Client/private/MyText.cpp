#include "stdafx.h"
#include "..\public\MyText.h"
#include "Text_Manager.h"

CMyText::CMyText(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CGameObject(pDevice, pDevice_Context)
{
}

CMyText::CMyText(const CMyText & rhs)
	: CGameObject(rhs)
{
}

HRESULT CMyText::NativeConstruct_Prototype()
{
	return E_FAIL;
}

HRESULT CMyText::NativeConstruct(void * pArg)
{
	Ready_Component(pArg);

	return S_OK;
}

_int CMyText::Tick(_float TimeDelta)
{
	return _int();
}

_int CMyText::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::FrameUI, this);
}

HRESULT CMyText::Render()
{
	for (auto& iter : m_pFonts)
	{
		if (nullptr != iter)
		{
			iter->Render();
		}
	}

	return S_OK;
}

void CMyText::Set_Pos(_fvector vPos)
{
	m_pMovementCom->Set_State(EState::Position, vPos);
}

void CMyText::Set_Text(_tchar * pText)
{
	lstrcpy(m_Text_Desc.szText, pText);

	Make_Font();
}

HRESULT CMyText::Ready_Component(void * pArg)
{
	memcpy(&m_Text_Desc, pArg, sizeof(TEXT_DESC));


	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Movement"), TEXT("Com_Movement"), (CComponent**)&m_pMovementCom, &m_Text_Desc.MoveDesc);
	//CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_VIBuffer_Rect"), TEXT("Com_VIBuffer_Rect"), (CComponent**)&m_pVIBufferCom);
	_float4 vTexUV = {0.f,0.f,1.f,1.f };

	CGameObject::Add_Component((_uint)ELevel::Static, TEXT("Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom);


	Make_Font();

	return S_OK;
}

void CMyText::Make_Font()
{
	Safe_Release_Vector(m_pFonts);

	m_pFonts.reserve(lstrlen(m_Text_Desc.szText));

	_vector vPos = m_pMovementCom->Get_State(EState::Position);
	_vector vIntv = XMVectorSet(0.f, 0.f, 0.f, 0.f);

	for (_int i = 0; i < lstrlen(m_Text_Desc.szText); ++i)
	{
		DWORD dwFont = m_Text_Desc.szText[i];
		FONTFILE_DATA* pData = CText_Manager::GetInstance()->Get_Font(dwFont);
		FONT_DESC* pFont = new FONT_DESC;

		if (nullptr != pData)
		{
			_vector vScaleLeft, vScaleRight;
			vScaleLeft = XMLoadFloat2(&pData->vTextureUV_Left);
			vScaleRight = XMLoadFloat2(&pData->vTextureUV_Right);

			pFont->iFontID = pData->iFontID;
			pFont->TextUV_Left = pData->vTextureUV_Left;
			pFont->TextUV_Right = pData->vTextureUV_Right;

			XMStoreFloat4(&pFont->vScale, (vScaleRight - vScaleLeft));

			// 자연스러운 스케일
			// 텍스처의uv는 구해졌다 이걸 비율대로 키운다?
			// 근데 U가 압도적으로 길어서 이건 그닥
			pFont->vScale.x = pFont->vScale.x * 512.f * m_Text_Desc.iScaleCount;
			pFont->vScale.y = pFont->vScale.y * 32.f * m_Text_Desc.iScaleCount;
			

			XMStoreFloat4(&pFont->vPos, vPos);

			CMyFont* pInstance = CMyFont::Create(m_pDevice, m_pDevice_Context, pFont);
			m_pFonts.emplace_back(pInstance);
			vPos += XMVectorSet(10.f * m_Text_Desc.iScaleCount, 0.f, 0.f, 0.f);
		}

		Safe_Delete(pFont);

	}

}

CMyText * CMyText::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context, void* pArg)
{
	CMyText* pInstance = new CMyText(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating Instance (CMonster) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CMyText::Clone_GameObject(void * pArg)
{
	return nullptr;
}

void CMyText::Free()
{
	Safe_Release(m_pMovementCom);
	Safe_Release(m_pRendererCom);

	for (auto& iter : m_pFonts)
		Safe_Release(iter);
	m_pFonts.clear();

	__super::Free();
}
