#pragma once
//Offsets //size 3FF0
#define OFFSET_ENTITYLIST			0x1911448
#define OFFSET_LOCAL_ENT			0x1cc0ce8
#define OFFSET_RENDER				0x41e3588
#define OFFSET_MATRIX				0x1B3BD0
#define OFFSET_PALYER_NAME_LIST		0x8382450

#define OFFSET_GAME_SCENE			0x119AD80

#define OFFSET_MOUSE_SENSITIVITY	0x1C62DE8
#define OFFSET_ADS_SENSITIVITY		(OFFSET_MOUSE_SENSITIVITY - 0xB430)


/*
uint64_t EntityList  // 48 8D 05 ? ? ? ? 44 2B
uint64_t ViewMatrix  //48 89 AB ? ? ? ? 4C 89 9B
uint64_t ViewRender  // 48 8B 0D ? ? ? ? 48 8D 15 ? ? ? ? 48 8B 01 48 3B C2 0F 85 ? ? ? ? 48 8B 81 ? ? ? ?
uint64_t LocalPlayer  //48 8B 05 ? ? ? ? 48 85 C0 74 07 44 88 B8 ? ? ? ?
uint64_t NameList  //48 63 43 38 48 8D 0D ? ? ? ? 48 03 C0

https://raw.fastgit.org/louis-dnb/ApexLegends/main/Offsets/Season%209/offsets30062021.md
*/

// 2285 峡谷 2355 世界尽头 17训练场 13大厅 2260 夜间王者峡谷 2281 闪点峡谷
//1377 奥林匹斯

//7FF71B830000

//7FF71BCD4F20

//Lmain	7FF71BC03DE0

#define OFFSET_TEAM					0x450
//m_iTeamNum
#define OFFSET_HEALTH				0x438 
//m_iHealth
#define OFFSET_NAME					0x589
//m_iName
#define OFFSET_SIG_NAME				0x580
//m_iSignifierName

#define OFFSET_SHIELD				0x170
//m_shieldHealth
#define OFFSET_MAX_SHIELD			0x174
//m_shieldHealthMax

#define OFFSET_LIFE_STATE			0x798  //>0 = dead
//m_lifeState
#define OFFSET_BLEED_OUT_STATE		0x26c8 //>0 = knocked
//m_bleedoutState

#define OFFSET_ITEM_ID				0x1698
//m_customScriptInt

#define OFFSET_ORIGIN				0x14C
//m_vecAbsOrigin
#define OFFSET_BONES				0xF38
//m_bConstrainBetweenEndpoints
#define OFFSET_AIMPUNCH				0x2430
//m_currentFrameLocalPlayer.m_vecPunchWeapon_Angle
#define OFFSET_CAMERAPOS			0x1E30
// Search CameraPos, first Result
#define OFFSET_VIEWANGLES			0x252c
//m_ammoPoolCapacity
#define OFFSET_DYNAMIC_VIEWANGLES   (OFFSET_VIEWANGLES - 0x10)

#define LAST_VISIBLE_TIME			0x1ACC

#define OFFSET_OBSERVER_MODE		0x338c 
//m_iObserverMode
#define OFFSET_OBSERVING_TARGET		0x3398 
//m_hObserverTarget

//#define OFFSET_HELMET_TYPE			0x41E4
//#define OFFSET_ARMOUR_TYPE			0x41E8


//#define OFFSET_WEAPON1				0x18F8
//#define OFFSET_WEAPON2				0x18FC
#define OFFSET_LAST_WEAPON			0x19ec
//m_latestPrimaryWeapons
#define OFFSET_WEAPON_AMMO_IN_CLIP	0x1644
//m_weaponNameIndex
#define OFFSET_WEAPON_NAME_IDX		0x17D0
//m_weaponNameIndex

#define OFFSET_BULLET_SPEED			0x1E50
#define OFFSET_BULLET_GRAVITY		OFFSET_BULLET_SPEED + 0x8

#define OFFSET_ZOOMING				0x1be1
//m_bZooming


/*
#define OFFSET_ENTITY_GLOW			0x278
#define OFFSET_GLOW_STYLE			OFFSET_ENTITY_GLOW + 0x4
#define OFFSET_GLOW_LIFETIME		OFFSET_ENTITY_GLOW + 0x70 //100000.f
#define OFFSET_GLOW_NEAR_RANGE		OFFSET_ENTITY_GLOW + 0x80
#define OFFSET_GLOW_FAR_RANGE		OFFSET_ENTITY_GLOW + 0x84
#define OFFSET_GLOW_CONTEXT			OFFSET_ENTITY_GLOW + 0x98 //1
#define OFFSET_GLOW_TEAM_MASK		OFFSET_ENTITY_GLOW + 0x9c //0xFFFFFFFF all of them
#define OFFSET_GLOW_FLAGE			OFFSET_ENTITY_GLOW + 0xA4 //32768
#define OFFSET_GLOW_OUTLINE			OFFSET_ENTITY_GLOW + 0x128 //1
*/


#define OFFSET_GLOW_ENABLE			0x3C8
#define OFFSET_GLOW_CONTEXT			0x3C8
#define OFFSET_GLOW_RANGE			0x2FC
#define OFFSET_GLOW_FADE			0x388
#define OFFSET_GLOW_COLORS			0x1D0


//#define OFFSET_GLOW_MAGIC			0x298
//#define OFFSET_GLOW_STYLE			0x29C
//
//#define OFFSET_ITEM_GLOW            0x2A8
//
//#define OFFSET_GLOW_ITEM_COLOR		0x1B8