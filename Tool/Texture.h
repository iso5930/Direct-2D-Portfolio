#pragma once

class CTexture
{
public:
	virtual	HRESULT	InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey = L"", const int& iCount = 0)PURE;
	virtual const TEXINFO* GetTexture(const TCHAR* pKey = L"", const int& iCount = 0)PURE;
	virtual void Release(void)PURE;

public:
	CTexture(void);
	virtual ~CTexture(void);
};