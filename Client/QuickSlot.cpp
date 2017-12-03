#include "StdAfx.h"
#include "QuickSlot.h"
#include "TextureMgr.h"
#include "Device.h"
#include "KeyMgr.h"
#include "ObjMgr.h"


CQuickSlot::CQuickSlot(void)
	:m_FlashFrame(0, 44, 0, 100), m_LightFrame(0, 19, 0, 50)
{

}

CQuickSlot::~CQuickSlot(void)
{
	Release();
}

HRESULT CQuickSlot::Initialize(void)
{	
	m_pSkillBG		= CTextureMgr::GetInstance()->GetTexture(L"BG");
	m_pSkillQuick	= CTextureMgr::GetInstance()->GetTexture(L"QuickSlot");
	m_pHpGuide		= CTextureMgr::GetInstance()->GetTexture(L"HP_Guid");
	m_pHP			= CTextureMgr::GetInstance()->GetTexture(L"HP");
	m_pTurn			= CTextureMgr::GetInstance()->GetTexture(L"Turn");

	D3DXMatrixTranslation(&m_matBG, 640.f, 665.f, 0.f);
	D3DXMatrixTranslation(&m_matQuick, 640.f, 674.f, 0.f);
	D3DXMatrixTranslation(&m_matTurn, 640.f, 574.f, 0.f);
	//D3DXMatrixTranslation(&m_matHP, 640.f, 608.f, 0.f);
	D3DXMatrixTranslation(&m_matHP, 509.f, 608.f, 0.f);
	D3DXMatrixTranslation(&m_matHpGuide, 640.f, 608.f, 0.f);

	m_dwTime = GetTickCount();

	m_bChange = false;
	m_iCur = 0;

	m_bRender = true;

	m_vecQuickSlot.reserve(4);

	for(int i = 0; i < 4; ++i)
	{
		m_vecQuickSlot.push_back(SKILL_END);
	}

	m_vecQuickSlot[0] = SKILL_NO1;
	m_vecQuickSlot[1] = SKILL_NO2;
	m_vecQuickSlot[2] = SKILL_NO3;
	m_vecQuickSlot[3] = SKILL_NO5;

	m_rcHp.left		= 0;
	m_rcHp.right	= 262;
	m_rcHp.top		= 0;
	m_rcHp.bottom	= 29;

	return S_OK;
}

int	CQuickSlot::Progress(void)
{
	FrameMove();

	KeyCheck(CKeyMgr::GetInstance()->GetKeyState());
	
	return 0;
}

void CQuickSlot::Render(void)
{
	if(m_bRender == false)
		return;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matBG);
	CDevice::GetInstance()->GetSprite()->Draw(m_pSkillBG->pTexture, NULL, &D3DXVECTOR3(m_pSkillBG->ImgInfo.Width / 2.f, m_pSkillBG->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	const TEXINFO* pTexInfo;

	for(int i = 0; i < 4; ++i)
	{
		if(m_vecQuickSlot[i] == SKILL_END)
			continue;

		switch(m_vecQuickSlot[i])
		{
		case SKILL_NO1:
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"QuickIcon", 0);
			break;

		case SKILL_NO2:
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"QuickIcon", 1);
			break;

		case SKILL_NO3:
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"QuickIcon", 2);
			break;	

		case SKILL_NO4:
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"QuickIcon", 3);
			break;

		case SKILL_NO5:
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"QuickIcon", 4);
			break;
		}
		
		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, 539.f + 67.f * i, 665.f, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//퀵슬롯 밑에 그려지나?
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matQuick);
	CDevice::GetInstance()->GetSprite()->Draw(m_pSkillQuick->pTexture, NULL, &D3DXVECTOR3(m_pSkillQuick->ImgInfo.Width / 2.f, m_pSkillQuick->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matTurn);
	CDevice::GetInstance()->GetSprite()->Draw(m_pTurn->pTexture, NULL, &D3DXVECTOR3(m_pTurn->ImgInfo.Width / 2.f, m_pTurn->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	DATA* pData = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetData();

	float fHPPersent = (float)100 - (((float)pData->iHp / (float)pData->iMaxHp) * 100);

	float fHpBar = (262 * fHPPersent / 100);

	m_rcHp.right = int(262 - fHpBar);
	
	//hp바 ㅋㅋ
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matHP);
	CDevice::GetInstance()->GetSprite()->Draw(m_pHP->pTexture, &m_rcHp, &D3DXVECTOR3(0.f, m_pHP->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matHpGuide);
	CDevice::GetInstance()->GetSprite()->Draw(m_pHpGuide->pTexture, NULL, &D3DXVECTOR3(m_pHpGuide->ImgInfo.Width / 2.f, m_pHpGuide->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	

	if(m_bChange)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillFlash", m_LightFrame.iCur);
	}
	else
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillLight", m_FlashFrame.iCur);
	}

	D3DXMATRIX matTrans;

	switch(m_iCur)
	{
	case 0:
		D3DXMatrixTranslation(&matTrans, 539, 665 - 2, 0.f);
		break;

	case 1:
		D3DXMatrixTranslation(&matTrans, 606, 665 - 2, 0.f);
		break;

	case 2:
		D3DXMatrixTranslation(&matTrans, 673, 665 - 2, 0.f);
		break;

	case 3:
		D3DXMatrixTranslation(&matTrans, 740, 665 - 2, 0.f);
		break;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	//font
	/*
	D3DXMatrixTranslation(&matTrans, 533.f, 627.f, 0.f);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"1", lstrlen(L"1"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	*/
}

void CQuickSlot::Release(void)
{
	
}

void CQuickSlot::FrameMove(void)
{
	if(m_bChange)
	{
		if(m_dwTime + m_LightFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_LightFrame.iCur > m_LightFrame.iEndFrame)
			{
				m_bChange = false;
				m_LightFrame.iCur = m_LightFrame.iStartFrame;
			}
		}
	}
	else
	{
		if(m_dwTime + m_FlashFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_FlashFrame.iCur > m_FlashFrame.iEndFrame)
			{
				m_FlashFrame.iCur = m_FlashFrame.iStartFrame;
			}
		}
	}	
}

void CQuickSlot::KeyCheck(DWORD dwKey)
{
	if(m_dwKey == dwKey)
		return;

	m_dwKey = dwKey;

	//스킬이 있을경우에만 사용이 가능하도록..

	if(dwKey & VK_1)
	{
		if(m_vecQuickSlot[0] != SKILL_END)
		{
			m_bChange = true;
			m_iCur = 0;
			m_LightFrame.iCur = m_LightFrame.iStartFrame;
		}
	}

	if(dwKey & VK_2)
	{
		if(m_vecQuickSlot[1] != SKILL_END)
		{
			m_bChange = true;
			m_iCur = 1;
			m_LightFrame.iCur = m_LightFrame.iStartFrame;
		}
	}

	if(dwKey & VK_3)
	{	
		if(m_vecQuickSlot[2] != SKILL_END)
		{
			m_bChange = true;
			m_iCur = 2;
			m_LightFrame.iCur = m_LightFrame.iStartFrame;
		}
	}

	if(dwKey & VK_4)
	{
		if(m_vecQuickSlot[3] != SKILL_END)
		{
			m_bChange = true;
			m_iCur = 3;
			m_LightFrame.iCur = m_LightFrame.iStartFrame;
		}
	}
}

eSkillType CQuickSlot::GetCursel(void)
{
	return m_vecQuickSlot[m_iCur];
}	