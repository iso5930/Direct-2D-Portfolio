#include "StdAfx.h"
#include "MainGame.h"
#include "Device.h"
#include "SceneMgr.h"
#include "TextureMgr.h"
#include "Player.h"
#include "ObjMgr.h"
#include "Astar.h"
#include "RenderMgr.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "DotMgr.h"
#include "CollisionMgr.h"
#include "SoundMgr.h"

CMainGame::CMainGame(void)
{
	/*AllocConsole(); 
	freopen( "CONOUT$",  "wt", stdout);
	SetConsoleTitleA("Debug");*/

	m_fTime = 0.f;
	ZeroMemory(m_szFps, sizeof(m_szFps));

	m_iCount = 0;

	srand(unsigned(time(NULL)));

	CSoundMgr::GetInstance()->LoadSoundFile();
}

CMainGame::~CMainGame(void)
{
	//FreeConsole();
	Release();
}

HRESULT CMainGame::Initialize(void)
{
	CTimeMgr::GetInstance()->InitTime();

	if(FAILED(CDevice::GetInstance()->InitD3D()))
	{
		ERR_MSG(L"디바이스 초기화 오류");
		return S_FALSE;
	}

	CSceneMgr::GetInstance()->SetScene(SCENE_LOGO);

	CObj* pObj = new CPlayer;

	pObj->Initialize();

	CObjMgr::GetInstance()->AddObject(pObj, OBJ_PLAYER);

	CCollisionMgr::GetInstance()->SetPlayer((CPlayer*)pObj);
	CCollisionMgr::GetInstance()->SetSkillList(&CObjMgr::GetInstance()->GetList()[OBJ_SKILL]);
	CCollisionMgr::GetInstance()->SetMonsterList(&CObjMgr::GetInstance()->GetList()[OBJ_MONSTER]);
	CCollisionMgr::GetInstance()->SetObjectList(&CObjMgr::GetInstance()->GetList()[OBJ_STRUCT]);
	CCollisionMgr::GetInstance()->SetCubeList(&CObjMgr::GetInstance()->GetList()[OBJ_CUBE]);
	CCollisionMgr::GetInstance()->SetBulletList(&CObjMgr::GetInstance()->GetList()[OBJ_MONSTER_BULLET]);

	g_bCreate = true;
	
	return S_OK;
}

int CMainGame::Progress(void)
{
	GetFocus();

	CKeyMgr::GetInstance()->SetKeyState();

	CTimeMgr::GetInstance()->SetTime();

	CSceneMgr::GetInstance()->UpDate();

	return 0;
}

void CMainGame::Render(void)
{
	CDevice::GetInstance()->Render_Begin();
	CSceneMgr::GetInstance()->Render();

	/*D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 1150, 70.f, 0.f);
	++m_iCount;

	m_fTime += CTimeMgr::GetInstance()->GetTime();

	if(m_fTime > 1.f)
	{
		m_fTime = 0.f;
		wsprintf(m_szFps, L"FPS : %d", m_iCount);
		m_iCount = 0;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szFps, lstrlen(m_szFps), NULL, NULL, D3DCOLOR_ARGB(255, 0, 255, 0));*/

	CDevice::GetInstance()->Render_End();
}

void CMainGame::Release(void)
{
	CObjMgr::GetInstance()->DestroyInstance();
	CDevice::GetInstance()->DestroyInstance();
	CSceneMgr::GetInstance()->DestroyInstance();
	CTextureMgr::GetInstance()->DestroyInstance();
	//CAstar::GetInstance()->DestroyInstance();
	CRenderMgr::GetInstance()->DestroyInstance();
	CTimeMgr::GetInstance()->DestroyInstance();
	CKeyMgr::GetInstance()->DestroyInstance();
	CDotMgr::GetInstance()->DestroyInstance();
	CCollisionMgr::GetInstance()->DestroyInstance();
	CSoundMgr::GetInstance()->DestroyInstance();
}