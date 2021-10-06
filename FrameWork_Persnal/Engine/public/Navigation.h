#pragma once

#ifndef __NAVIGATION_H__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_uint		iCurrentIndex = 0;
	}NAVI_DESC;
private:
	explicit CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context);
	explicit CNavigation(const CNavigation& rhs);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT NativeConstruct_Prototype(const char * pNaviFilePath, const char * pNaviFileName);
	virtual HRESULT NativeConstruct(void* pArg) override;

public:
	_bool IsMove(_fvector vOriginalPos, _fvector vDirection, _float* Cell_Y, _vector* vOutSlidingDir);
	_bool Get_CellPos(_fvector vMouseDir, _fvector vMousePos_World, _vector* vOutCellPos);

	void Get_CellOption(_fvector vPos);
	_fvector Get_Less_NearOption_Pos(_vector* vMyPos);

#ifdef _DEBUG
public:
	HRESULT Render_Navigation();

#endif

private:
	vector<class CCell*>				m_pCells;
	typedef vector<class CCell*>		CELLS;

	_int m_iNowCellOption = 0;

private:
	NAVI_DESC				m_NavigationDesc;

private:
	HRESULT Ready_CellNeighbor();

private:
	class CModelLoader* m_pModelLoader = nullptr;
	const aiScene*			m_pScene = nullptr;
	Assimp::Importer		m_Importer;



public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pDevice_Context, const char* pNavigationDataFile, const char * pNaviFileName);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
#define __NAVIGATION_H__
#endif // !__NAVIGATION_H__

/*
네비게이션 매쉬
이거 하나로 다 관리 할 생각

네비게이셔ㅑㄴ은 여러개의 셀(삼각형)을 가지고 있으며
셀을 이용해 움직일 수 있는 범위를 지정 해주는 느낌

한 변에 맞닿아 있는 이웃한 셀은 하나까지 이며
각 변의 개수 = 이웃 가능한 최대의 셀

이동하는 방향에 이웃한 셀이 있다면 움직일 수 있고
아니라면 길이 막힘 (낭떠러지)

징검다리와 유사하다
하나의 내비게이션 메쉬는 전체적인 징검다리
징검다리를 구성하는 하나의 돌덩이들은 셀







*/