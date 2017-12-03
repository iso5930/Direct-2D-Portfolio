#include "StdAfx.h"
#include "Player.h"
#include "Device.h"
#include "Mouse.h"
#include "Astar.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "TimeMgr.h"
#include "TextureMgr.h"
#include "Factory.h"
#include "Skill.h"
#include "QuickSlot.h"
#include "SkillEffect.h"
#include "DotMgr.h"
#include "SceneMgr.h"
#include "TransFocusBar.h"
#include "Monster.h"
#include "Cube.h"
#include "SoundMgr.h"


CPlayer::CPlayer(void)
	:m_StandFrame(1, 60, 1, 70), m_RunFrame(1, 38, 1, 10), m_StopFrame(1, 15, 1, 10), m_SocketFrame(1, 42, 1, 20), m_AttackFrame(1, 66, 1, 10), m_TransFrame(0, 34, 0, 30), m_TransPoint(0, 39, 0, 20),
	m_DashFrame(1, 26, 1, 10), m_ReloadLoop(0, 29, 0, 20), m_ReloadEnd(0, 13, 0, 10), m_SnipeCharge(0, 43, 0, 20), m_BlinkFrame(1, 130, 1, 2), m_Uppercut(1, 128, 1, 10), m_OverFlowFrame(0, 8, 0, 30),
	m_HitFrame(0, 19, 0, 40), m_CreateFrame(1, 340, 1, 10)
{
	m_pTexInfo = NULL;

	m_pAstar = NULL;
}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	m_tInfo.vPos.x = 1736.f;
	m_tInfo.vPos.y = 1479.f;
	m_tInfo.vLook.x = 1.f;
	m_tInfo.vLook.y = 0.f;
	m_tInfo.vDir.x = 1.f;
	m_tInfo.vDir.y = 0.f;

	D3DXMatrixIdentity(&m_tInfo.matWorld);

	m_pTexInfo = new TEXINFO;

	for(int i = 0; i < 5; ++i)
	{
		TEXINFO* pTexInfo = new TEXINFO;

		m_vecBlink.push_back(pTexInfo);
	}
	 
	m_bChange = false;

	m_eState = PLAYER_CREATE;

	m_dwTime = GetTickCount();

	m_dwTransTime = GetTickCount();

	LoadTexture();

	m_vCur = m_tInfo.vPos;

	m_PrevIndex = 1;

	//pList = CAstar::GetInstance()->GetBestList();

	g_bStore = false;

	m_dwMoveTime = GetTickCount();

	m_bCheckIn = false;

	m_pAstar = new CAstar;

	pList = m_pAstar->GetBestList();

	m_bTrans = false;

	m_eTransState = TRANS_END;

	m_bTransProgress = false;

	m_iPrevTransIndex = 999999;

	m_iCurTransIndex = 0;

	m_fTransStart = 98.5f;

	m_fTransEnd = 1181.f; //<- end는 고정된값.

	//m_iWalkCount = 0;

	m_bCharge = false;

	m_bBlink = false;

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 90.f;

	//test
	for(int i = 0; i < 5; ++i)
	{
		TEXINFO* pTexInfo = m_vecBlink[i];

		TCHAR szPath[MAX_PATH] = L"";
		TCHAR szFilePath[MAX_PATH] = L"";

		lstrcpy(szPath, L"../Texture/Player/Blink/%d.png");

		wsprintf(szFilePath, szPath, m_BlinkFrame.iStartFrame + 4 + i);

		D3DXGetImageInfoFromFile(szFilePath, &pTexInfo->ImgInfo);

		D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szFilePath,
			pTexInfo->ImgInfo.Width,
			pTexInfo->ImgInfo.Height,
			pTexInfo->ImgInfo.MipLevels,
			0,
			pTexInfo->ImgInfo.Format,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&pTexInfo->ImgInfo,
			NULL,
			&pTexInfo->pTexture);
	}

	m_iAlpha = 127;

	m_bOverFlow = false;

	m_bReload = false;

	m_bReloadEnd = false;

	m_dwReloadTime = GetTickCount();

	m_tData.iMaxHp = 1000;
	m_tData.iHp = 1000;

	m_bHit = false;

	m_dwHitTime = GetTickCount();

	m_strName = "Player";

	return S_OK;
}

