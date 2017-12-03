// MySheet.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MySheet.h"


// CMySheet

IMPLEMENT_DYNAMIC(CMySheet, CPropertySheet)

CMySheet::CMySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CMySheet::CMySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}
CMySheet::CMySheet(void)
{
	AddPage(&m_TilePage);
	AddPage(&m_ObjPage);
}

CMySheet::~CMySheet()
{
}


BEGIN_MESSAGE_MAP(CMySheet, CPropertySheet)
END_MESSAGE_MAP()


// CMySheet �޽��� ó�����Դϴ�.
BOOL CMySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  ���⿡ Ư��ȭ�� �ڵ带 �߰��մϴ�.
	
	SetActivePage(&m_TilePage);
	SetActivePage(&m_ObjPage);
	SetActivePage(&m_TilePage);

	return bResult;
}
