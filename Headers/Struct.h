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
	D3DXVECTOR3	vPos; //��ġ
	D3DXVECTOR3	vSize; //ũ��
	BYTE		byOption; //�浹 ���� & ������Ʈ�� ������ ���� / ���࿡ �̰� ������Ʈ�� ���ٸ� / ��ο� ID�� 3�� ���� �ϰ� 
	BYTE		byDrawID; //�׷��� Ÿ�� �����̰� 3���̶�� �� Ÿ�ϰ� ����Ǿ��ִ� ������Ʈ�� �̹����� �� ���� �ø���.
}TILE;

typedef struct tagObject
{
	D3DXVECTOR3 vPos; //�̹��� �� �׷��� ��ġ
	D3DXVECTOR3 vSize; //�׷��� ������
	D3DXVECTOR3 vCenter; //����...
	TCHAR		szObjKey[50];
	int			iIndex; //����Ǿ��ִ� Ÿ���� �ε���.
	int			iDrawID; //�׷��� ������Ʈ �̹���.
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
		fStartCur = Start; //������ġ
		fEndCur = End; //����ġ
		fLoopCur = Loop; //�߰��� ī��Ʈ ����.
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