#pragma once

class CObj;
class CPlayer;

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr);
private:
	CPlayer*		m_pPlayer;
	list<CObj*>*	m_pMonsterList;
	list<CObj*>*	m_pSkillList;
	list<CObj*>*	m_pObjectList;
	list<CObj*>*	m_pCubeList;
	list<CObj*>*	m_pBulletList;

public:
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void SetMonsterList(list<CObj*>* pMonsterList) { m_pMonsterList = pMonsterList; }
	void SetSkillList(list<CObj*>* pSkillList) { m_pSkillList = pSkillList; }
	void SetObjectList(list<CObj*>* pObjectList) { m_pObjectList = pObjectList; }
	void SetCubeList(list<CObj*>* pCubeList) { m_pCubeList = pCubeList; }
	void SetBulletList(list<CObj*>* pBulletList) { m_pBulletList = pBulletList; }

public:
	HRESULT Initialize(void);
	int		Progress(void);
	void	Render(void);
	void	Release(void);

public:
	bool PtInCircle(float _CircleX, float _CircleY, float _CircleWidth, float _CircleHeight, float _CursorX, float _CursorY);
	
public:
	CCollisionMgr(void);
	~CCollisionMgr(void);
};