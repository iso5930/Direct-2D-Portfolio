#include "StdAfx.h"
#include "Lobber.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "TimeMgr.h"
#include "Player.h"
#include "Factory.h"
#include "SkillEffect.h"

CLobber::CLobber(void)
	:m_AttackFrame(0, 14, 0, 30), m_RunFrame(0, 19, 0, 20), m_DeathFrame(0, 37, 0, 30)
{
	
}

CLobber::~CLobber(void)
{
	Release();
}

HRESULT CLobber::Initialize(void)
{	
	m_dwTime = GetTickCount();

	m_eState = MONSTER_ATTACK;

	m_pAstar = new CAstar;

	m_iPrevIndex = 1;

	m_vTarget = m_tInfo.vPos;

	m_iTargetIndex = 0;

	m_bCreate = true;

	m_dwTargetTime = GetTickCount();

	m_strName = "Lobber";

	m_tInfo.fCX = 95.f;
	m_tInfo.fCY = 115.f;

	m_tData.iMaxHp = 500;
	m_tData.iHp = 500;
	m_tData.iAtt = 20;
	
	m_dwHitTime = GetTickCount();

	m_rcHpBar.left = 0;
	m_rcHpBar.right = 196;
	m_rcHpBar.top = 0;
	m_rcHpBar.bottom = 60;

	m_bHit = false;

	m_iRestoreCount = 10;
	//m_iCurTileIndex = ((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->GetTileIndex(m_tInfo.vPos);
	//m_iPrevTileIndex = m_iCurTileIndex;

	m_bTrans = false;
	m_bTransProgress = false;

	m_bAttack = false;

	return S_OK;
}

int	CLobber::Progress(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();

	if(m_bTrans)
		return 0;

	if(m_bTransProgress)
	{
		m_RunFrame.dwFrameTime = 500;
		m_AttackFrame.dwFrameTime = 500;
		m_DeathFrame.dwFrameTime = 500;
	}
	else
	{
		m_RunFrame.dwFrameTime = 20;
		m_AttackFrame.dwFrameTime = 30;
		m_DeathFrame.dwFrameTime = 30;
	}

	if(m_eState == MONSTER_EGG)
	{
		FrameMove();

		D3DXVECTOR3 vDist = pObj->GetInfo()->vPos - m_tInfo.vPos;

		float fLength = D3DXVec3Length(&vDist);

		if(fLength <= 80)
		{
			D3DXVec3Normalize(&vDist, &vDist);
			m_tInfo.vPos += vDist * 1000 * GETTIME;

			if(fLength <= 5.f)
			{
				return 1;
			}
		}

		if(m_dwTargetTime + 1000 < GetTickCount())
		{
			m_dwTargetTime = GetTickCount();

			--m_iRestoreCount;
		}

		if(m_iRestoreCount == 0)
		{
			m_eState = MONSTER_ATTACK;
			m_tData.iHp = m_tData.iMaxHp / 2;
			D3DXVECTOR3 vEffectPos = m_tInfo.vPos;
			vEffectPos.y += 100;
			CObj* pObj = CFactory<CSkillEffect>::CreateObj(vEffectPos, EFFECT_LOBBER_DEATH);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
		}

		return 0;
	}

	if(m_bHit)
	{
		if(m_dwHitTime + 300 < GetTickCount())
		{
			m_dwHitTime = GetTickCount();
			m_bHit = false;
		}
	}

	if(m_bAttack)
	{
		if(m_dwTime + 1000 < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bAttack = false;
		}
		return 0;
	}

	D3DXVECTOR3 vPlayerPos = pObj->GetInfo()->vPos;

	D3DXVECTOR3 vDist = vPlayerPos - m_tInfo.vPos;

	float fLength = D3DXVec3Length(&vDist);	

	if(m_bCreate)
	{
		if(fLength <= 700.f)
		{
			m_bCreate = false;
		}
		else
		{
			return 0;
		}
	}

	if(fLength >= 300.f)
	{
		m_eState = MONSTER_RUN;

		if(m_dwTargetTime + 500 < GetTickCount())
		{
			m_dwTargetTime = GetTickCount();
			m_pAstar->AstarStartPos(m_tInfo.vPos, vPlayerPos);
			//m_iPrevTileIndex = m_iCurTileIndex;
			//m_iCurTileIndex = ((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->GetTileIndex(m_tInfo.vPos);
			//((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->SetTileIndex(m_iPrevTileIndex, m_iCurTileIndex);
		}
	}
	else
	{
		m_eState = MONSTER_ATTACK;
	}

	if(m_eState == MONSTER_RUN)
	{
		AstarMove();
	}

	SetAngle(m_vTarget);

	FrameMove();

	SyncHpBar();

	CObj::Progress();

	return 0;
}

