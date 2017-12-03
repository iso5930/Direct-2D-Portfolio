#include "StdAfx.h"
#include "MonsterBullet.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "..\Tool\Device.h"
#include "ObjMgr.h"
#include "Factory.h"
#include "SkillEffect.h"
#include "Player.h"

CMonsterBullet::CMonsterBullet(void)
{

}

CMonsterBullet::~CMonsterBullet(void)
{
	Release();
}

HRESULT CMonsterBullet::Initialize(void)
{
	switch(m_eType)
	{
	case BULLET_LADY:
		m_BulletFrame.iStartFrame = 0;
		m_BulletFrame.iEndFrame = 10;
		m_BulletFrame.iCur = 0;
		m_BulletFrame.dwFrameTime = 20;
		m_fScaleX = 0.3f;
		m_fScaleY = 0.3f;
		m_strName = "BulletLady";
		m_tInfo.fCX = 20.f;
		m_tInfo.fCY = 20.f;
		m_tData.iAtt = 20;
		m_iBulletSpeed = 1000;
		m_fScaleSpeed = 50.f;
		break;

	case BULLET_BOSS:
		m_BulletFrame.iStartFrame = 0;
		m_BulletFrame.iEndFrame = 14;
		m_BulletFrame.iCur = 0;
		m_BulletFrame.dwFrameTime = 20;
		m_strName = "BossBullet";
		m_tData.iAtt = 50;
		m_fScaleX = 0.5f;
		m_fScaleY = 0.5f;
		m_tInfo.fCX = 16.f;
		m_tInfo.fCY = 16.f;
		m_iBulletSpeed = 800;
		break;
	}

	m_dwTime = GetTickCount();

	m_bDeath = false;

	m_tInfo.vLook.x = 1;
	m_tInfo.vLook.y = 0;
	m_tInfo.vLook.z = 0;

	D3DXVECTOR3 vPlayerPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

	vPlayerPos.y -= 41.4f;

	D3DXVECTOR3 vDist = vPlayerPos - m_tInfo.vPos;

	D3DXVec3Normalize(&vDist, &vDist);

	float fAngle = D3DXVec3Dot(&vDist, &m_tInfo.vLook);

	fAngle = acosf(fAngle);

	if(m_tInfo.vPos.y < vPlayerPos.y)
	{
		fAngle = 2 * D3DX_PI - fAngle;
	}

	m_tInfo.fAngle = fAngle;

	m_tInfo.vDir = vDist;

	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();


	return S_OK;
}

int CMonsterBullet::Progress(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();


	if(m_bDeath)
	{
		return 1;
	}
	else if(m_tInfo.vPos.x > 4318 || m_tInfo.vPos.x < 0
		|| m_tInfo.vPos.y  > 2077 || m_tInfo.vPos.y < 0)
	{
		return 1;
	}
	else if(m_fScaleX <= 0.f || m_fScaleY <= 0.f)
	{
		return 1;
	}

	if(m_bTrans)
	{
		return 0;
	}

	if(m_bTransProgress)
	{
		switch(m_eType)
		{
		case BULLET_LADY:
			m_iBulletSpeed = 10;
			m_fScaleSpeed = 0.5f;
			break;

		case BULLET_BOSS:
			m_iBulletSpeed = 8;
			break;
		}
	}
	else
	{
		switch(m_eType)
		{
		case BULLET_LADY:
			m_iBulletSpeed = 1000;
			m_fScaleSpeed = 50;
			break;

		case BULLET_BOSS:
			m_iBulletSpeed = 800;
			break;
		}
	}

	switch(m_eType)
	{
	case BULLET_LADY:
		m_tInfo.vPos += m_tInfo.vDir * (float)m_iBulletSpeed * GETTIME;
		m_fScaleX -= 0.005f * m_fScaleSpeed * GETTIME;
		m_fScaleY -= 0.005f * m_fScaleSpeed * GETTIME;
		break;

	case BULLET_BOSS:
		m_tInfo.vPos += m_tInfo.vDir * (float)m_iBulletSpeed * GETTIME;
		break;
	}
	
	CObj::Progress();

	return 0;
}

void CMonsterBullet::Render(void)
{
	const TEXINFO* pTexInfo = NULL;

	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMATRIX matRotZ;

	float fX = 0.f;
	float fY = 0.f;
	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, -m_tInfo.fAngle);

	switch(m_eType)
	{
	case BULLET_LADY:
		D3DXMatrixScaling(&matScale, m_fScaleX, m_fScaleY, 0.f);
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LadyBullet", m_BulletFrame.iCur);

		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = pTexInfo->ImgInfo.Height / 2.f;

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

		m_tInfo.matWorld = matScale * matRotZ * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;

	case BULLET_BOSS:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"BossBullet", m_BulletFrame.iCur);

		fX = pTexInfo->ImgInfo.Width / 2.f;
		fY = pTexInfo->ImgInfo.Height / 2.f;

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

		m_tInfo.matWorld = matScale * matRotZ * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	}

	//HDC dc = GetDC(g_hWnd);
	//Rectangle(dc, m_Rect.left - g_Scroll.x, m_Rect.top - g_Scroll.y, m_Rect.right - g_Scroll.x, m_Rect.bottom - g_Scroll.y);

}

void CMonsterBullet::Release(void)
{
	//ÀÌÆåÆ® »ý¼º.
	D3DXVECTOR3 vEffectPos = m_tInfo.vPos;

	if(m_eType == BULLET_BOSS)
	{
		vEffectPos += m_tInfo.vDir * 20;

		CObj* pObj = CFactory<CSkillEffect>::CreateObj(vEffectPos, EFFECT_BOSS_BULLET_RELEASE);
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);

		pObj = CFactory<CSkillEffect>::CreateObj(vEffectPos, EFFECT_DAMAGE);
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
	}
};

bool CMonsterBullet::HitCheck(CObj* pObj)
{
	list<CObj*>::iterator iter = m_HitList.begin();
	list<CObj*>::iterator iter_end = m_HitList.end();

	for(iter; iter != iter_end; ++iter)
	{
		if(pObj == (*iter))
		{
			return false;
		}
	}

	m_HitList.push_back(pObj);

	m_bDeath = true;

	return true;
}