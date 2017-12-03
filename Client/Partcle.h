#pragma once
#include "obj.h"

class CPartcle : public CObj
{
private:
	//파티클의 타입이 있어야 된다?

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	CPartcle(void);
	virtual ~CPartcle(void);
};