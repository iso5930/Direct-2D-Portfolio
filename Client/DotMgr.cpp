#include "StdAfx.h"
#include "DotMgr.h"
#include "Obj.h"
#include "Player.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "Factory.h"
#include "Dot.h"
#include "TextureMgr.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDotMgr)

CDotMgr::CDotMgr(void)
{

}

CDotMgr::~CDotMgr(void)
{
	Release();
}

HRESULT CDotMgr::Initialize(void)
{	
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

	vector<TILE*>* vecTile = ((CBackGround*)pObj)->GetTile();
	
	for(int i = 0; i < TILECNTY; ++i)
	{
		for(int j = 0; j < TILECNTX; ++j)
		{
			int iIndex = i * TILECNTX + j;

			if(iIndex < 0 || iIndex >= TILECNTX * TILECNTY)
				continue;

			D3DXVECTOR3 vPos = (*vecTile)[iIndex]->vPos;
			int			DrawID = (*vecTile)[iIndex]->byDrawID;
			CObj* pObj = CFactory<CDot>::CreateObj(vPos, DrawID);
			m_vecDot.push_back(pObj);
			//처음부터 끝까지 복사..
		}
	}

	m_iType = 1; // 1stage;

	return S_OK;
}

int CDotMgr::Progress(void)
{
	vector<CObj*>::iterator iter = m_vecDot.begin();
	vector<CObj*>::iterator iter_end = m_vecDot.end();

	for(iter; iter != iter_end; ++iter)
	{
		(*iter)->Progress();
	}

	return 0;
}

void CDotMgr::Render(void)
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"TransBack");

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0.f, 0.f, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));


	int iTileCntX = 0;
	int iTileCntY = 0;

	switch(m_iType)
	{
	case 1:
		iTileCntX = TILECNTX;
		iTileCntY = TILECNTY;
		
		break;

	case 2:
		iTileCntX = STAGE2_TILECNTX;
		iTileCntY = STAGE2_TILECNTY;
		break;


	case 3:
		iTileCntX = BOSS_TILECNTX;
		iTileCntY = BOSS_TILECNTY;

		break;
	}

	int iStartX = int(g_Scroll.x / TILESIZEX);
	int iStartY = int(g_Scroll.y / (TILESIZEY / 2));

	int iEndX = (iStartX + WINCX / TILESIZEX) + 2;
	int iEndY = iStartY + WINCY / (TILESIZEY / 2);

	for(int i = iStartY; i < iEndY; ++i)
	{
		for(int j = iStartX; j < iEndX; ++j)
		{
			int iIndex = i * iTileCntX + j;

			if(iIndex < 0 || iIndex >= iTileCntX * iTileCntY)
				continue;

			m_vecDot[iIndex]->Render();
		}
	}
}

void CDotMgr::Release(void)
{
	vector<CObj*>::iterator iter = m_vecDot.begin();
	vector<CObj*>::iterator iter_end = m_vecDot.end();

	for(iter; iter != iter_end; ++iter)
	{
		delete *iter;
	}
	m_vecDot.clear();
}

void CDotMgr::ResetPos(void)
{
	vector<CObj*>::iterator iter = m_vecDot.begin();
	vector<CObj*>::iterator iter_end = m_vecDot.end();

	for(iter; iter != iter_end; ++iter)
	{
		((CDot*)*iter)->ResetPos();
	}
}

void CDotMgr::SetDotTile(int iType)
{
	Release(); //우선 지워주고.

	int iTileCntX;
	int iTileCntY;

	m_iType = iType;

	switch(iType)
	{
	case 1:
		iTileCntX = TILECNTX;
		iTileCntY = TILECNTY;
		break;

	case 2:
		iTileCntX = STAGE2_TILECNTX;
		iTileCntY = STAGE2_TILECNTY;
		break;

	case 3:
		iTileCntX = BOSS_TILECNTX;
		iTileCntY = BOSS_TILECNTY;
		break;
	}
	

	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front();

	vector<TILE*>* vecTile = ((CBackGround*)pObj)->GetTile();

	for(int i = 0; i < iTileCntY; ++i)
	{
		for(int j = 0; j < iTileCntX; ++j)
		{
			int iIndex = i * iTileCntX + j;

			if(iIndex < 0 || iIndex >= iTileCntX * iTileCntY)
				continue;

			D3DXVECTOR3 vPos = (*vecTile)[iIndex]->vPos;
			int			DrawID = (*vecTile)[iIndex]->byDrawID;
			CObj* pObj = CFactory<CDot>::CreateObj(vPos, DrawID);
			m_vecDot.push_back(pObj);
			//처음부터 끝까지 복사..
		}
	}
}