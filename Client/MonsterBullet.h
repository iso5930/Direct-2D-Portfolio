#pragma once
#include "obj.h"

class CMonsterBullet : public CObj
{
private:
	list<CObj*> m_HitList;

private:
	eMonsterBulletType	m_eType;
	Frame				m_BulletFrame;
	DWORD				m_dwTime;
	bool				m_bDeath;

private:
	float				m_fScaleX;
	float				m_fScaleY;
	int					m_iBulletSpeed;
	float				m_fScaleSpeed;

private:
	bool				m_bTrans;
	bool				m_bTransProgress;


public:
	void SetType(eMonsterBulletType _Type) { m_eType = _Type; }

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	bool HitCheck(CObj* pObj);

public:
	CMonsterBullet(void);
	virtual ~CMonsterBullet(void);
};