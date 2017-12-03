#include "StdAfx.h"
#include "BackGround.h"
#include "ToolView.h"
#include "TextureMgr.h"
#include "Device.h"
#include "MainFrm.h"
#include "SheetView.h"

CBackGround::CBackGround(void)
{
	m_bShow = true;
	m_bTileNum = false;
	m_bTileObj = false;
	m_bObjAlpha = false;
}

CBackGround::~CBackGround(void)
{
	Release();
}

void CBackGround::TileChange(const D3DXVECTOR3& vPos, const int& iDrawID, int iOption)
{
	int iIndex = GetTileIndex(vPos);

	if(iIndex == -1)
		return;

	m_vecTile[iIndex]->byDrawID = iDrawID;
	m_vecTile[iIndex]->byOption = iOption;
}

bool CBackGround::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{	
	float fGradient = (TILESIZEY / 2.f) / (TILESIZEX / 2.f);

	D3DXVECTOR3 vPoint[4] = 
	{
		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y - TILESIZEY / 2.f, 0),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x + TILESIZEX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x, m_vecTile[iIndex]->vPos.y + TILESIZEY / 2.f, 0.f),

		D3DXVECTOR3(m_vecTile[iIndex]->vPos.x - TILESIZEX / 2.f, m_vecTile[iIndex]->vPos.y, 0.f)
	};

	D3DXVECTOR3	vDirection[4] = 
	{
		vPoint[1] - vPoint[0], vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2], vPoint[0] - vPoint[3]
	};

	D3DXVECTOR3 vNormal[4] = 
	{
		D3DXVECTOR3(vDirection[0].y, -vDirection[0].x, 0.f),
		D3DXVECTOR3(vDirection[1].y, -vDirection[1].x, 0.f),
		D3DXVECTOR3(vDirection[2].y, -vDirection[2].x, 0.f),
		D3DXVECTOR3(vDirection[3].y, -vDirection[3].x, 0.f)
	};
	
	float fB[4] = 
	{
		vPoint[0].y - fGradient * vPoint[0].x,
		vPoint[1].y + fGradient * vPoint[1].x,
		vPoint[2].y - fGradient * vPoint[2].x,
		vPoint[3].y + fGradient * vPoint[3].x,
	};

	if(vPos.y - fGradient * vPos.x - fB[0] > 0 &&
		vPos.y + fGradient * vPos.x - fB[1] < 0 &&
		vPos.y - fGradient * vPos.x - fB[2] < 0 &&
		vPos.y + fGradient * vPos.x - fB[3] > 0)
	{
		return true;
	}

	return false;
	
}

int	CBackGround::GetTileIndex(const D3DXVECTOR3& vPos)
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

	
	int iTileCntY = 0;
	int iTileCntX = 0;

	switch(m_iType)
	{
	case 0:
		iTileCntX = TILECNTX;
		iTileCntY = TILECNTY;
		break;

	case 1:
		iTileCntX = STAGE2_TILECNTX;
		iTileCntY = STAGE2_TILECNTY;
		break;

	case 2:
		iTileCntX = BOSS_TILECNTX;
		iTileCntY = BOSS_TILECNTY;
		break;

	}

	if(fDist1 < fDist2)
	{	
		iIndexY *= 2;

		return iIndexY * iTileCntX + iIndexX;
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;
		
		return iIndexY2 * iTileCntX + iIndexX2;
	}
}

void CBackGround::SetMainView(CToolView* pView)
{
	m_pView = pView;
}

void CBackGround::Initialize(int Type)
{

	m_iType = Type;
	int iTileCntY = 0;
	int iTileCntX = 0;

	switch(Type)
	{
	case 0:
		iTileCntX = TILECNTX;
		iTileCntY = TILECNTY;
		CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Map/1Stage.png", L"Stage1");

		for(int i = 0; i < iTileCntY; ++i)
		{
			for(int j = 0; j < iTileCntX; ++j)
			{
				TILE* pTile = new TILE;

				float fX = (float)((i % 2) * TILESIZEX / 2) + j * TILESIZEX;
				float fY = i * (TILESIZEY / 2.f);

				pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
				pTile->vSize = D3DXVECTOR3(float(TILESIZEX), float(TILESIZEY), 0.f);
				pTile->byOption = 1;
				pTile->byDrawID = 1;

				m_vecTile.push_back(pTile);
			}
		}
		break;

	case 1:
		iTileCntX = STAGE2_TILECNTX;
		iTileCntY = STAGE2_TILECNTY;

		CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Map/2Stage.png", L"Stage2");

		for(int i = 0; i < iTileCntY; ++i)
		{
			for(int j = 0; j < iTileCntX; ++j)
			{
				TILE* pTile = new TILE;

				float fX = (float)((i % 2) * TILESIZEX / 2) + j * TILESIZEX;
				float fY = i * (TILESIZEY / 2.f);

				pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
				pTile->vSize = D3DXVECTOR3(float(TILESIZEX), float(TILESIZEY), 0.f);
				pTile->byOption = 1;
				pTile->byDrawID = 1;

				m_vecTile.push_back(pTile);
			}
		}

		break;

	case 2:
		iTileCntX = BOSS_TILECNTX;
		iTileCntY = BOSS_TILECNTY;
		CTextureMgr::GetInstance()->InsertTexture(L"../Texture/Map/BossStageEdit.png", L"BossStage");

		for(int i = 0; i < iTileCntY; ++i)
		{
			for(int j = 0; j < iTileCntX; ++j)
			{
				TILE* pTile = new TILE;

				float fX = (float)((i % 2) * TILESIZEX / 2) + j * TILESIZEX;
				float fY = i * (TILESIZEY / 2.f);

				pTile->vPos = D3DXVECTOR3(fX, fY, 0.f);
				pTile->vSize = D3DXVECTOR3(float(TILESIZEX), float(TILESIZEY), 0.f);
				pTile->byOption = 0;
				pTile->byDrawID = 0;

				m_vecTile.push_back(pTile);
			}
		}
		break;
	}

	


}

