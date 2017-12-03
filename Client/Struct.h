#pragma once
#include "obj.h"

class CStruct : public CObj
{
private:
	OBJECT* m_pStruct;

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void SetStruct(OBJECT* pStruct) { m_pStruct = pStruct; };

public:
	CStruct(void);
	virtual ~CStruct(void);
};