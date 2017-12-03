#include "StdAfx.h"
#include "SingleTexture.h"
#include "Device.h"
#include "TextureMgr.h"

CSingleTexture::CSingleTexture(void)
{
	m_pTexInfo = NULL;
}	

CSingleTexture::~CSingleTexture(void)
{
	Release();
}

HRESULT CSingleTexture::LoadTexture(PTCHAR pPath, int iCount/* = 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(pPath, &m_pTexInfo->ImgInfo)))
	{
		delete m_pTexInfo;
		ERR_MSG(L"로딩실패!!");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), pPath, m_pTexInfo->ImgInfo.Width, m_pTexInfo->ImgInfo.Height,
		m_pTexInfo->ImgInfo.MipLevels, 0,
		m_pTexInfo->ImgInfo.Format,
		D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, NULL, &m_pTexInfo->ImgInfo, NULL, &m_pTexInfo->pTexture)))
	{
		delete m_pTexInfo;
		ERR_MSG(L"로딩실패!!");
		return E_FAIL;
	}

	TCHAR szTemp[MAX_PATH] = L"Texture Loading : ";

	lstrcat(szTemp, pPath);

	CTextureMgr::GetInstance()->SetShowPath(szTemp);

	return S_OK;
}

const TEXINFO* CSingleTexture::GetTexture(int iCount/* = 0*/)
{
	return m_pTexInfo;
}

void CSingleTexture::Release(void)
{
	m_pTexInfo->pTexture->Release();
	delete m_pTexInfo;
	m_pTexInfo = NULL;
}