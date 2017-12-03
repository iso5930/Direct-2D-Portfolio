#include "StdAfx.h"
#include "Skill.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "Factory.h"
#include "ObjMgr.h"
#include "SkillEffect.h"
#include "SoundMgr.h"

CSkill::CSkill(void)
{

}

CSkill::~CSkill(void)
{
	Release();
}

HRESULT CSkill::Initialize(void)
{
	switch(m_eSkillType)
	{
	case SKILL_NO1:
		m_vOriginPos = m_tInfo.vPos;
		m_fScaleX = 0.5f;
		m_strName = "SkillNo1";
		m_tInfo.fCY = 105.f;
		m_tInfo.fCX = 229.f;
		m_tData.iAtt = 100;
		break;

	case SKILL_NO2:
		m_vOriginPos = m_tInfo.vPos;
		m_Frame.dwFrameTime = 20;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 27;
		m_Frame.iCur = 0;
		m_strName = "SkillNo2";
		m_tInfo.fCX = 20;
		m_tInfo.fCY = 20;
		m_tData.iAtt = 300;
		break;

	case SKILL_NO3:
		m_vOriginPos = m_tInfo.vPos;
		m_Frame.dwFrameTime = 10;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 29;
		m_Frame.iCur = 0;
		m_strName = "SkillNo3";
		m_tData.iAtt = 150;
		m_tInfo.fCY = 20;
		m_tInfo.fCX = 20;
		break;

	case SKILL_NO3_SUB:
		m_fJumpY = -43.f;
		m_vOriginPos = m_tInfo.vPos;
		m_Frame.dwFrameTime = 20;
		m_Frame.iStartFrame = 0;
		m_Frame.iEndFrame = 29;
		m_Frame.iCur = 0;
		m_bJump = false;
		m_strName = "SkillNo3_Sub";
		m_tInfo.fCX = 0;
		m_tInfo.fCY = 0;
		m_tData.iAtt = 20;
		break;
	}

	m_dwTime = GetTickCount();


	return S_OK;
}

int CSkill::Progress(void)
{
	switch(m_eSkillType)
	{
	case SKILL_NO1:
		m_tInfo.vPos += m_tInfo.vDir * 1000 * GETTIME;
		//m_tInfo.vPos += m_tInfo.vDir * 10 * GETTIME;
		m_fScaleX -= 0.05f * 100 * GETTIME;

		if(m_fScaleX < 0)
		{
			return 1;
		}

		break;

	case SKILL_NO2:
		m_tInfo.vPos += m_tInfo.vDir * 1000 * GETTIME;
		//m_tInfo.vPos += m_tInfo.vDir * 10 * GETTIME;
		break;

	case SKILL_NO3:
		{
			m_tInfo.vPos += m_tInfo.vDir * 1000 * GETTIME;
			//원래의 길이에서 200이상가면 스스로 소멸 하도록..

			D3DXVECTOR3 vDir = m_vOriginPos - m_tInfo.vPos;
			float fLength = D3DXVec3Length(&vDir);

			if(fLength >= m_fMaxLength)
			{
				return 1;
			}
		}
		break;

	case SKILL_NO3_SUB:
		{
			m_tInfo.vPos += m_tInfo.vDir * 100 * GETTIME;

			if(m_fJumpY < -76.f)
			{
				m_bJump = true;
			}

			if(m_bJump)
			{
				m_fJumpY += 0.5f * 350 * GETTIME;
			}
			else
			{
				m_fJumpY -= 0.5f * 350 * GETTIME;
			}

			if(m_fJumpY < - 5.f)
			{
				m_tInfo.fCX = 15;
				m_tInfo.fCY = 15;
			}
			
			if(m_fJumpY > 0)
			{
				return 1;
			}
		}
		break;
	}

	FrameMove();

	CObj::Progress();

	if(m_tInfo.vPos.x < 0 || m_tInfo.vPos.x > 4318 || m_tInfo.vPos.y < 0 || m_tInfo.vPos.y > 2077)
	{
		return 1;
	}

	return 0;
}

