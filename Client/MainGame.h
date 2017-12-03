#pragma once


class CMainGame
{
private:
	float	m_fTime;
	int		m_iCount;
	TCHAR	m_szFps[MAX_PATH];

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	CMainGame(void);
	~CMainGame(void);
};