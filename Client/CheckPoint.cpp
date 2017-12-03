#include "StdAfx.h"
#include "CheckPoint.h"
#include "TextureMgr.h"
#include "Device.h"

CCheckPoint::CCheckPoint(void)
	:m_StoreFrame(0, 29, 0, 30)
{
	m_dwTime = GetTickCount();
}

CCheckPoint::~CCheckPoint(void)
{
	Release();
}

HRESULT CCheckPoint::Initialize(void)
{
	m_tInfo.vPos.x = 1512.f;
	m_tInfo.vPos.y = 1513.f;

	return S_OK;
}

int	CCheckPoint::Progress(void)
{
	if(m_eType == CHECK_STORE)
	{
		FrameMove();
	}

	return 0;
}

void CCheckPoint::Render(void)
{	
	if(m_eType == CHECK_STORE)
	{
		
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SocketBottom", m_StoreFrame.iCur);
		D3DXMATRIX matTrans;
		D3DXMATRIX matScale;
		D3DXMATRIX matWorld;
		
		D3DXMatrixIdentity(&matWorld);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);
		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SocketTop", m_StoreFrame.iCur);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, (m_tInfo.vPos.y - 90) - g_Scroll.y, 0.f);
		
		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}

void CCheckPoint::Release(void)
{
	
}

void CCheckPoint::FrameMove(void)
{
	if(m_dwTime + m_StoreFrame.dwFrameTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		if(++m_StoreFrame.iCur > m_StoreFrame.iEndFrame)
		{
			m_StoreFrame.iCur = m_StoreFrame.iStartFrame;
		}
	}
}