// ObjPage.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "Tool.h"
#include "ObjPage.h"
#include "SheetView.h"
#include "MainFrm.h"
#include "MySheet.h"
#include "TextureMgr.h"
#include "BackGround.h"
#include "Mouse.h"
#include "ToolView.h"
#include "MiniView.h"
// CObjPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CObjPage, CPropertyPage)

CObjPage::CObjPage()
	: CPropertyPage(CObjPage::IDD)
	, m_fImageX(0)
	, m_fImageY(0)
	, m_AlphaCheck(FALSE)
{
	m_eObjEditType = OBJEDIT_END;
}

CObjPage::~CObjPage()
{
	map<PTCHAR, CImage*>::iterator iter = m_mapObjImage.begin();
	map<PTCHAR, CImage*>::iterator iter_end = m_mapObjImage.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete[] iter->first;
		delete iter->second;
	}

	iter = m_mapRealImage.begin();
	iter_end = m_mapRealImage.end();

	for(iter; iter!= iter_end; ++iter)
	{
		delete[] iter->first;
		delete iter->second;
	}
}

void CObjPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Obj_Type_Combo, m_ObjTypeCombo);

	DDX_Control(pDX, IDC_ObjImage_List, m_ObjImageList);
	DDX_Control(pDX, IDC_ObjView, m_ObjView);
	DDX_Text(pDX, IDC_ImageX, m_fImageX);
	DDX_Text(pDX, IDC_ImageY, m_fImageY);
	DDX_Control(pDX, IDC_Obj_List, m_ObjList);
	DDX_Check(pDX, IDC_AlphaCheck, m_AlphaCheck);
}


BEGIN_MESSAGE_MAP(CObjPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_Obj_Type_Combo, &CObjPage::OnCbnSelchangeObjTypeCombo)
	ON_LBN_SELCHANGE(IDC_ObjImage_List, &CObjPage::OnLbnSelchangeObjimageList)
	ON_BN_CLICKED(IDC_ObjAdd_Btn, &CObjPage::OnBnClickedObjaddBtn)
	ON_BN_CLICKED(IDC_Obj_Del_Btn, &CObjPage::OnBnClickedObjDelBtn)
	ON_BN_CLICKED(IDC_AlphaCheck, &CObjPage::OnBnClickedAlphacheck)
	ON_BN_CLICKED(IDC_Obj_Save_Btn, &CObjPage::OnBnClickedObjSaveBtn)
	ON_BN_CLICKED(IDC_Obj_Load_Btn, &CObjPage::OnBnClickedObjLoadBtn)
END_MESSAGE_MAP()


// CObjPage 메시지 처리기입니다.

BOOL CObjPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ObjTypeCombo.AddString(L"Cube");
	m_ObjTypeCombo.AddString(L"Fence");
	m_ObjTypeCombo.AddString(L"Struct");

	ObjImageInit(L"../Texture/Object/1Stage/Cube/PriView/Cube%d.png", L"Cube", L"Cube%d", 5);
	ObjImageInit(L"../Texture/Object/1Stage/Fence/PriView/Fence%d.png", L"Fence", L"Fence%d", 2);
	ObjImageInit(L"../Texture/Object/1Stage/Struct/PriView/Struct%d.png", L"Struct", L"Struct%d", 5);
	//프리뷰 이미지..

	RealImageInit(L"../Texture/Object/1Stage/Cube/Cube%d.png", L"Cube%d", 5);
	RealImageInit(L"../Texture/Object/1Stage/Fence/Fence%d.png", L"Fence%d", 2);
	RealImageInit(L"../Texture/Object/1Stage/Struct/Struct%d.png", L"Struct%d", 5);
	//실제 이미지

	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Object/1Stage/Cube/Cube%d.png", L"Cube", TEX_MULTI, L"Cube", 5);
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Object/1Stage/Fence/Fence%d.png", L"Fence", TEX_MULTI, L"Fence", 2);
	CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Object/1Stage/Struct/Struct%d.png", L"Struct", TEX_MULTI, L"Struct", 5);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CObjPage::RealImageInit(PTCHAR pPath, PTCHAR pFileName, int iCount)
{
	TCHAR szPath[MAX_PATH] = L"";

	PTCHAR pName = NULL;

	CImage* Image = NULL;

	for(int i = 0; i < iCount; ++i)
	{
		Image = new CImage;

		pName = new TCHAR[50];

		wsprintf(pName, pFileName, i);

		wsprintf(szPath, pPath, i);

		Image->Load(szPath);

		m_mapRealImage.insert( make_pair(pName, Image) );
	}
}
void CObjPage::ObjImageInit(PTCHAR pPath, PTCHAR pObjKey, PTCHAR pFileName, int iCount)
{
	TCHAR szPath[MAX_PATH] = L"";

	CImage* Image = NULL;

	vector<PTCHAR> vecTemp;

	PTCHAR pName;

	for(int i = 0; i < iCount; ++i)
	{
		pName = new TCHAR[50];

		Image = new CImage;

		wsprintf(szPath, pPath, i);
		wsprintf(pName, pFileName, i);

		Image->Load(szPath);

		m_mapObjImage.insert( make_pair( pName, Image) );

		vecTemp.push_back(pName);
	}

	m_mapImageKey.insert( make_pair(pObjKey, vecTemp) );
}

