
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "Main_Level.h"
#include "public\Renderer.h"

HWND g_hWnd;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	// TODO: 여기에 생성 코드를 추가합니다.
	Safe_AddRef(m_pGameInstance);


}

CToolView::~CToolView()
{
	Safe_Release(m_pRenderer);
	Safe_Release(m_pDevice);
	Safe_Release(m_pDevice_Context);

	Safe_Release(m_pGameInstance);


	/* 엔진 내에서 사용된 객체들을 정리한다. */
	CGameInstance::Release_Engine();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pGameInstance->Tick(0.00001f);

	m_pGameInstance->Clear_Back_Buffer(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_Depth_Stencil_Buffer();

	m_pRenderer->Draw_Renderer();
	//m_pGameInstance->Render_Level();

	m_pGameInstance->Present();

	// 터미널로 깃 인덱스 초기화

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	g_hWnd = m_hWnd;
	CMainFrame* pMain = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());

	RECT rcMain{};
	pMain->GetWindowRect(&rcMain);
	SetRect(&rcMain, 0, 0, rcMain.right - rcMain.left, rcMain.bottom - rcMain.top);

	RECT rcView{};
	GetClientRect(&rcView);
	int iGapX = (rcMain.right - rcView.right) + 1;
	int iGapY = (rcMain.bottom - rcView.bottom) + 1;
	pMain->SetWindowPos(nullptr, 0, 0, 1280 + iGapX, 720 + iGapY, PM_NOREMOVE);


	// 초기화 작업입닌당
	if (FAILED(m_pGameInstance->Initialize(nullptr, g_hWnd, CGraphic_Device::TYPE_WINMODE, g_iWinCX, g_iWinCY, &m_pDevice, &m_pDevice_Context)))
		return;

	if (m_pGameInstance->Reserve_Container((_uint)ELevel_Tool::End))
		return;

	if (Ready_Component_PrototypeForStatic())
		return;

	if (Ready_DefaultLevel())
		return;

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
}

HRESULT CToolView::Ready_Component_PrototypeForStatic()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel_Tool::Main, TEXT("Component_Renderer"), m_pRenderer = CRenderer::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer);


	if (FAILED(m_pGameInstance->Add_Prototype((_uint)ELevel_Tool::Main, TEXT("Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pDevice_Context, L"../../Client/Bin/Shader/Shader_Default.hlsl", "DefaultTechnique"))))
		return E_FAIL;


	return S_OK;
}

HRESULT CToolView::Ready_DefaultLevel()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	if (FAILED(m_pGameInstance->SetUp_CurrentLevel(CMain_Level::Create(m_pDevice, m_pDevice_Context))))
		return E_FAIL;

	return S_OK;
}

const _tchar * CToolView::Shader_Path(wstring wstrFileName)
{
	wstrFileName = L"../../Client/Bin/Shader/" + wstrFileName;
	return wstrFileName.c_str();
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기
