#pragma once

class CObj
{
protected:
	Info	m_tInfo;
	DATA	m_tData;
	RECT	m_Rect;
	string	m_strName;
	DWORD	m_dwHitTime;
	bool	m_bHit;

public:
	Info*	GetInfo(void) { return &m_tInfo; }
	DATA*	GetData(void) { return &m_tData; }
	void	SetPos(D3DXVECTOR3 _Pos) { m_tInfo.vPos = _Pos; }
	void	SetDir(D3DXVECTOR3 _Dir) { m_tInfo.vDir = _Dir; }
	void	SetAngle(float _Angle) { m_tInfo.fAngle = _Angle; }

public:
	virtual HRESULT Initialize(void)PURE;
	virtual int		Progress(void)PURE;
	virtual void	Render(void)PURE;
	virtual void	Release(void)PURE;

public:
	RECT* GetRect(void) { return &m_Rect; }

public:
	CObj(void);
	virtual ~CObj(void);
};