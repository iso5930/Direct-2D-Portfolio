#include "StdAfx.h"
#include "SkillStore.h"
#include "TextureMgr.h"
#include "Device.h"
#include "Mouse.h"
#include "ObjMgr.h"
#include "QuickSlot.h"
#include "SoundMgr.h"

CSkillStore::CSkillStore(void)
	:m_IntroFrame(0, 40, 0, 30), m_LoopFrame(0, 89, 0, 30), m_OutroFrame(0, 9, 0, 30)
{
	
}

CSkillStore::~CSkillStore(void)
{
	Release();
}


HRESULT CSkillStore::Initialize(void)
{
	m_eState = STORE_INTRO;

	m_dwTime = GetTickCount();

	m_vAddBtnPos.x = 293.f;
	m_vAddBtnPos.y = 513.f;
	m_vAddBtnPos.z = 0.f;

	m_vReleaseBtnPos.x = 536.f;
	m_vReleaseBtnPos.y = 513.f;
	m_vReleaseBtnPos.z = 0.f;

	m_vReturnBtnPos.x = 1044.f;
	m_vReturnBtnPos.y = 573.f;
	m_vReturnBtnPos.z = 0.f;

	m_vAddFontPos.x = 270.f;
	m_vAddFontPos.y = 503.f;
	m_vAddFontPos.z = 0.f;

	m_vReleaseFontPos.x = 460.f;
	m_vReleaseFontPos.y = 503.f;
	m_vReleaseFontPos.z = 0.f;

	m_vReturnFontPos.x = 1023.f;
	m_vReturnFontPos.y = 562.f;
	m_vReturnFontPos.z = 0.f;

	m_vLagerCurPos.x = 372.f;
	m_vLagerCurPos.y = 192.f;
	m_vLagerCurPos.z = 0.f;

	m_fBtnCX = 184.f;
	m_fBtnCY = 48.f;

	m_rcBtn_Add.left	= long(m_vAddBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Add.right	= long(m_vAddBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Add.top		= long(m_vAddBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Add.bottom	= long(m_vAddBtnPos.y + m_fBtnCY / 2);
	//추가 버튼

	m_rcBtn_Release.left	= long(m_vReleaseBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Release.right	= long(m_vReleaseBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Release.top		= long(m_vReleaseBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Release.bottom	= long(m_vReleaseBtnPos.y + m_fBtnCY / 2);
	//제거 버튼

	m_rcBtn_Return.left		= long(m_vReturnBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Return.right	= long(m_vReturnBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Return.top		= long(m_vReturnBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Return.bottom	= long(m_vReturnBtnPos.y + m_fBtnCY / 2);
	//뒤로 버튼

	m_bBtn_Add_Over			= false;
	m_bBtn_Release_Over		= false;
	m_bBtn_Return_Over		= false;

	m_bBtn_Add_Down			= false;
	m_bBtn_Release_Down		= false;
	m_bBtn_Return_Down		= false;

	m_bBtn_Add_Off			= false;
	m_bBtn_Release_Off		= false;
	m_bBtn_Return_Off		= false;

	m_bSkillCheck = false;

	m_bStoreWindow = false;

	m_vCurPos.x = 237.5f;
	m_vCurPos.y = 161.5f;

	m_vecIcon.reserve(5);
	m_vecQuickIcon.reserve(4);

	RECT rcIcon = { 200, 136, 266, 202 };

	m_vecIcon.push_back(rcIcon);

	rcIcon.left = 325;
	rcIcon.right = 391;

	m_vecIcon.push_back(rcIcon);

	rcIcon.left = 451;
	rcIcon.right = 517;

	m_vecIcon.push_back(rcIcon);

	rcIcon.left = 577;
	rcIcon.right = 643;

	m_vecIcon.push_back(rcIcon);

	rcIcon.left = 703;
	rcIcon.right = 769;

	m_vecIcon.push_back(rcIcon);

	/*rcIcon.left += 126;
	rcIcon.right += 126;

	m_vecIcon.push_back(rcIcon);*/

	////// 퀵슬롯 선택 렉트
	rcIcon.left = 310;
	rcIcon.right = 422;
	rcIcon.bottom = 254;
	rcIcon.top = 124;

	m_vecQuickIcon.push_back(rcIcon);

	rcIcon.left = 478;
	rcIcon.right = 590;

	m_vecQuickIcon.push_back(rcIcon);

	rcIcon.left = 646;
	rcIcon.right = 758;

	m_vecQuickIcon.push_back(rcIcon);

	rcIcon.left = 814;
	rcIcon.right = 926;

	m_vecQuickIcon.push_back(rcIcon);

	lstrcpy(m_szAddName, L"설치");

	m_iCurIndex = 0;
	m_iLagerCurIndex = 0;

	m_vLagerCurPos.x = 372.f;
	m_vLagerCurPos.y = 190.f;

	return S_OK;
}

int	CSkillStore::Progress(void)
{
	if(g_bStore == false)
	{
		return 0;
	}

	SetButton();

	POINT ptMouse = CMouse::GetPoint();

	if(m_bBtn_Add_Off == false)
	{
		if(PtInRect(&m_rcBtn_Add, ptMouse))
		{
			m_bBtn_Add_Over	= true;

			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_bBtn_Add_Down = true;

				if(m_bStoreWindow == false)
				{
					m_bStoreWindow = true;
					CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
					((CQuickSlot*)pObj)->SetRender(false);
				}
				else
				{	
					CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
					(*((CQuickSlot*)pObj)->GetQuickSlot())[m_iLagerCurIndex] = (eSkillType)m_iCurIndex;
				}
			}
		}
	}
	
	if(m_bBtn_Release_Off == false)
	{
		if(PtInRect(&m_rcBtn_Release, ptMouse))
		{	
			m_bBtn_Release_Over	= true;

			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_bBtn_Release_Down = true;

				if(m_bStoreWindow)
				{
					//윈도우 창이 떠있을때.
					CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
					(*((CQuickSlot*)pObj)->GetQuickSlot())[m_iLagerCurIndex] = SKILL_END;
				}
				else
				{
					CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
					vector<eSkillType> vecQuickSlot = *((CQuickSlot*)pObj)->GetQuickSlot();

					for(size_t i = 0; i < vecQuickSlot.size(); ++i)
					{
						if(vecQuickSlot[i] == m_iCurIndex)
						{
							(*((CQuickSlot*)pObj)->GetQuickSlot())[i] = SKILL_END;
						}
					}
					//초기화면일때.
				}
			}
		}
	}
		
	if(PtInRect(&m_rcBtn_Return, ptMouse))
	{
		m_bBtn_Return_Over = true;

		if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_bBtn_Return_Down = true;

			if(m_bStoreWindow == true)
			{
				m_bStoreWindow = false;
				CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
				((CQuickSlot*)pObj)->SetRender(true);
			}
			else
			{
				m_eState = STORE_OUTRO;
				CSoundMgr::GetInstance()->PlaySkillSound(L"StoreOut.wav");
			}		
		}
	}
	else
	{
		if(m_bStoreWindow)
		{
			for(size_t i = 0; i < m_vecQuickIcon.size(); ++i)
			{
				if(PtInRect(&m_vecQuickIcon[i], ptMouse))
				{
					if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						m_vLagerCurPos.x = 372.f + (167 * i);
						m_iLagerCurIndex = i;
					}
					break;
				}
			}
		}
		else
		{
			for(size_t i = 0; i < m_vecIcon.size(); ++i)
			{
				if(PtInRect(&m_vecIcon[i], ptMouse))
				{
					if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					{
						m_vCurPos.x = 237.5f + (127 * i);
						m_iCurIndex = i;
					}
					break;	
				}
			}
		}
	}

	if(m_bStoreWindow)
	{
		//윈도우 창이 떠있을때.
		//LargeIcon에 스킬이 있다면 설치 비활성화.
		CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
		vector<eSkillType> vecQuickSlot = *((CQuickSlot*)pObj)->GetQuickSlot();

		for(size_t i = 0; i < vecQuickSlot.size(); ++i)
		{
			if(vecQuickSlot[i] == m_iCurIndex)
			{
				m_bSkillCheck = true;
				//이미 있다..
				break;
			}

			if(i == 3)
			{
				m_bSkillCheck = false;
			}
		}

		if(vecQuickSlot[m_iLagerCurIndex] == SKILL_END && m_bSkillCheck == false)
		{
			//이거면 스킬이 없다는 뜻.
			m_bBtn_Release_Off = true;
			m_bBtn_Add_Off = false;
		}
		else
		{
			m_bBtn_Add_Off = true;
			m_bBtn_Release_Off = false;
		}
		//아니면 이미 장착이 되어있다면 설치 비활성화
	}
	else
	{
		//초기화면 일때..
		CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();
		vector<eSkillType> vecQuickSlot = *((CQuickSlot*)pObj)->GetQuickSlot();

		for(size_t i = 0; i < vecQuickSlot.size(); ++i)
		{
			if(vecQuickSlot[i] == m_iCurIndex)
			{
				m_bBtn_Add_Off = true;
				m_bBtn_Release_Off = false;
				break;
			}

			if(i == 3)
			{
				m_bBtn_Add_Off = false;
				m_bBtn_Release_Off = true;
			}
		}
	}

	if(m_bStoreWindow == true)
	{
		m_vAddBtnPos.x = 295.f;
		m_vAddBtnPos.y = 624.f;

		m_vReleaseBtnPos.x = 493.f;
		m_vReleaseBtnPos.y = 624.f;

		m_vReturnBtnPos.x = 1040.f;
		m_vReturnBtnPos.y = 624.f;

		m_vAddFontPos.x = 276.f;
		m_vAddFontPos.y = 611.f;

		m_vReleaseFontPos.x = 416.f;
		m_vReleaseFontPos.y = 611.f;

		m_vReturnFontPos.x = 1023.f;
		m_vReturnFontPos.y = 611.f;
	}
	else
	{
		m_vAddBtnPos.x = 293.f;
		m_vAddBtnPos.y = 513.f;

		m_vReleaseBtnPos.x = 536.f;
		m_vReleaseBtnPos.y = 513.f;

		m_vReturnBtnPos.x = 1044.f;
		m_vReturnBtnPos.y = 573.f;

		m_vAddFontPos.x = 270.f;
		m_vAddFontPos.y = 503.f;

		m_vReleaseFontPos.x = 460.f;
		m_vReleaseFontPos.y = 503.f;

		m_vReturnFontPos.x = 1023.f;
		m_vReturnFontPos.y = 562.f;
	}

	FrameMove();

	return 0;
}

void CSkillStore::Render(void)
{
	if(g_bStore == false)
	{
		return;
	}

	D3DXMATRIX matScall;
	D3DXMATRIX matTrans;

	D3DXMatrixScaling(&matScall, 1.7f, 1.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

	D3DXMATRIX matWorld;

	D3DXMatrixIdentity(&matWorld);

	matWorld = matScall * matTrans;

	const TEXINFO* pTexInfo;

	switch(m_eState)
	{
	case STORE_INTRO:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIntro", m_IntroFrame.iCur);
		break;

	case STORE_LOOP:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreLoop", m_LoopFrame.iCur);
		break;

	case STORE_OUTRO:
		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreOutro", m_OutroFrame.iCur);
		break;
	}

	CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	//백그라운드

	//============= 추가창 1 인트로 스프라이트 끝나고..

	if(m_eState == STORE_LOOP) //스토어 1번 일때
	{
		if(m_bStoreWindow == false)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreBackGround", 0);
			D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillBack");
			D3DXMatrixTranslation(&matTrans, 233.f, 169.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			//스킬 백 아이콘.. 이게 퀵슬롯에 들어가 있다면 0번 아니라면 1번.
			
			D3DXMatrixTranslation(&matTrans, 360.f, 169.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 487.f, 169.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 614.f, 169.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 741.f, 169.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			//127
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", 0);
			D3DXMatrixTranslation(&matTrans, 234.f, 169.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", 1);
			D3DXMatrixTranslation(&matTrans, 362.f, 169.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", 2);
			D3DXMatrixTranslation(&matTrans, 490.f, 169.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", 3);
			D3DXMatrixTranslation(&matTrans, 616.f, 169.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", 4);
			D3DXMatrixTranslation(&matTrans, 744.f, 169.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			//스킬 커서
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillCur", 0);
			D3DXMatrixTranslation(&matTrans, m_vCurPos.x, m_vCurPos.y, 0.f); //요거를 변수로 주자.
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		else
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreBackGround", 1);
			D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_QUICKSLOT].front();

			vector<eSkillType> vecQuick = *((CQuickSlot*)pObj)->GetQuickSlot();

			for(size_t i = 0; i < vecQuick.size(); ++i)
			{		
				if(vecQuick[i] == SKILL_END)
					continue;

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LargeSkill", vecQuick[i]);
				D3DXMatrixTranslation(&matTrans, 367.f + 166.f * i, 197.f, 0.f);
				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"LargeFrame");
			D3DXMatrixTranslation(&matTrans, 617, 191, 0);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillCur", 1);
			D3DXMatrixTranslation(&matTrans, m_vLagerCurPos.x, m_vLagerCurPos.y, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillBack");
			D3DXMatrixTranslation(&matTrans, 272.5f, 542.5f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"StoreIcon", m_iCurIndex);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			//여기서 퀵슬롯에 있는거 출력...
		
			//font 
			D3DXMatrixTranslation(&matTrans, 361.f, 134.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"1", lstrlen(L"1"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 528.f, 134.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"2", lstrlen(L"2"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

			D3DXMatrixTranslation(&matTrans, 695.f, 134.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"3", lstrlen(L"3"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			D3DXMatrixTranslation(&matTrans, 862.f, 134.f, 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

			CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"4", lstrlen(L"4"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
	
		//버튼 출력...
		//각 버튼마다의 렉트가 있어야 한다...
		//추가 버튼
		if(m_bBtn_Add_Off == true)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Base");
		}
		else if(m_bBtn_Add_Down)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Down");
		}
		else if(m_bBtn_Add_Over)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Over");
		}
		else
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Base");
		}
		D3DXMatrixTranslation(&matTrans, m_vAddBtnPos.x, m_vAddBtnPos.y, 0.f); //이게 위치..?
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		//릴리스 버튼
		if(m_bBtn_Release_Off == true)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Base");
		}
		else if(m_bBtn_Release_Down)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Down");
		}
		else if(m_bBtn_Release_Over)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Over");
		}
		else
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Base");
		}
		D3DXMatrixTranslation(&matTrans, m_vReleaseBtnPos.x, m_vReleaseBtnPos.y, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		
		//뒤로 버튼
		if(m_bBtn_Return_Down)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Down");
		}
		else if(m_bBtn_Return_Over)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Over");
		}
		else
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Base");
		}
		D3DXMatrixTranslation(&matTrans, m_vReturnBtnPos.x, m_vReturnBtnPos.y, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		//========================= 폰트 라인...

		//Font
		D3DXMatrixTranslation(&matTrans, m_vAddFontPos.x, m_vAddFontPos.y, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"설치", lstrlen(L"설치"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixTranslation(&matTrans, m_vReleaseFontPos.x, m_vReleaseFontPos.y, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"설치된 항목 제거", lstrlen(L"설치된 항목 제거"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		//font
		D3DXMatrixTranslation(&matTrans, m_vReturnFontPos.x, m_vReturnFontPos.y, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), L"뒤로", lstrlen(L"뒤로"), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		TCHAR szSkillName[40] = L"";

		switch(m_iCurIndex)
		{
		case 0:
			lstrcpy(szSkillName, L"내려 찍기()");
			break;

		case 1:
			lstrcpy(szSkillName, L"붕괴 시키기()");
			break;

		case 2:
			lstrcpy(szSkillName, L"전격 클리스터()");
			break;

		case 3:
			lstrcpy(szSkillName, L"순보()");
			break;

		case 4:
			lstrcpy(szSkillName, L"도태 시키기()");
			break;
		}

		D3DXMatrixTranslation(&matTrans, 209.f, 425.f, 0.f);
		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetFont2()->DrawTextW(CDevice::GetInstance()->GetSprite(), szSkillName, lstrlen(szSkillName), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_bBtn_Add_Off)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Off");
			D3DXMatrixTranslation(&matTrans, m_vAddBtnPos.x, m_vAddBtnPos.y, 0.f); //이게 위치..?
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if(m_bBtn_Release_Off)
		{
			pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Button_Off");
			D3DXMatrixTranslation(&matTrans, m_vReleaseBtnPos.x, m_vReleaseBtnPos.y, 0.f); //이게 위치..?
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

	}
}

void CSkillStore::Release(void)
{

}

void CSkillStore::FrameMove(void)
{
	switch(m_eState)
	{
	case STORE_INTRO:
		if(m_dwTime + m_IntroFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_IntroFrame.iCur > m_IntroFrame.iEndFrame)
			{
				m_eState = STORE_LOOP;
				m_IntroFrame.iCur = m_IntroFrame.iStartFrame;
			}
		}
		break;

	case STORE_LOOP:
		if(m_dwTime + m_LoopFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_LoopFrame.iCur > m_LoopFrame.iEndFrame)
			{
				m_LoopFrame.iCur = m_LoopFrame.iStartFrame;
			}
		}
		break;

	case STORE_OUTRO:
		if(m_dwTime + m_OutroFrame.dwFrameTime < GetTickCount())
		{
			m_dwTime = GetTickCount();
			if(++m_OutroFrame.iCur > m_OutroFrame.iEndFrame)
			{
				g_bStore = false;
				m_eState = STORE_INTRO;
				m_IntroFrame.iCur = m_IntroFrame.iStartFrame;
				m_LoopFrame.iCur = m_LoopFrame.iStartFrame;
				m_OutroFrame.iCur = m_OutroFrame.iStartFrame;
			}
		}
		break;
	}
}
void CSkillStore::SetButton(void)
{
	m_rcBtn_Add.left	= long(m_vAddBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Add.right	= long(m_vAddBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Add.top		= long(m_vAddBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Add.bottom	= long(m_vAddBtnPos.y + m_fBtnCY / 2);
	//추가 버튼

	m_rcBtn_Release.left	= long(m_vReleaseBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Release.right	= long(m_vReleaseBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Release.top		= long(m_vReleaseBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Release.bottom	= long(m_vReleaseBtnPos.y + m_fBtnCY / 2);
	//제거 버튼

	m_rcBtn_Return.left		= long(m_vReturnBtnPos.x - m_fBtnCX / 2);
	m_rcBtn_Return.right	= long(m_vReturnBtnPos.x + m_fBtnCX / 2);
	m_rcBtn_Return.top		= long(m_vReturnBtnPos.y - m_fBtnCY / 2);
	m_rcBtn_Return.bottom	= long(m_vReturnBtnPos.y + m_fBtnCY / 2);

	m_bBtn_Add_Over			= false;
	m_bBtn_Release_Over		= false;
	m_bBtn_Return_Over		= false;

	m_bBtn_Add_Down			= false;
	m_bBtn_Release_Down		= false;
	m_bBtn_Return_Down		= false;
}