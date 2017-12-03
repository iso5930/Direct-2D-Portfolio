#include "StdAfx.h"
#include "Boss.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MonsterBullet.h"
#include "Factory.h"
#include "Player.h"
#include "Lobber.h"
#include "BossLobber.h"

CBoss::CBoss(void) 
	:m_DeathFrame(0, 94, 0, 20), m_ChageFrame(0, 86, 0, 20), m_AttackFrame(0, 13, 0, 20)
{
	
}

CBoss::~CBoss(void)
{
	Release();
}

void CBoss::HitAction(int Att)
{
	m_tData.iHp -= Att;

	HpBarSync();

	if(m_tData.iHp <= 0)
	{
		m_tData.iHp = 0;
		m_eState = MONSTER_DEATH;
	}

	switch(m_iSummerCount)
	{
	case 0:
		if(m_tData.iHp <= 3000)
		{
			++m_iSummerCount;
			m_AttType = BOSS_ATT2;
			m_bChage = false;
			m_iCount = 0;
		}
		break;

	case 1:
		if(m_tData.iHp <= 2000)
		{
			++m_iSummerCount;
			m_AttType = BOSS_ATT2;
			m_bChage = false;
			m_iCount = 0; 
		}
		break;

	case 2:
		if(m_tData.iHp <= 1000)
		{
			++m_iSummerCount;
			m_AttType = BOSS_ATT2;
			m_bChage = false;
			m_iCount = 0;
		}
		break;
	}

	//2번 정도 소환하자..
}

HRESULT CBoss::Initialize(void)
{
	m_dwTime = GetTickCount();
	m_dwChangeTime = GetTickCount();
	m_dwSummerTime = GetTickCount();

	m_eState = MONSTER_ATTACK;

	m_vBulletPos = m_tInfo.vPos;
	m_vBulletPos.x += 157;

	m_tInfo.vLook.x = 1;
	m_tInfo.vLook.y = 0;
	m_tInfo.vLook.z = 0;

	m_bTrans = false;
	m_bTransProgress = false;
	m_bChage = false;

	m_tData.iMaxHp = 5000;
	m_tData.iHp = 5000;
	m_tData.iAtt = 50;

	m_rcHpBar.top = 0;
	m_rcHpBar.left = 0;
	m_rcHpBar.right = 196;
	m_rcHpBar.bottom = 60;

	m_AttType = BOSS_ATT1;

	m_iSummerCount = 0;

	m_tInfo.fCX = 207.f;
	m_tInfo.fCY = 190.f;

	m_strName = "Boss";
	m_bDeath = false;

	m_dwCreateTime = 100;
	
	return S_OK;
}

int	CBoss::Progress(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();

	if(m_bTrans)
	{
		return 0;
	}

	if(m_bTransProgress)
	{
		m_DeathFrame.dwFrameTime = 200;
		m_ChageFrame.dwFrameTime = 200;
		m_AttackFrame.dwFrameTime = 200;
		m_dwCreateTime = 2000;
	}
	else
	{
		m_DeathFrame.dwFrameTime = 20;
		m_ChageFrame.dwFrameTime = 20;
		m_AttackFrame.dwFrameTime = 20;
		m_dwCreateTime = 200;
	}

	if(m_eState == MONSTER_DEATH || m_bDeath)
	{
		FrameMove();
		m_bChage = false;
		return 0;
	}

	if(m_AttType != BOSS_ATT2)
	{
		if(m_dwChangeTime + 5000 < GetTickCount())
		{
			m_dwChangeTime = GetTickCount();

			m_eState = MONSTER_ATTACK;
			m_AttType = BOSS_ATT1;
			m_bChage = true;
		}
	}
	
	if(m_AttType == BOSS_ATT2)
	{
		Attack2();
	}

	if(m_bChage || m_AttType == BOSS_ATT2)
	{
		FrameMove();
	}
	else
	{
		SetAngle();
	}

	if(m_eState == MONSTER_ATTACK && m_bChage == false)
	{
		Attack();
	}

	CObj::Progress();
	return 0;
}

