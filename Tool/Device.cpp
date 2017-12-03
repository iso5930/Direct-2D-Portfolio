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

	D3DCAPS9 DeviceCaps; //��ġ�� ������ �����ϰ� �����ϴ� ����..

	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if(FAILED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		//�����Ұ�� ����.
		ERR_MSG(L"����̽� ���� !!");
		return E_FAIL;
	}

	DWORD vp;

	if(DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}
	//������ ���� ���ѿ����� ��ġ�� �ϴ��� cpu�� �ϴ��ĸ� ����

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	SetParameters(d3dpp);

	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pd3dDevice)))
	{
		ERR_MSG(L"����̽� �ʱ�ȭ ����!!");
		return E_FAIL;
	}

	if(FAILED(D3DXCreateSprite(m_pd3dDevice, &m_pSprite)))
	{
		ERR_MSG(L"��������Ʈ ���� ����!!");
		return E_FAIL;
	}

	D3DXFONT_DESCW FontInfo;
	ZeroMemory(&FontInfo, sizeof(D3DXFONT_DESCW));

	FontInfo.Height = 10;
	FontInfo.Width = 5;

	if(FAILED(D3DXCreateFontIndirect(m_pd3dDevice, &FontInfo, &m_pFont)))
	{
		ERR_MSG(L"��Ʈ ���� ����");
		return E_FAIL;
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
	d3dpp.Windowed = TRUE; //â���� ��üȭ�� ���.

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

void CDevice::Render_Begin(void)
{
	m_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 0, 0, 255), 1.f, 0);

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

void CDevice::Render_End(HWND _hWnd)
{
	m_pSprite->End();
	m_pd3dDevice->EndScene();

	m_pd3dDevice->Present(NULL, NULL, _hWnd, NULL);
}

LPD3DXFONT CDevice::GetFont(void)
{
	return m_pFont;
}