#include "StdAfx.h"
#include "Dot.h"
#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"
#include "TimeMgr.h"

CDot::CDot(void)
{

}

CDot::~CDot(void)
{
	Release();
}

HRESULT CDot::Initialize(void)
{
	m_OriginPos = m_tInfo.vPos;

	m_pTexInfo = CTextureMgr::GetInstance()->GetTexture(L"TransDot");

	return S_OK;
}

int CDot::Progress(void)
{
	D3DXVECTOR3 vCur = CObjMgr::GetInstance()->GetList()[OBJ_PLAYER].front()->GetInfo()->vPos;

	D3DXVECTOR3 vDist = vCur - m_tInfo.vPos;

	D3DXVec3Normalize(&vDist, &vDist);

	m_tInfo.vPos += vDist * 1000 * GETTIME;

	return 0;
}

void CDot::Render(void)
{
	if(m_iDrawID != 0)
	{
		return;
	}

	D3DXVECTOR3 vPos = m_tInfo.vPos - g_Scroll;

	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;

	D3DXMatrixScaling(&matScale, 0.7f, 0.7f, 0.f);
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(m_pTexInfo->pTexture, NULL, &D3DXVECTOR3(m_pTexInfo->ImgInfo.Width / 2.f, m_pTexInfo->ImgInfo.Height / 2.f, 0.f), NULL, D3DCOLOR_ARGB(125, 255, 255, 255));
}

void CDot::Release(void)
{

}

void CDot::ResetPos(void)
{
	m_tInfo.vPos = m_OriginPos;
}