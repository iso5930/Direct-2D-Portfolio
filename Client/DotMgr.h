#pragma once

class CObj;

class CDotMgr
{
private:
	DECLARE_SINGLETON(CDotMgr);

private:
	vector<CObj*>	m_vecDot;
	int				m_iType;

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	void	ResetPos(void);
	void	SetDotTile(int iType);

public:
	CDotMgr(void);
	~CDotMgr(void);
};