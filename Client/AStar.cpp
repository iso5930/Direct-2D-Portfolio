#include "StdAfx.h"
#include "AStar.h"
#include "BackGround.h"
#include "ObjMgr.h"
#include "SceneMgr.h"

CAstar::CAstar(void)
{
	m_iStartIndex = 0;
	m_iEndIndex = 0;
}

CAstar::~CAstar(void)
{
	Release();
}

void CAstar::AstarStart(int iStartIndex, int iGoalIndex)
{
	if(iStartIndex == iGoalIndex)
		return;

	CBackGround* pBack = (CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

	vector<TILE*>* pvecTile = pBack->GetTile();
	
	if((*pvecTile)[iGoalIndex]->byOption == 1 || (*pvecTile)[iGoalIndex]->byOption == 2 || (*pvecTile)[iGoalIndex]->byOption == 4)
	{
		return;
	}

	Release();

	m_iStartIndex = iStartIndex;
	m_iEndIndex = iGoalIndex;

	MakeRoute(pBack, pvecTile);
}

void CAstar::MakeRoute(CBackGround* pBack, vector<TILE*>* pvecTile)
{
	NODE* pParent = new NODE;
	pParent->iIndex = m_iStartIndex;

	pParent->pParent = NULL;

	pParent->fCost = 0.f;

	m_CloseList.push_back(pParent);


	NODE* pNode = NULL;
	int iIndex = 0;

	int iCount = 0;

	eSceneType SceneType = CSceneMgr::GetInstance()->GetSceneType();

	int iTileCntX = 0;
	int iTileCntY = 0;

	switch(SceneType)
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

	while(true)
	{
		if(iCount >= 100)
		{
			break;
		}

		iIndex = pParent->iIndex - iTileCntX * 2;

		if(pParent->iIndex >= iTileCntX * 2 && (*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}

		iIndex = pParent->iIndex + iTileCntX * 2;

		if(pParent->iIndex < (iTileCntX * iTileCntY) - (iTileCntX * 2) &&
			(*pvecTile)[iIndex]->byOption == 0 && CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}

		iIndex = pParent->iIndex - 1;

		if(pParent->iIndex % iTileCntX != 0 && (*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}
		
		iIndex = pParent->iIndex + 1;

		if(pParent->iIndex % iTileCntX != iTileCntX - 1 &&
			(*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}
		
		iIndex = pParent->iIndex - (iTileCntX - (pParent->iIndex / iTileCntX) % 2);

		if(pParent->iIndex >= iTileCntX &&
			pParent->iIndex % (iTileCntX * 2) != (iTileCntX * 2) - 1 &&
			(*pvecTile)[iIndex]->byOption == 0 && CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}
		
		iIndex = pParent->iIndex + (iTileCntX + (pParent->iIndex / iTileCntX) % 2);

		if(pParent->iIndex < (iTileCntX * iTileCntY) - TILECNTX &&
			pParent->iIndex % (iTileCntX * 2) != iTileCntX * 2 - 1 &&
			(*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}

		iIndex = pParent->iIndex + (iTileCntX + (pParent->iIndex / iTileCntX) % 2) - 1;

		if(pParent->iIndex < (iTileCntX * iTileCntY) - TILECNTX &&
			pParent->iIndex % (iTileCntX * 2) != 0 &&
			(*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}

		iIndex = pParent->iIndex - (iTileCntX - (pParent->iIndex / iTileCntX) % 2) - 1;

		if(pParent->iIndex >= iTileCntX &&
			pParent->iIndex % (iTileCntX * 2) != 0 &&
			(*pvecTile)[iIndex]->byOption == 0 &&
			CheckList(iIndex))
		{
			pNode = MakeNode(iIndex, pParent, pvecTile);
			m_OpenList.push_back(pNode);
		}
		
		m_OpenList.sort(ComPare);

		if(m_OpenList.empty())
		{
			break;
		}

		list<NODE*>::iterator iter = m_OpenList.begin();
		pParent = *iter;

		m_CloseList.push_back(*iter);
		m_OpenList.erase(iter);

		if(pParent->iIndex == m_iEndIndex)
		{
			while(true)
			{
				m_BestList.push_back(pParent->iIndex);
				pParent = pParent->pParent;

				if(pParent->iIndex == m_iStartIndex)
					break;
			}

			m_BestList.reverse();

			break;
		}
		++iCount;
	}
}

bool CAstar::CheckList(int iIndex)
{
	list<NODE*>::iterator iter = m_OpenList.begin();
	list<NODE*>::iterator iter_end = m_OpenList.end();

	for(iter; iter != iter_end; ++iter)
	{
		if((*iter)->iIndex == iIndex)
			return false;
	}

	iter = m_CloseList.begin();
	iter_end = m_CloseList.end();

	for(iter; iter != iter_end; ++iter)
	{
		if((*iter)->iIndex == iIndex)
			return false;
	}

	return true;
}

NODE* CAstar::MakeNode(int iIndex, NODE* pParent, vector<TILE*>* pTile)
{
	NODE* pNode = new NODE;
	pNode->iIndex = iIndex;
	pNode->pParent = pParent;

	D3DXVECTOR3 vDistance = (*pTile)[iIndex]->vPos - (*pTile)[pParent->iIndex]->vPos;

	float fPCost = D3DXVec3Length(&vDistance);

	vDistance = (*pTile)[iIndex]->vPos - (*pTile)[m_iEndIndex]->vPos;

	float fGCost = D3DXVec3Length(&vDistance);

	pNode->fCost = fPCost + fGCost;

	return pNode;
}

int CAstar::GetTileIndex(D3DXVECTOR3 vPos)
{
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;

	int iIndexX;
	int iIndexY;
	int iIndexX2;
	int iIndexY2;

	iIndexX = int((vPos.x + (TILESIZEX / 2)) / TILESIZEX);
	iIndexY = int((vPos.y + (TILESIZEY / 2)) / TILESIZEY);

	v1.x = float(iIndexX * TILESIZEX);
	v1.y = float(iIndexY * TILESIZEY);
	v1.z = 0.f;

	iIndexX2 = int(vPos.x / TILESIZEX);
	iIndexY2 = int(vPos.y / TILESIZEY);

	v2.x = float(iIndexX2 * TILESIZEX);
	v2.y = float(iIndexY2 * TILESIZEY);
	v2.z = 0.f;

	v2.x += TILESIZEX / 2;
	v2.y += TILESIZEY / 2;

	//홀수 타일 픽킹

	D3DXVECTOR3 vDist1 = vPos - v1;
	D3DXVECTOR3 vDist2 = vPos - v2;

	float fDist1 = D3DXVec3Length(&vDist1);
	float fDist2 = D3DXVec3Length(&vDist2);

	eSceneType SceneType = CSceneMgr::GetInstance()->GetSceneType();

	int iTileCntX = 0;
	int iTileCntY = 0;

	switch(SceneType)
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

	if(fDist1 < fDist2)
	{	
		iIndexY *= 2;

		return iIndexY * iTileCntX + iIndexX;
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;
		
		return iIndexY2 * iTileCntX + iIndexX2;
	}
}	

void CAstar::AstarStartPos(D3DXVECTOR3 vStartPos, D3DXVECTOR3 vGoalPos)
{
	m_iStartIndex = GetTileIndex(vStartPos);

	m_iEndIndex = GetTileIndex(vGoalPos);

	AstarStart(m_iStartIndex, m_iEndIndex);
}

list<int>* CAstar::GetBestList(void)
{
	return &m_BestList;
}

void CAstar::Release(void)
{
	list<NODE*>::iterator iter = m_OpenList.begin();
	list<NODE*>::iterator iter_end = m_OpenList.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete *iter;
	}
	m_OpenList.clear();
	
	iter = m_CloseList.begin();
	iter_end = m_CloseList.end();
	
	for(iter; iter != iter_end; ++iter)
	{
		delete *iter;
	}
	m_CloseList.clear();

	m_BestList.clear();
}