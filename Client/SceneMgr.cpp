#include "StdAfx.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Logo.h"
#include "Stage.h"
#include "BossStage.h"
#include "Stage2.h"
#include "EndingScene.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr(void)
{
	m_pScene = NULL;
}

CSceneMgr::~CSceneMgr(void)
{
	Release();
}

void CSceneMgr::SetScene(eSceneType eType)
{
	m_bChange = true;
	m_eNextScene = eType;
}

void CSceneMgr::Initialize(void)
{
	m_eScene = SCENE_LOGO;
}

int CSceneMgr::UpDate(void)
{
	if(m_bChange)
	{
		m_bChange = false;

		if(m_pScene != NULL)
		{
			delete m_pScene;
			m_pScene = NULL;
		}

		switch(m_eNextScene)
		{
		case SCENE_LOGO:
			m_pScene = new CLogo;
			m_pScene->Initialize();
			break;
		case SCENE_STAGE:
			m_pScene = new CStage;
			m_pScene->Initialize();
			break;

		case SCENE_STAGE2:
			m_pScene = new Stage2;
			m_pScene->Initialize();
			break;

		case SCENE_BOSS:
			m_pScene = new CBossStage;
			m_pScene->Initialize();
			break;

		case SCENE_ENDING:
			m_pScene = new CEndingScene;
			m_pScene->Initialize();
			break;
		}

		m_eScene = m_eNextScene;
	}
	else
	{
		m_pScene->UpDate();
	}
	
	return 0;
}

void CSceneMgr::Render(void)
{
	m_pScene->Render();
}

void CSceneMgr::Release(void)
{
	if(m_pScene)
	{
		delete m_pScene;
		m_pScene = NULL;
	}
}