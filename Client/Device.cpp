#include "StdAfx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice);

CDevice::CDevice(void)
:m_pD3D(NULL)
{

}

CDevice::~CDevice(void)
{
	Release();
}
LPDIRECT3DDEVICE9 CDevice::GetDevice(void)
{
	return m_pd3dDevice;
}
LPD3DXSPRITE CDevice::GetSprite(void)
{
	return m_pSprite;
}

HRESULT CDevice::InitD3D(void)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DCAPS9 DeviceCaps; //장치의 성능을 조사하고 저장하는 변수..

	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if(FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		//실패할경우 들어옴.
		ERR_MSG(L"디바이스 오류 !!");
		return E_FAIL;
	}

	DWORD vp;

	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	}
	//정점과 조명에 대한연산을 장치가 하느냐 cpu가 하느냐를 결정

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pd3dDevice)))
	{
		ERR_MSG(L"디바이스 초기화 실패!!");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pSprite)))
	{
		ERR_MSG(L"스프라이트 생성 실패!!");
		return E_FAIL;
	}

	AddFontResourceEx(L"../Resource/Font/N-Gage.ttf", FR_PRIVATE, 0);

	D3DXFONT_DESCW FontInfo;

	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.Height = 20;
	FontInfo.Width = 10;
	FontInfo.Weight = FW_BOLD;
	FontInfo.CharSet = DEFAULT_CHARSET;

	lstrcpy(FontInfo.FaceName, L"N-Gage");

	if(FAILED(D3DXCreateFontIndirect(m_pd3dDevice, &FontInfo, &m_pFont)))
	{
		int iA = 0;
	}

	AddFontResourceEx(L"../Resource/Font/YTTE08.ttf", FR_PRIVATE, 0);

	D3DXFONT_DESCW FontInfo2;

	ZeroMemory(&FontInfo2, sizeof(D3DXFONT_DESCW));

	FontInfo2.Height = 20;
	FontInfo2.Width = 10;
	FontInfo2.Weight = FW_NORMAL;
	FontInfo2.CharSet = DEFAULT_CHARSET;

	lstrcpy(FontInfo2.FaceName, L"양재튼튼체B");

	if(FAILED(D3DXCreateFontIndirect(m_pd3dDevice, &FontInfo2, &m_pFont2)))
	{
		ERR_MSG(L"Font2 생성 실패");
	}


	AddFontResourceEx(L"../Resource/Font/YTTE08.ttf", FR_PRIVATE, 0);

	D3DXFONT_DESCW FontInfo3;

	ZeroMemory(&FontInfo3, sizeof(D3DXFONT_DESCW));

	FontInfo3.Height = 11;
	FontInfo3.Width = 6;
	FontInfo3.Weight = FW_NORMAL;
	FontInfo3.CharSet = DEFAULT_CHARSET;

	lstrcpy(FontInfo3.FaceName, L"양재튼튼체B");

	if(FAILED(D3DXCreateFontIndirect(m_pd3dDevice, &FontInfo3, &m_pFont3)))
	{
		ERR_MSG(L"Font3 생성 실패");
	}

	return S_OK;

}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = g_hWnd;
	d3dpp.Windowed = TRUE; //창모드와 전체화면 모드.

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 0), 1.f, 0);

	m_pd3dDevice->BeginScene();

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CDevice::Render_End(void)
{
	m_pSprite->End();
	m_pd3dDevice->EndScene();
	m_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}

void CDevice::Release(void)
{
	m_pSprite->Release();
	m_pd3dDevice->Release();
	m_pD3D->Release();
}

LPD3DXFONT CDevice::GetFont(void)
{
	return m_pFont;
}
LPD3DXFONT CDevice::GetFont2(void)
{
	return m_pFont2;
}

LPD3DXFONT CDevice::GetFont3(void)
{
	return m_pFont3;
}

LPD3DXFONT CDevice::GetFont4(void)
{
	return m_pFont4;
}
