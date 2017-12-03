#include "StdAfx.h"
#include "BossStage.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "DotMgr.h"
#include "TextureMgr.h"
#include "Factory.h"
#include "Boss.h"
#include "Cube.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

CBossStage::CBossStage(void)
{

}

CBossStage::~CBossStage(void)
{
	Release();
}

HRESULT CBossStage::Initialize(void)
{
	((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->SetTile(3);
	CDotMgr::GetInstance()->SetDotTile(3);

	CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->SetPos(D3DXVECTOR3(WINCX / 2.f, WINCY / 2.f, 0.f));

	//보스 생성..

	CObj* pObj = CFactory<CBoss>::CreateObj(D3DXVECTOR3(1040.f, 120.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);

	pObj = CFactory<CCube>::CreateObj(1273);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1307);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1308);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1342);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1343);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1344);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1376);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1377);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1378);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1379);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1411);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1412);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1414);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1415);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1445);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1446);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1449);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1450);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1481);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1485);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1386);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1352);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1317);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1283);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1248);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1284);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1319);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1355);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1390);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1426);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1461);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1421);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	CSoundMgr::GetInstance()->PlayBGM(L"Impossible.wav");

	return S_OK;
}

int CBossStage::UpDate(void)
{
	CObjMgr::GetInstance()->Progress();

	bool bBossDeath = ((CBoss*)CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].front())->GetDeath();

	if(bBossDeath && CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].size() == 1)
	{
		D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

		int iIndex = GetTileIndex(vPos);

		if(iIndex == 611 || iIndex == 612)
		{
			CSceneMgr::GetInstance()->SetScene(SCENE_ENDING);
		}
	}

	return 0;
}

void CBossStage::Render(void)
{
	CObjMgr::GetInstance()->Render();
}

void CBossStage::Release(void)
{
	
}

int CBossStage::GetTileIndex(D3DXVECTOR3 vPos)
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

		return iIndexY * BOSS_TILECNTX + iIndexX;
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;

		return iIndexY2 * BOSS_TILECNTX + iIndexX2;
	}	
}