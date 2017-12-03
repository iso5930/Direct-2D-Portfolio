#pragma once

class CTexture;

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	map<CString, CTexture*> m_mapTexture;
	TCHAR m_szShowPath[MAX_PATH];
	CRITICAL_SECTION*	m_pCrt;

public:
	HRESULT LoadTexture(PTCHAR pPath, PTCHAR pKey, eTextureType eType = TEX_SINGLE, int iCount = 0);
	const TEXINFO* GetTexture(PTCHAR pKey, int iCount = 0);
	void Release(void);

public:
	HRESULT Release(PTCHAR pKey);
	TCHAR* GetShowPath(void);
	void SetShowPath(PTCHAR pPath);
	void SetCritical(CRITICAL_SECTION*	pCrt);

private:
	CTextureMgr(void);
	~CTextureMgr(void);
};