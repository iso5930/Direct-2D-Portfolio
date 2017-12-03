#include "StdAfx.h"
#include "BackGround.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "Player.h"
#include "TimeMgr.h"

CBackGround::CBackGround(void)
{

}

CBackGround::~CBackGround(void)
{
	Release();
}

HRESULT CBackGround::Initialize(void)
{
	SetTile(1);

	m_fTransMask = 1.f;

	m_dwTime = GetTickCount();

	m_iStage = 1;

	return S_OK;
}

int CBackGround::Progress(void)
{
	return 0;
}

void CBackGround::Render(void)
{
	D3DXMATRIX matTrans;
	D3DXMatrixIdentity(&matTrans);

	const TEXINFO* pTexture = NULL;

	if(m_iStage == 1)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"1Stage");
	}
	else if(m_iStage == 3)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"BossStage");
	}
	else if(m_iStage == 2)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"2Stage");
	}
	
	D3DXMatrixTranslation(&matTrans, 0 - g_Scroll.x, 0 - g_Scroll.y, 0);
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	bool bTrans = ((CPlayer*)pObj)->GetTrans();

	if(bTrans == false)
	{
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(0, 0, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_iStage == 1)
		{
			D3DXMATRIX matScale;
			D3DXMATRIX matWorld;

			D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
			pTexture = CTextureMgr::GetInstance()->GetTexture(L"Dead");

			D3DXMatrixTranslation(&matTrans, 1814.f - g_Scroll.x, 1507.f - g_Scroll.y, 0);

			float fX = pTexture->ImgInfo.Width / 2.f;
			float fY = pTexture->ImgInfo.Height / 2.f;

			matWorld = matScale * matTrans;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(fX, fY, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

			pTexture = CTextureMgr::GetInstance()->GetTexture(L"Warp");
			D3DXMatrixTranslation(&matTrans, 3706.f - g_Scroll.x, 381.f - g_Scroll.y, 0.f);

			fX = pTexture->ImgInfo.Width / 2.f;
			fY = pTexture->ImgInfo.Height / 2.f;

			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(fX, fY, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
	else
	{
		const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Mask");

		D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

		D3DXMATRIX matScale;
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);

		m_fTransMask += 1 * 12 * GETTIME;

		D3DXMatrixScaling(&matScale, m_fTransMask, m_fTransMask, 0.f);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(pTexInfo->ImgInfo.Width / 2.f, pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	/*if(m_iStage == 3)
	{
		int iStartX = int(g_Scroll.x / TILESIZEX);
		int iStartY = int(g_Scroll.y / (TILESIZEY / 2));

		int iEndX = (iStartX + WINCX / TILESIZEX) + 2;
		int iEndY = iStartY + WINCY / (TILESIZEY / 2);

		for(int i = iStartY; i < iEndY; ++i)
		{
			for(int j = iStartX; j < iEndX; ++j)
			{
				int iIndex = i * BOSS_TILECNTX + j;

				if(iIndex < 0 || iIndex >= BOSS_TILECNTX * BOSS_TILECNTY)
					continue;
				
				D3DXMATRIX matTrans;
				D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x - g_Scroll.x, m_vecTile[iIndex]->vPos.y - g_Scroll.y, 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

				TCHAR szName[10] = L"";

				_itow_s(iIndex, szName, 10);

				CDevice::GetInstance()->GetFont3()->DrawTextW(CDevice::GetInstance()->GetSprite(), szName, lstrlen(szName), NULL, NULL, D3DCOLOR_ARGB(255,0,0,0));
			}
		}
	}*/
}

void CBackGround::Release(void)
{
	vector<TILE*>::iterator iter = m_vecTile.begin();
	vector<TILE*>::iterator iter_end = m_vecTile.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete (*iter);
	}
	m_vecTile.clear();
}

void CBackGround::SetTile(int iStage)
{
	vector<TILE*>::iterator iter = m_vecTile.begin();
	vector<TILE*>::iterator iter_end = m_vecTile.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete (*iter);
	}
	m_vecTile.clear();

	HANDLE hFile;
	DWORD dwByte = 0;

	m_iStage = iStage;

	switch(iStage)
	{
	case 1:
		hFile = CreateFile(L"../Data/Map/1Stage.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		
		if(hFile == NULL)
		{
			ERR_MSG(L"1스테이지 맵데이터 로드 실패");
		}

		while(true)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				delete pTile;
				break;
			}
			m_vecTile.push_back(pTile);
		}
		break;

	case 2:
		//2스테이지 생성..
		hFile = CreateFile(L"../Data/Map/2Stage.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == NULL)
		{
			ERR_MSG(L"2스테이지 맵데이터 로드 실패");
		}

		while(true)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				delete pTile;
				break;
			}
			m_vecTile.push_back(pTile);
		}
		break;

	case 3:
		hFile = CreateFile(L"../Data/Map/BossStage.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if(hFile == NULL)
		{
			ERR_MSG(L"Boss 스테이지 맵데이터 로드 실패");
		}

		while(true)
		{
			TILE* pTile = new TILE;

			ReadFile(hFile, pTile, sizeof(TILE), &dwByte, NULL);

			if(dwByte == 0)
			{
				delete pTile;
				break;
			}
			m_vecTile.push_back(pTile);
		}
		break;
	}

	CloseHandle(hFile);
}

int CBackGround::GetTileIndex(D3DXVECTOR3 vPos)
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

		if(m_iStage == 1)
		{
			return iIndexY * TILECNTX + iIndexX;
		}
		else if(m_iStage == 3)
		{
			return iIndexY * BOSS_TILECNTX + iIndexX;
		}
		else // 2스테이지 일때
		{
			return iIndexY * STAGE2_TILECNTX + iIndexX;
		}
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;

		if(m_iStage == 1)
		{
			return iIndexY2 * TILECNTX + iIndexX2;
		}
		else if(m_iStage == 3)
		{
			return iIndexY2 * BOSS_TILECNTX + iIndexX2;
		}
		else // 2스테이지 일때.
		{
			return iIndexY2 * STAGE2_TILECNTX + iIndexX2;
		}
	}	
}

void CBackGround::SetTileIndex(int PrevIndex, int CurIndex)
{
	m_vecTile[PrevIndex]->byDrawID = 0;
	m_vecTile[PrevIndex]->byOption = 0;
	m_vecTile[CurIndex]->byDrawID = 1;
	m_vecTile[CurIndex]->byOption = 1;
}

void CBackGround::SetTransMask(float fScale)
{
	m_fTransMask = fScale;
}