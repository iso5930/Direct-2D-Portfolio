#pragma once
#include "obj.h"

class CAstar;

class CPlayer : public CObj
{
private:
	ePlayerState	m_eState;
	eTransState		m_eTransState;

private:
	Frame			m_CreateFrame;
	TEXINFO*		m_pTexInfo;
	Frame			m_StandFrame;
	Frame			m_RunFrame;
	Frame			m_StopFrame;
	Frame			m_SocketFrame;
	Frame			m_AttackFrame;
	Frame			m_BlinkFrame;

	//Ʈ���� �����϶�
	Frame			m_TransFrame;
	Frame			m_TransPoint;
	Frame			m_DashFrame;

	//���ε� ���϶�.
	Frame			m_ReloadLoop;
	Frame			m_ReloadEnd;
	bool			m_bReload;
	bool			m_bReloadEnd;
	DWORD			m_dwReloadTime;

	//����() ��ų ����
	Frame			m_SnipeCharge;
	bool			m_bCharge;

	//����() 
	bool				m_bBlink;
	vector<TEXINFO*>	m_vecBlink;
	int					m_iAlpha;

	//������
	Frame			m_Uppercut;
	Frame			m_OverFlowFrame;
	bool			m_bOverFlow;

	//��Ʈ
	Frame			m_HitFrame;

private:
	DWORD			m_dwTime;
	bool			m_bChange;
	D3DXVECTOR3		m_vCur;
	int				m_PrevIndex;

private:
	DWORD			m_dwMoveTime;
	bool			m_bCheckIn;
	bool			m_bHit;

	//Ʈ���� ����
	bool			m_bTrans;
	DWORD			m_dwTransTime;
	vector<TRANS*>	m_vecTrans;
	D3DXVECTOR3		m_vOriginPos;
	bool			m_bTransProgress;
	int				m_iPrevTransIndex;
	int				m_iCurTransIndex;

	float			m_fTransStart;
	float			m_fTransEnd;
	
private:
	list<int>*		pList;
	CAstar*			m_pAstar;

private:
	list<CObj*>		m_HitList;

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void AstarMove(void);
	void BlinkMove(void);

public:
	void	SetScroll(void);
	void	FrameMove(void);
	HRESULT LoadTexture(void);
	void	SetAngle(D3DXVECTOR3 vDir);

public:
	bool			GetTrans(void){ return m_bTrans; }
	bool			GetTransProgress(void) {return m_bTransProgress; }
	eTransState		GetTransState(void) { return m_eTransState; }
	void			CreateSkill(eSkillType _eType = SKILL_END);
	void			PlayerHit(int iAtt);
	vector<TRANS*>*	GetvecTrans(void) { return &m_vecTrans; }
	FOCUS			GetFocusBar(void);

public:
	void TransSetAngle(D3DXVECTOR3 vDir);
	void TransReload(void);
	void SkillCollision(void);
	bool HitCheck(CObj* pObj);


public:
	CPlayer(void);
	virtual ~CPlayer(void);
};