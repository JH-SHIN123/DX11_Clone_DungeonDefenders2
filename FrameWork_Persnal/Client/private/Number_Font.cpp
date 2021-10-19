#include "stdafx.h"
#include "..\public\Number_Font.h"

CNumber_Font::CNumber_Font(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
	: CUI_2D(pDevice, pDevice_Context)
{
}

CNumber_Font::CNumber_Font(const CNumber_Font & rhs)
	: CUI_2D(rhs)
{
}

HRESULT CNumber_Font::NativeConstruct_Prototype()
{
	return S_OK;
}

HRESULT CNumber_Font::NativeConstruct(void * pArg)
{
	NUMBERFONT_DESC Data;

	memcpy(&Data, pArg, sizeof(NUMBERFONT_DESC));

	__super::NativeConstruct(&Data.UI_Desc);

	Ready_Component(&Data);

	return S_OK;
}

_int CNumber_Font::Tick(_float TimeDelta)
{
	return _int();
}

_int CNumber_Font::Late_Tick(_float TimeDelta)
{
	return m_pRendererCom->Add_GameObjectToRenderer(ERenderGroup::Option_UI_1, this);
}

HRESULT CNumber_Font::Render()
{
	if (nullptr == m_pBufferRectCom)
		return UPDATE_ERROR;

	m_pBufferRectCom->Set_Variable("ViewMatrix", &XMMatrixTranspose(GET_INDENTITY_MATRIX), sizeof(_matrix));
	m_pBufferRectCom->Set_Variable("ProjMatrix", &XMMatrixTranspose(GET_ORTHO_SPACE), sizeof(_matrix));

	_matrix WorldMatrix = m_pMovementCom->Get_WorldMatrix();
	_vector vOffSet = XMVectorSet(m_vOffSetPos.x, m_vOffSetPos.y, 0.f, 0.f);

	for (_int i = 0; i < m_iBufferSize; ++i)
	{
		m_pBufferRectCom->Set_Variable("WorldMatrix", &XMMatrixTranspose(WorldMatrix), sizeof(_matrix));
		m_pBufferRectCom->Set_ShaderResourceView("g_DiffuseTexture", m_pTextureCom->Get_ShaderResourceView(m_pNumberBuffer[i]));
		m_pBufferRectCom->Render(1);

		WorldMatrix.r[3] += vOffSet;
	}

	return S_OK;
}

void CNumber_Font::Set_Number(_int * pNumberBuffer)
{
	Safe_Delete_Array(m_pNumberBuffer);

	m_pNumberBuffer = pNumberBuffer;
}

void CNumber_Font::Set_Number(char pNumber[MAX_PATH])
{
	if (m_iBufferSize != strlen(pNumber) + 1)
	{
		Safe_Delete_Array(m_pNumberBuffer);
		m_iBufferSize = (_int)strlen(pNumber) + 1;
		m_pNumberBuffer = new _int[m_iBufferSize];
	}

	for (_int i = 0; i < m_iBufferSize; ++i)
	{
		char pNum = pNumber[i];
		m_pNumberBuffer[i] = _int(pNum - '0');
	}
}

HRESULT CNumber_Font::Ready_Component(NUMBERFONT_DESC* pArg)
{
	HRESULT hr = S_OK;

	m_iBufferSize = pArg->iBufferSize;
	m_pNumberBuffer = pArg->pNumberBuffer;
	//memcpy(m_pNumberBuffer, pArg->pNumberBuffer, sizeof(_int) * m_iBufferSize);

	m_vOffSetPos = { m_pMovementCom->Get_Scale(EState::Right) * 0.5f, 0.f};

	return S_OK;
}

void CNumber_Font::Move_Check(_float TimeDelta)
{
}

CNumber_Font * CNumber_Font::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pDevice_Context)
{
	CNumber_Font* pInstance = new CNumber_Font(pDevice, pDevice_Context);
	if (FAILED(pInstance->NativeConstruct_Prototype()))
	{
		MSG_BOX("Failed to Creating Instance (CNumber_Font) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject * CNumber_Font::Clone_GameObject(void * pArg)
{
	CNumber_Font* pInstance = new CNumber_Font(*this);
	if (FAILED(pInstance->NativeConstruct(pArg)))
	{
		MSG_BOX("Failed to Creating clone (CNumber_Font) ");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CNumber_Font::Free()
{
	Safe_Delete_Array(m_pNumberBuffer);
	__super::Free();
}
