#pragma once

#include "Texture.h"

class CSingleTexture : public CTexture
{
private:
	TEXINFO* m_pTexInfo;

public:
	virtual HRESULT LoadTexture(PTCHAR pPath, int iCount = 0); //그냥 텍스쳐 로딩할때 사용하는것..
	virtual const TEXINFO* GetTexture(int iCount = 0);
	virtual void Release(void);

public:
	CSingleTexture(void);
	virtual~CSingleTexture(void);
};