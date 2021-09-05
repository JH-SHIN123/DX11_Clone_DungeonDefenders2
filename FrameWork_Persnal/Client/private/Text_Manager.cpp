#include "stdafx.h"
#include "..\public\Text_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CText_Manager)

CText_Manager::CText_Manager()
{

}

HRESULT CText_Manager::Ready_Font()
{
	m_Font.reserve(128);

	wifstream fin;
	fin.open("../Bin/Data/Font/ID_U_V_X_Y.txt");

	if (!fin.fail())
	{
		TCHAR szID[MAX_PATH] = L"";
		TCHAR szU[MAX_PATH] = L"";
		TCHAR szV[MAX_PATH] = L"";
		TCHAR szX[MAX_PATH] = L"";
		TCHAR szY[MAX_PATH] = L"";

		while (true)
		{
			fin.getline(szID, MAX_PATH, L'|');
			fin.getline(szU, MAX_PATH, L'|');
			fin.getline(szV, MAX_PATH, L'|');
			fin.getline(szX, MAX_PATH, L'|');
			fin.getline(szY, MAX_PATH, L'|');

			if (fin.eof())
				break;

			FONTFILE_DATA* pPathInfo = new FONTFILE_DATA;
			pPathInfo->iFontID = _ttoi(szID);
			pPathInfo->vTextureUV_Left.x = (_float)_ttof(szU);
			pPathInfo->vTextureUV_Left.y = (_float)_ttof(szV);
			pPathInfo->vTextureUV_Right.x = (_float)_ttof(szX);
			pPathInfo->vTextureUV_Right.y = (_float)_ttof(szY);

			//pPathInfo->iFontID += 32;
			m_Font.emplace_back(pPathInfo);
		}
		fin.close();
	}


	// UV¼¼ÆÃ
	for (_int i = 1; i < m_Font.size(); ++i)
	{
		_float2 vUV_Left, vUV_Right;
		vUV_Left = m_Font[i]->vTextureUV_Left;
		vUV_Right = m_Font[i]->vTextureUV_Right;
		vUV_Right.x += vUV_Left.x;
		vUV_Right.y += vUV_Left.y;

		//_float
		m_Font[i]->vTextureUV_Left.x = vUV_Left.x / 1024.f;
		m_Font[i]->vTextureUV_Left.y = vUV_Left.y / 128.f;

		m_Font[i]->vTextureUV_Right.x = vUV_Right.x / 1024.f;
		m_Font[i]->vTextureUV_Right.y = vUV_Right.y / 128.f;
	}


	return S_OK;
}

FONTFILE_DATA * CText_Manager::Get_Font(_uint ID)
{
	for (auto& iter : m_Font)
	{
		if (iter != nullptr)
		{
			if (iter->iFontID == ID)
				return iter;
		}
	}

	return nullptr;
}

void CText_Manager::Free()
{
	for (auto& iter : m_Font)
	{
		Safe_Delete(iter);
	}
	m_Font.clear();
}
