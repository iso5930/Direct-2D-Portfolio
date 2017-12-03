#include "StdAfx.h"
#include "Logo.h"
#include "TextureMgr.h"
#include "Device.h"
#include "SceneMgr.h"

CLogo::CLogo(void)
	:m_IntroFrame(0, 29, 0, 30), m_LoopFrame(0, 59, 0, 30), m_OutFrame(0, 9, 0, 30)
{
	memset(&m_Crt, 0, sizeof(CRITICAL_SECTION));
}

CLogo::~CLogo(void)
{
	Release();
}

HRESULT	CLogo::Initialize(void)
{
	m_dwTime = GetTickCount();

	lstrcpy(szName, L"LogoIntro");

	m_eLogoState = LOGO_INTRO;

	m_bLoading = false;

	InitializeCriticalSection(&m_Crt);

	CTextureMgr::GetInstance()->SetCritical(&m_Crt);

	//
	//쓰레드 여는것..
	
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/LoadingBar/LoadingBar%d.png", L"LoadingBar", TEX_MULTI, 2);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Intro/%d.png", L"LogoIntro", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Loop/%d.png", L"LogoLoop", TEX_MULTI, 60);

	m_pLoadBar_Top = const_cast<TEXINFO*>(CTextureMgr::GetInstance()->GetTexture(L"LoadingBar", 0));
	m_pLoadBar_Bottom = const_cast<TEXINFO*>(CTextureMgr::GetInstance()->GetTexture(L"LoadingBar", 1));

	/*CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Intro/%d.png", L"LogoIntro", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Loop/%d.png", L"LogoLoop", TEX_MULTI, 60);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Outro/%d.png", L"LogoOutro", TEX_MULTI, 10);*/

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, TextureLoadingThread, this, 0, NULL);

	m_rcLoading.left	= 0;
	m_rcLoading.top		= 0;
	m_rcLoading.bottom	= 80;
	m_rcLoading.right	= 146;

	m_fLoading = 146.f;

	m_dwLoadTime = GetTickCount();
	
	return S_OK;
}

int CLogo::UpDate(void)
{
	FrameMove();

	m_rcLoading.right = (long)m_fLoading;

	if(GetAsyncKeyState(VK_RETURN) && m_bLoading)
	{
		m_eLogoState = LOGO_OUTRO;
		lstrcpy(szName, L"LogoOutro");
	}

	return 0;
}

