#pragma once
#include "monster.h"

class CBoss : public CMonster
{
private:
	eMonsterState m_eState;

private:
	D3DXVECTOR3 m_vBulletPos;
	RECT		m_rcHpBar;
	
private:
	Frame	m_DeathFrame;
	Frame	m_ChageFrame;
	Frame	m_AttackFrame;
	int		m_iHeadIndex;
	int		m_iBodyIndex;
	int		m_iPrevIndex;
	int		m_iSummerCount;
	int		m_iCount;

private:
	bool m_bTrans;
	bool m_bTransProgress;
	bool m_bChage;
	bool m_bDeath;

private:
	eBossAttType m_AttType;

private:
	DWORD m_dwTime;
	DWORD m_dwChangeTime;
	DWORD m_dwSummerTime;
	DWORD m_dwCreateTime;
	
public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);
	virtual void	HitAction(int Att);

public:
	void SetAngle(void);
	void FrameMove(void);
	void Attack(void);
	void HpBarSync(void);
	void Attack2(void);
	bool GetDeath(void) { return m_bDeath; }

public:
	CBoss(void);
	virtual ~CBoss(void);



};