int	CBackGround::Progress(void)
{
	return 0;
}

void CBackGround::Render(void)
{
	if(m_iType == 0)
	{
		D3DXMATRIX	matTrans;
		D3DXMatrixIdentity(&matTrans);

		const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stage1");

		D3DXMatrixTranslation(&matTrans, float(0 - m_pView->GetScrollPos(0)), float(0 - m_pView->GetScrollPos(1)), 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(0, 0, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMatrixIdentity(&matTrans);

		if(m_bShow == false)
			return;

		int iStartX = m_pView->GetScrollPos(0) / TILESIZEX;
		int iStartY = m_pView->GetScrollPos(1) / (TILESIZEY / 2);

		int iEndX = (iStartX + WINCX / TILESIZEX) + 2;
		int iEndY = iStartY + WINCY / (TILESIZEY / 2);

		for(int i = iStartY; i < iEndY; ++i)
		{
			for(int j = iStartX; j < iEndX; ++j)
			{
				int iIndex = i * TILECNTX + j;

				if(iIndex < 0 || iIndex >= TILECNTX * TILECNTY)
					continue;

				const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x - m_pView->GetScrollPos(0), m_vecTile[iIndex]->vPos.y - m_pView->GetScrollPos(1), 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(TILESIZEX / 2.f, TILESIZEY / 2.f, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

				if(m_bTileNum)
				{
					TCHAR	szBuf[MAX_PATH] = L"";

					wsprintf(szBuf, L"%d", iIndex);

					CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				}
			}
		}

		if(m_vecObj.empty())
		{
			return;
		}

		int iObjectAlpha = 0;

		if(m_bObjAlpha == true)
		{
			iObjectAlpha = 120;
		}
		else
		{
			iObjectAlpha = 255;
		}

		multimap<float, OBJECT*> SortMap;

		for(size_t i = 0; i < m_vecObj.size(); ++i)
		{
			//정렬		
			SortMap.insert( make_pair(m_vecObj[i]->vPos.y, m_vecObj[i]) );
		}

		multimap<float, OBJECT*>::iterator iter = SortMap.begin();
		multimap<float, OBJECT*>::iterator iter_end = SortMap.end();

		for(iter; iter != iter_end; ++iter)
		{
			OBJECT* pObj = iter->second;
			const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(pObj->szObjKey, pObj->szObjKey, pObj->iDrawID);
			D3DXMatrixTranslation(&matTrans, pObj->vPos.x - m_pView->GetScrollPos(0), pObj->vPos.y - m_pView->GetScrollPos(1), 0.f);
			CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(pObj->vCenter.x, pObj->vCenter.y, 0.f),NULL,D3DCOLOR_ARGB(iObjectAlpha, 255, 255, 255));
		}
	}
	else if(m_iType == 1)
	{
		D3DXMATRIX	matTrans;

		const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stage2");

		D3DXMatrixTranslation(&matTrans, float(0 - m_pView->GetScrollPos(0)), float(0 - m_pView->GetScrollPos(1)), 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(0, 0, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_bShow == false)
			return;

		int iStartX = m_pView->GetScrollPos(0) / TILESIZEX;
		int iStartY = m_pView->GetScrollPos(1) / (TILESIZEY / 2);

		int iEndX = (iStartX + WINCX / TILESIZEX) + 2;
		int iEndY = iStartY + WINCY / (TILESIZEY / 2);

		for(int i = iStartY; i < iEndY; ++i)
		{
			for(int j = iStartX; j < iEndX; ++j)
			{
				int iIndex = i * STAGE2_TILECNTX + j;

				if(iIndex < 0 || iIndex >= STAGE2_TILECNTX * STAGE2_TILECNTY)
					continue;

				const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x - m_pView->GetScrollPos(0), m_vecTile[iIndex]->vPos.y - m_pView->GetScrollPos(1), 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(TILESIZEX / 2.f, TILESIZEY / 2.f, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

				if(m_bTileNum)
				{
					TCHAR	szBuf[MAX_PATH] = L"";

					wsprintf(szBuf, L"%d", iIndex);

					CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				}
			}
		}
	}
	else if(m_iType == 2)
	{
		D3DXMATRIX	matTrans;

		const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"BossStage");

		D3DXMatrixTranslation(&matTrans, float(0 - m_pView->GetScrollPos(0)), float(0 - m_pView->GetScrollPos(1)), 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(0, 0, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_bShow == false)
			return;

		int iStartX = m_pView->GetScrollPos(0) / TILESIZEX;
		int iStartY = m_pView->GetScrollPos(1) / (TILESIZEY / 2);

		int iEndX = (iStartX + WINCX / TILESIZEX) + 2;
		int iEndY = iStartY + WINCY / (TILESIZEY / 2);

		for(int i = iStartY; i < iEndY; ++i)
		{
			for(int j = iStartX; j < iEndX; ++j)
			{
				int iIndex = i * BOSS_TILECNTX + j;

				if(iIndex < 0 || iIndex >= BOSS_TILECNTX * BOSS_TILECNTY)
					continue;

				const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMatrixTranslation(&matTrans, m_vecTile[iIndex]->vPos.x - m_pView->GetScrollPos(0), m_vecTile[iIndex]->vPos.y - m_pView->GetScrollPos(1), 0.f);

				CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(TILESIZEX / 2.f, TILESIZEY / 2.f, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));
			
				if(m_bTileNum)
				{
					TCHAR	szBuf[MAX_PATH] = L"";

					wsprintf(szBuf, L"%d", iIndex);

					CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), szBuf, lstrlen(szBuf), NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

				}
			}
		}
	}
}

void CBackGround::Release(void)
{
	for(size_t i = 0; i < m_vecTile.size(); ++i)
	{
		delete m_vecTile[i];
	}

	m_vecTile.clear();
}

void CBackGround::MiniViewRender(void)
{
	if(m_iType == 0)
	{
		D3DXMATRIX	matTrans;
		D3DXMATRIX	matScale;
		D3DXMATRIX	matWorld;
		D3DXMatrixIdentity(&matTrans);
		D3DXMatrixIdentity(&matScale);
		D3DXMatrixIdentity(&matWorld);

		const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stage1");

		D3DXMatrixTranslation(&matTrans, 0, 0, 0.f);

		D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0);

		matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(0, 0, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));

		if(m_bShow == false)
			return;

		for(int i = 0; i < TILECNTY; ++i)
		{
			for(int j = 0; j < TILECNTX; ++j)
			{
				int iIndex = i * TILECNTX + j;

				const TEXINFO*	pTexture = 
					CTextureMgr::GetInstance()->GetTexture(
					L"Back", L"Tile", m_vecTile[iIndex]->byDrawID);

				D3DXMATRIX matWorld, matTrans, matScale;
				D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0);
				D3DXMatrixTranslation(&matTrans, 
					m_vecTile[iIndex]->vPos.x * 0.3f,
					m_vecTile[iIndex]->vPos.y * 0.3f, 0.f);

				matWorld = matScale * matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

				CDevice::GetInstance()->GetSprite()->
					Draw(pTexture->pTexture,
					NULL, &D3DXVECTOR3(TILESIZEX / 2.f, TILESIZEY / 2.f, 0.f), NULL,
					D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		if(m_vecObj.empty())
		{
			return;
		}

		multimap<float, OBJECT*> SortMap;

		for(size_t i = 0; i < m_vecObj.size(); ++i)
		{
			//정렬		
			SortMap.insert( make_pair(m_vecObj[i]->vPos.y, m_vecObj[i]) );
		}

		multimap<float, OBJECT*>::iterator iter = SortMap.begin();
		multimap<float, OBJECT*>::iterator iter_end = SortMap.end();

		for(iter; iter != iter_end; ++iter)
		{
			OBJECT* pObj = iter->second;
			const TEXINFO* pTexture = CTextureMgr::GetInstance()->GetTexture(pObj->szObjKey, pObj->szObjKey, pObj->iDrawID);

			D3DXMATRIX matWorld, matTrans, matScale;
			D3DXMatrixScaling(&matScale, 0.3f, 0.3f, 0);
			D3DXMatrixTranslation(&matTrans, pObj->vPos.x * 0.3f, pObj->vPos.y * 0.3f, 0.f);

			matWorld = matScale * matTrans;
			CDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture,NULL,&D3DXVECTOR3(pObj->vCenter.x, pObj->vCenter.y, 0.f),NULL,D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}