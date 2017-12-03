#pragma once

class CObj;

class CRenderMgr
{
	DECLARE_SINGLETON(CRenderMgr)

private:
	multimap<float, CObj*> m_MultiObj;

public:
	void AddObj(float fKey, CObj* pObj);
	void Release(void);
	void Render(void);

public:
	CRenderMgr(void);
	~CRenderMgr(void);
};