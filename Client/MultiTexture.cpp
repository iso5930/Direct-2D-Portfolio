#include "StdAfx.h"
#include "MultiTexture.h"
#include "Device.h"
#include "TextureMgr.h"
CMultiTexture::CMultiTexture(void)
{
	
}

CMultiTexture::~CMultiTexture(void)
{
	Release();	
}

HRESULT CMultiTexture::LoadTexture(PTCHAR pPath, int iCount/* = 0*/)
{
	TCHAR szPath[MAX_PATH] = L"";

	for(int i = 0; i < iCount; ++i)
	{
		wsprintf(szPath, pPath, i);

		TEXINFO* pTexInfo = new TEXINFO;

		ZeroMemory(pTexInfo, sizeof(TEXINFO));

		if(FAILED(D3DXGetImageInfoFromFile(szPath, &pTexInfo->ImgInfo)))
		{
			ERR_MSG(szPath);
			delete pTexInfo;
			return E_FAIL;
		}

		if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), szPath, pTexInfo->ImgInfo.Width, pTexInfo->ImgInfo.Height,
			pTexInfo->ImgInfo.MipLevels, 0,
			pTexInfo->ImgInfo.Format,
			D3DPOOL_MANAGED, D3DX_DEFAULT,D3DX_DEFAULT, NULL, &pTexInfo->ImgInfo, NULL, &pTexInfo->pTexture)))
		{
			ERR_MSG(szPath);
			delete pTexInfo;
			return E_FAIL;
		}

		TCHAR szTemp[MAX_PATH] = L"Texture Loading : ";

		lstrcat(szTemp, szPath);

		CTextureMgr::GetInstance()->SetShowPath(szTemp);
		m_vecTexInfo.push_back(pTexInfo);
	}
	
	return S_OK;
}

const TEXINFO* CMultiTexture::GetTexture(int iCount/* = 0*/)
{
	return m_vecTexInfo[iCount];
}

void CMultiTexture::Release(void)
{	
	vector<TEXINFO*>::iterator iter = m_vecTexInfo.begin();
	vector<TEXINFO*>::iterator iter_end = m_vecTexInfo.end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->pTexture->Release();
		delete (*iter);
	}
	m_vecTexInfo.clear();
}