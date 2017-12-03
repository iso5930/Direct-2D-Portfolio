#pragma once

class CBackGround;

class CAstar
{
	//DECLARE_SINGLETON(CAstar)

private:
	list<NODE*>		m_OpenList;
	list<NODE*>		m_CloseList;
	list<int>		m_BestList;
	int				m_iStartIndex;
	int				m_iEndIndex;

public:
	void AstarStart(int iStartIndex, int iGoalIndex);
	void MakeRoute(CBackGround* pBack, vector<TILE*>* pvecTile);
	bool CheckList(int iIndex);
	NODE* MakeNode(int iIndex, NODE* pParent, vector<TILE*>* pTile);
	int GetTileIndex(D3DXVECTOR3 vPos);
	void AstarStartPos(D3DXVECTOR3 vStartPos, D3DXVECTOR3 vGoalPos);
	list<int>* GetBestList(void);
	static bool ComPare(NODE* pNode1, NODE* pNode2)
	{
		return pNode1->fCost < pNode2->fCost;
	}
	void Release(void);

public:
	CAstar(void);
	~CAstar(void);
};