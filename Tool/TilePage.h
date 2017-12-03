#pragma once
#include "afxwin.h"


// CTilePage 대화 상자입니다.

class CTilePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTilePage)
public:
	map<CString, CImage*> m_mapPngImage;

public:
	CTilePage();
	virtual ~CTilePage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TILEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_TileCombo;
	CStatic m_TileView;
	CButton m_ShowTileBtn;
	CComboBox m_mapCombo;
	int m_TileRadio;

	afx_msg void OnCbnSelchangeTilecombo();
	afx_msg void OnBnClickedPathbtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeMapCombo();
	afx_msg void OnBnClickedSaveBtn();
	afx_msg void OnBnClickedLoadBtn();
	afx_msg void OnBnClickedShowtileCheck();
	
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
};