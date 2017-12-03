#include "StdAfx.h"
#include "Stage.h"
#include "BackGround.h"
#include "ObjMgr.h"
#include "Struct.h"
#include "QuickSlot.h"
#include "SkillStore.h"
#include "CheckPoint.h"
#include "Factory.h"
#include "Lobber.h"
#include "DotMgr.h"
#include "Cube.h"
#include "Lady.h"
#include "SceneMgr.h"
#include "TransFocusBar.h"
#include "TextureMgr.h"
#include "Device.h"
#include "SoundMgr.h"
#include "TimeMgr.h"
#include "Mouse.h"

CStage::CStage(void)
	:m_CutinFrame(1, 2, 0, 200)
{
	memset(&m_Crt, 0, sizeof(CRITICAL_SECTION));
}

CStage::~CStage(void)
{
	Release();
}

HRESULT	CStage::Initialize(void)
{
	InitializeCriticalSection(&m_Crt);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, TextureThread, this, 0, NULL);

	CObj* pObj = new CQuickSlot;
	pObj->Initialize();
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_QUICKSLOT);

	pObj = new CBackGround;
	((CBackGround*)pObj)->SetTile(1);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_BACKGROUND);

	pObj = new CCheckPoint;
	pObj->Initialize();
	((CCheckPoint*)pObj)->SetState(CHECK_STORE); //  스토어 위치? 그대로 두자..
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CHECK_POINT);

	//여기서 1Stage Struct Loading

	HANDLE hFile;

	DWORD dwByte = 0;

	hFile = CreateFile(L"../Data/Object/1Stage.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	pObj = new CSkillStore;
	pObj->Initialize();

	CObjMgr::GetInstance()->AddObject(pObj, OBJ_STORE);


	pObj = new CTransFocusBar;
	pObj->Initialize();
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_TRANSFOCUSBAR);
	
	////몬스터 잠시만 안녕
	/*pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(1636.f, 1479.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);*/

	/*
	pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(1636.f, 1579.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

	pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(1636.f, 1679.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);*/

	/*pObj = CFactory<CLady>::CreateObj(D3DXVECTOR3(1636.f, 1479.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);*/

	if(hFile == NULL)
	{
		ERR_MSG(L"1Stage 오브젝트 로드 실패");
	}

	while(true)
	{
		CObj* pObj = new CStruct;

		OBJECT* pStruct = new OBJECT;

		ReadFile(hFile, pStruct, sizeof(OBJECT), &dwByte, NULL);

		if(dwByte == 0)
		{
			delete pObj;
			delete pStruct;
			break;
		}

		((CStruct*)pObj)->SetStruct(pStruct);

		pObj->Initialize();

		CObjMgr::GetInstance()->AddObject(pObj, OBJ_STRUCT);
	}

	CloseHandle(hFile);

	pObj = CFactory<CCube>::CreateObj(5452);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5381);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(5163);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5236);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5308);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	
	pObj = CFactory<CCube>::CreateObj(5453);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5526);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5598);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(5235);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5307);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5380);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	
	pObj = CFactory<CCube>::CreateObj(5525);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5597);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(5670);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	CDotMgr::GetInstance()->Initialize();

	CObjMgr::GetInstance()->Progress();

	m_dwTime = GetTickCount() + 1800;
	m_dwVoiceTime = GetTickCount();

	m_bLoading = false;
	m_bTemp = false;

	m_bHey = false;
	m_SoundEnd = false;

	m_fAlpha = 255;

	m_bLast = false;
	m_bLast2 = false;

	m_bCreate = false;
	
	//CSoundMgr::GetInstance()->PlayBGM(L"OldFriends.wav");
	//CSoundMgr::GetInstance()->PlaySkillSound(L"CommonPull.wav"); 
	//컷신이 끝났을때..

	return S_OK;
}

