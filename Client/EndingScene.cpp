#include "StdAfx.h"
#include "EndingScene.h"
#include "SoundMgr.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "Obj.h"

CEndingScene::CEndingScene(void)
{

}

CEndingScene::~CEndingScene(void)
{
	Release();
}

HRESULT CEndingScene::Initialize(void)
{
	CSoundMgr::GetInstance()->PlayBGM(L"Ending.mp3");

	CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->SetPos(D3DXVECTOR3(WINCX / 2, WINCY / 2, 0));

	return S_OK;
}

int CEndingScene::UpDate(void)
{
	return 0;
}

void CEndingScene::Render(void)
{
	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"Ending");

	D3DXMATRIX matTrans;

	D3DXMatrixTranslation(&matTrans, 0, 0, 0);

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(0, 0, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CEndingScene::Release(void)
{

}