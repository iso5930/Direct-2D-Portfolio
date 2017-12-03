#pragma once
#include "obj.h"

class CBackGround : public CObj
{
private:
	vector<TILE*>	m_vecTile;
	int				m_iStage;

private:
	float m_fTransMask;
	DWORD m_dwTime;

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	void	SetTile(int iStage);
	vector<TILE*>* GetTile(void) { return &m_vecTile; };
	int		GetTileIndex(D3DXVECTOR3 vPos);
	void	SetTileIndex(int PrevIndex, int CurIndex);

public:
	void SetTransMask(float fScale);

public:
	CBackGround(void);
	virtual ~CBackGround(void);
};