void CObjPage::OnCbnSelchangeObjTypeCombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int iIndex = m_ObjTypeCombo.GetCurSel();
	
	switch(iIndex)
	{
	case 0:
		{
			m_ObjImageList.ResetContent();
			map<PTCHAR, vector<PTCHAR>>::iterator iter;

			iter = m_mapImageKey.find(L"Cube");

			vector<PTCHAR>::iterator veciter = (*iter).second.begin();
			vector<PTCHAR>::iterator veciter_end = (*iter).second.end();

			for(veciter; veciter != veciter_end; ++veciter)
			{
				m_ObjImageList.AddString((*veciter));
			}
			break;
		}

	case 1:
		{
			m_ObjImageList.ResetContent();
			map<PTCHAR, vector<PTCHAR>>::iterator iter;

			iter = m_mapImageKey.find(L"Fence");

			vector<PTCHAR>::iterator veciter = (*iter).second.begin();
			vector<PTCHAR>::iterator veciter_end = (*iter).second.end();

			for(veciter; veciter != veciter_end; ++veciter)
			{
				m_ObjImageList.AddString((*veciter));
			}
			break;
		}

	case 2:
		{
			m_ObjImageList.ResetContent();
			map<PTCHAR, vector<PTCHAR>>::iterator iter;

			iter = m_mapImageKey.find(L"Struct");

			vector<PTCHAR>::iterator veciter = (*iter).second.begin();
			vector<PTCHAR>::iterator veciter_end = (*iter).second.end();

			for(veciter; veciter != veciter_end; ++veciter)
			{
				m_ObjImageList.AddString((*veciter));
			}
			break;
		}


		break;
	}
	UpdateData(FALSE);
}

void CObjPage::OnLbnSelchangeObjimageList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//이미지가 체인지 되었을때.
	int iIndex = m_ObjImageList.GetCurSel();

	if(iIndex == -1)
		return;

	TCHAR pName[30] = L"";

	m_ObjImageList.GetText(iIndex, pName);

	if(!lstrcmp(pName, L"Cube0"))
	{
		m_fImageY = 300.f;
	}
	else if(!lstrcmp(pName, L"Cube1"))
	{
		m_fImageY = 150.f;
	}
	else if(!lstrcmp(pName, L"Cube2"))
	{
		m_fImageY = 350.f;
	}
	else if(!lstrcmp(pName, L"Cube3"))
	{
		m_fImageY = 380.f;
	}
	else if(!lstrcmp(pName, L"Cube4"))
	{
		m_fImageY = 105.f;
	}
	else if(!lstrcmp(pName, L"Fence0"))
	{
		m_fImageY = 90.f;
	}
	else if(!lstrcmp(pName, L"Fence1"))
	{
		m_fImageY = 90.f;
	}
	else if(!lstrcmp(pName, L"Struct0"))
	{
		m_fImageY = 500.f;
	}
	else if(!lstrcmp(pName, L"Struct1"))
	{
		m_fImageY = 450.f;
	}
	else if(!lstrcmp(pName, L"Struct2"))
	{
		m_fImageY = 450.f;
	}
	else if(!lstrcmp(pName, L"Struct3"))
	{
		m_fImageY = 350.f;
	}
	else if(!lstrcmp(pName, L"Struct4"))
	{
		m_fImageY = 186.f;
	}

	map<PTCHAR, CImage*>::iterator iter = m_mapObjImage.begin();
	map<PTCHAR, CImage*>::iterator iter_end = m_mapObjImage.end();

	iter = find_if(iter, iter_end, STRCOMPARE(pName));

	m_ObjView.SetBitmap(*iter->second);

	iter = m_mapRealImage.begin();
	iter_end = m_mapRealImage.end();

	iter = find_if(iter, iter_end, STRCOMPARE(pName));

	int iWidth = iter->second->GetWidth();
	int iHeight = iter->second->GetHeight();

	TCHAR szWidth[10] = L"";
	TCHAR szHeight[10] = L"";

	_itow_s(iWidth, szWidth, 10);
	_itow_s(iHeight, szHeight, 10);

	SetDlgItemText(IDC_ImageCX, szWidth);
	SetDlgItemText(IDC_ImageCY, szHeight);

	m_fImageX = GetDlgItemInt(IDC_ImageCX) / 2.f;

	UpdateData(FALSE);
}

