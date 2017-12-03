#pragma once

class CTexture
{
public:
	virtual HRESULT LoadTexture(PTCHAR pPath, int iCount = 0)PURE; //�׳� �ؽ��� �ε��Ҷ� ����ϴ°�..
	virtual const TEXINFO* GetTexture(int iCount = 0)PURE;
	virtual void Release(void)PURE;
public:
	CTexture(void);
	virtual~CTexture(void);
};