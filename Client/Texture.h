#pragma once

class CTexture
{
public:
	virtual HRESULT LoadTexture(PTCHAR pPath, int iCount = 0)PURE; //그냥 텍스쳐 로딩할때 사용하는것..
	virtual const TEXINFO* GetTexture(int iCount = 0)PURE;
	virtual void Release(void)PURE;
public:
	CTexture(void);
	virtual~CTexture(void);
};