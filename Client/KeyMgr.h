#pragma once

class CKeyMgr
{
	DECLARE_SINGLETON(CKeyMgr);
private:
	DWORD m_dwKey;

public:
	DWORD GetKeyState(void);
	void SetKeyState(void);

public:
	CKeyMgr(void);
	~CKeyMgr(void);
};