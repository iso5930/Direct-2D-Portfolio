#pragma once
#include "obj.h"

class CCube : public CObj
{
private:
	eCubeState		m_eState;
	int				m_iTileIndex;
	vector<TILE*>*	m_vecTile;
	bool			m_bCreate;
	
private:
	Frame m_CreateFrame;
	Frame m_LoopFrame;
	Frame m_DeathFrame;
	DWORD m_dwTime;

public:
	void FrameMove(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	void CubeHitAction(int Att);
	void SetTileIndex(int Index) { m_iTileIndex = Index; }

public:
	CCube(void);
	virtual ~CCube(void);
};