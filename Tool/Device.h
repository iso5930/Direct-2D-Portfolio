#pragma once

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	LPD3DXSPRITE		m_pSprite;
	LPD3DXFONT			m_pFont;

public:
	LPDIRECT3DDEVICE9		GetDevice(void);
	LPD3DXSPRITE			GetSprite(void);

public:
	HRESULT InitD3D(void);
	void	SetParameters(D3DPRESENT_PARAMETERS& d3dpp);
	void	Render_Begin(void);
	void	Render_End(void);
	void	Release(void);

public:
	void Render_End(HWND _hWnd);
	LPD3DXFONT GetFont(void);

private:
	CDevice(void);
	~CDevice(void);
};