void CLobber::Render(void)
{
	const TEXINFO* pTexInfo = NULL;

	float fX;
	float fY;

	if(m_eState == MONSTER_EGG)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"EggHead", m_EggHeadFrame.iCur);
			
		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = (float)pTexInfo->ImgInfo.Height;

		D3DXMATRIX matTrans;
		D3DXMATRIX matScale;

		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y - 25.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;
		
		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"EggBody", m_EggBodyFrame.iCur);

		fY = 0.f;
		
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szCount[10] = L"";

		_itow_s(m_iRestoreCount, szCount, 10);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y - 75.f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szCount, lstrlen(szCount), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		return;
	}

	switch(m_eState)
	{
	case MONSTER_ATTACK:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LobberAttack", m_AttackFrame.iCur);
		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = (pTexInfo->ImgInfo.Height / 2.f) + 84.f;
		break;

	case MONSTER_RUN:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LobberRun", m_RunFrame.iCur);
		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = (pTexInfo->ImgInfo.Height / 2.f) + 94.f;
		break;

	case MONSTER_DEATH:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Monster_Death", m_DeathFrame.iCur);
		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = pTexInfo->ImgInfo.Height / 2.f;
		break;
	}

	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	//HDC m_hdc = GetDC(g_hWnd);

	//Rectangle(m_hdc, int(m_Rect.left - g_Scroll.x), int(m_Rect.top - g_Scroll.y), int(m_Rect.right - g_Scroll.x), int(m_Rect.bottom - g_Scroll.y));
	
	if(m_bHit)
	{	
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	else
	{
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(m_eState != MONSTER_DEATH)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MonsterHpBar");

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 40, m_tInfo.vPos.y - g_Scroll.y - 130, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255)); //µÚ ¹è°æ..

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &m_rcHpBar, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 0, 255, 0));

		//text ======================

		TCHAR szName[10] = L"Lobber";

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 146, m_tInfo.vPos.y - g_Scroll.y - 145, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szName, lstrlen(szName), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szHp[50] = L"";
		TCHAR szTemp[10] = L"";

		_itow_s(m_tData.iHp, szTemp, 10);

		lstrcat(szHp, szTemp);

		lstrcat(szHp, L" / ");

		_itow_s(m_tData.iMaxHp, szTemp, 10);

		lstrcat(szHp, szTemp);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 133, m_tInfo.vPos.y - g_Scroll.y - 118, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szHp, lstrlen(szHp), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}	
}

void CLobber::Release(void)
{
	delete m_pAstar;
	m_pAstar = NULL;

	//CObj* pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_LOBBER_TARGETING);
	//CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
}

