#ifndef Factory_h__
#define Factory_h__
#include "Skill.h"
#include "MonsterBullet.h"

template<typename T>
class CFactory abstract		//추상 클래스로 만들어준다.
{
public:
	static CObj*	CreateObj(void)
	{
		CObj* pObj = new T;
		pObj->Initialize();
		return pObj;
	}

	static CObj*	CreateObj(int Index)
	{
		CObj* pObj = new T;
		((CCube*)pObj)->SetTileIndex(Index);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, int	DrawID)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		((CDot*)pObj)->SetDrawID(DrawID);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, bool True)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, eSkillEffect _Type)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		((CSkillEffect*)pObj)->SetType(_Type);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, float _Length, eSkillType _Type)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		((CSkill*)pObj)->SetType(_Type);
		((CSkill*)pObj)->SetLength(_Length);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, D3DXVECTOR3 _Dir, float _Angle, eMonsterBulletType _Type)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		pObj->SetDir(_Dir);
		pObj->SetAngle(_Angle);
		((CMonsterBullet*)pObj)->SetType(_Type);
		pObj->Initialize();

		return pObj;
	}

	static CObj*	CreateObj(D3DXVECTOR3 _Pos, D3DXVECTOR3 _Dir, float _Angle, eSkillType _Type, float _Length = 0)
	{
		CObj* pObj = new T;
		pObj->SetPos(_Pos);
		pObj->SetDir(_Dir);
		pObj->SetAngle(_Angle);
		((CSkill*)pObj)->SetType(_Type);
		((CSkill*)pObj)->SetLength(_Length);
		pObj->Initialize();

		return pObj;
	}
};

#endif //Factory_h__