void CObjPage::OnBnClickedObjaddBtn()
{
	UpdateData(TRUE);

	((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_TilePage.m_TileCombo.SetCurSel(2);
	
	m_eObjEditType = OBJEDIT_ADD;

	UpdateData(FALSE);
}

void CObjPage::OnBnClickedObjDelBtn()
{
	UpdateData(TRUE);

	((CSheetView*)((CMainFrame*)AfxGetMainWnd())->m_pSheetView)->m_pMySheet->m_TilePage.m_TileCombo.SetCurSel(0);

	m_eObjEditType = OBJEDIT_DELETE;
	
	UpdateData(FALSE);
}


void CObjPage::ObjListAdd(PTCHAR pName)
{
	UpdateData(TRUE);

	m_ObjList.AddString(pName);

	UpdateData(FALSE);
}

void CObjPage::ObjListDelete(int iIndex)
{
	UpdateData(TRUE);
	
	m_ObjList.DeleteString(iIndex);

	UpdateData(FALSE);
}
void CObjPage::OnBnClickedAlphacheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if(m_AlphaCheck == TRUE)
	{
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bObjAlpha = true;
	}
	else
	{
		((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_bObjAlpha = false;
	}
	
	((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
	((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);

	UpdateData(FALSE);
}

void CObjPage::OnBnClickedObjSaveBtn()
{
	CFileDialog FileDlg(FALSE, L"*.*", NULL, OFN_EXPLORER, NULL, NULL);

	TCHAR szCurDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH - 1, szCurDir);

	szCurDir[MAX_PATH - 1] = 0;

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strFileName;

	if(FileDlg.DoModal() == IDOK)
	{
		strFileName = FileDlg.GetPathName();
		
		vector<OBJECT*> vecObj = ((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecObj;

		HANDLE hFile = CreateFile(strFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if(hFile == NULL)
		{
			AfxMessageBox(L"저장 실패");
			return;
		}

		DWORD dwByte = 0;

		int iCount = vecObj.size();

		for(int i = 0; i < iCount; ++i)
		{
			WriteFile(hFile, vecObj[i], sizeof(OBJECT), &dwByte, NULL);
		}

		AfxMessageBox(L"저장 성공");

		CloseHandle(hFile);
	}
}
void CObjPage::OnBnClickedObjLoadBtn()
{
	CFileDialog FileDlg(FALSE, L"*.*", NULL, OFN_EXPLORER, NULL, NULL);

	TCHAR szCurDir[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH - 1, szCurDir);

	szCurDir[MAX_PATH - 1] = 0;

	FileDlg.m_ofn.lpstrInitialDir = szCurDir;

	CString strFileName;

	if(FileDlg.DoModal() == IDOK)
	{
		strFileName = FileDlg.GetPathName();

		vector<OBJECT*>* vecObj = &((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecObj;
		vector<TILE*>* vecTile = &((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecTile;
		
		HANDLE hFile = CreateFile(strFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(hFile == NULL)
		{
			AfxMessageBox(L"로드 실패");
			return;
		}

		DWORD dwByte = 0;

		int iCount = vecObj->size();

		for(int i = 0; i < iCount; ++i)
		{
			delete (*vecObj)[i];
		}
		vecObj->clear();

		while(true)
		{
			OBJECT* pObject = new OBJECT;

			ReadFile(hFile, pObject, sizeof(OBJECT), &dwByte, NULL);

			if(dwByte == 0)
			{
				delete pObject;
				break;
			}

			((CMainFrame*)AfxGetMainWnd())->m_pMainView->m_pBack->m_vecTile[pObject->iIndex]->byDrawID = 2;

			vecObj->push_back(pObject);
		}	

		CloseHandle(hFile);

		AfxMessageBox(L"로드 성공!");

		((CMainFrame*)AfxGetMainWnd())->m_pMainView->OnDraw(NULL);
		((CMainFrame*)AfxGetMainWnd())->m_pMiniView->OnDraw(NULL);
	}
}