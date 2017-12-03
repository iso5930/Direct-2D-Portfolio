#include "StdAfx.h"
#include "RenderMgr.h"
#include "Obj.h"

IMPLEMENT_SINGLETON(CRenderMgr)

CRenderMgr::CRenderMgr(void)
{

}

CRenderMgr::~CRenderMgr(void)
{
	Release();
}

void CRenderMgr::AddObj(float fKey, CObj* pObj)
{
	m_MultiObj.insert( make_pair(fKey, pObj) );
}

void CRenderMgr::Release(void)
{
	m_MultiObj.clear();
}

void CRenderMgr::Render(void)
{
	multimap<float, CObj*>::iterator iter = m_MultiObj.begin();
	multimap<float, CObj*>::iterator iter_end = m_MultiObj.end();

	for(iter; iter != iter_end; ++iter)
	{
		iter->second->Render();
	}
}