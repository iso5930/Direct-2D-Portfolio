#pragma once

class CTexture;

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)
private:
	map<const TCHAR*, CTexture*> m_MapTexture;

public:
	HRESULT InsertTexture(const TCHAR* pFilePath, const TCHAR* pObjKey, eTextureType eType = TEX_SINGLE, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	const	TEXINFO* GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey = L"", const int& iCnt = 0);
	void	Release(void);
	
private:
	CTextureMgr(void);
	~CTextureMgr(void);
};