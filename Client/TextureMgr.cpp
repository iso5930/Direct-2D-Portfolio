#include "StdAfx.h"
#include "TextureMgr.h"
#include "Texture.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureMgr);

CTextureMgr::CTextureMgr(void)
{

}

CTextureMgr::~CTextureMgr(void)
{
	Release();
}

HRESULT CTextureMgr::LoadTexture(PTCHAR pPath, PTCHAR pKey, eTextureType eType/* = TEX_SINGLE*/, int iCount/* = 0*/)
{
	CTexture* pTexture = NULL;

	map<CString, CTexture*>::iterator iter = m_mapTexture.begin();

	iter = m_mapTexture.find(pKey);

	if(iter == m_mapTexture.end())
	{
		switch(eType)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;

		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		pTexture->LoadTexture(pPath, iCount);

		m_mapTexture.insert( make_pair( pKey, pTexture) );
	}

	return S_OK;
}

const TEXINFO* CTextureMgr::GetTexture(PTCHAR pKey, int iCount/* = 0*/)
{
	map<CString, CTexture*>::iterator iter = m_mapTexture.begin();

	iter = m_mapTexture.find(pKey);

	return iter->second->GetTexture(iCount);
}

void CTextureMgr::Release(void)
{
	map<CString, CTexture*>::iterator iter = m_mapTexture.begin();
	map<CString, CTexture*>::iterator iter_end = m_mapTexture.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete (*iter).second;
	}

	m_mapTexture.clear();
	
}

HRESULT CTextureMgr::Release(PTCHAR pKey)
{
	//이름으로 받은것만 릴리즈를 해주자...
	map<CString, CTexture*>::iterator iter = m_mapTexture.find(pKey);

	if(iter == m_mapTexture.end())
	{
		ERR_MSG(L"지울 텍스쳐가 없습니다");
		return E_FAIL;
	}

	delete (*iter).second;
	iter = m_mapTexture.erase(iter);

	return S_OK;
}

TCHAR* CTextureMgr::GetShowPath(void)
{ 
	//EnterCriticalSection(m_pCrt);
	//LeaveCriticalSection(m_pCrt);

	return m_szShowPath; 
}

void CTextureMgr::SetShowPath(PTCHAR pPath)
{
	//EnterCriticalSection(m_pCrt);

	lstrcpy(m_szShowPath, pPath); 

	//LeaveCriticalSection(m_pCrt);
}

void CTextureMgr::SetCritical(CRITICAL_SECTION*	pCrt)
{
	m_pCrt = pCrt;
}