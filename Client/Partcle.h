#pragma once
#include "obj.h"

class CPartcle : public CObj
{
private:
	//��ƼŬ�� Ÿ���� �־�� �ȴ�?

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CPartcle(void);
	virtual ~CPartcle(void);
};