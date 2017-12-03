#include "StdAfx.h"
#include "Stage2.h"
#include "ObjMgr.h"
#include "BackGround.h"
#include "DotMgr.h"
#include "Factory.h"
#include "Lady.h"
#include "Cube.h"
#include "SoundMgr.h"
#include "SceneMgr.h"

Stage2::Stage2(void)
{

}

Stage2::~Stage2(void)
{
	Release();
}

HRESULT Stage2::Initialize(void)
{
	((CBackGround*)CObjMgr::GetInstance()->GetList()[OBJ_BACKGROUND].front())->SetTile(2);
	CDotMgr::GetInstance()->SetDotTile(2);

	CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->SetPos(D3DXVECTOR3(1059.f, 1083.f, 0.f));

	CObj* pObj = CFactory<CLady>::CreateObj(D3DXVECTOR3(1343.f, 517.f, 0.f));
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_MONSTER);
	//레이디 생성..

	pObj = CFactory<CCube>::CreateObj(1167);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1205);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1242);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1280);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1317);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1355);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1392);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1430);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1466);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1503);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1539);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1576);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1612);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1649);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2092);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1458);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(1422);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1385);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1349);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1312);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1751);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);


	pObj = CFactory<CCube>::CreateObj(1789);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1752);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1715);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2128);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1495);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(2055);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2019);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1982);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2020);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2057);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2095);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2132);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);



	pObj = CFactory<CCube>::CreateObj(2100);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2062);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2025);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1987);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1950);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1912);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(2063);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	pObj = CFactory<CCube>::CreateObj(2027);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1990);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1954);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);
	pObj = CFactory<CCube>::CreateObj(1917);
	CObjMgr::GetInstance()->AddObject(pObj, OBJ_CUBE);

	//큐브 위치 정해주기...

	CSoundMgr::GetInstance()->PlayBGM(L"Vanishing Point.mp3");

	return S_OK;
}

int Stage2::UpDate(void)
{
	CObjMgr::GetInstance()->Progress();

	if(CObjMgr::GetInstance()->GetList()[OBJ_MONSTER].size() == 0)
	{
		D3DXVECTOR3 vPos = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

		int iIndex = GetTileIndex(vPos);

		if(iIndex == 1119 || iIndex == 1155 || iIndex == 1156 || iIndex == 1193 || iIndex == 1194 || iIndex == 1230)
		{
			CSceneMgr::GetInstance()->SetScene(SCENE_BOSS);
		}
	}

	return 0;
}

void Stage2::Render(void)
{
	CObjMgr::GetInstance()->Render();
}

void Stage2::Release(void)
{
	CObjMgr::GetInstance()->Release(OBJ_MONSTER);
	CObjMgr::GetInstance()->Release(OBJ_MONSTER_BULLET);
	CObjMgr::GetInstance()->Release(OBJ_CUBE);
	CObjMgr::GetInstance()->Release(OBJ_SKILL);
	CObjMgr::GetInstance()->Release(OBJ_SKILL_EFFECT);		
}

int Stage2::GetTileIndex(D3DXVECTOR3 vPos)
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

		return iIndexY * STAGE2_TILECNTX + iIndexX;
	}
	else
	{
		iIndexY2 = iIndexY2 * 2 + 1;

		return iIndexY2 * STAGE2_TILECNTX + iIndexX2;
	}	
}