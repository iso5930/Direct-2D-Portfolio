#pragma once

class CBackGround;

class CScene
{
public:
	virtual	HRESULT	Initialize(void)PURE;
	virtual	int		UpDate(void)PURE;
	virtual	void	Render(void)PURE;
	virtual	void	Release(void)PURE;

public:
	CScene(void);
	virtual ~CScene(void);
};
