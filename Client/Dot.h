#pragma once
#include "obj.h"

class CDot : public CObj
{
private:
	D3DXVECTOR3 m_OriginPos;
	int			m_iDrawID;
	const TEXINFO* m_pTexInfo;

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	void ResetPos(void);
	void SetDrawID(int DrawID) { m_iDrawID = DrawID; }

public:
	CDot(void);
	virtual ~CDot(void);

};