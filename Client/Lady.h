#pragma once
#include "monster.h"

class CLady :
	public CMonster
{
private:
	eMonsterState m_eState;

private:
	int m_iPrevIndex;
	int m_iBodyIndex;
	int	m_iAttTime;

private:
	RECT m_rcHpBar;

private:
	D3DXVECTOR3 m_vBulletPos;
	D3DXVECTOR3 m_vCretePos;
	D3DXVECTOR3 m_vOriginBullet;

private:
	Frame m_AttackFrame;
	Frame m_DeathFrame;
	Frame m_HitFrame;

private:
	DWORD m_dwHitTime;
	DWORD m_dwTime;
	DWORD m_dwAstar;
	DWORD m_dwAttackTime;
	DWORD m_dwBulletTime;

private:
	bool m_bTrans;
	bool m_bTransProgress;
	bool m_bAttack;
	bool m_bAttackWait;
	bool m_bDeath;

private:
	D3DXVECTOR3 m_vTarget;

public:
	void SetAngle(D3DXVECTOR3 vPos);
	void FrameMove(void);
	void AStarMove(void);
	void SyncHpBar(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
	virtual void HitAction(int Att);

public:
	CLady(void);
	virtual ~CLady(void);
};