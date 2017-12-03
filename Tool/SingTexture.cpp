#include "StdAfx.h"
#include "SingTexture.h"
#include "Device.h"

CSingTexture::CSingTexture(void)
{
	m_pTexInfo = NULL;
}

CSingTexture::~CSingTexture(void)
{
	Release();
}

HRESULT	CSingTexture::InsertTexture(const TCHAR* pPath, const TCHAR* pStateKey/* = L""*/, const int& iCount/* = 0*/)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if(FAILED(D3DXGetImageInfoFromFile(pPath, &m_pTexInfo->ImgInfo)))
	{
		MessageBox(g_hWnd, pPath, L"로딩 실패", MB_OK);
		return E_FAIL;
	}

	if(FAILED(D3DXCreateTextureFromFileEx(CDevice::GetInstance()->GetDevice(), 
		pPath, m_pTexInfo->ImgInfo.Width, m_pTexInfo->ImgInfo.Height, 
		m_pTexInfo->ImgInfo.MipLevels, 0,
		m_pTexInfo->ImgInfo.Format, D3DPOOL_MANAGED, D3DX_DEFAULT, 
		D3DX_DEFAULT, NULL, &m_pTexInfo->ImgInfo, NULL, &m_pTexInfo->pTexture)))
	{
		MessageBox(g_hWnd, pPath, L"텍스쳐 생성 실패", MB_OK);
		return E_FAIL;
	}
	
	return S_OK;
}

const TEXINFO* CSingTexture::GetTexture(const TCHAR* pKey/* = L""*/, const int& iCount/* = 0*/)
{
	return m_pTexInfo;
}

void CSingTexture::Release(void)
{
	delete m_pTexInfo;
}