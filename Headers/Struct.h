#ifndef Struct_h__
#define Struct_h__

typedef struct tagTexture
{
	LPDIRECT3DTEXTURE9	pTexture;
	D3DXIMAGE_INFO		ImgInfo;
}TEXINFO;

typedef struct tagStrCompare
{
	template<typename T>
	bool operator()(T Data)
	{
		return !lstrcmp(Data.first, m_pString);
	}
	const TCHAR*	m_pString;
	tagStrCompare(const TCHAR* pKey)
		:m_pString(pKey){}
}STRCOMPARE;

typedef struct	tagTile
{
	D3DXVECTOR3	vPos; //위치
	D3DXVECTOR3	vSize; //크기
	BYTE		byOption; //충돌 판정 & 오브젝트의 중점을 관리 / 만약에 이게 오브젝트를 띄운다면 / 드로우 ID는 3번 으로 하고 
	BYTE		byDrawID; //그려질 타일 만약이게 3번이라면 이 타일과 연결되어있는 오브젝트의 이미지를 그 위에 올린다.
}TILE;

typedef struct tagObject
{
	D3DXVECTOR3 vPos; //이미지 가 그려질 위치
	D3DXVECTOR3 vSize; //그려질 사이즈
	D3DXVECTOR3 vCenter; //중점...
	TCHAR		szObjKey[50];
	int			iIndex; //연결되어있는 타일의 인덱스.
	int			iDrawID; //그려질 오브젝트 이미지.
}OBJECT;

typedef struct tagInfo
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vDir;
	D3DXMATRIX	matWorld;
	float		fAngle;
	float		fCX;
	float		fCY;
}Info;

typedef struct tagFrame
{
	int		iStartFrame;
	int		iEndFrame;
	int		iCur;
	DWORD	dwFrameTime;
	tagFrame(){}
	tagFrame(int StartFrame, int EndFrame, int Cur, DWORD FrameTime)
	{
		iStartFrame = StartFrame;
		iEndFrame = EndFrame;
		iCur = Cur;
		dwFrameTime = FrameTime;
	}
}Frame;

typedef struct tagAstarNode
{
	float			fCost;
	int				iIndex;
	tagAstarNode*	pParent;
}NODE;

typedef struct tagTrans
{
	DWORD				dwKeyState;
	D3DXVECTOR3			vPos;
	ePlayerState		PlayerType;
	eSkillType			SkillType;
	float				fStartCur;
	float				fEndCur;
	float				fLoopCur;
	vector<D3DXVECTOR3> vecNextPos;
	int					iPosIndex;
	
	tagTrans(){}
	tagTrans(DWORD _Key, D3DXVECTOR3 _Pos, float Start, float End, float Loop, ePlayerState State = PLAYER_END, eSkillType Type = SKILL_END)
	{
		dwKeyState = _Key;
		vPos = _Pos;
		fStartCur = Start; //시작위치
		fEndCur = End; //끝위치
		fLoopCur = Loop; //중간의 카운트 숫자.
		PlayerType = State;
		SkillType = Type;
	}
}TRANS;

typedef struct tagData
{
	int iMaxHp;
	int iHp;
	int iAtt;
}DATA;

typedef struct tagFocus
{	
	int		iCurTransIndex;
	float	fTransStart;
	float	fTransEnd;
	bool	bReload;
	bool	bReloadEnd;

	tagFocus(){}
	tagFocus(int iCur, float fStart, float fEnd, bool Reload, bool ReloadEnd)
	{
		iCurTransIndex = iCur;
		fTransStart = fStart;
		fTransEnd = fEnd;
		bReload = Reload;
		bReloadEnd = ReloadEnd;
	}
	
}FOCUS;



#endif // Struct_h__