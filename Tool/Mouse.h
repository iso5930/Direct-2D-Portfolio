#pragma once

class CMouse
{
public:
	static D3DXVECTOR3	GetPos(void)
	{
		POINT	ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
	
		return D3DXVECTOR3((float)ptMouse.x, (float)ptMouse.y, 0.f);
	}
public:
	CMouse(void);
	~CMouse(void);
};
