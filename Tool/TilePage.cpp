// TilePage.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Tool.h"
#include "TilePage.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "BackGround.h"
#include "MiniView.h"

// CTilePage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTilePage, CPropertyPage)

CTilePage::CTilePage()
	: CPropertyPage(CTilePage::IDD)
	, m_TileRadio(0)
{

}

CTilePage::~CTilePage()
{
	map<CString, CImage*>::iterator iter = m_mapPngImage.begin();
	map<CString, CImage*>::iterator iter_end = m_mapPngImage.end();

	for(iter; iter != iter_end; ++iter)
	{
		iter->second->Destroy();
		delete(iter->second);
	}
	m_mapPngImage.clear();
}

void CTilePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TileCombo, m_TileCombo);
	DDX_Control(pDX, IDC_TileImageView, m_TileView);
	DDX_Control(pDX, IDC_MAP_COMBO, m_mapCombo);
	DDX_Control(pDX, IDC_SHOWTILE_CHECK, m_ShowTileBtn);
}


BEGIN_MESSAGE_MAP(CTilePage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_TileCombo, &CTilePage::OnCbnSelchangeTilecombo)
	ON_BN_CLICKED(IDC_PathBtn, &CTilePage::OnBnClickedPathbtn)
	ON_CBN_SELCHANGE(IDC_MAP_COMBO, &CTilePage::OnCbnSelchangeMapCombo)
	ON_BN_CLICKED(IDC_SAVE_BTN, &CTilePage::OnBnClickedSaveBtn)
	ON_BN_CLICKED(IDC_LOAD_BTN, &CTilePage::OnBnClickedLoadBtn)
	ON_BN_CLICKED(IDC_SHOWTILE_CHECK, &CTilePage::OnBnClickedShowtileCheck)
	ON_BN_CLICKED(IDC_RADIO1, &CTilePage::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CTilePage::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CTilePage::OnBnClickedRadio3)
END_MESSAGE_MAP()


// CTilePage 메시지 처리기입니다.

void CTilePage::OnCbnSelchangeTilecombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	CString strSelectName;

	int iSelectNum = m_TileCombo.GetCurSel();

	m_TileCombo.GetLBText(iSelectNum, strSelectName);

	map<CString, CImage*>::iterator iter = m_mapPngImage.find(strSelectName);
	
	if(iter == m_mapPngImage.end())
		return;

	m_TileView.SetBitmap(*iter->second);

	TCHAR szWidth[10] = L"";
	TCHAR szHeight[10] = L"";

	int iWidth = iter->second->GetWidth();
	int iHeight = iter->second->GetHeight();

	_itow_s(iWidth, szWidth, 10); 
	_itow_s(iHeight, szHeight, 10);
	
	SetDlgItemText(IDC_TileCX, szWidth);
	SetDlgItemText(IDC_TILECY, szHeight);
	UpdateData(FALSE);
}
void CTilePage::OnBnClickedPathbtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CFileDialog FileDlg(FALSE, L"*.*", NULL, OFN_EXPLORER, NULL, NULL);

	TCHAR szCurDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH - 1, szCurDir);

	szCurDir[MAX_PATH - 1] = 0;

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strFileName;

	if(FileDlg.DoModal() == IDOK)
	{
		strFileName = FileDlg.GetPathName();
	}
	SetDlgItemText(IDC_PATH_STATIC, strFileName);

	UpdateData(FALSE);
}
BOOL CTilePage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CImage* pPngImage = new CImage;

	CString strImageName = L"Tile0";

	pPngImage->Load(L"../Texture/Tile/Tile0.png");

	m_mapPngImage.insert(make_pair(strImageName, pPngImage));

	m_TileCombo.AddString(strImageName);

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Tile/Tile1.png");
	strImageName = L"Tile1";

	m_mapPngImage.insert(make_pair(strImageName, pPngImage));
	m_TileCombo.AddString(strImageName);

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Tile/Tile2.png");
	strImageName = L"Tile2";

	m_mapPngImage.insert(make_pair(strImageName, pPngImage));
	m_TileCombo.AddString(strImageName);

	pPngImage = new CImage;
	pPngImage->Load(L"../Texture/Tile/Tile3.png");
	strImageName = L"Tile3";

	m_mapPngImage.insert(make_pair(strImageName, pPngImage));
	m_TileCombo.AddString(strImageName);

	//맵 콤보박스
	m_mapCombo.AddString(L"Stage1");
	m_mapCombo.AddString(L"Stage2");
	m_mapCombo.AddString(L"Boss");

	m_mapCombo.SetCurSel(0);
	m_TileCombo.SetCurSel(0);

	OnCbnSelchangeMapCombo();
	OnCbnSelchangeTilecombo();

	CheckDlgButton(IDC_SHOWTILE_CHECK, TRUE);

	CheckDlgButton(IDC_RADIO1, TRUE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTilePage::OnCbnSelchangeMapCombo()
{
	//추가할 기능 1스테이지의 cnt와 2스테이지의 cnt처리
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	
	TCHAR szMapCX[10] = L"";
	TCHAR szMapCY[10] = L"";


	int iMapCX = 0;
	int iMapCY = 0;

	int iCur = m_mapCombo.GetCurSel();

	switch(iCur)
	{
	case 0:
		iMapCX = TILESIZEX * TILECNTX;
		iMapCY = (TILESIZEY / 2) * TILECNTY;
		break;

	case 1:
		iMapCX = TILESIZEX * STAGE2_TILECNTX;
		iMapCY = (TILESIZEX / 2) * STAGE2_TILECNTY;
		break;

	case 2:
		iMapCX = BOSS_TILECNTX * TILESIZEX;
		iMapCY = BOSS_TILECNTY * (TILESIZEY / 2);
		break;
	}

	_itow_s(iMapCX, szMapCX, 10); 
	_itow_s(iMapCY, szMapCY, 10);

	SetDlgItemText(IDC_MAPCX, szMapCX);
	SetDlgItemText(IDC_MAPCY, szMapCY);

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->Release();
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->Initialize(iCur);
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);

	UpdateData(FALSE);
}

