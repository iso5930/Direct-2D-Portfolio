#include "StdAfx.h"
#include "TransFocusBar.h"
#include "Player.h"
#include "ObjMgr.h"
#include "TextureMgr.h"
#include "Device.h"

CTransFocusBar::CTransFocusBar(void)
{

}

CTransFocusBar::~CTransFocusBar(void)
{
	Release();
}

HRESULT CTransFocusBar::Initialize(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	m_vecTrans = ((CPlayer*)pObj)->GetvecTrans();

	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();
	m_FocusBar = ((CPlayer*)pObj)->GetFocusBar();

	return S_OK;
}

int CTransFocusBar::Progress(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();
	m_bTrans = ((CPlayer*)pObj)->GetTrans();
	m_bTransProgress = ((CPlayer*)pObj)->GetTransProgress();
	m_FocusBar = ((CPlayer*)pObj)->GetFocusBar();

	//매 프로그래스 마다 갱신..

	return 0;
}

void CTransFocusBar::Render(void)
{
	if(m_bTrans || m_bTransProgress)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Bar");

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 32.5f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		//포커스 바..

		vector<TRANS*>::iterator iter = m_vecTrans->begin();
		vector<TRANS*>::iterator iter_end = m_vecTrans->end();

		for(int i = 0; i < m_FocusBar.iCurTransIndex; ++i)
		{
			++iter;
		}

		for(iter; iter != iter_end; ++iter)
		{
			if((*iter)->dwKeyState == VK_LBUTTON)
			{
				//걷는거 출력..
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Walk_Start");

				D3DXMatrixTranslation(&matTrans, (*iter)->fStartCur, 32.5f, 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				int iCount = (int)(*iter)->fLoopCur;

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Walk_Loop");

				for(int i = 0; i < iCount; ++i)
				{
					matTrans._41 = (*iter)->fStartCur + i * 10.f + 10.f;

					CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
					CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Walk_Icon");
				matTrans._41 = (*iter)->fStartCur + ((*iter)->fEndCur - (*iter)->fStartCur) / 2.f;
				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Walk_End");

				matTrans._41 = (*iter)->fEndCur;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else if((*iter)->dwKeyState == VK_RBUTTON)
			{
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Skill_Start");

				D3DXMatrixTranslation(&matTrans, (*iter)->fStartCur, 32.5f, 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				
				int iCount = (int)(*iter)->fLoopCur;
				/*TCHAR szSkillName[50] = L"";

				
				}*/
				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Skill_Loop");

				for(int i = 0; i < iCount; ++i)
				{
					matTrans._41 = (*iter)->fStartCur + i * 15.f + 15.f;

					CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
					CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
				}

				pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Skill_End");

				matTrans._41 = (*iter)->fEndCur;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				D3DXMatrixTranslation(&matTrans, (*iter)->fStartCur + 10.f, 32.5f, 0.f);

				switch((*iter)->SkillType)
				{
				case SKILL_NO1:
					//lstrcpy(szSkillName, L"내려 찍기()");
					pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillText", 0);
					break;

				case SKILL_NO2:
					//lstrcpy(szSkillName, L"붕괴 시키기()");
					pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillText", 1);
					break;

				case SKILL_NO3:
					//lstrcpy(szSkillName, L"전격 클리스터()");
					pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillText", 2);
					break;

				case SKILL_NO4:
					//lstrcpy(szSkillName, L"순보()");
					pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillText", 3);
					break;

				case SKILL_NO5:
					//lstrcpy(szSkillName, L"도태 시키기()");
					pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"SkillText", 4);
					break;
				}
				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
				CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Start");
		D3DXMatrixTranslation(&matTrans, m_FocusBar.fTransStart, 32.5f, 0.f);
		

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		//시작 지점.

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Loop");

		int iEnd = int(m_FocusBar.fTransEnd - m_FocusBar.fTransStart - FOCUS_START) / 5;

		for(int i = 0; i < iEnd; ++i)
		{
			matTrans._41 = (m_FocusBar.fTransStart + 8.5f + 2.5f) + i * 5.f;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_End");

		D3DXMatrixTranslation(&matTrans, 1181.f, 32.5f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	else if(m_FocusBar.bReload || m_FocusBar.bReloadEnd)
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Bar");

		D3DXMATRIX matTrans;
		D3DXMatrixTranslation(&matTrans, WINCX / 2.f, 32.5f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		//포커스 바..

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Start");
		D3DXMatrixTranslation(&matTrans, m_FocusBar.fTransStart, 32.5f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		//시작 지점.

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_Loop");

		int iEnd = int(m_FocusBar.fTransEnd - m_FocusBar.fTransStart - FOCUS_START) / 5;

		for(int i = 0; i < iEnd; ++i)
		{
			matTrans._41 = (m_FocusBar.fTransStart + 8.5f + 2.5f) + i * 5.f;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Focus_End");

		D3DXMatrixTranslation(&matTrans, 1181.f, 32.5f, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

void CTransFocusBar::Release(void)
{
	
}
