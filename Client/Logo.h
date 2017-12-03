#pragma once
#include "scene.h"

class CLogo : public CScene
{
private:
	TCHAR		szName[50];
	eLogoState	m_eLogoState;

private:
	Frame		m_IntroFrame;
	Frame		m_LoopFrame;
	Frame		m_OutFrame;
	DWORD		m_dwTime;

private:
	//�ε���
	bool		m_bLoading;
	TEXINFO*	m_pLoadBar_Top;
	TEXINFO*	m_pLoadBar_Bottom;
	RECT		m_rcLoading;
	float		m_fLoading;

private:
	//�����带 ����ϱ� ���� ������...
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;

private:
	DWORD		m_dwLoadTime;

private:
	//�����带 ����ϱ����� �Լ�..
	static unsigned int __stdcall	TextureLoadingThread(void* pArg);
	CRITICAL_SECTION				GetCrt(void);
	HANDLE							GetThread(void);

public:
	virtual	HRESULT	Initialize(void);
	virtual	int		UpDate(void);
	virtual	void	Render(void);
	virtual	void	Release(void);

public:
	void FrameMove(void);
	void LoadingPlus(int iCount);

public:
	CLogo(void);
	virtual ~CLogo(void);
};