int	CStage::UpDate(void)
{
	if(g_bCreate == false)
	{
		CObjMgr::GetInstance()->Progress();

		//플레이어가 만약에 큐브를 지나갔다면 몬스터를 생성하는데 위치가....

		if(m_bCreate == false)
		{
			D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

			int iIndex = GetTileIndex(vPos);

			if(iIndex == 5452 || iIndex == 5381 || iIndex == 5163 || iIndex == 5236 || iIndex == 5308 || iIndex == 5453
				|| iIndex == 5526 || iIndex == 5598 || iIndex == 5235 || iIndex == 5307 || iIndex == 5380 || iIndex == 5525 || iIndex == 5597 || iIndex == 5670)
			{
				m_bCreate = true;
				CObj* pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(3719.f, 933.f, 0.f));
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

				pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(3087.f, 537.f, 0.f));
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

				pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(3555.f, 727.f, 0.f));
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

				pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(3372.f, 477.f, 0.f));
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

				pObj = CFactory<CLobber>::CreateObj(D3DXVECTOR3(3906.f, 528.f, 0.f));
				CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);
			}
		}
	
		if(m_bLast2 == false)
		{
			if(m_SoundEnd)
			{
				if(m_bHey && m_bTemp)
				{
					if(m_dwVoiceTime + 1500 < GetTickCount())
					{
						m_bLast = true;
						m_bTemp = false;
						m_dwVoiceTime = GetTickCount();
					}
				}
				if(m_bHey == false)
				{
					if(m_dwVoiceTime + 3300 < GetTickCount())
					{
						m_bHey = true;
						m_bTemp = true;
						m_dwVoiceTime = GetTickCount();
						CSoundMgr::GetInstance()->PlaySkillSound(L"PullTher.wav");
					}
				}
				if(m_bLast)
				{
					if(m_dwVoiceTime + 3000 < GetTickCount())
					{
						m_dwVoiceTime = GetTickCount();
						m_bLast = false;
						m_bLast2 = true;
					}
				}
			}
		}
	}

	if(GetAsyncKeyState('I'))
	{
		CSceneMgr::GetInstance()->SetScene(SCENE_BOSS);
		CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetData()->iHp = 0;
	}
	
	DWORD dwState;
	GetExitCodeThread(m_hThread, &dwState);

	if(dwState != STILL_ACTIVE && m_bLoading == false)
	{
		m_bLoading = true;
		CloseHandle(m_hThread);
		DeleteCriticalSection(&m_Crt);
	}

	if(CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].size() == 0 && dwState != STILL_ACTIVE)
	{
		D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;
		
		int iIndex = GetTileIndex(vPos);

		if(iIndex == 1790 || iIndex == 1862 || iIndex == 1861 || iIndex == 1934 || iIndex == 1933 || iIndex == 2005)
		{
			CSceneMgr::GetInstance()->SetScene(SCENE_STAGE2);
		}
	}
	
	if(g_bCreate)
	{
		if(m_dwTime + m_CutinFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(m_SoundEnd == false)
			{
				if(++m_CutinFrame.iCur > m_CutinFrame.iEndFrame)
				{
					m_CutinFrame.iCur = m_CutinFrame.iStartFrame;
				}
			}
			else
			{
				m_CutinFrame.iCur = 1;
			}
		}

		if(m_dwVoiceTime + 2000 < GetTickCount())
		{
			m_dwVoiceTime = GetTickCount();

			if(m_bHey == false)
			{
				m_bHey = true;
				m_bTemp = true;
				CSoundMgr::GetInstance()->PlaySkillSound(L"HeyRed.wav");
			}
			else if(m_bTemp)
			{
				m_bTemp = false;
				CSoundMgr::GetInstance()->PlaySkillSound(L"NotEscape.wav");
			}
		}

		if(m_bTemp == false && m_bHey == true)
		{
			if(m_dwVoiceTime + 1500 < GetTickCount())
			{
				m_dwVoiceTime = GetTickCount();
				m_SoundEnd = true;
			}
		}

		if(m_SoundEnd)
		{
			m_fAlpha -= 0.5f * 1000 * GETTIME;

			if(m_fAlpha < 0)
			{
				g_bCreate = false;
				CSoundMgr::GetInstance()->PlayBGM(L"OldFriends.wav");
				CSoundMgr::GetInstance()->PlaySkillSound(L"CommonPull.wav"); 
				
				m_dwVoiceTime = GetTickCount();

				m_bTemp = false;
				m_bHey = false;
			}
		}
	}

	return 0;
}

