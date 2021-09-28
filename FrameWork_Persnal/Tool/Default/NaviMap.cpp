// NaviMap.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviMap.h"
#include "afxdialogex.h"


// CNaviMap 대화 상자입니다.

IMPLEMENT_DYNAMIC(CNaviMap, CDialog)

CNaviMap::CNaviMap(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_NAVIMAP, pParent)
{

}

CNaviMap::~CNaviMap()
{
}

void CNaviMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, ListBox_Navi);
}


BEGIN_MESSAGE_MAP(CNaviMap, CDialog)
END_MESSAGE_MAP()


// CNaviMap 메시지 처리기입니다.