void CLobber::FrameMove(void)
{
	if(m_bCreate)
	{
		return;
	}

	switch(m_eState)
	{
	case MONSTER_ATTACK:
		if(m_dwTime + m_AttackFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_AttackFrame.iCur > m_AttackFrame.iEndFrame)
			{
				m_AttackFrame.iCur = m_AttackFrame.iStartFrame;
			}

			if(m_AttackFrame.iCur == m_AttackFrame.iEndFrame - 1)
			{
				D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;
				CObj* pObj = CFactory<CSkillEffect>::CreateObj(vPos, EFFECT_LOBBER_TARGETING);
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
				m_bAttack = true;
			}
		}
		break;

	case MONSTER_RUN:
		if(m_dwTime + m_RunFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_RunFrame.iCur > m_RunFrame.iEndFrame)
			{
				m_RunFrame.iCur = m_RunFrame.iStartFrame;
			}
		}
		break;

	case MONSTER_EGG:
		if(m_dwTime + m_EggBodyFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_EggBodyFrame.iCur > m_EggBodyFrame.iEndFrame)
			{
				m_EggBodyFrame.iCur = m_EggBodyFrame.iStartFrame;
			}
			if(++m_EggHeadFrame.iCur > m_EggHeadFrame.iEndFrame)
			{
				m_EggHeadFrame.iCur = m_EggHeadFrame.iStartFrame;
			}
		}
		break;
	}
}

void CLobber::AstarMove(void)
{
	list<int>* pBestList = m_pAstar->GetBestList();

	if(pBestList->empty())
	{
		return;
	}

	CBackGround* pBack = (CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

	const vector<TILE*>* pVecTile = pBack->GetTile();

	int iMoveIndex = pBestList->front();

	m_vTarget = (*pVecTile)[iMoveIndex]->vPos;

	m_tInfo.vDir = (*pVecTile)[iMoveIndex]->vPos - m_tInfo.vPos;

	float fDist = D3DXVec3Length(&m_tInfo.vDir);

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	if(m_bTransProgress)
	{
		m_tInfo.vPos += m_tInfo.vDir * 10 * GETTIME;
	}
	else
		m_tInfo.vPos += m_tInfo.vDir * 150 * GETTIME;

	if(fDist < 5.f)
		pBestList->pop_front();
}
void CLobber::SetAngle(D3DXVECTOR3 vDir)
{
	float fX = vDir.x - m_tInfo.vPos.x;
	float fY = vDir.y - m_tInfo.vPos.y;

	float fDist = sqrt(fX * fX + fY * fY);

	float fAngle = acosf(fX / fDist) * 180 / D3DX_PI;

	if(m_tInfo.vPos.y < vDir.y)
	{
		fAngle = 360 - fAngle;
	}

	float fSource = 11.25f;

	int iIndex = 1;

	for(int i = 1; i < 33; ++i)
	{
		if (fAngle <= fSource * i)
		{
			iIndex = i;
			break;
		}
	}

	if(iIndex == m_iPrevIndex)
		return;

	m_iPrevIndex = iIndex;

	switch(m_eState)
	{
	case MONSTER_RUN:
		m_RunFrame.iEndFrame = iIndex * 20 - 1;
		m_RunFrame.iStartFrame = m_RunFrame.iEndFrame - 19;
		m_RunFrame.iCur = m_RunFrame.iStartFrame;		
		break;

	case MONSTER_ATTACK:
		m_AttackFrame.iEndFrame = iIndex * 15 - 1;
		m_AttackFrame.iStartFrame = m_AttackFrame.iEndFrame - 14;
		m_AttackFrame.iCur = m_AttackFrame.iStartFrame;
		break;
	}
}

void CLobber::SyncHpBar(void)
{
	float fHPPersent = (float)100 - (((float)m_tData.iHp / (float)m_tData.iMaxHp) * 100);

	float fHpBar = (196 * fHPPersent / 100);

	m_rcHpBar.right = int(196 - fHpBar);
}

void CLobber::HitAction(int Att)
{
	if(m_eState == MONSTER_ATTACK || m_eState == MONSTER_RUN)
	{
		m_bHit = true;

		m_tData.iHp -= Att;

		if(m_tData.iHp <= 0)
		{
			m_eState = MONSTER_EGG;
			m_iRestoreCount = 10;
			//-100

			D3DXVECTOR3 vEffectPos = m_tInfo.vPos;
			vEffectPos.y += 100;
			CObj* pObj = CFactory<CSkillEffect>::CreateObj(vEffectPos, EFFECT_LOBBER_DEATH);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
		}

		m_dwHitTime = GetTickCount();
	}
}