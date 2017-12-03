#pragma once
#include "Scene.h"

class CStage : public CScene
{
private:
	Frame m_CutinFrame;
	DWORD m_dwTime;
	bool m_bLoading;
	bool m_bHey;
	bool m_bTemp;
	DWORD m_dwVoiceTime;
	bool m_SoundEnd;
	float m_fAlpha;
	bool m_bLast;
	bool m_bLast2;
	bool m_bCreate;

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;

private:
	static unsigned int __stdcall	TextureThread(void* pArg);
	CRITICAL_SECTION				GetCrt(void);
	HANDLE							GetThread(void);

public:
	virtual	HRESULT	Initialize(void);
	virtual	int		UpDate(void);
	virtual	void	Render(void);
	virtual	void	Release(void);

public:
	int GetTileIndex(D3DXVECTOR3 vPos);

public:
	CStage(void);
	virtual ~CStage(void);
};