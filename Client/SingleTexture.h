#pragma once

#include "Texture.h"

class CSingleTexture : public CTexture
{
private:
	TEXINFO* m_pTexInfo;

public:
	virtual HRESULT LoadTexture(PTCHAR pPath, int iCount = 0); //�׳� �ؽ��� �ε��Ҷ� ����ϴ°�..
	virtual const TEXINFO* GetTexture(int iCount = 0);
	virtual void Release(void);

public:
	CSingleTexture(void);
	virtual~CSingleTexture(void);
};