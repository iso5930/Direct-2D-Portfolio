#ifndef Include_h__
#define Include_h__

#include <iostream>
using namespace std;

#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <time.h>
#include <string>

//DirectX �� ����ϱ� ���ؼ� �߰��ϰų� �����ؾ� �ϴ� ���ϵ�!!!
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//PathRelativePathTo �Լ��� ����ϱ� ���ؼ� �߰�/�����ؾ� �ϴ� ���ϵ�..
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

extern	HWND	g_hWnd;
extern  D3DXVECTOR3 g_Scroll;
extern	bool	g_bStore;
extern	bool	g_bCreate;

//�̹��� Ŭ������ ����ϱ� ���ؼ� ����...
#include <atlimage.h>

//_finddata_t ����ϱ� ���ؼ� ����...
#include <io.h>

#include "Define.h"
#include "Constant.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"

//#include <timer.hpp>
//using namespace boost;


#endif // Include_h__