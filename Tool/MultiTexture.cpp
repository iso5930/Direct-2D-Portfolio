#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"

CMultiTexture::CMultiTexture(void)
{

}

CMultiTexture::~CMultiTexture(void)
{
	Release();
}

HRESULT	CMultiTexture::InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey/*= L""*/, const int& iCount/* = 0*/)
{	
	TCHAR	szPath[MAX_PATH] = L"";
	vector<TEXINFO*>	vecTexture;

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, pPath, i);
		TEXINFO*	pTexInfo = new TEXINFO;
		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImgInfo)))
		{
			MessageBox(g_hWnd, pPath, L"Error", MB_OK);
			delete pTexInfo;

			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), szPath, pTexInfo->ImgInfo.Width,	pTexInfo->ImgInfo.Height, pTexInfo->ImgInfo.MipLevels,
			0, pTexInfo->ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &pTexInfo->ImgInfo, NULL, &pTexInfo->pTexture)))
		{
			MessageBox(g_hWnd, pPath, L"Error", MB_OK);
			delete pTexInfo;

			return E_FAIL;
		}
		vecTexture.push_back(pTexInfo);
	}

	TCHAR*	pKey = new TCHAR[256];
	lstrcpy(pKey, pStateKey);

	m_MapMultiTexture.insert(make_pair(pKey, vecTexture));

	return S_OK;
}

const TEXINFO* CMultiTexture::GetTexture(const TCHAR* pKey/* = L""*/, const int& iCount/* = 0*/)
{
	map<const TCHAR*, vector<TEXINFO*>>::iterator iter = m_MapMultiTexture.begin();
	map<const TCHAR*, vector<TEXINFO*>>::iterator iter_end = m_MapMultiTexture.end();

	iter = find_if(iter, iter_end, STRCOMPARE(pKey));

	if(iter == iter_end)
		return NULL;

	return iter->second[iCount];
}

void CMultiTexture::Release(void)
{
	map<const TCHAR*, vector<TEXINFO*>>::iterator MapIter = m_MapMultiTexture.begin();
	map<const TCHAR*, vector<TEXINFO*>>::iterator MapIter_end = m_MapMultiTexture.end();

	for(MapIter; MapIter != MapIter_end; ++MapIter)
	{
		delete[] MapIter->first;

		for(size_t i = 0; i < MapIter->second.size(); ++i)
		{
			MapIter->second[i]->pTexture->Release();
			delete MapIter->second[i];
		}
		MapIter->second.clear();
	}
	m_MapMultiTexture.clear();
}