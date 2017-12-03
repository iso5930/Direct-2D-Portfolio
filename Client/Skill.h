#pragma once
#include "obj.h"

class CSkill : public CObj
{
private:
	list<CObj*> m_HitList;

private:
	eSkillType m_eSkillType;

private:
	Frame		m_Frame;
	DWORD		m_dwTime;
	D3DXVECTOR3 m_vOriginPos;
	float		m_fJumpY;
	bool		m_bJump;
	float		m_fScaleX;
	float		m_fMaxLength;

public:
	virtual HRESULT Initialize(void);
	virtual int		Progress(void);
	virtual void	Render(void);
	virtual void	Release(void);

public:
	void SetType(eSkillType _Type){ m_eSkillType = _Type; }
	void AddHitList(CObj* pObj);
	bool HitCheck(CObj* pObj);
	eSkillType GetSkillType(void) { return m_eSkillType; }
	void SetLength(float fLength);

public:
	float GetJumpY(void) {return m_fJumpY; }
	
public:
	void FrameMove(void);

public:
	CSkill(void);
	virtual ~CSkill(void);
};