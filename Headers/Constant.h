#ifndef Constant_h__
#define Constant_h__

const int WINCX = 1280;
const int WINCY = 720;

const int TILESIZEX = 60;
const int TILESIZEY = 30;

//1Stage
const int TILECNTX = 72;
const int TILECNTY = 138;

//2Stage
const int STAGE2_TILECNTX = 37;
const int STAGE2_TILECNTY = 90;

//BossStage
const int BOSS_TILECNTX = 35;
const int BOSS_TILECNTY = 81;

const DWORD VK_1	= 0x00000001;
const DWORD VK_2	= 0x00000002;
const DWORD VK_3	= 0x00000004;
const DWORD VK_4	= 0x00000008;

const DWORD SKILL_1 = 0x00000001;
const DWORD SKILL_2 = 0x00000002;
const DWORD SKILL_3 = 0x00000004;
const DWORD SKILL_4 = 0x00000008;
const DWORD SKILL_5 = 0x00000010;

const float FOCUS_MAX	= 1100.f;
const float FOCUS_LOOP	= 5.f;
const float FOCUS_START = 17.f;
const float FOCUS_END	= 17.f;

const float FOCUS_SKILL_START	= 9.f;
const float FOCUS_SKILL_LOOP	= 14.f;
const float FOCUS_SKILL_END		= 8.f;

const float FOCUS_WALK_START	= 12.f;
const float FOCUS_WALK_LOOP		= 8.f;
const float FOCUS_WALK_END		= 12.f;

#endif // Constant_h__