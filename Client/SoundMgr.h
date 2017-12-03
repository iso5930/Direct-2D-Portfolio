#pragma once

class CSoundMgr
{
private:
	static CSoundMgr* m_pInstance;

public:
	static CSoundMgr* GetInstance(void)
	{
		if (m_pInstance == NULL)
		{
			m_pInstance = new CSoundMgr;
		}
		return m_pInstance;
	}

	static void DestroyInstance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}
private:
	map<TCHAR*, FMOD_SOUND*>	m_mapSound;
	FMOD_SYSTEM*	m_pSystem;
	FMOD_CHANNEL*	m_pBGM;
	FMOD_CHANNEL*	m_pSkillSound;

private:
	FMOD_RESULT		m_Result;		//실행잘되는지 확인하는 변수.
	unsigned int m_iVersion;

public:
	void Init(void);
	void LoadSoundFile(void);
	void PlayBGM(TCHAR* pSoundKey);
	void PlaySkillSound(TCHAR* pSoundKey);
	void StopBGM(void);

public:
	map<TCHAR*, FMOD_SOUND*>* GetSoundMap(void) { return &m_mapSound; }

private:
	void ErrorCheck(FMOD_RESULT _result);
	
public:
	CSoundMgr(void);
	~CSoundMgr(void);
};