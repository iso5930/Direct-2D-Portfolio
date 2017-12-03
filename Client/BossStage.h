#pragma once
#include "scene.h"

class CBossStage : public CScene
{
public:
	virtual HRESULT Initialize(void);
	virtual int UpDate(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	int GetTileIndex(D3DXVECTOR3 vPos);

public:
	CBossStage(void);
	virtual ~CBossStage(void);
};