int CPlayer::Progress(void)
{
	if(g_bStore)
		return 0;
	else
	{
		int iIndex = m_pAstar->GetTileIndex(m_tInfo.vPos);

		CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

		vector<TILE*> vecTile = *((CBackGround*)pObj)->GetTile();

		if(vecTile[iIndex]->byDrawID == 3)
		{
			if(m_bCheckIn == false)
			{
				m_pAstar->Release();
				m_eState = PLAYER_SOCKET;
				m_bCheckIn = true;
			}
		}
		else if(vecTile[iIndex]->byDrawID == 0)
		{
			if(m_bCheckIn)
			{
				m_bCheckIn = false;
			}
		}
	}//스킬 상점 들어가는것.

	//m_AttackFrame.dwFrameTime = 0;
	//플레이어가 Create 상태일때는 공격이나 다른상태 X <- 이거 수정해야 함...

	if(m_bHit)
	{
		if(m_dwHitTime + m_HitFrame.dwFrameTime < GetTickCount())
		{
			m_dwHitTime = GetTickCount();
			if(++m_HitFrame.iCur > m_HitFrame.iEndFrame)
			{
				m_HitFrame.iCur = m_HitFrame.iStartFrame;
				m_bHit = false;
			}
		}
	}

	if(m_bTransProgress)
	{
		//m_AttackFrame.dwFrameTime = 0;

		if(m_vecTrans.size() == 0)
		{
			m_bTransProgress = false;
			return 0;
		}//아무 것도 입력을 안햇을때의 예외처리..

		if(m_vecTrans.size() <= (size_t)m_iCurTransIndex)
		{
			for(size_t i = 0; i < m_vecTrans.size(); ++i)
			{
				delete m_vecTrans[i];
			}
			m_vecTrans.clear();

			m_bTransProgress = false;

			m_bReload = true;

			return 0;
		}

		if(m_vecTrans[m_iCurTransIndex]->dwKeyState == VK_LBUTTON)
		{
			m_eState = PLAYER_DASH;
			
			int iIndex = m_vecTrans[m_iCurTransIndex]->iPosIndex;

			if((unsigned)iIndex < m_vecTrans[m_iCurTransIndex]->vecNextPos.size())
			{
				m_pAstar->AstarStartPos(m_tInfo.vPos, m_vecTrans[m_iCurTransIndex]->vecNextPos[iIndex]);
			}
		
			if(m_pAstar->GetBestList()->empty())
			{
				++m_vecTrans[m_iCurTransIndex]->iPosIndex;
			}

			if((unsigned)m_vecTrans[m_iCurTransIndex]->iPosIndex > m_vecTrans[m_iCurTransIndex]->vecNextPos.size())
			{
				++m_iCurTransIndex;
			}
		}
		else if(m_vecTrans[m_iCurTransIndex]->dwKeyState == VK_RBUTTON)
		{
			if(m_iCurTransIndex != m_iPrevTransIndex)
			{
				m_iPrevTransIndex = m_iCurTransIndex;
				m_eState = m_vecTrans[m_iCurTransIndex]->PlayerType;
				m_vCur = m_vecTrans[m_iCurTransIndex]->vPos;

				if(m_vecTrans[m_iCurTransIndex]->SkillType == SKILL_NO4)
				{
					m_bBlink = true;
				}
				else if(m_vecTrans[m_iCurTransIndex]->SkillType == SKILL_NO2)
				{
					m_bCharge = true;
				}

				switch(m_vecTrans[m_iCurTransIndex]->SkillType)
				{
				case SKILL_NO1:
					CSoundMgr::GetInstance()->PlaySkillSound(L"Slam.wav");
					break;

				case SKILL_NO2:
					CSoundMgr::GetInstance()->PlaySkillSound(L"Snipe.wav");
					break;

				case SKILL_NO4:
					CSoundMgr::GetInstance()->PlaySkillSound(L"Blink.wav");
					m_bBlink = true;
					break;
				}
			}
		}
	}
	else
	{
		if((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && m_dwMoveTime + 150 < GetTickCount())
		{
			if(m_eState != PLAYER_SOCKET && m_eState != PLAYER_ATTACK)
			{
				m_dwMoveTime = GetTickCount();
				m_vCur = CMouse::GetPos();
				m_vCur += g_Scroll;
				m_pAstar->AstarStartPos(m_tInfo.vPos, m_vCur);
				m_eState = PLAYER_RUN;

				if(m_bTrans)
				{
					if(m_vecTrans.empty() || m_vecTrans.back()->dwKeyState != VK_LBUTTON)
					{
						TRANS* pTrans = new TRANS;

						pTrans->dwKeyState = VK_LBUTTON;
						pTrans->PlayerType = PLAYER_RUN;
						//pTrans->vPos = m_vCur;
						pTrans->vecNextPos.push_back(m_vCur);
						pTrans->fStartCur = m_fTransStart - 3.5f;
						pTrans->fLoopCur = 1;
						pTrans->fEndCur = pTrans->fStartCur - 3.f + (pTrans->fLoopCur * 10) + 10.f;
						m_fTransStart = pTrans->fEndCur + 8.5f;
						pTrans->iPosIndex = 0;
						m_vecTrans.push_back(pTrans);
					}
					else
					{
						m_vecTrans.back()->fEndCur = m_vecTrans.back()->fStartCur - 3.f + (m_vecTrans.back()->fLoopCur * 10) + 10.f;
						m_vecTrans.back()->vecNextPos.push_back(m_vCur);
						m_fTransStart = m_vecTrans.back()->fEndCur + 8.5f;
					}
				}
			}			
		}

		if((GetAsyncKeyState(VK_RBUTTON) & 0x8000) && m_dwMoveTime + 300 < GetTickCount())
		{
			m_dwMoveTime = GetTickCount();

			if(m_eState != PLAYER_ATTACK && m_eState != PLAYER_BLINK && m_eState != PLAYER_UPPERCUT)
			{
				m_pAstar->Release();

				CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
				eSkillType eType = ((CQuickSlot*)pObj)->GetCursel();

				m_vCur = CMouse::GetPos();
				m_vCur += g_Scroll;

				if(m_bReload || m_bReloadEnd)
				{
					if(eType == SKILL_NO4)
					{
						m_eState = PLAYER_BLINK;
						CSoundMgr::GetInstance()->PlaySkillSound(L"Blink.wav");
						m_bBlink = true;
					}
				}
				else
				{
					switch(eType)
					{
					case SKILL_NO1:
						m_eState = PLAYER_ATTACK;
						CSoundMgr::GetInstance()->PlaySkillSound(L"Slam.wav");
						break;

					case SKILL_NO2:
						m_eState = PLAYER_ATTACK;
						CSoundMgr::GetInstance()->PlaySkillSound(L"Snipe.wav");
						m_bCharge = true;
						break;

					case SKILL_NO3:
						m_eState = PLAYER_ATTACK;
						break;

					case SKILL_NO4:
						m_eState = PLAYER_BLINK;
						CSoundMgr::GetInstance()->PlaySkillSound(L"Blink.wav");
						m_bBlink = true;
						break;

					case SKILL_NO5:
						m_eState = PLAYER_UPPERCUT;
						break;
					}
				}

				if(m_bTrans)
				{
					CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();

					eSkillType eType = ((CQuickSlot*)pObj)->GetCursel();

					TRANS* pTrans = new TRANS;

					float fPrevTransStart = m_fTransStart;

					switch(eType)
					{
					case SKILL_NO1:
						pTrans->fLoopCur = 7;
						break;

					case SKILL_NO2:
						pTrans->fLoopCur = 14;
						m_bCharge = false;
						break;

					case SKILL_NO3:
						pTrans->fLoopCur = 9;
						break;

					case SKILL_NO4:
						pTrans->fLoopCur = 5; 
						break;

					case SKILL_NO5:
						pTrans->fLoopCur = 20;
						break;
					}

					CSoundMgr::GetInstance()->PlaySkillSound(L"FocusSkill.wav");

					pTrans->dwKeyState = VK_RBUTTON;
					pTrans->PlayerType = m_eState;
					pTrans->SkillType = eType;
					pTrans->vPos = m_vCur; //마우스 지점 저장.
					pTrans->fStartCur = m_fTransStart - 3.5f;
					pTrans->fEndCur = pTrans->fStartCur - 3.f + (pTrans->fLoopCur * 15) + 15.f;
					m_fTransStart = pTrans->fEndCur + 8.5f;

					if(m_fTransStart > m_fTransEnd)
					{
						m_bOverFlow = true;
						m_fTransStart = fPrevTransStart;
						m_eState = PLAYER_STAND;
						delete pTrans;
					}
					else
					{
						m_vecTrans.push_back(pTrans);
					}
				}
			}	
		}

		if((GetAsyncKeyState(VK_SPACE) & 0x8000) && m_dwMoveTime + 500 < GetTickCount())
		{
			m_dwMoveTime = GetTickCount();

			m_pAstar->Release();

			if(m_eState == PLAYER_RUN)
			{
				m_eState = PLAYER_STOP;
			}
			if(m_bTrans)
			{
				m_eTransState = TRANS_EXIT;

				m_tInfo.vPos = m_vOriginPos; 

				m_iCurTransIndex = 0;
				m_iPrevTransIndex = 9999;
				m_PrevIndex = 9999;

				m_eState = PLAYER_STAND;

				return 0;
			}
			else if (m_bReload == false && m_bReloadEnd == false)
			{
				m_bTrans = true;
				m_TransFrame.iCur = m_TransFrame.iStartFrame;
				m_eTransState = TRANS_START;

				m_iCurTransIndex = 0;
				m_iPrevTransIndex = 9999;

				m_vOriginPos = m_tInfo.vPos; //원래 위치로 돌아갈수있도록 저장..

				m_fTransStart = 98.5f;

				CDotMgr::GetInstance()->ResetPos();

				CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

				((CBackGround*)pObj)->SetTransMask(1.f);

				CSoundMgr::GetInstance()->PlaySkillSound(L"FocusStart.wav");
			}
		}
	}

	if(m_eState == PLAYER_RUN || m_eState == PLAYER_DASH)
	{
		AstarMove();
	}

	if(m_bTrans)
	{
		TransSetAngle(m_vCur);
	}
	else
	{
		SetAngle(m_vCur);
	}

	FrameMove();

	if(m_bReloadEnd || m_bReload)
	{
		TransReload();
	}

	if(m_bBlink)
	{
		BlinkMove();
	}

	/*if(g_bCreate == false)
	{*/
		SetScroll();
	//}

	if(m_bChange)
	{
		m_bChange = false;

		if(m_pTexInfo->pTexture != NULL)
		{
			m_pTexInfo->pTexture->Release();
		}
		LoadTexture();
	}

	CObj::Progress();

	return 0;
}

void CPlayer::Render(void)
{
	if(m_bTrans)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"TransIntroEffect", m_TransFrame.iCur);

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(m_bReload || m_bReloadEnd)
	{
		const TEXINFO* pTexInfo = NULL;

		if(m_bReload)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Reload_Loop", m_ReloadLoop.iCur);
		}
		else
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Reload_End", m_ReloadEnd.iCur);
		}

		D3DXMATRIX matScale;
		D3DXMATRIX matTrans;
		D3DXMATRIX matWorld;
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - 100.f - g_Scroll.y, 0.f);
		D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

		matWorld = matScale * matTrans;

		float fX = pTexInfo->ImgInfo.Width / 2.f;
		float fY = pTexInfo->ImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));			
	}

	if(m_eState == PLAYER_BLINK)
	{
		for(int i = 0; i < 5; ++i)
		{
			const TEXINFO* pTexInfo = m_vecBlink[i];

			D3DXMATRIX matTrans;
			D3DXMATRIX matScale;

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);

			float fX = pTexInfo->ImgInfo.Width / 2.f;
			float fY = (pTexInfo->ImgInfo.Height / 2.f) + 106.f;

			D3DXVECTOR3 vShadow = m_tInfo.vPos - m_tInfo.vDir * (55.f * i);

			D3DXMatrixTranslation(&matTrans, vShadow.x - g_Scroll.x, vShadow.y - g_Scroll.y, 0.f);

			m_tInfo.matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

			int iAlpha = m_iAlpha - i * 10;
			
			if(iAlpha < 0)
			{
				iAlpha = 0;
			}

			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(iAlpha, 255, 255, 255));						
		}
	}

	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.7f);

	float fX = m_pTexInfo->ImgInfo.Width / 2.f;
	float fY = (m_pTexInfo->ImgInfo.Height / 2.f) + 106.f;

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(m_pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	if(!pList->empty())
	{
		CBackGround* pBack = (CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

		const vector<TILE*>* pvecTile = pBack->GetTile();

		list<int>::iterator iter = pList->begin();
		list<int>::iterator iter_end = pList->end();

		int iA = 0;

		for(iter; iter != iter_end; ++iter)
		{
			TCHAR szTemp[10] = L"";

			_itow_s(iA, szTemp, 10);

			int iIndex = (*iter);
			D3DXVECTOR3 vPos = (*pvecTile)[iIndex]->vPos;

			D3DXMATRIX matTrans;
			D3DXMatrixTranslation(&matTrans, vPos.x - g_Scroll.x, vPos.y - g_Scroll.y, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szTemp, lstrlen(szTemp), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			++iA;
		}
	}

	//여기 부터 옮겨야함..
	if(m_bTrans || m_bTransProgress)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Point", m_TransPoint.iCur);

		for(size_t i = m_iCurTransIndex; i < m_vecTrans.size(); ++i)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Point", m_TransPoint.iCur);
			D3DXMatrixTranslation(&matTrans, m_vecTrans[i]->vPos.x - g_Scroll.x, m_vecTrans[i]->vPos.y - g_Scroll.y, 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, m_vecTrans[i]->vPos.x - g_Scroll.x, (m_vecTrans[i]->vPos.y - g_Scroll.y) - 20.f, 0.f);

			TCHAR szSkillName[50] = L"";

			switch(m_vecTrans[i]->SkillType)
			{
			case SKILL_NO1:
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MiniTex", 0);
				break;

			case SKILL_NO2:
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MiniTex", 1);
				break;

			case SKILL_NO3:
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MiniTex", 2);
				break;

			case SKILL_NO4:
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MiniTex", 3);
				break;

			case SKILL_NO5:
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"MiniTex", 4);
				break;
			}

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			//CDevice::GetInstance()->GetFont3()->DrawTextW(CDevice::GetInstance()->GetSprite(), szSkillName, lstrlen(szSkillName), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if(m_bOverFlow)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"FocusOverFlow", m_OverFlowFrame.iCur);

		D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	if(m_bCharge)
	{
		if(m_AttackFrame.iStartFrame + 16 == m_AttackFrame.iCur)
		{
			const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Snipe_Charge", m_SnipeCharge.iCur);

			float fAngle = m_PrevIndex * 11.25f;

			fAngle -= 15.f;

			float fX = m_tInfo.vPos.x - 3.f;
			float fY = m_tInfo.vPos.y - 127.f;

			fX += 27.3f * cosf(D3DXToRadian(fAngle));
			fY -= 15.75f * sinf(D3DXToRadian(fAngle));

			D3DXMatrixTranslation(&matTrans, fX - g_Scroll.x, fY - g_Scroll.y, 0.f);

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}

	if(m_bHit)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"PlayerHit", m_HitFrame.iCur);

		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - g_Scroll.x, m_tInfo.vPos.y - g_Scroll.y, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	//HDC dc = GetDC(g_hWnd);
	//Rectangle(dc, m_Rect.left - g_Scroll.x, m_Rect.top - g_Scroll.y, m_Rect.right - g_Scroll.x, m_Rect.bottom - g_Scroll.y);
}

void CPlayer::Release(void)
{
	if(m_pTexInfo->pTexture != NULL)
	{
		m_pTexInfo->pTexture->Release();
	}
	delete m_pTexInfo;

	if(m_pAstar != NULL)
	{
		delete m_pAstar;
		m_pAstar = NULL;
	}
}

void CPlayer::FrameMove(void)
{	
	if(m_bTrans)
	{
		if(m_bOverFlow)
		{
			if(m_dwTransTime + m_OverFlowFrame.dwFrameTime < GetTickCount())
			{
				m_dwTransTime = GetTickCount();
				if(++m_OverFlowFrame.iCur > m_OverFlowFrame.iEndFrame)
				{
					m_OverFlowFrame.iCur = m_OverFlowFrame.iStartFrame;
					m_bOverFlow = false;
				}
			}
		}

		switch(m_eTransState)
		{
		case TRANS_START:
			if(m_dwTransTime + m_TransFrame.dwFrameTime < GetTickCount())
			{
				m_dwTransTime = GetTickCount();
				if(++m_TransFrame.iCur > m_TransFrame.iEndFrame)
				{
					m_TransFrame.iCur = m_TransFrame.iEndFrame;
					m_eTransState = TRANS_LOOP;
				}
			}
			break;

		case TRANS_LOOP:
			if(m_dwTransTime + m_TransPoint.dwFrameTime < GetTickCount())
			{
				m_dwTransTime = GetTickCount();
				if(++m_TransPoint.iCur > m_TransPoint.iEndFrame)
				{
					m_TransPoint.iCur = m_TransPoint.iStartFrame;
				}
			}
			break;

		case TRANS_EXIT:
			if(m_dwTransTime + 10 < GetTickCount())
			{
				m_dwTransTime = GetTickCount();

				if(--m_TransFrame.iCur < m_TransFrame.iStartFrame)
				{
					m_eTransState = TRANS_END;
					m_bTrans = false;
					m_bTransProgress = true;
				}
			}
			break;
		}
	}

	if(m_bTransProgress)
	{
		if(m_eTransState == TRANS_END)
		{
			if(m_dwTransTime + 10 < GetTickCount())
			{
				m_dwTransTime = GetTickCount();
				if(++m_TransPoint.iCur > m_TransPoint.iEndFrame)
				{
					m_TransPoint.iCur = m_TransPoint.iStartFrame;
				}
			}
		}
	}

	switch(m_eState)
	{
	case PLAYER_STAND:
		if(m_dwTime + m_StandFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;
			if(++m_StandFrame.iCur > m_StandFrame.iEndFrame)
			{
				m_StandFrame.iCur = m_StandFrame.iStartFrame;
			}
		}
		break;

	case PLAYER_RUN:
		if(m_dwTime + m_RunFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;
	
			if(++m_RunFrame.iCur > m_RunFrame.iEndFrame)
			{
				m_RunFrame.iCur = m_RunFrame.iStartFrame;
			}
		}
		break;

	case PLAYER_STOP:
		if(m_dwTime + m_StopFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;

			if(++m_StopFrame.iCur > m_StopFrame.iEndFrame)
			{
				m_StandFrame.iEndFrame = m_PrevIndex * 60;
				m_StandFrame.iStartFrame = m_StandFrame.iEndFrame - 59;
				m_StandFrame.iCur = m_StandFrame.iStartFrame;

				m_eState = PLAYER_STAND;
				m_StopFrame.iCur = m_StopFrame.iStartFrame;
			}
		}
		break;

	case PLAYER_SOCKET:
		if(m_dwTime + m_SocketFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;

			if(m_SocketFrame.iCur == m_SocketFrame.iEndFrame - 5)
			{
				g_bStore = true;
				CSoundMgr::GetInstance()->PlaySkillSound(L"StoreStart.wav");
			}

			if(++m_SocketFrame.iCur > m_SocketFrame.iEndFrame)
			{
				m_SocketFrame.iCur = m_SocketFrame.iEndFrame;
				m_eState = PLAYER_STAND;
				m_StandFrame.iEndFrame = (m_PrevIndex * 2) * 60;
				m_StandFrame.iStartFrame = m_StandFrame.iEndFrame - 59;
				m_StandFrame.iCur = m_StandFrame.iStartFrame;
			}
	
		}
		break;

	case PLAYER_ATTACK:
		if(m_dwTime + m_AttackFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;

			if(m_bCharge)
			{
				if(m_AttackFrame.iStartFrame + 16 == m_AttackFrame.iCur)
				{
					if(++m_SnipeCharge.iCur > m_SnipeCharge.iEndFrame)
					{
						m_SnipeCharge.iCur = m_SnipeCharge.iStartFrame;
						m_bCharge = false;
					}
					return;
				}
			}
			
			if(m_AttackFrame.iStartFrame + 28 == m_AttackFrame.iCur)
			{
				if(m_bTransProgress)
				{
					CreateSkill(m_vecTrans[m_iCurTransIndex]->SkillType);
				}
				else if (m_eTransState == TRANS_END)
				{
					CreateSkill();
				}
			}

			if(++m_AttackFrame.iCur > m_AttackFrame.iEndFrame)
			{
				m_eState = PLAYER_STAND;
				m_AttackFrame.iCur = m_AttackFrame.iStartFrame;

				if(m_bTransProgress)
				{
					++m_iCurTransIndex;
				}

				m_StandFrame.iEndFrame = m_PrevIndex * 60;
				m_StandFrame.iStartFrame = m_StandFrame.iEndFrame - 59;
				m_StandFrame.iCur = m_StandFrame.iStartFrame;
			}
		}
		break;

	case PLAYER_DASH:
		if(m_dwTime + m_DashFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;
			if(++m_DashFrame.iCur > m_DashFrame.iEndFrame)
			{
				m_DashFrame.iCur = m_DashFrame.iStartFrame;
			}
		}
		break;

	case PLAYER_BLINK:
		if(m_dwTime + m_BlinkFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;
			if(++m_BlinkFrame.iCur > m_BlinkFrame.iEndFrame)
			{
				m_eState = PLAYER_STAND;
				m_BlinkFrame.iCur = m_BlinkFrame.iStartFrame;

				m_StandFrame.iEndFrame = m_PrevIndex * 60;
				m_StandFrame.iStartFrame = m_StandFrame.iEndFrame - 59;
				m_StandFrame.iCur = m_StandFrame.iStartFrame;

				if(m_bTransProgress)
				{
					++m_iCurTransIndex;
				}
			}
		}
		break;

	case PLAYER_UPPERCUT:
		if(m_dwTime + m_Uppercut.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;

			if(m_bTrans == false)
			{
				if(m_Uppercut.iStartFrame + 63 < m_Uppercut.iCur && m_Uppercut.iCur > m_Uppercut.iStartFrame + 74)
				{
					SkillCollision();
				}
				else
					m_HitList.clear();
			}
			
			if(++m_Uppercut.iCur > m_Uppercut.iEndFrame)
			{
				m_eState = PLAYER_STAND;
				m_Uppercut.iCur = m_Uppercut.iStartFrame;

				m_StandFrame.iEndFrame = (m_PrevIndex * 4) * 60;
				m_StandFrame.iStartFrame = m_StandFrame.iEndFrame - 59;
				m_StandFrame.iCur = m_StandFrame.iStartFrame;

				m_HitList.clear();

				if(m_bTransProgress)
				{
					++m_iCurTransIndex;
				}
			}
		}
		break;

	case PLAYER_CREATE:
		if(m_dwTime + m_CreateFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			m_bChange = true;

			if(++m_CreateFrame.iCur > m_CreateFrame.iEndFrame)
			{
				m_eState = PLAYER_STAND;
				m_CreateFrame.iCur = m_CreateFrame.iStartFrame;
			}
		}
		break;
	}
}

HRESULT CPlayer::LoadTexture(void)
{
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	TCHAR szPath[MAX_PATH] = L"";
	TCHAR szFilePath[MAX_PATH] = L"";
	
	switch(m_eState)
	{
	case PLAYER_STAND:
		lstrcpy(szPath, L"../Texture/Player/Wating/%d.png");
		wsprintf(szFilePath, szPath, m_StandFrame.iCur);
		break;

	case PLAYER_RUN:
		lstrcpy(szPath, L"../Texture/Player/Run/%d.png");
		wsprintf(szFilePath, szPath, m_RunFrame.iCur);
		break;

	case PLAYER_STOP:
		lstrcpy(szPath, L"../Texture/Player/Stop/%d.png");
		wsprintf(szFilePath, szPath, m_StopFrame.iCur);
		break;

	case PLAYER_SOCKET:
		lstrcpy(szPath, L"../Texture/Player/Socket/%d.png");
		wsprintf(szFilePath, szPath, m_SocketFrame.iCur);
		break;

	case PLAYER_ATTACK:
		lstrcpy(szPath, L"../Texture/Player/Attack/%d.png");
		wsprintf(szFilePath, szPath, m_AttackFrame.iCur);
		break;

	case PLAYER_DASH:
		lstrcpy(szPath, L"../Texture/Player/Dash/%d.png");
		wsprintf(szFilePath, szPath, m_DashFrame.iCur);
		break;

	case PLAYER_BLINK:
		lstrcpy(szPath, L"../Texture/Player/Blink/%d.png");
		wsprintf(szFilePath, szPath, m_BlinkFrame.iCur);
		break;

	case PLAYER_UPPERCUT:
		lstrcpy(szPath, L"../Texture/Player/Uppercut/%d.png");
		wsprintf(szFilePath, szPath, m_Uppercut.iCur);
		break;

	case PLAYER_CREATE:
		lstrcpy(szPath, L"../Texture/Player/SwordPull/%d.png");
		wsprintf(szFilePath, szPath, m_CreateFrame.iCur);
		break;
	}

	HRESULT hr = D3DXGetImageInfoFromFile(szFilePath, &m_pTexInfo->ImgInfo);
	

	D3DXCreateTextureFromFileEx(
		CDevice::GetInstance()->GetDevice(),
		szFilePath,
		m_pTexInfo->ImgInfo.Width,
		m_pTexInfo->ImgInfo.Height,
		m_pTexInfo->ImgInfo.MipLevels,
		0,
		m_pTexInfo->ImgInfo.Format,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		NULL,
		&m_pTexInfo->ImgInfo,
		NULL,
		&m_pTexInfo->pTexture);

	return S_OK;
}

void CPlayer::SetScroll(void)
{
	g_Scroll.x = float(m_tInfo.vPos.x - WINCX / 2);
	g_Scroll.y = float(m_tInfo.vPos.y - WINCY / 2);

	eSceneType eType = CSceneMgr::GetInstance()->GetSceneType();

	switch(eType)
	{
	case SCENE_STAGE:

		if(g_Scroll.x < 0)
		{
			g_Scroll.x = 0;
		}
		else if(g_Scroll.x > 3030)
		{
			g_Scroll.x = 3030;
		}

		if(g_Scroll.y < 0)
		{
			g_Scroll.y = 0;
		}
		else if(g_Scroll.y > 1343)
		{
			g_Scroll.y = 1343;
		}
		break;

	case SCENE_STAGE2:
		if(g_Scroll.x < 0)
		{
			g_Scroll.x = 0;
		}
		else if(g_Scroll.x > 876)
		{
			g_Scroll.x = 876;
		}

		if(g_Scroll.y < 0)
		{
			g_Scroll.y = 0;
		}
		else if(g_Scroll.y > 644)
		{
			g_Scroll.y = 644;
		}
		break;

	case SCENE_BOSS:
		if(g_Scroll.x < 0)
		{
			g_Scroll.x = 0;
		}
		else if(g_Scroll.x > 800)
		{
			g_Scroll.x = 800;
		}

		if(g_Scroll.y < 0)
		{
			g_Scroll.y = 0;
		}
		else if(g_Scroll.y > 480)
		{
			g_Scroll.y = 480;
		}
		break;
	}

	//c/out << g_Scroll.x << " , " << g_Scroll.y << endl;

}

void CPlayer::SetAngle(D3DXVECTOR3 vDir)
{	
	float fX = vDir.x - m_tInfo.vPos.x;
	float fY = vDir.y - m_tInfo.vPos.y;

	float fDist = sqrt( fX * fX + fY * fY );

	float fAngle = acosf(fX / fDist) * 180 / D3DX_PI;

	if(m_tInfo.vPos.y < vDir.y)
	{
		fAngle = 360 - fAngle; 
	}

	switch(m_eState)
	{
	case PLAYER_STAND:
	
		break;

	case PLAYER_RUN:
		{
			float fSource = 5.625f;

			int iIndex = 0;
			
			for(int i = 1; i < 65; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}
		
			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			
			m_RunFrame.iEndFrame = iIndex * 38;
			m_RunFrame.iStartFrame = m_RunFrame.iEndFrame - 37;
			m_RunFrame.iCur = m_RunFrame.iStartFrame;
		}
		break;

	case PLAYER_STOP:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_StopFrame.iEndFrame = iIndex * 15;
			m_StopFrame.iStartFrame = m_StopFrame.iEndFrame - 14;
			m_StopFrame.iCur = m_StopFrame.iStartFrame;
		}
		break;

	case PLAYER_SOCKET:
		{
			float fSource = 22.5f;

			int iIndex = 0;

			for(int i = 1; i < 17; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
			{
				return;
			}
		
			m_PrevIndex = iIndex;
			m_SocketFrame.iEndFrame = iIndex * 42;
			m_SocketFrame.iStartFrame = m_SocketFrame.iEndFrame - 41;
			m_SocketFrame.iCur = m_SocketFrame.iStartFrame;
		}
		break;

	case PLAYER_ATTACK:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_AttackFrame.iEndFrame = iIndex * 66;
			m_AttackFrame.iStartFrame = m_AttackFrame.iEndFrame - 65;
			m_AttackFrame.iCur = m_AttackFrame.iStartFrame;

		}
		break;

	case PLAYER_DASH:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_DashFrame.iEndFrame = iIndex * 26;
			m_DashFrame.iStartFrame = m_DashFrame.iEndFrame - 25;
			m_DashFrame.iCur = m_DashFrame.iStartFrame;
		}
		break;

	case PLAYER_BLINK:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_BlinkFrame.iEndFrame = iIndex * 130;
			m_BlinkFrame.iStartFrame = m_BlinkFrame.iEndFrame - 129;
			m_BlinkFrame.iCur = m_BlinkFrame.iStartFrame;
		}
		break;

	case PLAYER_UPPERCUT:
		{
			float fSource = 45.f;

			int iIndex = 0;

			for(int i = 1; i < 9; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
			{
				return;
			}

			m_PrevIndex = iIndex;
			m_Uppercut.iEndFrame = iIndex * 128;
			m_Uppercut.iStartFrame = m_Uppercut.iEndFrame - 127;
			m_Uppercut.iCur = m_Uppercut.iStartFrame;
		}
		break;
	}
}
void CPlayer::AstarMove(void)
{		
	list<int>* pBestList = m_pAstar->GetBestList();

	if(pBestList->empty())
	{
		if(m_bTransProgress)
		{
			m_eState = PLAYER_STAND;
		}
		else
		{
			m_eState = PLAYER_STOP;
		}
		return;
	}

	CBackGround* pBack = (CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

	const vector<TILE*>* pvecTile = pBack->GetTile();

	int iMoveIndex = pBestList->front();

	m_vCur = (*pvecTile)[iMoveIndex]->vPos;

	m_tInfo.vDir = (*pvecTile)[iMoveIndex]->vPos - m_tInfo.vPos;

	float fDistance = D3DXVec3Length(&m_tInfo.vDir);

	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	//m_tInfo.vPos += m_tInfo.vDir * 0.5f;

	if(m_bTransProgress)
	{
		m_tInfo.vPos += m_tInfo.vDir * 450 * GETTIME;
	}
	else
	{
		m_tInfo.vPos += m_tInfo.vDir * 150 * GETTIME;
	}

	/*if(fDistance < 5.f)
		pBestList->pop_front();*/

	if(m_pAstar->GetTileIndex(m_tInfo.vPos) == pBestList->front())
	{
		pBestList->pop_front();

		if(m_bTrans)
		{
			m_fTransStart += 10.f;
			++m_vecTrans.back()->fLoopCur;
			m_vecTrans.back()->fEndCur += 10.f;
		}
		else if(m_bTransProgress)
		{
			//--m_iWalkCount;
			//m_fWalkEnd -= 10.f;
			//--m_vecTrans.back()->fLoopCur;
			//m_vecTrans.back()->fEndCur -= 10.f;
		}
	}
}

void CPlayer::BlinkMove(void)
{
	D3DXVECTOR3 vDist = m_vCur - m_tInfo.vPos;

	D3DXVec3Normalize(&vDist, &vDist);

	m_tInfo.vDir = vDist;

	m_tInfo.vPos = m_vCur - vDist;

	vector<TEXINFO*>::iterator iter = m_vecBlink.begin();
	vector<TEXINFO*>::iterator iter_end = m_vecBlink.end();

	for(iter; iter != iter_end; ++iter)
	{
		if((*iter)->pTexture != NULL)
		{
			(*iter)->pTexture->Release();
		}
	}

	iter = m_vecBlink.begin();
	
	for(int i = 0; i < 5; ++i)
	{
		TCHAR szPath[MAX_PATH] = L"";
		TCHAR szFilePath[MAX_PATH] = L"";

		lstrcpy(szPath, L"../Texture/Player/Blink/%d.png");

		wsprintf(szFilePath, szPath, m_BlinkFrame.iStartFrame + 4 + i);

		D3DXGetImageInfoFromFile(szFilePath, &(*iter)->ImgInfo);

		D3DXCreateTextureFromFileEx(
			CDevice::GetInstance()->GetDevice(),
			szFilePath,
			(*iter)->ImgInfo.Width,
			(*iter)->ImgInfo.Height,
			(*iter)->ImgInfo.MipLevels,
			0,
			(*iter)->ImgInfo.Format,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			NULL,
			&(*iter)->ImgInfo,
			NULL,
			&(*iter)->pTexture);

		++iter;
	}

	m_bBlink = false;

	//이동 제한은 나중에...
}


void CPlayer::CreateSkill(eSkillType _eType)
{	
	float fAngle = m_PrevIndex * 11.25f;

	fAngle -= 10.f;

	D3DXVECTOR3 vPos = m_tInfo.vPos;

	vPos.x += 48.3f * cosf(D3DXToRadian(fAngle));
	vPos.y -= 26.6f * sinf(D3DXToRadian(fAngle));

	
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
	eSkillType eType = ((CQuickSlot*)pObj)->GetCursel();

	if(m_bTransProgress)
	{
		eType = _eType;
	}

	pObj = CFactory<CSkillEffect>::CreateObj(vPos, EFFECT_DECAL);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
	//여기까지는 바닥 이펙트 입니당.

	switch(eType)
	{
	case SKILL_NO1:
		{
			D3DXVECTOR3 vEffectPos = vPos;
			D3DXVECTOR3 vLook = D3DXVECTOR3(1.f, 0.f, 0.f);

			D3DXVECTOR3 vDir = m_vCur - m_tInfo.vPos;

			D3DXVec3Normalize(&vDir, &vDir);

			vEffectPos += 10 * vDir;

			float fSkillAngle = D3DXVec3Dot(&vLook, &vDir);

			fSkillAngle = acos(fSkillAngle);

			if(m_tInfo.vPos.y < m_vCur.y)
			{
				fSkillAngle = 2 * D3DX_PI - fSkillAngle;
			}

			pObj = CFactory<CSkillEffect>::CreateObj(vEffectPos, EFFECT_SLAM_FIRE);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);

			pObj = CFactory<CSkill>::CreateObj(vPos, vDir, fSkillAngle, SKILL_NO1);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL);
		}
		break;

	case SKILL_NO2:
		{
			//============================= 이게 2번 스킬
			D3DXVECTOR3 vSkillPos = vPos;

			vSkillPos.y -= 46.f; //스킬 나가는위치

			D3DXVECTOR3 vLook = D3DXVECTOR3(1.f, 0.f, 0.f);

			D3DXVECTOR3 vDir = m_vCur - m_tInfo.vPos;

			D3DXVec3Normalize(&vDir, &vDir);

			float fSkillAngle = D3DXVec3Dot(&vLook, &vDir);

			fSkillAngle = acosf(fSkillAngle);

			if(m_tInfo.vPos.y < m_vCur.y)
			{
				fSkillAngle = 2 * D3DX_PI - fSkillAngle;
			}
			//이게 2번 스킬...

			pObj = CFactory<CSkill>::CreateObj(vSkillPos, vDir, fSkillAngle, eType);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL);

			//cout << vDir.x << endl;
			//cout << vDir.y << endl;
		}
		break;

	case SKILL_NO3:
		{	
			CSoundMgr::GetInstance()->PlaySkillSound(L"ClusterStart.wav");
			D3DXVECTOR3 vSkillPos = vPos;

			vSkillPos.y -= 46.f; //스킬 나가는위치

			D3DXVECTOR3 vLook = D3DXVECTOR3(1.f, 0.f, 0.f);

			D3DXVECTOR3 vDir = m_vCur - m_tInfo.vPos;

			D3DXVECTOR3 vLength = m_vCur - vSkillPos;

			float fLength = D3DXVec3Length(&vLength);

			D3DXVec3Normalize(&vDir, &vDir);

			float fSkillAngle = D3DXVec3Dot(&vLook, &vDir);

			fSkillAngle = acosf(fSkillAngle);

			if(m_tInfo.vPos.y < m_vCur.y)
			{
				fSkillAngle = 2 * D3DX_PI - fSkillAngle;
			}

			pObj = CFactory<CSkill>::CreateObj(vSkillPos, vDir, fSkillAngle, eType, fLength);
			CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL);
		}
		break;
	}

}

