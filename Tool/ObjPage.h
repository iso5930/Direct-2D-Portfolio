#pragma once
#include "afxwin.h"

// CObjPage 대화 상자입니다.

class CObjPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CObjPage)
public:
	map<PTCHAR, CImage*> m_mapObjImage;
	map<PTCHAR, vector<PTCHAR>> m_mapImageKey;
	map<PTCHAR, CImage*> m_mapRealImage;
	eOBJEDIT m_eObjEditType;

public:
	void ObjListAdd(PTCHAR pName);
	void ObjListDelete(int iIndex);

public:
	void ObjImageInit(PTCHAR pPath, PTCHAR pObjKey, PTCHAR pFileName, int iCount);
	void RealImageInit(PTCHAR pPath, PTCHAR pFileName, int iCount);

public:
	CObjPage();
	virtual ~CObjPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_ObjImageList;
	CStatic m_ObjView;
	CComboBox m_ObjTypeCombo;
	float m_fImageX;
	float m_fImageY;	

	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeObjTypeCombo();
	afx_msg void OnLbnSelchangeObjimageList();	
	afx_msg void OnBnClickedObjaddBtn();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedObjDelBtn();
	afx_msg void OnBnClickedObjAdjustBtn();
	CListBox m_ObjList;
	BOOL m_AlphaCheck;
	afx_msg void OnBnClickedAlphacheck();
	afx_msg void OnBnClickedObjSaveBtn();
	afx_msg void OnBnClickedObjLoadBtn();
};