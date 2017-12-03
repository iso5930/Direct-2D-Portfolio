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

//DirectX 를 사용하기 위해서 추가하거나 포함해야 하는 파일들!!!
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

//PathRelativePathTo 함수를 사용하기 위해서 추가/포함해야 하는 파일들..
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

extern	HWND	g_hWnd;
extern  D3DXVECTOR3 g_Scroll;
extern	bool	g_bStore;
extern	bool	g_bCreate;

//이미지 클래스를 사용하기 위해서 포함...
#include <atlimage.h>

//_finddata_t 사용하기 위해서 포함...
#include <io.h>

#include "Define.h"
#include "Constant.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"

//#include <timer.hpp>
//using namespace boost;


#endif // Include_h__