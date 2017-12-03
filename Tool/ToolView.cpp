
// ToolView.cpp : CToolView 클래스의 구현
//
#include "stdafx.h"
#include "Tool.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "Device.h"
#include "TextureMgr.h"
#include "BackGround.h"
#include "MainFrm.h"
#include "Mouse.h"
#include "SheetView.h"
#include "MySheet.h"
#include "MiniView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_iIndex = 0;

}

CToolView::~CToolView()
{
	CDevice::GetInstance()->DestroyInstance();
	delete m_pBack;
	CTextureMgr::GetInstance()->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDevice::GetInstance()->Render_Begin();
	m_pBack->Render();
	CDevice::GetInstance()->Render_End();
}
// CToolView 인쇄

void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

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

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

void CToolView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(TILESIZEX * TILECNTX, (TILESIZEY / 2) * TILECNTY));

	CMainFrame*	pFrame = (CMainFrame*)AfxGetMainWnd();
	
	RECT	rcWindow;
	pFrame->GetWindowRect(&rcWindow);

	SetRect(&rcWindow, 0, 0, rcWindow.right - rcWindow.left,
		rcWindow.bottom - rcWindow.top);

	RECT rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWindow.right - rcMainView.right);
	float fColFrm = float(rcWindow.bottom - rcMainView.bottom);

	pFrame->SetWindowPos(NULL, 0, 0,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm), SWP_NOZORDER);
	
	g_hWnd = m_hWnd;

	if(FAILED(CDevice::GetInstance()->InitD3D()))
	{
		MessageBox(L"디바이스 초기화 실패!");
	}

	m_pBack = new CBackGround;
	m_pBack->Initialize();
	m_pBack->SetMainView(this);

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Tile/Tile%d.png", L"Back", TEX_MULTI, L"Tile", 4);
}
void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	D3DXVECTOR3 vMousePos = CMouse::GetPos();

	vMousePos.x += GetScrollPos(0);
	vMousePos.y += GetScrollPos(1);

	if(((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->GetSafeHwnd() == NULL)
		return;

	m_iIndex = ((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_TilePage.m_TileCombo.GetCurSel();

	if(m_iIndex == -1)
		m_iIndex = 0;

	eOBJEDIT eObjEditType = ((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_eObjEditType;

	if(eObjEditType != OBJEDIT_END)//END가 아닐때만.
	{	
		switch(eObjEditType)
		{
		case OBJEDIT_ADD:
			{
				((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_TilePage.m_TileCombo.SetCurSel(0);
				((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_eObjEditType = OBJEDIT_END;
				OBJECT* pNewObject = new OBJECT;
				pNewObject->iDrawID = ((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_ObjImageList.GetCurSel();
				int iIndex = m_pBack->GetTileIndex(vMousePos);
				if(iIndex == -1)
				{
					break;
				}
				TILE* pTile = m_pBack->m_vecTile[iIndex];
				pNewObject->iIndex = iIndex;

				int iNum = ((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_ObjTypeCombo.GetCurSel();
				PTCHAR pName = new TCHAR[20];
				((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_ObjTypeCombo.GetLBText(iNum, pName);
				lstrcpy(pNewObject->szObjKey, pName);

				const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(pName, pName, pNewObject->iDrawID); 

				float fY = ((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.m_fImageY;
				
				pNewObject->vPos = pTile->vPos;
				pNewObject->vSize.x = float(pTexture->ImgInfo.Width);
				pNewObject->vSize.y = float(pTexture->ImgInfo.Height);
				pNewObject->vSize.z = 0.f;
				pNewObject->vCenter.x = pTexture->ImgInfo.Width / 2.f;
				pNewObject->vCenter.y = fY;
				pNewObject->vCenter.z = 0.f;
				m_pBack->m_vecObj.push_back(pNewObject);

				((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.ObjListAdd(pName);
			}	
			break;

		case OBJEDIT_DELETE:
			{
				int iIndex = m_pBack->GetTileIndex(vMousePos);

				if(iIndex == -1)
					break;

				vector<OBJECT*>::iterator iter = m_pBack->m_vecObj.begin();
				vector<OBJECT*>::iterator iter_end = m_pBack->m_vecObj.end();

				int iCount = 0;
				for(iter; iter != iter_end; ++iter)
				{
					if((*iter)->iIndex == iIndex)
					{
						delete (*iter);
						(*iter) = NULL;

						iter = m_pBack->m_vecObj.erase(iter);
						((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_ObjPage.ObjListDelete(iCount);

						break;
					}
					++iCount;
				}
			}
			break;
		}
	}

	///////////////////////////////////////////////////
	switch(m_iIndex)
	{
	case 0:
		m_pBack->TileChange(vMousePos, m_iIndex, 0); //지나가는 타일
		break;

	case 1:
		m_pBack->TileChange(vMousePos, m_iIndex, 1); //못지나가는 타일
		break;

	case 2:
		m_pBack->TileChange(vMousePos, m_iIndex, 2); //오브젝트가 있는 타일 / 
		break;

	case 3:
		m_pBack->TileChange(vMousePos, m_iIndex, 0); //그... 밟으면 주변의 오브젝트에 영향을 주는 타일.?
		break;
	}
	
	////////////////////// 여기서 타일 옵션 변경하자...!!

	OnDraw(NULL);
	
	//((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);

	CScrollView::OnLButtonDown(nFlags, point);
}
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	//// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(GetAsyncKeyState(VK_LBUTTON))
	{
		OnLButtonDown(nFlags, point);
	}

	CScrollView::OnMouseMove(nFlags, point);
}