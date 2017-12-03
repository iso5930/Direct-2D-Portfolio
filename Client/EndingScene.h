#pragma once
#include "scene.h"

class CEndingScene : public CScene
{
public:
	virtual	HRESULT	Initialize(void);
	virtual	int		UpDate(void);
	virtual	void	Render(void);
	virtual	void	Release(void);

public:
	CEndingScene(void);
	virtual ~CEndingScene(void);
};