void CStage::Render(void)
{
	if(g_bCreate == false)
	{
		CObjMgr::GetInstance()->Render();

		if(m_bLast2 == false)
		{
			if(m_bLast)
			{
				const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutText", 4);

				D3DXMATRIX matTrans;

				D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 78.f, 0.f);

				float fX = pTexInfo->ImgInfo.Width / 2.f;
				float fY = pTexInfo->ImgInfo.Height / 2.f;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			if(m_bTemp)
			{
				const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutText", 3);

				D3DXMATRIX matTrans;

				D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 78.f, 0.f);

				float fX = pTexInfo->ImgInfo.Width / 2.f;
				float fY = pTexInfo->ImgInfo.Height / 2.f;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else if(m_bHey == false)
			{
				const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutText", 2);

				D3DXMATRIX matTrans;

				D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 78.f, 0.f);

				float fX = pTexInfo->ImgInfo.Width / 2.f;
				float fY = pTexInfo->ImgInfo.Height / 2.f;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}
	}
	if(g_bCreate)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutSence", m_CutinFrame.iCur);

		D3DXMATRIX matTrans;

		D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

		float fX = pTexInfo->ImgInfo.Width / 2.f;
		float fY = pTexInfo->ImgInfo.Height / 2.f;

		int iAlpha = (int)m_fAlpha;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));

		if(m_SoundEnd == false)
		{
			if(m_bTemp == false && m_bHey == true)
			{
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutText", 1);

				D3DXMATRIX matTrans;

				D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 78.f, 0.f);

				float fX = pTexInfo->ImgInfo.Width / 2.f;
				float fY = pTexInfo->ImgInfo.Height / 2.f;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else if(m_bHey)
			{
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"CutText", 0);

				D3DXMATRIX matTrans;

				D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 78.f, 0.f);

				float fX = pTexInfo->ImgInfo.Width / 2.f;
				float fY = pTexInfo->ImgInfo.Height / 2.f;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}		
	}
}

void CStage::Release(void)
{
	CObjMgr::GetInstance()->Release(OBJ_STRUCT);
	CObjMgr::GetInstance()->Release(OBJ_MONSTER);
	CObjMgr::GetInstance()->Release(OBJ_MONSTER_BULLET);
	CObjMgr::GetInstance()->Release(OBJ_SKILL);
	CObjMgr::GetInstance()->Release(OBJ_SKILL_EFFECT);
	CObjMgr::GetInstance()->Release(OBJ_CUBE);
	CObjMgr::GetInstance()->Release(OBJ_STORE);
}

int CStage::GetTileIndex(D3DXVECTOR3 vPos)
{
	D3DXVECTOR3 v1;
	D3DXVECTOR3 v2;

	int iIndexX;
	int iIndexY;
	int iIndexX2;
	int iIndexY2;

	iIndexX = int((vPos.x + (TILESIZEX / 2)) / TILESIZEX);
	iIndexY = int((vPos.y + (TILESIZEY / 2)) / TILESIZEY);

	v1.x = float(iIndexX * TILESIZEX);
	v1.y = float(iIndexY * TILESIZEY);
	v1.z = 0.f;

	iIndexX2 = int(vPos.x / TILESIZEX);
	iIndexY2 = int(vPos.y / TILESIZEY);

	v2.x = float(iIndexX2 * TILESIZEX);
	v2.y = float(iIndexY2 * TILESIZEY);
	v2.z = 0.f;

	v2.x += TILESIZEX / 2;
	v2.y += TILESIZEY / 2;

	//홀수 타일 픽킹

	D3DXVECTOR3 vDist1 = vPos - v1;
	D3DXVECTOR3 vDist2 = vPos - v2;

	float fDist1 = D3DXVec3Length(&vDist1);
	float fDist2 = D3DXVec3Length(&vDist2);

	
	if(fDist1 < fDist2)
	{	
		iIndexY *= 2;

		return iIndexY * TILECNTX + iIndexX;
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;

		return iIndexY2 * TILECNTX + iIndexX2;
	}
}	

unsigned int __stdcall CStage::TextureThread(void* pArg)
{
	CStage*	pLogo = (CStage*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Boss/Body/BossBody%d.png", L"BossBody", TEX_MULTI, 128);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Boss/Death/BossDeath%d.png", L"BossDeath", TEX_MULTI, 95);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Boss/Head/BossHead%d.png", L"BossHead", TEX_MULTI, 256);
	//이거 멀티 쓰레드로 처리하자.. // 로딩중에는 로딩 화면이 뜨도록..

	//보스 이펙트
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/BossCharge/BossCharge%d.png", L"BossCharge", TEX_MULTI, 87);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Boss/Bullet/BossBullet%d.png", L"BossBullet", TEX_MULTI, 15);
	LeaveCriticalSection(&pLogo->GetCrt());
	_endthreadex(0);

	return 1;
}

CRITICAL_SECTION CStage::GetCrt(void)
{
	return m_Crt;
}

HANDLE CStage::GetThread(void)
{
	return m_hThread;
}