#include "StdAfx.h"
#include "KeyMgr.h"

IMPLEMENT_SINGLETON(CKeyMgr);

CKeyMgr::CKeyMgr(void)
{
	m_dwKey = 0;
}

CKeyMgr::~CKeyMgr(void)
{

}

DWORD CKeyMgr::GetKeyState(void)
{
	return m_dwKey;
}

void CKeyMgr::SetKeyState(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState('1') & 0x8000)
	{
		m_dwKey |= VK_1;
	}
	if(GetAsyncKeyState('2') & 0x8000)
	{
		m_dwKey |= VK_2;
	}
	if(GetAsyncKeyState('3') & 0x8000)
	{
		m_dwKey |= VK_3;
	}
	if(GetAsyncKeyState('4') & 0x8000)
	{
		m_dwKey |= VK_4;
	}
}