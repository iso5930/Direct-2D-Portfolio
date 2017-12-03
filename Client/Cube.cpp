#include "StdAfx.h"
#include "Cube.h"
#include "Device.h"
#include "TextureMgr.h"
#include "TimeMgr.h"
#include "BackGround.h"
#include "ObjMgr.h"
#include "Player.h"

CCube::CCube(void)
	:m_LoopFrame(0, 59, 0, 10), m_CreateFrame(0, 8, 0, 30), m_DeathFrame(0, 10, 0, 30)
{
	
}

CCube::~CCube(void)
{
	Release();
}

HRESULT CCube::Initialize(void)
{
	m_dwTime = GetTickCount();

	m_tData.iMaxHp = 800;
	m_tData.iHp = 800;

	m_tInfo.fCX = 46.f;
	m_tInfo.fCY = 88.f;

	m_eState = CUBE_WAIT;

	m_strName = "Cube";

	m_vecTile = ((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->GetTile();

	(*m_vecTile)[m_iTileIndex]->byDrawID = 4;
	(*m_vecTile)[m_iTileIndex]->byOption = 4;

	m_tInfo.vPos = (*m_vecTile)[m_iTileIndex]->vPos;

	return S_OK;
}

int CCube::Progress(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();
	bool bTrans = ((CPlayer*)pObj)->GetTrans();

	if(bTrans)
	{
		return 0;
	}

	if(m_bCreate)
	{
		D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;
		D3DXVECTOR3 vDist = vPos - m_tInfo.vPos;

		float fLength = D3DXVec3Length(&vDist);

		if(fLength <= 400)
		{
			m_eState = CUBE_CREATE;
			m_bCreate = false;
		}
	}

	if(m_bHit)
	{
		if(m_dwHitTime + 300 < GetTickCount())
		{
			m_dwHitTime = GetTickCount();
			m_bHit = false;
		}
	}

	if(m_eState == CUBE_END)
	{
		if(m_dwHitTime + 100 < GetTickCount())
		{
			m_dwHitTime = GetTickCount();
			m_tData.iHp += 25;
		}

		if(m_tData.iHp >= m_tData.iMaxHp)
		{
			m_eState = CUBE_CREATE;
			(*m_vecTile)[m_iTileIndex]->byDrawID = 4;
			(*m_vecTile)[m_iTileIndex]->byOption = 4;
		}
	}

	if(m_eState != CUBE_WAIT)
	{
		FrameMove();
		CObj::Progress();
	}
	
	return 0;
}

void CCube::Render(void)
{
	if(m_eState == CUBE_END || m_eState == CUBE_WAIT)
	{
		return;
	}

	const TEXINFO* pTexInfo = NULL;

	switch(m_eState)
	{
	case CUBE_CREATE:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CubeCreate", m_CreateFrame.iCur);
		break;

	case CUBE_LOOP:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CubeLoop", m_LoopFrame.iCur);
		break;

	case CUBE_DEATH:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CubeDeath", m_DeathFrame.iCur);
		break;
	}

	D3DXMATRIX matTrans;
	D3DXMATRIX	matScale;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);
	D3DXMatrixScaling(&matScale, 0.37f, 0.37f, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	if(m_bHit)
	{
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (pTexInfo->ImgInfo.Height / 2.f) + 68.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 0, 0));
	}
	else
	{
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (pTexInfo->ImgInfo.Height / 2.f) + 68.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CCube::Release(void)
{
	
}

void CCube::CubeHitAction(int Att)
{
	if(m_eState == CUBE_LOOP)
	{
		m_bHit = true;

		m_tData.iHp -= Att;

		if(m_tData.iHp <= 0)
		{
			m_eState = CUBE_DEATH;
			(*m_vecTile)[m_iTileIndex]->byDrawID = 0;
			(*m_vecTile)[m_iTileIndex]->byOption = 0;
		}

		m_dwHitTime = GetTickCount();
	}
}

void CCube::FrameMove(void)
{
	switch(m_eState)
	{
	case CUBE_CREATE:
		if(m_dwTime + m_CreateFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_CreateFrame.iCur > m_CreateFrame.iEndFrame)
			{
				m_CreateFrame.iCur = m_CreateFrame.iStartFrame;
				m_eState = CUBE_LOOP;
			}
		}
		break;

	case CUBE_LOOP:
		if(m_dwTime + m_LoopFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_LoopFrame.iCur > m_LoopFrame.iEndFrame)
			{
				m_LoopFrame.iCur = m_LoopFrame.iStartFrame;
			}
		}
		break;

	case CUBE_DEATH:
		if(m_dwTime + m_DeathFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_DeathFrame.iCur > m_DeathFrame.iEndFrame)
			{
				m_eState = CUBE_END;
				m_DeathFrame.iCur = m_DeathFrame.iStartFrame;
			}
		}
		break;
	}
}