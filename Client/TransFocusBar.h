#pragma once
#include "obj.h"

class CTransFocusBar : public CObj
{
private:
	bool			m_bTrans;
	bool			m_bTransProgress;
	vector<TRANS*>* m_vecTrans;
	FOCUS			m_FocusBar;

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	CTransFocusBar(void);
	virtual ~CTransFocusBar(void);
};