#include "StdAfx.h"
#include "Struct.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "Player.h"

CStruct::CStruct(void)
{
	m_pStruct = NULL;
}

CStruct::~CStruct(void)
{
	Release();
}

HRESULT CStruct::Initialize(void)
{
	m_tInfo.vPos = m_pStruct->vPos;

	return S_OK;
}

int	CStruct::Progress(void)
{
	return 0;
}

void CStruct::Render(void)
{
	CObj* pObj = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front();

	bool bTrans = ((CPlayer*)pObj)->GetTrans();

	if(bTrans)
	{
		return;
	}

	const TEXINFO* pTexInfo = CTextureMgr::GetInstance()->GetTexture(m_pStruct->szObjKey, m_pStruct->iDrawID);

	D3DXMATRIX matTrans;

	float fX = m_pStruct->vCenter.x;
	float fY = m_pStruct->vCenter.y;

	D3DXMatrixTranslation(&matTrans, m_pStruct->vPos.x - g_Scroll.x, m_pStruct->vPos.y - g_Scroll.y, 0.f);
	
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);

	CDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, NULL, &D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CStruct::Release(void)
{
	if(m_pStruct)
	{
		delete m_pStruct;
	}
}