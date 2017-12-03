
// ToolView.cpp : CToolView Ŭ������ ����
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
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
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

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
// CToolView �μ�

void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
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


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����

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
		MessageBox(L"����̽� �ʱ�ȭ ����!");
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

	if(eObjEditType != OBJEDIT_END)//END�� �ƴҶ���.
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
		m_pBack->TileChange(vMousePos, m_iIndex, 0); //�������� Ÿ��
		break;

	case 1:
		m_pBack->TileChange(vMousePos, m_iIndex, 1); //���������� Ÿ��
		break;

	case 2:
		m_pBack->TileChange(vMousePos, m_iIndex, 2); //������Ʈ�� �ִ� Ÿ�� / 
		break;

	case 3:
		m_pBack->TileChange(vMousePos, m_iIndex, 0); //��... ������ �ֺ��� ������Ʈ�� ������ �ִ� Ÿ��.?
		break;
	}
	
	////////////////////// ���⼭ Ÿ�� �ɼ� ��������...!!

	OnDraw(NULL);
	
	//((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);

	CScrollView::OnLButtonDown(nFlags, point);
}
void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	//// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if(GetAsyncKeyState(VK_LBUTTON))
	{
		OnLButtonDown(nFlags, point);
	}

	CScrollView::OnMouseMove(nFlags, point);
}