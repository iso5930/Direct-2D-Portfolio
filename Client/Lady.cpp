#include "StdAfx.h"
#include "Lady.h"
#include "Astar.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "TimeMgr.h"
#include "Player.h"
#include "MonsterBullet.h"
#include "Factory.h"
#include "SkillEffect.h"

CLady::CLady(void)
:m_DeathFrame(0, 11, 0, 30), m_AttackFrame(0, 7, 0, 40)
{
	
}	

CLady::~CLady(void)
{
	
}

HRESULT CLady::Initialize(void)
{
	m_pAstar = new CAstar;

	m_iPrevIndex = 0;

	m_eState = MONSTER_WAIT;

	m_strName = "Lady";

	m_bTrans = false;
	m_bTransProgress = false; 

	m_dwAstar = GetTickCount();
	m_dwTime = GetTickCount();
	m_dwHitTime = GetTickCount();
	m_dwAttackTime = GetTickCount();
	m_dwBulletTime = GetTickCount();

	m_tData.iHp = 1000;
	m_tData.iMaxHp = 1000;

	m_vBulletPos = m_tInfo.vPos;
	m_vBulletPos.y -= 95.f;	//95정도 위에
	m_vBulletPos.x += 35.f; 

	m_vCretePos = m_tInfo.vPos;
	m_vCretePos.y -= 95.f;
	m_vOriginBullet = m_vBulletPos;

	m_tInfo.vLook.x = 1;
	m_tInfo.vLook.y = 0;
	m_tInfo.vLook.z = 0;

	m_bAttackWait = false;

	m_rcHpBar.left = 0;
	m_rcHpBar.right = 196;
	m_rcHpBar.top = 0;
	m_rcHpBar.bottom = 60;

	m_tInfo.fCX = 35.f;
	m_tInfo.fCY = 96.6f;

	m_iAttTime = 30;

	m_bDeath = false;

	return S_OK;
}

int CLady::Progress(void)
{
	if(m_tData.iHp <= 0)
	{
		m_eState = MONSTER_DEATH;
	}

	if(m_bDeath)
	{
		return 1;
	}

	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();
	D3DXVECTOR3 vPlayerPos = pObj->GetInfo()->vPos;
	D3DXVECTOR3 vDist = vPlayerPos - m_tInfo.vPos;

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();

	if(m_bTrans)
	{
		return 0;
	}

	if(m_bTransProgress)
	{
		m_iAttTime = 300;
		m_DeathFrame.dwFrameTime = 300;
		m_AttackFrame.dwFrameTime = 400;
	}
	else
	{
		m_iAttTime = 30;
		m_DeathFrame.dwFrameTime = 30;
		m_AttackFrame.dwFrameTime = 40;
	}

	if(m_eState == MONSTER_DEATH)
	{
		FrameMove();

		return 0;
	}

	float fLength = D3DXVec3Length(&vDist);
	
	if(m_bAttack)
	{
		D3DXVec3Normalize(&vDist, &vDist);

		float fAngle = D3DXVec3Dot(&vDist, &m_tInfo.vLook);

		fAngle = acosf(fAngle);

		if(m_tInfo.vPos.y < vPlayerPos.y)
		{
			fAngle = 2 * D3DX_PI - fAngle;
		}

		D3DXMATRIX matRotZ;
		D3DXMATRIX matTrans;
		D3DXMATRIX matWorld;

		D3DXMatrixRotationZ(&matRotZ, -fAngle);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y - 95.f, 0.f);

		matWorld = matRotZ * matTrans;

		m_vBulletPos = m_vOriginBullet;
		m_vBulletPos -= m_vCretePos;

		D3DXVec3TransformCoord(&m_vBulletPos, &m_vBulletPos, &matWorld);

		SetAngle(vPlayerPos);
		
		if(m_dwAttackTime + 500 < GetTickCount())
		{
			m_dwAttackTime = GetTickCount();
			m_bAttack = false;
			m_bAttackWait = false;
		}

		if(m_AttackFrame.iCur == m_AttackFrame.iEndFrame - 1)
		{
			if(m_dwTime + m_iAttTime < GetTickCount())
			{
				m_dwTime = GetTickCount();
				m_AttackFrame.iCur = m_AttackFrame.iEndFrame;

				CObj* pObj = CFactory<CMonsterBullet>::CreateObj(m_vBulletPos, vDist, fAngle, BULLET_LADY);
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER_BULLET);
			}
		}
		else
		{
			if(m_dwTime + m_iAttTime < GetTickCount())
			{
				m_dwTime = GetTickCount();
				m_AttackFrame.iCur = m_AttackFrame.iEndFrame - 1;
			}
		}
		return 0;
	}

	if(fLength >= 500)
	{
		m_eState = MONSTER_RUN;

		if(m_dwHitTime + 700 < GetTickCount())
		{
			m_dwHitTime = GetTickCount();

			m_pAstar->AstarStartPos(m_tInfo.vPos, vPlayerPos);
		}
	}
	else
	{
		if(m_dwAttackTime + 1000 < GetTickCount())
		{
			m_dwAttackTime = GetTickCount();
			m_eState = MONSTER_ATTACK;
			m_bAttackWait = true;
		}
		else
		{
			if(m_bAttackWait == false)
			{
				if(fLength >= 250)
				{
					m_eState = MONSTER_RUN;

					if(m_dwHitTime + 500 < GetTickCount())
					{
						m_dwHitTime = GetTickCount();

						m_pAstar->AstarStartPos(m_tInfo.vPos, vPlayerPos);
					}
				}
				else
				{
					m_eState = MONSTER_WAIT;
				}
			}
		}
	}

	if(m_eState == MONSTER_RUN)
	{
		AStarMove();
	}

	if(m_eState == MONSTER_ATTACK || m_eState == MONSTER_WAIT)
	{
		SetAngle(vPlayerPos);
	}
	else
	{
		SetAngle(m_vTarget);//타일 기반
	}

	FrameMove();

	CObj::Progress();

	return 0;
}

