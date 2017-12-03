#include "StdAfx.h"
#include "Obj.h"
#include "Skill.h"

CObj::CObj(void)
{
	ZeroMemory(&m_tInfo, sizeof(Info));
	ZeroMemory(&m_tData, sizeof(DATA));
	m_dwHitTime = GetTickCount();
	m_bHit = false;
}

CObj::~CObj(void)
{

}

int CObj::Progress(void)
{
	if(m_strName == "Cube")
	{
		m_Rect.left		= long(m_tInfo.vPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right	= long(m_tInfo.vPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top		= long(m_tInfo.vPos.y - m_tInfo.fCY + 12.f);
		m_Rect.bottom	= long(m_tInfo.vPos.y + 12.f);
	}
	else if(m_strName == "SkillNo1")
	{
		D3DXVECTOR3 vCollPos = m_tInfo.vPos;

		vCollPos += m_tInfo.vDir * 50;
		m_Rect.left		= long(vCollPos.x - 10);
		m_Rect.right	= long(vCollPos.x + 10);
		m_Rect.top		= long(vCollPos.y - 10);
		m_Rect.bottom	= long(vCollPos.y + 10);
	}
	else if (m_strName == "SkillNo3_Sub")
	{
		float fJumpY = ((CSkill*)this)->GetJumpY();
		m_Rect.left		= long(m_tInfo.vPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right	= long(m_tInfo.vPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top		= long((m_tInfo.vPos.y + fJumpY) - m_tInfo.fCY / 2.f);
		m_Rect.bottom	= long((m_tInfo.vPos.y + fJumpY) + m_tInfo.fCY / 2.f);
	}
	else if(m_strName == "Lobber")
	{
		m_Rect.left = long(m_tInfo.vPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right = long(m_tInfo.vPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top = long(m_tInfo.vPos.y - m_tInfo.fCY / 2.f) - 30;
		m_Rect.bottom = long(m_tInfo.vPos.y + m_tInfo.fCY / 2.f) - 47;
	}
	else if(m_strName == "Player")
	{
		//-62만큼
		D3DXVECTOR3 vCollPos = m_tInfo.vPos;
		vCollPos.y -= 50;

		m_Rect.left = long(vCollPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right = long(vCollPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top = long(vCollPos.y - m_tInfo.fCY / 2.f);
		m_Rect.bottom = long(vCollPos.y + m_tInfo.fCY / 2.f);
	}
	else if(m_strName == "Lady")
	{
		D3DXVECTOR3 vCollPos = m_tInfo.vPos;
		vCollPos.y -= 48.3f;
		
		m_Rect.left = long(vCollPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right = long(vCollPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top = long(vCollPos.y - m_tInfo.fCY / 2.f);
		m_Rect.bottom = long(vCollPos.y + m_tInfo.fCY / 2.f);
	}
	else if(m_strName == "BossBullet")
	{
		D3DXVECTOR3 vCollPos = m_tInfo.vPos;

		//vCollPos.x += 53.f;

		float fX = m_tInfo.vPos.x;
		float fY = m_tInfo.vPos.y;

		fX += 53.f * cosf(m_tInfo.fAngle);
		fY -= 53.f * sinf(m_tInfo.fAngle);

		m_Rect.left		= long(fX - m_tInfo.fCX / 2.f);
		m_Rect.right	= long(fX + m_tInfo.fCX / 2.f);
		m_Rect.top		= long(fY - m_tInfo.fCY / 2.f);
		m_Rect.bottom	= long(fY + m_tInfo.fCY / 2.f);
	}
	else if(m_strName == "Boss")
	{
		D3DXVECTOR3 vCollPos = m_tInfo.vPos;
		vCollPos.y += 73.f;
		
		m_Rect.left		= long(vCollPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right	= long(vCollPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top		= long(vCollPos.y - m_tInfo.fCY / 2.f);
		m_Rect.bottom	= long(vCollPos.y + m_tInfo.fCY / 2.f);
	}
	else
	{
		m_Rect.left		= long(m_tInfo.vPos.x - m_tInfo.fCX / 2.f);
		m_Rect.right	= long(m_tInfo.vPos.x + m_tInfo.fCX / 2.f);
		m_Rect.top		= long(m_tInfo.vPos.y - m_tInfo.fCY / 2.f);
		m_Rect.bottom	= long(m_tInfo.vPos.y + m_tInfo.fCY / 2.f);
		//기본 렉트..
	}

	return 0;
}