void CLogo::Render(void)
{
	const TEXINFO* pTexInfo;

	switch(m_eLogoState)
	{
	case LOGO_INTRO:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(szName, m_IntroFrame.iCur);
		break;

	case LOGO_LOOP:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(szName, m_LoopFrame.iCur);
		break;

	case LOGO_OUTRO:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(szName, m_OutFrame.iCur);
		break;
	}

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


	//로딩바=======================

	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 633.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(m_pLoadBar_Bottom->pTexture, NULL, &D3DXVECTOR3(m_pLoadBar_Bottom->ImgInfo.Width / 2.f, m_pLoadBar_Bottom->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	//음... 한 5초정도로 로딩시간을?

	CDevice::GetInstance()->GetSprite()->Draw(m_pLoadBar_Top->pTexture, &m_rcLoading, &D3DXVECTOR3(m_pLoadBar_Top->ImgInfo.Width / 2.f, m_pLoadBar_Top->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMatrixTranslation(&matTrans, 50.f, 680.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), CTextureMgr::GetInstance()->GetShowPath(), lstrlen(CTextureMgr::GetInstance()->GetShowPath()), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}	

void CLogo::Release(void)
{
	CTextureMgr::GetInstance()->Release(L"LogoLoop");
	CTextureMgr::GetInstance()->Release(L"LogoOutro");
}

void CLogo::FrameMove(void)
{
	switch(m_eLogoState)
	{
	case LOGO_INTRO:

		if(m_dwTime + m_IntroFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_IntroFrame.iCur > m_IntroFrame.iEndFrame)
			{
				m_eLogoState = LOGO_LOOP;
				lstrcpy(szName, L"LogoLoop");
				m_IntroFrame.iCur = m_IntroFrame.iStartFrame;
				CTextureMgr::GetInstance()->Release(L"LogoIntro");
			}
		}
		break;
	
	case LOGO_LOOP:
		if(m_dwTime + m_LoopFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_LoopFrame.iCur > m_LoopFrame.iEndFrame)
			{
				DWORD dwState;
				GetExitCodeThread(m_hThread, &dwState);

				if(dwState != STILL_ACTIVE && m_bLoading == false)
				{
					m_bLoading = true;
					CloseHandle(m_hThread);
					DeleteCriticalSection(&m_Crt);

					m_fLoading = 1280;

					CTextureMgr::GetInstance()->SetShowPath(L"Press Enter");
				}
				m_LoopFrame.iCur = m_LoopFrame.iStartFrame;
			}
		}
		break;
	
	case LOGO_OUTRO:
		if(m_dwTime + m_OutFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();

			if(++m_OutFrame.iCur > m_OutFrame.iEndFrame)
			{
				m_OutFrame.iCur = m_OutFrame.iEndFrame;
				CSceneMgr::GetInstance()->SetScene(SCENE_STAGE);
			}
		}
		break;
	}
}

void CLogo::LoadingPlus(int iCount)
{
	m_fLoading += 0.315f * iCount; 
}

unsigned int __stdcall CLogo::TextureLoadingThread(void* pArg)
{
	CLogo*	pLogo = (CLogo*)pArg;

	EnterCriticalSection(&pLogo->GetCrt());

	//CoInitialize(NULL);

	CTextureMgr::GetInstance()->SetShowPath(L"Load Texture...");
	
	//CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Loop/%d.png", L"LogoLoop", TEX_MULTI, 60);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Logo/Outro/%d.png", L"LogoOutro", TEX_MULTI, 10);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Map/1Stage.png", L"1Stage");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Object/1Stage/Cube/Cube%d.png", L"Cube", TEX_MULTI, 5);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Object/1Stage/Fence/Fence%d.png", L"Fence", TEX_MULTI, 2);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Object/1Stage/Struct/Struct%d.png", L"Struct", TEX_MULTI, 5);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Map/2Stage.png", L"2Stage");

	pLogo->LoadingPlus(23);

	//스킬 퀵슬롯. UI
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/HP.png", L"HP");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/HP_Guid.png", L"HP_Guid");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/QuickSlot.png", L"QuickSlot");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/Skill_BG.png", L"BG");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/Turn.png", L"Turn");

	pLogo->LoadingPlus(5);

	//스킬 반짝이
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/SkillLight/%d.png", L"SkillLight", TEX_MULTI, 45);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/SkillFlash/%d.png", L"SkillFlash", TEX_MULTI, 20);

	pLogo->LoadingPlus(65);

	//스킬 샵
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Intro/%d.png", L"StoreIntro", TEX_MULTI, 41);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Loop/%d.png", L"StoreLoop", TEX_MULTI, 90);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Outro/%d.png", L"StoreOutro", TEX_MULTI, 10);

	pLogo->LoadingPlus(141);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/BackGround/Store%d.png", L"StoreBackGround", TEX_MULTI, 2);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Back/SkillBack.png", L"SkillBack");

	pLogo->LoadingPlus(3);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Cur/SkillCur%d.png", L"SkillCur", TEX_MULTI, 2);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Button/Button_Base.png", L"Button_Base");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Button/Button_Down.png", L"Button_Down");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Button/Button_Over.png", L"Button_Over");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Button/Button_Off.png", L"Button_Off");

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Icon/%d.png", L"StoreIcon", TEX_MULTI, 5);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Large/Frame.png", L"LargeFrame");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/Icon/%d.png", L"QuickIcon", TEX_MULTI, 5);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Large/Skill/%d.png", L"LargeSkill", TEX_MULTI, 5);

	pLogo->LoadingPlus(22);

	//Socket 출력..
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Socket/Bottom/%d.png", L"SocketBottom", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/SkillStore/Socket/Top/%d.png", L"SocketTop", TEX_MULTI, 30);

	pLogo->LoadingPlus(60);

	

	//공격 이펙트
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Player/Effect/Attack/%d.png", L"AttackEffect", TEX_MULTI, 26);

	pLogo->LoadingPlus(26);

	//몬스터
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lobber/Attack/%d.png", L"LobberAttack", TEX_MULTI, 480);
	pLogo->LoadingPlus(480);
	//CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lobber/Glitch/LobberGlitch%d.png", L"LobberGlitch", TEX_MULTI, 24);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lobber/Run/LobberRun%d.png", L"LobberRun", TEX_MULTI, 640);

	
	pLogo->LoadingPlus(640);

	//트랜스 상태
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/Transition/%d.png", L"TransIntroEffect", TEX_MULTI, 35);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/Dot/Dot.png", L"TransDot", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Mask.png", L"Mask", TEX_SINGLE);

	pLogo->LoadingPlus(37);

	//트랜스 상태 포커스 바
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Bar.png", L"Focus_Bar", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Start.png", L"Focus_Start", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Loop.png", L"Focus_Loop", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_End.png", L"Focus_End", TEX_SINGLE);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Skill_Start.png", L"Focus_Skill_Start", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Skill_Loop.png", L"Focus_Skill_Loop", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Skill_End.png", L"Focus_Skill_End", TEX_SINGLE);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Walk_Icon.png", L"Focus_Walk_Icon", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Walk_Start.png", L"Focus_Walk_Start", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Walk_Loop.png", L"Focus_Walk_Loop", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/Focus_Walk_End.png", L"Focus_Walk_End", TEX_SINGLE);

	pLogo->LoadingPlus(11);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/Focus_Point/%d.png", L"Focus_Point", TEX_MULTI, 40);

	pLogo->LoadingPlus(40);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/Reload/Loop/Reload%d.png", L"Reload_Loop", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/Reload/End/Reload_End%d.png", L"Reload_End", TEX_MULTI, 14);

	pLogo->LoadingPlus(44);

	//스킬 2번
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Snipe/Charge/%d.png", L"Snipe_Charge", TEX_MULTI, 44);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Snipe/Bullet/particle_spiky_01.png", L"Snipe_Bullet", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Snipe/Line/particle_line_02.png", L"Snipe_Line", TEX_SINGLE);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Snipe/HelixShot/HelixShot%d.png", L"Snipe_Shot", TEX_MULTI, 28);

	pLogo->LoadingPlus(44);
	pLogo->LoadingPlus(1);
	pLogo->LoadingPlus(1);
	pLogo->LoadingPlus(28);

	//스킬 3번 전격 클리스터...
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Bomb/Core/BombCore%d.png", L"Bomb_Core", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Bomb/Second/BombSecond%d.png", L"Bomb_Second", TEX_MULTI, 30);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Bomb/Shadow.png", L"Bomb_Shadow");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/BombCoreRelease/BombCoreRelease%d.png", L"BombCoreRelease", TEX_MULTI, 9);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/BombSecondRelease/BombSecondRelease%d.png", L"BombSecondRelease", TEX_MULTI, 29);

	pLogo->LoadingPlus(60);
	pLogo->LoadingPlus(10);
	pLogo->LoadingPlus(29);

	//스킬 1번
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/SlamFire/Slam%d.png", L"SlamFire", TEX_MULTI, 13);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Skill/Slam/Bullet.png", L"SlamBullet");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/SlamSqarue/SlamSquare%d.png", L"SlamSquare", TEX_MULTI, 12);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/BackGround.png", L"TransBack");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Focus_Bar/OverFlow/FocusOverFlow%d.png", L"FocusOverFlow", TEX_MULTI, 9);

	pLogo->LoadingPlus(16);

	pLogo->LoadingPlus(10);

	//여기가지 1829장

	//큐브 오브젝트 아직 추가 안햇음...

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Cube/Loop/CubeLoop%d.png", L"CubeLoop", TEX_MULTI, 60);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Cube/Create/%d.png", L"CubeCreate", TEX_MULTI, 9);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Cube/Death/%d.png", L"CubeDeath", TEX_MULTI, 11);

	pLogo->LoadingPlus(80);

	//몬스터 죽을때 이펙트
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lobber/Death/Monster_Death%d.png", L"Monster_Death", TEX_MULTI, 37);

	pLogo->LoadingPlus(37);

	//몬스터 hp바
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/UI/MonsterHp/MonsterHp.png", L"MonsterHpBar");

	//Egg
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Egg/Head/EggHead%d.png", L"EggHead", TEX_MULTI, 88);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Egg/Body/EggBody%d.png", L"EggBody", TEX_MULTI, 32);

	pLogo->LoadingPlus(89);
	pLogo->LoadingPlus(32);

	//Lobber 타겟팅 이펙트
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/LobberTargeting/LobberTargeting%d.png", L"LobberTargeting", TEX_MULTI, 36);
	pLogo->LoadingPlus(36);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/LobberTargetAttack/LobberTaraetAttack%d.png", L"LobberTargetAttack", TEX_MULTI, 51);
	pLogo->LoadingPlus(51);

	//플레이어 히트 이펙트.
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/PlayerHit/PlayerHit%d.png", L"PlayerHit", TEX_MULTI, 20);
	pLogo->LoadingPlus(20);

	//Lady
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/Body/LadyBody%d.png", L"LadyBody", TEX_MULTI, 64);
	pLogo->LoadingPlus(64);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/Death/LadyDeath%d.png", L"LadyDeath", TEX_MULTI, 384);
	pLogo->LoadingPlus(384);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/Eyes/LadyEyes%d.png", L"LadyEyes", TEX_MULTI, 64);
	pLogo->LoadingPlus(64);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/EyesAttack/LadyAttack%d.png", L"LadyAttack", TEX_MULTI, 256);
	pLogo->LoadingPlus(256);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/EyesClosed/LadyEyesClose%d.png", L"LadyClose", TEX_MULTI, 64);
	pLogo->LoadingPlus(64);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/Glitch/LadyHit%d.png", L"LadyHit", TEX_MULTI, 18);
	pLogo->LoadingPlus(18);

	//LadyEffect
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/LadyWarp/LadyWarp%d.png", L"LadyWarp", TEX_MULTI, 80);
	pLogo->LoadingPlus(80);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Monster/Lady/Bullet/LadyBullet%d.png", L"LadyBullet", TEX_MULTI, 11);
	pLogo->LoadingPlus(11);

	//BossStage
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Map/BossStage.png", L"BossStage");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/BossBulletRelease/BossBulletRelease%d.png", L"BossBulletRelease", TEX_MULTI, 5);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Text/Text%d.png", L"CutText", TEX_MULTI, 5);
	pLogo->LoadingPlus(6);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Effect/DamageEffect/DamageEffect%d.png", L"DamageEffect", TEX_MULTI, 17);
	pLogo->LoadingPlus(17);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Text/Skil%d.png", L"SkillText", TEX_MULTI, 5);

	//오프닝 컷신
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Cutin/CutScene%d.png", L"CutSence", TEX_MULTI, 3);
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/boxer_dead_stabbed_02B.png", L"Dead");
	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Warp.png", L"Warp");

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Text/MiniSkill%d.png", L"MiniTex", TEX_MULTI, 5);

	CTextureMgr::GetInstance()->LoadTexture(L"../Texture/Cutin/Ending.png", L"Ending");

	pLogo->LoadingPlus(7);

	//한장당 0.315f 씩 증가.


	LeaveCriticalSection(&pLogo->GetCrt());
	_endthreadex(0);

	return 1;
}

CRITICAL_SECTION CLogo::GetCrt(void)
{
	return m_Crt;
}

HANDLE CLogo::GetThread(void)
{
	return m_hThread;
}