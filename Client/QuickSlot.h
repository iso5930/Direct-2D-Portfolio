#pragma once
#include "obj.h"

class CQuickSlot : public CObj
{
private:
	vector<eSkillType>	m_vecQuickSlot;

private:
	const TEXINFO*	m_pSkillBG;
	const TEXINFO*	m_pSkillQuick;
	const TEXINFO*	m_pHpGuide;
	const TEXINFO*	m_pHP;
	const TEXINFO*	m_pTurn;

private:
	D3DXMATRIX	m_matBG;
	D3DXMATRIX	m_matQuick;
	D3DXMATRIX	m_matHpGuide;
	D3DXMATRIX	m_matHP;
	D3DXMATRIX	m_matTurn;

private:
	Frame	m_FlashFrame;
	Frame	m_LightFrame;
	DWORD	m_dwTime;

private:
	bool	m_bChange;
	int		m_iCur;
	DWORD	m_dwKey;

private:
	bool m_bRender;
	RECT m_rcHp;
		
public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	void FrameMove(void);
	void KeyCheck(DWORD dwKey);
	void SetRender(bool _Render){ m_bRender = _Render; };

public:
	vector<eSkillType>* GetQuickSlot(void) { return &m_vecQuickSlot; };
	eSkillType GetCursel(void);

public:
	CQuickSlot(void);
	virtual ~CQuickSlot(void);
};