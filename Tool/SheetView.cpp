// SheetView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "SheetView.h"
#include "MySheet.h"


// CSheetView

IMPLEMENT_DYNCREATE(CSheetView, CView)

CSheetView::CSheetView()
{
	m_pMySheet = NULL;
}

CSheetView::~CSheetView()
{
	if(m_pMySheet != NULL)
	{
		delete m_pMySheet;
		m_pMySheet = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSheetView, CView)
END_MESSAGE_MAP()


// CSheetView 그리기입니다.

void CSheetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CSheetView 진단입니다.

#ifdef _DEBUG
void CSheetView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CSheetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSheetView 메시지 처리기입니다.

void CSheetView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_pMySheet = new CMySheet;
	m_pMySheet->Create(this, WS_CHILD | WS_VISIBLE);
	m_pMySheet->MoveWindow(0, 0, 300, 530);
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}