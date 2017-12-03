#pragma once

class CToolView;

class CBackGround
{
public:
	vector<TILE*>	m_vecTile;
	CToolView*		m_pView;
	bool			m_bShow;
	bool			m_bTileNum;
	bool			m_bTileObj;
	vector<OBJECT*> m_vecObj;
	bool			m_bObjAlpha;
	int				m_iType;

public:
	void	TileChange(const D3DXVECTOR3& vPos, const int& iDrawID, int iOption);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	int		GetTileIndex(const D3DXVECTOR3& vPos);

public:
	void SetMainView(CToolView* pView);

public:
	void	Initialize(int Type = 0);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	void MiniViewRender(void);

public:
	CBackGround(void);
	~CBackGround(void);
};