void CLady::Render(void)
{
	const TEXINFO* pTexInfo = NULL;
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	if(m_eState != MONSTER_DEATH)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyBody", m_iBodyIndex);
	}
	else
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyDeath", m_DeathFrame.iCur);
	}

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//바디

	if(m_eState == MONSTER_DEATH)
	{
		return;
	}

	if(m_eState == MONSTER_ATTACK)
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyAttack", m_AttackFrame.iCur);
	}
	else if(m_eState == MONSTER_WAIT)
	{	
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyClose", m_iPrevIndex);
	}
	else
	{
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyEyes", m_iPrevIndex);
	}

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y - 99.4f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//머리
//=====================================================================================
	//UI
	pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MonsterHpBar");

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 20.f, m_tInfo.vPos.y - g_Scroll.y - 150.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255)); //뒤 배경..

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &m_rcHpBar, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 0, 255, 0));

	//text ======================

	TCHAR szName[10] = L"Lady";

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 146, m_tInfo.vPos.y - g_Scroll.y - 165.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szName, lstrlen(szName), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	TCHAR szHp[50] = L"";
	TCHAR szTemp[10] = L"";

	_itow_s(m_tData.iHp, szTemp, 10);

	lstrcat(szHp, szTemp);

	lstrcat(szHp, L" / ");

	_itow_s(m_tData.iMaxHp, szTemp, 10);

	lstrcat(szHp, szTemp);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x + 98, m_tInfo.vPos.y - g_Scroll.y - 140, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szHp, lstrlen(szHp), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CLady::Release(void)
{
	delete m_pAstar;
	m_pAstar = NULL;
}
void CLady::SyncHpBar(void)
{
	float fHPPersent = (float)100 - (((float)m_tData.iHp / (float)m_tData.iMaxHp) * 100);

	float fHpBar = (196 * fHPPersent / 100);

	m_rcHpBar.right = int(196 - fHpBar);
}

void CLady::HitAction(int Att)
{
	m_tData.iHp -= Att;

	SyncHpBar();

	//피격 이펙트 & 순간이동 이펙트
	//사라질때 한번.

	if(m_tData.iHp <= 0)
	{
		m_tData.iHp = 0;
	}

	vector<TILE*> vecTile = (*((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->GetTile());

	for(int i = 0; i < 100; ++i)
	{
		int iX = int(rand() % 1280) + (int)g_Scroll.x;
		int iY = int(rand() % 720) + (int)g_Scroll.y;

		D3DXVECTOR3 vBlink((float)iX, (float)iY, 0.f);

		int iIndex = m_pAstar->GetTileIndex(vBlink);

		if(vecTile[iIndex]->byOption == 0)
		{
			CObj* pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_LADY_WARP);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
			m_tInfo.vPos = vecTile[iIndex]->vPos;
			break;
		}
	}
}

void CLady::SetAngle(D3DXVECTOR3 vPos)
{
	float fX = vPos.x - m_tInfo.vPos.x;
	float fY = vPos.y - m_tInfo.vPos.y;

	float fDist = sqrt( fX * fX + fY * fY );

	float fAngle = acosf(fX / fDist) * 180 / D3DX_PI;

	if(m_tInfo.vPos.y < vPos.y)
	{
		fAngle = 360 - fAngle;
	}
	//여기서 부터 구하는 공식.

	float fSource;
	int iIndex = 0;

	fSource = 5.625f;

	for(int i = 0; i < 64; ++i)
	{
		if(fAngle <= fSource * (i + 1))
		{
			iIndex = i;
			break;
		}
	}

	m_iBodyIndex = iIndex;

	switch(m_eState)
	{
	case MONSTER_ATTACK:
		fSource = 11.25f;

		for(int i = 0; i < 32; ++i)
		{
			if(fAngle <= fSource * (i + 1))
			{
				iIndex = i;
				break;
			}
		}

		if(m_iPrevIndex == iIndex)
		{
			return;
		}

		m_iPrevIndex = iIndex;
		m_AttackFrame.iEndFrame = (iIndex + 1) * 8 - 1;
		m_AttackFrame.iStartFrame = m_AttackFrame.iEndFrame - 7;
		m_AttackFrame.iCur = m_AttackFrame.iStartFrame;

		if(m_bAttack)
		{
			m_AttackFrame.iCur = m_AttackFrame.iEndFrame - 1;
		}
		break;

	case MONSTER_DEATH:
		fSource = 11.25f;

		for(int i = 0; i < 32; ++i)
		{
			if(fAngle <= fSource * (i + 1))
			{
				iIndex = i;
				break;
			}
		}

		if(m_iPrevIndex == iIndex)
		{
			return;
		}

		m_iPrevIndex = iIndex;
		m_AttackFrame.iEndFrame = (iIndex + 1) * 12 - 1;
		m_AttackFrame.iStartFrame = m_AttackFrame.iEndFrame - 11;
		m_AttackFrame.iCur = m_AttackFrame.iStartFrame;

		break;

	default:
		fSource = 5.625f;

		for(int i = 0; i < 64; ++i)
		{
			if(fAngle <= fSource * (i + 1))
			{
				iIndex = i;
				break;
			}
		}

		m_iPrevIndex = iIndex;
		break;
	}
}

void CLady::FrameMove(void)
{
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
				m_bAttack = true;
				m_dwAttackTime = GetTickCount();
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
				CObj* pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_LOBBER_DEATH);
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
			}
		}
		break;
	}
}

void CLady::AStarMove(void)
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
