#pragma once

class CObj;

class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	list<CObj*> m_ObjList[OBJ_END];

public:
	void AddObject(CObj* pObj, eObjType Type);
	list<CObj*>* GetList(void){ return m_ObjList; };

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);
	void	Release(eObjType Type);

private:
	CObjMgr(void);
	~CObjMgr(void);
};