FOCUS CPlayer::GetFocusBar(void)
{
	return FOCUS(m_iCurTransIndex, m_fTransStart, m_fTransEnd, m_bReload, m_bReloadEnd);
}

void CPlayer::TransSetAngle(D3DXVECTOR3 vDir)
{
	float fX = vDir.x - m_tInfo.vPos.x;
	float fY = vDir.y - m_tInfo.vPos.y;

	float fDist = sqrt( fX * fX + fY * fY );

	float fAngle = acosf(fX / fDist) * 180 / D3DX_PI;

	if(m_tInfo.vPos.y < vDir.y)
	{
		fAngle = 360 - fAngle; 
	}

	switch(m_eState)
	{
	case PLAYER_STAND:

		break;

	case PLAYER_RUN:
		{
			float fSource = 5.625f;

			int iIndex = 0;

			for(int i = 1; i < 65; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;

			m_RunFrame.iEndFrame = iIndex * 38;
			m_RunFrame.iStartFrame = m_RunFrame.iEndFrame - 37;
			m_RunFrame.iCur = m_RunFrame.iStartFrame;
		}
		break;

	case PLAYER_STOP:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_StopFrame.iEndFrame = iIndex * 15;
			m_StopFrame.iStartFrame = m_StopFrame.iEndFrame - 14;
			m_StopFrame.iCur = m_StopFrame.iStartFrame;

		}
		break;

	case PLAYER_ATTACK:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;

			CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
			eSkillType eType = ((CQuickSlot*)pObj)->GetCursel();

			m_AttackFrame.iEndFrame = iIndex * 66;
			m_AttackFrame.iStartFrame = m_AttackFrame.iEndFrame - 65;
			m_AttackFrame.iCur = m_AttackFrame.iStartFrame;

			switch(eType)
			{
			case SKILL_NO2:
				m_AttackFrame.iEndFrame = m_AttackFrame.iStartFrame + 13; 
				break;

			case SKILL_NO3:
				m_AttackFrame.iStartFrame += 26;
				m_AttackFrame.iEndFrame -= 25;
				m_AttackFrame.iCur = m_AttackFrame.iStartFrame;
				break;
			}
		}
		break;

	case PLAYER_DASH:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_DashFrame.iEndFrame = iIndex * 26;
			m_DashFrame.iStartFrame = m_DashFrame.iEndFrame - 25;
			m_DashFrame.iCur = m_DashFrame.iStartFrame;
		}
		break;

	case PLAYER_BLINK:
		{
			float fSource = 11.25f;

			int iIndex = 0;

			for(int i = 1; i < 33; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
				return;

			m_PrevIndex = iIndex;
			m_BlinkFrame.iEndFrame = iIndex * 130;
			m_BlinkFrame.iStartFrame = m_BlinkFrame.iEndFrame - 129;
			m_BlinkFrame.iCur = m_BlinkFrame.iStartFrame;
		}
		break;

	case PLAYER_UPPERCUT:
		{
			float fSource = 45.f;

			int iIndex = 0;

			for(int i = 1; i < 9; ++i)
			{
				if(fAngle <= fSource * i)
				{
					iIndex = i;
					break;
				}
			}

			if(iIndex == m_PrevIndex)
			{
				return;
			}

			m_PrevIndex = iIndex;
			m_Uppercut.iEndFrame = iIndex * 128;
			m_Uppercut.iStartFrame = m_Uppercut.iEndFrame - 127;
			m_Uppercut.iCur = m_Uppercut.iStartFrame;

			m_Uppercut.iEndFrame -= 62;
			m_Uppercut.iStartFrame += 55;
			m_Uppercut.iCur = m_Uppercut.iStartFrame;
		}
		break;
	}
}

