#pragma once
#include "obj.h"

class CSkillEffect : public CObj
{
private:
	eSkillEffect m_eType;

private:
	Frame	m_Frame;
	DWORD	m_dwTime;

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void SetType(eSkillEffect _Type){ m_eType = _Type; }

public:
	CSkillEffect(void);
	virtual ~CSkillEffect(void);
};