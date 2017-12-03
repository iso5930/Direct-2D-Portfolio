#include "StdAfx.h"
#include "SkillEffect.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Factory.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Player.h"

CSkillEffect::CSkillEffect(void)
{

}

CSkillEffect::~CSkillEffect(void)
{
	Release();
}

HRESULT CSkillEffect::Initialize(void)
{
	m_dwTime = GetTickCount();

	switch(m_eType)
	{
	case EFFECT_DECAL:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 25;
		m_Frame.dwFrameTime = 20;
		break;

	case EFFECT_BOMB_CORE:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 8;
		m_Frame.dwFrameTime = 15;
		m_tInfo.vPos.y -= 46.f;
		break;

	case EFFECT_BOMB_SECOND:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 28;
		m_Frame.dwFrameTime = 10;
		break;

	case EFFECT_SLAM_FIRE:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 12;
		m_Frame.dwFrameTime = 20;
		break;

	case EFFECT_SLAM_SQUARE:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 11;
		m_Frame.dwFrameTime = 10;
		break;

	case EFFECT_LOBBER_DEATH:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 36;
		m_Frame.dwFrameTime = 30;
		break;

	case EFFECT_LOBBER_TARGETING:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 35;
		m_Frame.dwFrameTime = 30;
		break;

	case EFFECT_LOBBER_TARGET_ATTACK:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 50;
		m_Frame.dwFrameTime = 20;
		break;

	case EFFECT_LADY_WARP:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 79;
		m_Frame.dwFrameTime = 5;
		break;

	case EFFECT_BOSS_BULLET_RELEASE:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 4;
		m_Frame.dwFrameTime = 10;
		break;

	case EFFECT_DAMAGE:
		m_Frame.iCur = 0;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 16;
		m_Frame.dwFrameTime = 50;
		break;
	}

	return S_OK;
}

int	CSkillEffect::Progress(void)
{
	bool bTrans = ((CPlayer*)CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front())->GetTrans();
	bool bTransProgress = ((CPlayer*)CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front())->GetTransProgress();

	if(bTrans)
	{
		return 0;
	}

	if(bTransProgress)
	{
		switch(m_eType)
		{
		case EFFECT_DECAL:
			m_Frame.dwFrameTime = 200;
			break;

		case EFFECT_BOMB_CORE:
			m_Frame.dwFrameTime = 150;
			break;

		case EFFECT_BOMB_SECOND:
			m_Frame.dwFrameTime = 100;
			break;

		case EFFECT_SLAM_FIRE:
			m_Frame.dwFrameTime = 200;
			break;

		case EFFECT_SLAM_SQUARE:
			m_Frame.dwFrameTime = 100;
			break;

		case EFFECT_LOBBER_DEATH:
			m_Frame.dwFrameTime = 300;
			break;

		case EFFECT_LOBBER_TARGETING:
			m_Frame.dwFrameTime = 300;
			break;

		case EFFECT_LOBBER_TARGET_ATTACK:
			m_Frame.dwFrameTime = 200;
			break;

		case EFFECT_LADY_WARP:
			m_Frame.dwFrameTime = 50;
			break;

		case EFFECT_BOSS_BULLET_RELEASE:
			m_Frame.dwFrameTime = 100;
			break;

		case EFFECT_DAMAGE:
			m_Frame.dwFrameTime = 500;
			break;
		}
	}
	else
	{
		switch(m_eType)
		{
		case EFFECT_DECAL:
			m_Frame.dwFrameTime = 20;
			break;

		case EFFECT_BOMB_CORE:
			m_Frame.dwFrameTime = 15;
			break;

		case EFFECT_BOMB_SECOND:
			m_Frame.dwFrameTime = 10;
			break;

		case EFFECT_SLAM_FIRE:
			m_Frame.dwFrameTime = 20;
			break;

		case EFFECT_SLAM_SQUARE:
			m_Frame.dwFrameTime = 10;
			break;

		case EFFECT_LOBBER_DEATH:
			m_Frame.dwFrameTime = 30;
			break;

		case EFFECT_LOBBER_TARGETING:
			m_Frame.dwFrameTime = 30;
			break;

		case EFFECT_LOBBER_TARGET_ATTACK:
			m_Frame.dwFrameTime = 20;
			break;

		case EFFECT_LADY_WARP:
			m_Frame.dwFrameTime = 5;
			break;

		case EFFECT_BOSS_BULLET_RELEASE:
			m_Frame.dwFrameTime = 10;
			break;

		case EFFECT_DAMAGE:
			m_Frame.dwFrameTime = 50;
			break;
		}
	}

	if(m_dwTime + m_Frame.dwFrameTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		if(++m_Frame.iCur > m_Frame.iEndFrame)
		{
			//m_Frame.iCur = m_Frame.iStartFrame;
			return 1;
		}
	}

	return 0;
}

void CSkillEffect::Render(void)
{
	const TEXINFO* pTexInfo = NULL;

	D3DXMATRIX matScale;
	D3DXMATRIX matTrans;
	D3DXMATRIX matWorld;

	switch(m_eType)
	{
	case EFFECT_DECAL:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"AttackEffect", m_Frame.iCur);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_BOMB_CORE:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BombCoreRelease", m_Frame.iCur);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_BOMB_SECOND:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BombSecondRelease", m_Frame.iCur);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height - 81.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_SLAM_FIRE:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SlamFire", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_LOBBER_DEATH:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Monster_Death", m_Frame.iCur);
	
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			matWorld = matScale * matTrans;
			
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_LOBBER_TARGETING:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LobberTargeting", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_LOBBER_TARGET_ATTACK:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LobberTargetAttack", m_Frame.iCur);
			
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_LADY_WARP:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyWarp", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_BOSS_BULLET_RELEASE:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossBulletRelease", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case EFFECT_DAMAGE:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"DamageEffect", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, (float)pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;
	}
}

void CSkillEffect::Release(void)
{
	if(m_eType == EFFECT_LOBBER_TARGETING)
	{
		D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

		if(CCollisionMgr::GetInstance()->PtInCircle(m_tInfo.vPos.x, m_tInfo.vPos.y, 355, 177, vPos.x, vPos.y))
		{
			//여기서 플레이어의 HP를 깍는다..

			((CPlayer*)CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front())->PlayerHit(100);

			//플레이어에 HIT함수 만들어서 하자..

			CObj* pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_LOBBER_TARGET_ATTACK);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
		}
	}
}