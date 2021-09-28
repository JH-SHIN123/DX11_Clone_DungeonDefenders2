#pragma once
#include "afxwin.h"


// CNaviMap 대화 상자입니다.

class CNaviMap : public CDialog
{
	DECLARE_DYNAMIC(CNaviMap)

public:
	CNaviMap(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNaviMap();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIMAP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox ListBox_Navi;
};
