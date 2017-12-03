#include "StdAfx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "RenderMgr.h"
#include "Player.h"
#include "DotMgr.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CObjMgr);

CObjMgr::CObjMgr(void)
{

}

CObjMgr::~CObjMgr(void)
{
	Release();
}

void CObjMgr::AddObject(CObj* pObj, eObjType Type)
{
	m_ObjList[Type].push_back(pObj);
}

HRESULT CObjMgr::Initialize(void)
{
	return S_OK;
}

int CObjMgr::Progress(void)
{
	CCollisionMgr::GetInstance()->Progress();

	for(int i = 0; i < OBJ_STRUCT; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end;)
		{
			if((*iter)->Progress() == 1)
			{
				delete (*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	for(int i = OBJ_STRUCT; i < OBJ_STORE; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end;)
		{
			if((*iter)->Progress() == 1)
			{
				delete (*iter);
				iter = m_ObjList[i].erase(iter);
			}
			else
			{
				CRenderMgr::GetInstance()->AddObj((*iter)->GetInfo()->vPos.y, (*iter));
				++iter;
			}
		}
	}

	for(int i = OBJ_STORE; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Progress();
		}
	}

	CPlayer* pPlayer = (CPlayer*)m_ObjList[OBJ_PLAYER].front();

	if(pPlayer->GetTransState() == TRANS_EXIT)
	{
		CDotMgr::GetInstance()->Progress();	
	}

	return 0;
}

void CObjMgr::Render(void)
{
	CPlayer* pPlayer = (CPlayer*)m_ObjList[OBJ_PLAYER].front();

	bool bTrans = pPlayer->GetTrans();

	if(bTrans)
	{
		CDotMgr::GetInstance()->Render();
	}

	for(int i = 0; i < OBJ_STRUCT; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}

	CRenderMgr::GetInstance()->Render();

	for(int i = OBJ_STORE; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}

	CRenderMgr::GetInstance()->Release();
}

void CObjMgr::Release(void)
{
	for(int i = 0; i < OBJ_END; ++i)
	{
		list<CObj*>::iterator iter = m_ObjList[i].begin();
		list<CObj*>::iterator iter_end = m_ObjList[i].end();

		for(iter; iter != iter_end; ++iter)
		{
			delete (*iter);
			*iter = NULL;
		}
		m_ObjList[i].clear();
	}
}

void CObjMgr::Release(eObjType Type)
{
	list<CObj*>::iterator iter = m_ObjList[Type].begin();
	list<CObj*>::iterator iter_end = m_ObjList[Type].end();

	for(iter; iter != iter_end; ++iter)
	{
		delete (*iter);
		*iter = NULL;
	}

	m_ObjList[Type].clear();
}
