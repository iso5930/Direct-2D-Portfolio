#include "StdAfx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "Cube.h"
#include "Skill.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "Monster.h"
#include "MonsterBullet.h"
#include "Player.h"
#include "SceneMgr.h"
#include "Factory.h"
#include "SkillEffect.h"

IMPLEMENT_SINGLETON(CCollisionMgr);

CCollisionMgr::CCollisionMgr(void)
{

}

CCollisionMgr::~CCollisionMgr(void)
{
	Release();
}

HRESULT CCollisionMgr::Initialize(void)
{
	return S_OK;
}

int CCollisionMgr::Progress(void)
{
	if(m_pPlayer->GetTrans())
	{
		return 0;
	}

	list<CObj*>::iterator Mon_Iter = m_pMonsterList->begin();
	list<CObj*>::iterator Mon_Iter_end = m_pMonsterList->end();
	
	list<CObj*>::iterator Skill_Iter = m_pSkillList->begin();
	list<CObj*>::iterator Skill_Iter_end = m_pSkillList->end();

	list<CObj*>::iterator Cube_Iter = m_pCubeList->begin();
	list<CObj*>::iterator Cube_Iter_end = m_pCubeList->end();

	list<CObj*>::iterator Obj_Iter = m_pObjectList->begin();
	list<CObj*>::iterator Obj_Iter_end = m_pObjectList->end();

	list<CObj*>::iterator Bullet_Iter = m_pBulletList->begin();
	list<CObj*>::iterator Bullet_Iter_end = m_pBulletList->end();

	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();
	vector<TILE*>* vecTile = ((CBackGround*)pObj)->GetTile();

	Skill_Iter = m_pSkillList->begin();
	Skill_Iter_end = m_pSkillList->end();

	eSceneType eType = CSceneMgr::GetInstance()->GetSceneType();

	int iTileCntX = 0;
	int iTileCntY = 0;

	switch(eType)
	{
	case SCENE_STAGE:
		iTileCntX = TILECNTX;
		iTileCntY = TILECNTY;
		break;

	case SCENE_STAGE2:
		iTileCntX = STAGE2_TILECNTX;
		iTileCntY = STAGE2_TILECNTY;
		break;

	case SCENE_BOSS:
		iTileCntX = BOSS_TILECNTX;
		iTileCntY = BOSS_TILECNTY;
		break;
	}

	/*if(eType != SCENE_STAGE2)
	{*/
		for(Skill_Iter; Skill_Iter != Skill_Iter_end;)
		{
			int iIndex = ((CBackGround*)pObj)->GetTileIndex((*Skill_Iter)->GetInfo()->vPos);

			if(iIndex >= iTileCntX * iTileCntY)
			{
				delete *Skill_Iter;
				Skill_Iter = m_pSkillList->erase(Skill_Iter);
			}
			else if(((*vecTile)[iIndex])->byDrawID == 1 || ((*vecTile)[iIndex])->byDrawID == 2)
			{
				delete *Skill_Iter;
				Skill_Iter = m_pSkillList->erase(Skill_Iter);
			}
			else
			{
				++Skill_Iter;
			}
		}
	//}
	//오브젝트 vs 스킬
	
	//큐브 vs 스킬
	for(Cube_Iter; Cube_Iter != Cube_Iter_end; ++Cube_Iter)
	{
		RECT* pSource = (*Cube_Iter)->GetRect();

		Skill_Iter = m_pSkillList->begin();
		Skill_Iter_end = m_pSkillList->end();

		for(Skill_Iter; Skill_Iter != Skill_Iter_end;)
		{
			RECT* pSkill = (*Skill_Iter)->GetRect();

			RECT rcTemp;

			if(IntersectRect(&rcTemp, pSource, pSkill))
			{
				if(((CSkill*)(*Skill_Iter))->HitCheck((*Cube_Iter)))
				{
					((CCube*)(*Cube_Iter))->CubeHitAction((*Skill_Iter)->GetData()->iAtt);

					if(((CSkill*)*Skill_Iter)->GetSkillType() == SKILL_NO3 || ((CSkill*)*Skill_Iter)->GetSkillType() == SKILL_NO3_SUB)
					{
						delete (*Skill_Iter);
						Skill_Iter = m_pSkillList->erase(Skill_Iter);
					}
					else
					{
						++Skill_Iter;
					}
				}
				else
					++Skill_Iter;
			}
			else
				++Skill_Iter;
		}
	}

	//몬스터 vs 스킬
	Skill_Iter = m_pSkillList->begin();
	Skill_Iter_end = m_pSkillList->end();

	for(Skill_Iter; Skill_Iter != Skill_Iter_end; ++Skill_Iter)
	{
		RECT* rcSource = (*Skill_Iter)->GetRect();

		Mon_Iter = m_pMonsterList->begin();
		Mon_Iter_end = m_pMonsterList->end();
		for(Mon_Iter; Mon_Iter != Mon_Iter_end; ++Mon_Iter)
		{
			RECT* rcMonster = (*Mon_Iter)->GetRect();

			RECT rcTemp;

			if(IntersectRect(&rcTemp, rcSource, rcMonster))
			{
				if(((CSkill*)*Skill_Iter)->HitCheck((*Mon_Iter)))
				{
					((CMonster*)*Mon_Iter)->HitAction((*Skill_Iter)->GetData()->iAtt);
					CObj* pObj = CFactory<CSkillEffect>::CreateObj((*Skill_Iter)->GetInfo()->vPos, EFFECT_BOMB_CORE);
					CObjMgr::GetInstance()->AddObject(pObj,OBJ_SKILL_EFFECT);
				}
			}
		}
	}

	//몬스터 불렛 vs 오브젝트
	for(Bullet_Iter; Bullet_Iter != Bullet_Iter_end; ++Bullet_Iter)
	{
		RECT* rcSource = (*Bullet_Iter)->GetRect();

		Cube_Iter = m_pCubeList->begin();
		Cube_Iter_end = m_pCubeList->end();
		for(Cube_Iter; Cube_Iter != Cube_Iter_end; ++Cube_Iter)
		{		
			RECT* rcObject = (*Cube_Iter)->GetRect();

			RECT rcTemp;

			if(IntersectRect(&rcTemp, rcSource, rcObject))
			{
				if(((CMonsterBullet*)*Bullet_Iter)->HitCheck((*Cube_Iter)))
				{
					((CCube*)(*Cube_Iter))->CubeHitAction((*Bullet_Iter)->GetData()->iAtt);
				}
			}
		}
	}

	//몬스터 불렛 vs 플레이어
	Bullet_Iter = m_pBulletList->begin();
	Bullet_Iter_end = m_pBulletList->end();

	RECT* rcSource = m_pPlayer->GetRect();

	for(Bullet_Iter; Bullet_Iter != Bullet_Iter_end; ++Bullet_Iter)
	{
		RECT* rcBullet = (*Bullet_Iter)->GetRect();	
		RECT rcTemp;

		if(IntersectRect(&rcTemp, rcSource, rcBullet))
		{
			if(((CMonsterBullet*)*Bullet_Iter)->HitCheck((m_pPlayer)))
			{
				m_pPlayer->PlayerHit((*Bullet_Iter)->GetData()->iAtt);
			}
		}
	}

	return 0;
}

void CCollisionMgr::Render(void)
{

}

void CCollisionMgr::Release(void)
{

}

bool CCollisionMgr::PtInCircle(float _CircleX, float _CircleY, float _CircleWidth, float _CircleHeight, float _CursorX, float _CursorY)
{
	if((_CircleX - _CursorX) * (_CircleX - _CursorX) / (_CircleWidth * _CircleWidth) + (_CircleY - _CursorY) * (_CircleY - _CursorY) / (_CircleHeight * _CircleHeight) <= 0.25)
	{
		return true;
	}
	else
	{
		return false;
	}
}