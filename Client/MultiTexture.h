#pragma once

#include "Texture.h"

class CMultiTexture : public CTexture
{
private:
	vector<TEXINFO*> m_vecTexInfo;

public:
	virtual HRESULT LoadTexture(PTCHAR pPath, int iCount = 0); //�׳� �ؽ��� �ε��Ҷ� ����ϴ°�..
	virtual const TEXINFO* GetTexture(int iCount = 0);
	virtual void Release(void);

public:
	CMultiTexture(void);
	virtual~CMultiTexture(void);
};