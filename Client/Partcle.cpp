#include "StdAfx.h"
#include "Partcle.h"

CPartcle::CPartcle(void)
{

}

CPartcle::~CPartcle(void)
{
	Release();
}

HRESULT CPartcle::Initialize(void)
{
	return S_OK;
}

int	CPartcle::Progress(void)
{
	return 0;
}

void CPartcle::Render(void)
{

}

void CPartcle::Release(void)
{

}