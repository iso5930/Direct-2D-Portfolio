#pragma once
#include "scene.h"

class Stage2 : public CScene
{
public:
	virtual HRESULT Initialize(void);
	virtual int UpDate(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	int GetTileIndex(D3DXVECTOR3 vPos);
public:
	Stage2(void);
	virtual ~Stage2(void);

};