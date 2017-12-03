#pragma once
#include "obj.h"

class CSkillStore : public CObj
{
private:
	eStoreState m_eState;

private:
	Frame m_IntroFrame;
	Frame m_LoopFrame;
	Frame m_OutroFrame;

private:
	DWORD m_dwTime;

private:
	RECT m_rcBtn_Add;
	RECT m_rcBtn_Release;
	RECT m_rcBtn_Return;

private:
	vector<RECT> m_vecIcon;
	vector<RECT> m_vecQuickIcon;

private:
	bool m_bBtn_Add_Over;
	bool m_bBtn_Release_Over;
	bool m_bBtn_Return_Over;

	bool m_bBtn_Add_Down;
	bool m_bBtn_Release_Down;
	bool m_bBtn_Return_Down;

	bool m_bBtn_Add_Off;
	bool m_bBtn_Release_Off;
	bool m_bBtn_Return_Off;

	bool m_bStoreWindow;

	bool m_bSkillCheck;

private:
	TCHAR m_szAddName[30];

private:
	D3DXVECTOR3 m_vCurPos;
	D3DXVECTOR3 m_vLagerCurPos;
	D3DXVECTOR3 m_vAddBtnPos;
	D3DXVECTOR3 m_vReleaseBtnPos;
	D3DXVECTOR3 m_vReturnBtnPos;

	float		m_fBtnCX;
	float		m_fBtnCY;

	int			m_iCurIndex;
	int			m_iLagerCurIndex;

private:
	D3DXVECTOR3 m_vAddFontPos;
	D3DXVECTOR3 m_vReleaseFontPos;
	D3DXVECTOR3 m_vReturnFontPos;

public:
	void FrameMove(void);

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void SetButton(void);

public:
	CSkillStore(void);
	virtual ~CSkillStore(void);
};