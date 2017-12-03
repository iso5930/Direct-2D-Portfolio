#include "StdAfx.h"
#include "TextureMgr.h"
#include "SingTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr);

CTextureMgr::CTextureMgr(void)
{

}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

HRESULT CTextureMgr::InsertTexture(const TCHAR* pFilePath, const TCHAR* pObjKey, eTextureType eType/* = TEX_SINGLE*/, const TCHAR* pStateKey/* = L""*/, const int& iCnt/* = 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter = m_MapTexture.begin();
	map<const TCHAR*, CTexture*>::iterator iter_end = m_MapTexture.end();

	iter = find_if(iter, iter_end, STRCOMPARE(pObjKey));

	if(iter == m_MapTexture.end())
	{
		CTexture* pTexture = NULL;
		switch(eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if(FAILED(pTexture->InsertTexture(pFilePath, pStateKey, iCnt)))
		{
			MessageBox(g_hWnd, pFilePath, L"로딩 실패!!", MB_OK);
			delete pTexture;
			return E_FAIL;
		}

		TCHAR* pKey = new TCHAR[MAX_PATH];
		lstrcpy(pKey, pObjKey);

		m_MapTexture.insert( make_pair(pKey, pTexture) );
	}
	return S_OK;
}

const TEXINFO* CTextureMgr::GetTexture(const TCHAR* pObjKey, const TCHAR* pStateKey/* = L""*/, const int& iCnt/* = 0*/)
{
	map<const TCHAR*, CTexture*>::iterator iter = m_MapTexture.begin();
	map<const TCHAR*, CTexture*>::iterator iter_end = m_MapTexture.end();

	iter = find_if(iter, iter_end, STRCOMPARE(pObjKey));

	if(iter == iter_end)
		return NULL;

	return iter->second->GetTexture(pStateKey, iCnt);
}

void CTextureMgr::Release(void)
{
	map<const TCHAR*, CTexture*>::iterator iter = m_MapTexture.begin();
	map<const TCHAR*, CTexture*>::iterator iter_end = m_MapTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete[] iter->first;
		delete iter->second;
	}
	m_MapTexture.clear();
}