void CSkill::Render(void)
{
	const TEXINFO* pTexInfo = NULL;

	D3DXMATRIX matTrans;
	D3DXMatrixIdentity(&matTrans);

	D3DXMATRIX matScale;
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixScaling(&matScale, 0.5f, 0.5f, 0.f);

	switch(m_eSkillType)
	{
	case SKILL_NO1:
		{
			D3DXMatrixScaling(&matScale, 0.3f, m_fScaleX, 0.f);

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SlamBullet");

			D3DXMATRIX matRotZ;
			D3DXMatrixRotationZ(&matRotZ, -m_tInfo.fAngle);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			m_tInfo.matWorld = matScale * matRotZ * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case SKILL_NO2:
		{
			D3DXMATRIX matRotZ;

			D3DXMatrixIdentity(&matRotZ);

			D3DXMatrixRotationZ(&matRotZ, -m_tInfo.fAngle);

			//길게 뻗어나가는 라인을 그리자...
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Snipe_Line");
			
			D3DXVECTOR3 vDist = m_vOriginPos - m_tInfo.vPos;

			float fLength = D3DXVec3Length(&vDist);

			RECT rcLine;

			rcLine.top = 0;
			rcLine.bottom = pTexInfo->ImgInfo.Height;

			rcLine.left = long(pTexInfo->ImgInfo.Width - fLength);
			rcLine.right = pTexInfo->ImgInfo.Width;
			
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			m_tInfo.matWorld = matRotZ * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &rcLine, &D3DXVECTOR3(fLength, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			//휘리릭 하는 바람.. 50% 로 출력 
			//길이는 110 / 2 니깐 55정도

			//총알 부분
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Snipe_Bullet");
			
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			m_tInfo.matWorld = matScale * matRotZ * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				
			int iLoop = int(fLength / 55);

			if(iLoop > 11)
			{
				iLoop = 11;
			}

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			for(int i = 0; i < iLoop; ++i)
			{
				int iIndex = m_Frame.iCur - i;

				if(iIndex < 0)
				{
					iIndex = m_Frame.iEndFrame - i;
				}

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Snipe_Shot", iIndex);

				float fX = m_tInfo.vPos.x - m_tInfo.vDir.x * (55.f + (27.5f * i));
				float fY = m_tInfo.vPos.y - m_tInfo.vDir.y * (55.f + (27.5f * i));

				D3DXMatrixTranslation(&matTrans, fX - g_Scroll.x, fY - g_Scroll.y, 0.f);

				m_tInfo.matWorld = matScale * matRotZ * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}			
		}
		break;

	case SKILL_NO3:
		{
			D3DXMATRIX matRotZ;

			D3DXMatrixIdentity(&matRotZ);

			D3DXMatrixRotationZ(&matRotZ, -m_tInfo.fAngle);

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Bomb_Core", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			m_tInfo.matWorld = matScale * matRotZ * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;

	case SKILL_NO3_SUB:
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Bomb_Shadow");

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(125, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Bomb_Second", m_Frame.iCur);

			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y + m_fJumpY, 0.f);

			m_tInfo.matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		break;
	}
}

void CSkill::Release(void)
{
	if(m_eSkillType == SKILL_NO3)
	{
		D3DXVECTOR3 vDir;
		vDir.x = 1.f;
		vDir.y = 0.f;
		vDir.z = 0.f;

		D3DXVECTOR3 vPos = m_tInfo.vPos;

		vPos.y += 46.f;

		CObj* pObj = CFactory<CSkill>::CreateObj(m_tInfo.vPos, vDir, 0.f, SKILL_NO3_SUB);
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL);

		D3DXMATRIX matRotateZ;

		for(int i = 1; i < 7; ++i)
		{
			D3DXMatrixRotationZ(&matRotateZ, D3DXToRadian(51.f));

			D3DXVec3TransformNormal(&vDir, &vDir, &matRotateZ);

			pObj = CFactory<CSkill>::CreateObj(m_tInfo.vPos, vDir, 0.f, SKILL_NO3_SUB);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL);
		}

		pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_BOMB_CORE);
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
	}

	else if(m_eSkillType == SKILL_NO3_SUB)
	{
		CObj* pObj = CFactory<CSkillEffect>::CreateObj(m_tInfo.vPos, EFFECT_BOMB_SECOND);
		CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);

		CSoundMgr::GetInstance()->PlaySkillSound(L"ClusterSecond.wav");
	}
}

void CSkill::FrameMove(void)
{
	if(m_dwTime + m_Frame.dwFrameTime < GetTickCount())
	{
		m_dwTime = GetTickCount();

		if(++m_Frame.iCur > m_Frame.iEndFrame)
		{
			m_Frame.iCur = m_Frame.iStartFrame;
		}
	}
}

void CSkill::AddHitList(CObj* pObj)
{
	m_HitList.push_back(pObj);
}

bool CSkill::HitCheck(CObj* pObj)
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

	return true;
}

void CSkill::SetLength(float fLength)
{
	if(fLength >= 200)
	{
		m_fMaxLength = 200;
	}
	else
	{
		m_fMaxLength = fLength;
	}
}