void CPlayer::TransReload(void)
{
	if(m_bReload)
	{
		if(m_dwReloadTime + m_ReloadLoop.dwFrameTime < GetTickCount())
		{
			m_dwReloadTime = GetTickCount();
			
			m_fTransStart -= 10.f;

			if(m_fTransStart <= 98.5f)
			{
				m_fTransStart = 98.5f;
				m_bReload = false;
				m_bReloadEnd = true;
			}

			if(++m_ReloadLoop.iCur > m_ReloadLoop.iEndFrame)
			{
				m_ReloadLoop.iCur = m_ReloadLoop.iStartFrame;
			}
		}
	}
	else if(m_bReloadEnd)
	{
		if(m_dwReloadTime + m_ReloadEnd.dwFrameTime < GetTickCount())
		{
			m_dwReloadTime = GetTickCount();

			if(++m_ReloadEnd.iCur > m_ReloadEnd.iEndFrame)
			{
				m_ReloadEnd.iCur = m_ReloadEnd.iStartFrame;
				m_bReloadEnd = false;
			}
		}
	}
}

void CPlayer::SkillCollision(void)
{
	//도태 시키기 충돌처리 함수
	if(m_eState == PLAYER_UPPERCUT)
	{
		D3DXVECTOR3 vSkillCollisionPos = m_tInfo.vPos;
		
		//vSkillCollisionPos.x += 84.f;
		vSkillCollisionPos.y -= 138.f;
	
		float fCX = 247.8f;
		float fCY = 241.f;
		RECT rcSkill;

		rcSkill.left = long(vSkillCollisionPos.x - fCX / 2.f);
		rcSkill.top = long(vSkillCollisionPos.y - fCY / 2.f);
		rcSkill.right = long(vSkillCollisionPos.x + fCX / 2.f);
		rcSkill.bottom = long(vSkillCollisionPos.y + fCY / 2.f);

		//일단 충돌 박스는 그렷드아....

		//몬스터 / 큐브와의 충돌처리만 하자...

		list<CObj*>::iterator Mon_Iter = CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].begin();
		list<CObj*>::iterator Mon_Iter_End = CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].end();

		list<CObj*>::iterator Cube_Iter = CObjMgr::GetInstance()->GetList()[OBJ_CUBE].begin();
		list<CObj*>::iterator Cube_Iter_End = CObjMgr::GetInstance()->GetList()[OBJ_CUBE].end();

		RECT rcTemp;

		for(Mon_Iter; Mon_Iter != Mon_Iter_End; ++Mon_Iter)
		{
			RECT* rcMonster = (*Mon_Iter)->GetRect();

			if(IntersectRect(&rcTemp, &rcSkill, rcMonster))
			{
				if(HitCheck((*Mon_Iter)))
				{
					((CMonster*)*Mon_Iter)->HitAction(150);
					CObj* pObj = CFactory<CSkillEffect>::CreateObj((*Mon_Iter)->GetInfo()->vPos, EFFECT_BOMB_CORE);
					CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
				}
			}
		}

		for(Cube_Iter; Cube_Iter != Cube_Iter_End; ++Cube_Iter)
		{
			RECT* rcCube = (*Cube_Iter)->GetRect();

			if(IntersectRect(&rcTemp, &rcSkill, rcCube))
			{
				if(HitCheck((*Cube_Iter)))
				{
					((CCube*)(*Cube_Iter))->CubeHitAction(150);
					CObj* pObj = CFactory<CSkillEffect>::CreateObj((*Cube_Iter)->GetInfo()->vPos, EFFECT_BOMB_CORE);
					CObjMgr::GetInstance()->AddObject(pObj, OBJ_SKILL_EFFECT);
				}
			}
		}
	}
}

bool CPlayer::HitCheck(CObj* pObj)
{
	list<CObj*>::iterator iter = m_HitList.begin();
	list<CObj*>::iterator iter_end = m_HitList.end();

	for(iter; iter != iter_end; ++iter)
	{	
		if(pObj == (*iter))
		{
			return false;
		}
	}

	m_HitList.push_back(pObj);

	return true;
}

void CPlayer::PlayerHit(int iAtt)
{
	if(m_bHit == false)
	{
		m_bHit = true;

		m_tData.iHp -= iAtt;

		if(m_tData.iHp <= 0)
		{
			m_tData.iHp = 0;
		}

		m_dwHitTime = GetTickCount();
	}
}