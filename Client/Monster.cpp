#include "StdAfx.h"
#include "Monster.h"

CMonster::CMonster(void)
	:m_EggHeadFrame(0, 87, 0, 30), m_EggBodyFrame(0, 31, 0, 30)
{
	m_pAstar = NULL;

}

CMonster::~CMonster(void)
{

}