void CBoss::Render(void)
{
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossBody", m_iBodyIndex);

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

	float fX = pTexInfo->ImgInfo.Width / 2.f;
	float fY = pTexInfo->ImgInfo.Height / 2.f;
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_bDeath)
	{
		return;
	}

	if(m_eState == MONSTER_DEATH)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossDeath", m_DeathFrame.iCur);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y + 73.f, 0.f);

		m_tInfo.matWorld = matTrans;
	}
	else if(m_AttType == BOSS_ATT2)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossDeath", m_AttackFrame.iCur);
		
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y + 73.f, 0.f);

		m_tInfo.matWorld = matTrans;
	}
	else
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossHead", m_iHeadIndex);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y + 73.f, 0.f);

		m_tInfo.matWorld = matScale * matTrans;
	}

	fX = pTexInfo->ImgInfo.Width / 2.f;
	fY = pTexInfo->ImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(m_bChage)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossCharge", m_ChageFrame.iCur);

		D3DXMatrixTranslation(&matTrans, m_vBulletPos.x - g_Scroll.x, m_vBulletPos.y - g_Scroll.y, 0.f);

		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = pTexInfo->ImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MonsterHpBar");

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 36.f - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y + 40.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &m_rcHpBar, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 0, 0, 0));

	//text

	TCHAR szName[10] = L"Spine";

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 144.f, m_tInfo.vPos.y - g_Scroll.y + 26.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szName, lstrlen(szName), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szHp[50] = L"";
	TCHAR szTemp[10] = L"";

	_itow_s(m_tData.iHp, szTemp, 10);

	lstrcat(szHp, szTemp);

	lstrcat(szHp, L" / ");

	_itow_s(m_tData.iMaxHp, szTemp, 10);

	lstrcat(szHp, szTemp);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 105.f, m_tInfo.vPos.y - g_Scroll.y + 52.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szHp, lstrlen(szHp), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	//HDC dc =GetDC(g_hWnd);

	//Rectangle(dc, m_Rect.left - g_Scroll.x, m_Rect.top - g_Scroll.y, m_Rect.right - g_Scroll.x, m_Rect.bottom - g_Scroll.y);
}

void CBoss::Release(void)
{
	
}

void CBoss::SetAngle(void)
{
	D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

	float fX = vPos.x - m_tInfo.vPos.x;
	float fY = vPos.y - m_tInfo.vPos.y; 

	float fDist = sqrt( fX * fX + fY * fY );

	float fAngle = acosf(fX / fDist) * 180 / D3DX_PI;

	if(m_tInfo.vPos.y < vPos.y)
	{
		fAngle = 360 - fAngle;
	}
	
	float fSource = 1.40625f;

	int iIndex = 0;

	for(int i = 1; i <= 257; ++i)
	{
		if(fAngle <= fSource * i)
		{
			iIndex = i;
			break;
		}
	}

	m_iHeadIndex = iIndex - 1;
	m_iBodyIndex = m_iHeadIndex / 2;
}

void CBoss::FrameMove(void)
{
	if(m_AttType == BOSS_ATT2)
	{
		if(m_dwTime + m_AttackFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_AttackFrame.iCur > m_AttackFrame.iEndFrame)
			{
				m_AttackFrame.iCur = m_AttackFrame.iStartFrame;
			}
		}
	}

	switch(m_eState)
	{
	case MONSTER_ATTACK:
		if(m_dwTime + m_ChageFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_ChageFrame.iCur > m_ChageFrame.iEndFrame)
			{
				m_ChageFrame.iCur = m_ChageFrame.iStartFrame;
				m_bChage = false;
			}
		}
		break;

	case MONSTER_DEATH:
		if(m_dwTime + m_DeathFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_DeathFrame.iCur > m_DeathFrame.iEndFrame)
			{
				m_bDeath = true;
				m_DeathFrame.iCur = m_DeathFrame.iStartFrame;
			}
		}
		break;
	}	
}

void CBoss::Attack(void)
{
	D3DXVECTOR3 vPlayerPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

	D3DXVECTOR3 vDist = vPlayerPos - m_tInfo.vPos;

	D3DXVec3Normalize(&vDist, &vDist);

	float fAngle = D3DXVec3Dot(&vDist, &m_tInfo.vLook);
	
	fAngle = acosf(fAngle);

	if(m_tInfo.vPos.y < vPlayerPos.y)
	{
		fAngle = 2 * D3DX_PI - fAngle;
	}
	
	float fX = m_tInfo.vPos.x;
	float fY = m_tInfo.vPos.y + 129.f;

	fX += 90.f * cosf(fAngle);
	fY -= 39.f * sinf(fAngle);
	
	m_vBulletPos.x = fX;
	m_vBulletPos.y = fY;

	if(m_dwTime + m_dwCreateTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		D3DXVECTOR3 vBulletPos = m_vBulletPos;

		vBulletPos += vDist * 20;

		CObj* pObj = CFactory<CMonsterBullet>::CreateObj(vBulletPos, vDist, fAngle, BULLET_BOSS);

		CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER_BULLET);
	}
}

void CBoss::HpBarSync(void)
{
	float fHPPersent = (float)100 - (((float)m_tData.iHp / (float)m_tData.iMaxHp) * 100);

	float fHpBar = (196 * fHPPersent / 100);

	m_rcHpBar.right = int(196 - fHpBar);
}

void CBoss::Attack2(void)
{
	//소환 패턴...
	float fX = 800;
	float fY = 192;

	if(m_iCount > 10)
	{
		m_AttType = BOSS_ATT1;
	}

	if(m_dwSummerTime + 300 < GetTickCount())
	{
		m_dwSummerTime = GetTickCount();

		fX += rand() % 533;
		fY += rand() % 246;

		CObj* pObj = CFactory<CBossLobber>::CreateObj(D3DXVECTOR3(fX, fY, 0.f));
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);
		++m_iCount;
	}
}