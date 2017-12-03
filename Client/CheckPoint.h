#pragma once
#include "obj.h"

class CCheckPoint : public CObj
{
private:
	eCheckType	m_eType;
	Frame		m_StoreFrame;
	DWORD		m_dwTime;
	
public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void FrameMove(void);
	void SetState(eCheckType _Type) { m_eType = _Type; };

public:
	CCheckPoint(void);
	virtual ~CCheckPoint(void);
};