void CTilePage::OnBnClickedSaveBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilePath;

	GetDlgItemText(IDC_PATH_STATIC, strFilePath);

	if(strFilePath == L"")
	{
		AfxMessageBox(L"경로를 먼저 지정하세요.");
		return;
	}

	vector<TILE*> vecTile = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecTile;

	HANDLE hFile = CreateFile(strFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"저장 실패");
		return;
	}

	DWORD dwByte = 0;

	int iCount = vecTile.size();

	for(int i = 0; i < iCount; ++i)
	{
		WriteFile(hFile, vecTile[i], sizeof(TILE), &dwByte, NULL);
	}

	AfxMessageBox(L"저장 성공");

	CloseHandle(hFile);
}

void CTilePage::OnBnClickedLoadBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFilePath;

	GetDlgItemText(IDC_PATH_STATIC, strFilePath);

	if(strFilePath == L"")
	{
		AfxMessageBox(L"경로를 먼저 지정하세요.");
		return;
	}

	vector<TILE*>* vecTile = &((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecTile;

	HANDLE hFile = CreateFile(strFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == NULL)
	{
		AfxMessageBox(L"로드 실패");
		return;
	}

	DWORD dwByte = 0;

	int iCount = vecTile->size();

	for(int i = 0; i < iCount; ++i)
	{
		delete (*vecTile)[i];
	}
	vecTile->clear();

	while(true)
	{
		TILE* pTile = new TILE;

		ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

		if(dwByte == 0)
		{
			delete pTile;
			break;
		}

		vecTile->push_back(pTile);
	}	

	CloseHandle(hFile);

	AfxMessageBox(L"로드 성공!");

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
}

void CTilePage::OnBnClickedShowtileCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	BOOL bCheck = IsDlgButtonChecked(IDC_SHOWTILE_CHECK);

	if(bCheck == 0)
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bShow = false;
	else
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bShow = true;

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
}

void CTilePage::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileObj = false;
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileNum = false;

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
}

void CTilePage::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileObj = false;
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileNum = true;

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
}

void CTilePage::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileObj = true;
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bTileNum = false;

	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
}