#pragma once

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)
private:
	CScene*		m_pScene;
	eSceneType	m_eNextScene;
	eSceneType	m_eScene;
	bool		m_bChange;

public:
	void	SetScene(eSceneType eType);
	void	Initialize(void);
	int		UpDate(void);
	void	Render(void);
	void	Release(void);

public:
	eSceneType GetSceneType(void) { return m_eScene; }

public:
	CSceneMgr(void);
	~CSceneMgr(void);
};