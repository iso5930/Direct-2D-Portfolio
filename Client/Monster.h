#pragma once
#include "obj.h"

class CAstar;

class CMonster : public CObj
{
protected:
	CAstar*		m_pAstar;
	Frame		m_EggHeadFrame;
	Frame		m_EggBodyFrame;

public:
	virtual HRESULT Initialize(void)PURE;
	virtual int		Progress(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;

public:
	virtual void	HitAction(int Att)PURE;

public:
	CMonster(void);
	virtual ~CMonster(void);
};