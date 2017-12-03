#pragma once
#include "monster.h"

class CBossLobber : public CMonster
{
private:
	eMonsterState	m_eState;
	bool			m_bCreate;
	bool			m_bHit;
	bool			m_bSummer;

private:
	Frame	m_RunFrame;
	Frame	m_AttackFrame;
	Frame	m_DeathFrame;

private:
	DWORD	m_dwTime;
	DWORD	m_dwTargetTime;
	int		m_iEggTime;

private:
	int		m_iPrevIndex;
	int		m_iTargetIndex;
	int		m_iRestoreCount;

	//int		m_iPrevTileIndex;
	//int		m_iCurTileIndex;

private:
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vPlayerPos;
	RECT		m_rcHpBar;

private:
	bool	m_bTrans;
	bool	m_bTransProgress;
	bool	m_bAttack;
	float	m_fJump;
	bool	m_bJump;
	float	m_fTime;

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	virtual void HitAction(int Att);

public:
	void FrameMove(void);
	void AstarMove(void);
	void SetAngle(D3DXVECTOR3 vDir);
	void SyncHpBar(void);

public:
	CBossLobber(void);
	virtual ~CBossLobber(void);
};
