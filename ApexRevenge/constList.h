#pragma once
#include <math.h>

struct Matrix {
	float matrix[16];
};

struct Bone {
	char bone1[0xCC];
	float x;
	char bone2[0xC];
	float y;
	char bone3[0xC];
	float z;
};


static float glow_fade[] = { INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY, INFINITY };
static float glow_color_knock_down[] = { 10.0f , 10.0f ,0 };
static float glow_color_enemy[] = { 10.0f , 0 ,0 };
static float glow_color_allied[] = { 0 , 10.0f ,0 };
static float glow_color_enemy_healthlow[] = { 10.0f , 3.0f ,0 };
static float glow_color_aimbot[] = { 0 , 10.0f ,10.0f };


#define d3dcolor_ammo_light1 D3DCOLOR_ARGB(255, 247, 204, 167)
#define d3dcolor_ammo_energy1 D3DCOLOR_ARGB(255, 225, 237, 159)
#define d3dcolor_ammo_shotgun1 D3DCOLOR_ARGB(255, 253, 154, 134)
#define d3dcolor_ammo_heavy1 D3DCOLOR_ARGB(255, 182, 230, 211)
#define d3dcolor_ammo_sniper1 D3DCOLOR_ARGB(255, 191, 192, 253)

#define d3dcolor_rate_red D3DCOLOR_ARGB(255, 220,20,60)
#define d3dcolor_rate_gold D3DCOLOR_ARGB(255, 255, 215, 0)
#define d3dcolor_rate_purple D3DCOLOR_ARGB(255, 255, 0, 255)
#define d3dcolor_rate_blue D3DCOLOR_ARGB(255, 0,191,255)
#define d3dcolor_rate_white D3DCOLOR_ARGB(255, 232, 232, 232)


namespace HandleWeaponID{
enum  WeaponID{
	BARE_HANDS = 21,

	RE45 = 68,
	P2020 = 84,
	R301 = 0,
	R99 = 83,
	ALTERNATOR = 67,
	G7_SCOUT = 75,

	REPEATER = 89,
	WINGMAN = 87,
	SPITFIRE = 85,
	HEMLOCK = 76,
	FLATLINE = 74,

	SENTINEL = 1,
	CHARGE_RIFLE = 69,
	LONGBOW = 71,
	TRIPLE_TAKE = 86,

	VOLTSMG = 88,
	DEVOTION = 70,
	HAVOC = 72,
	LSTAR = 78,

	PEACEKEEPER = 82,
	PROWLER = 81,
	KRABER = 77,
	
	MOZAMBIQUE = 80,
	EVA8_AUTO = 73,
	MASTIFF = 79,
};

}



static int weapon_sniper_list[] = {
	HandleWeaponID::WeaponID::KRABER,
	HandleWeaponID::WeaponID::SENTINEL,
	HandleWeaponID::WeaponID::CHARGE_RIFLE,
	HandleWeaponID::WeaponID::LONGBOW,
	HandleWeaponID::WeaponID::TRIPLE_TAKE
};



const char ItemName[][16]{
	"",
	"���ױ���",
	"��Ȯ",
	"��Ȯ",
	"",
	"",
	"",
	"L-Star",
	"L-Star",
	"",
	"",
	"",
	"���ֿ�",
	"���ֿ�",
	"",
	"",
	"",
	"רע��ǹ",
	"רע��ǹ",
	"",
	"",
	"",
	"���ؾ�",
	"���ؾ�",
	"",
	"",
	"",
	"ƽ�в�ǹ",
	"ƽ�в�ǹ",
	"",
	"",
	"",
	"��ķ���",
	"��ķ���",
	"",
	"",
	"",
	"G7���ǹ",
	"G7���ǹ",
	"",
	"",
	"",
	"ת����",
	"ת����",
	"",
	"",
	"",
	"R99",
	"R99",
	"",
	"",
	"���޳��",
	"���޳��",
	"",
	"",
	"",
	"",
	"���ܳ��",
	"",
	"",
	"",
	"",
	"������",
	"������",
	"",
	"",
	"",
	"���ܲ�ǹ",
	"���ܲ�ǹ",
	"",
	"",
	"",
	"����ǹ",
	"����ǹ",
	"",
	"",
	"",
	"R301",
	"R301",
	"",
	"",
	"",
	"EVA-8",
	"EVA-8",
	"",
	"",
	"��ƽ����",
	"��ƽ����",
	"Īɣ�ȿ�",
	"Īɣ�ȿ�",
	"",
	"",
	"",
	"С����",
	"С����",
	"",
	"",
	"",
	"P2020",
	"P2020",
	"",
	"",
	"",
	"RE45",
	"RE45",
	"",
	"",
	"",
	"�ڱ���",
	"�ڱ���",
	"",
	"",
	"",

	"���͵�",
	"������",
	"����",
	"���͵�",
	"�ѻ���",

	"������",
	"��˰�",
	"������",
	"ע����",
	"����",
	"С���",

	"��ͷ��",
	"��ͷ��",
	"��ͷ��",
	"��ͷ��",
	"�׼�",
	"����",
	"�ϼ�",
	"���",
	"",
	"�����׼�",
	"��������",
	"�����ϼ�",
	"���",
	"�׶�",
	"����",
	"�϶�",
	"���",
	"�ױ���",
	"������",
	"�ϱ���",
	"�𱳰�",

	"���ȼ�",
	"��Ƭ��",
	"�绡��",

	"��������",
	"2����",
	"����Բ��",
	"1-2����",
	"������",
	"3����",
	"2-4����",
	"6����",
	"4-8����",
	"4-10��",

	"��ǹ��",
	"��ǹ��",
	"��ǹ��",
	"��ǹ��",
	"���ᵯϻ",
	"���ᵯϻ",
	"���ᵯϻ",
	"���ص�ϻ",
	"���ص�ϻ",
	"���ص�ϻ",
	"������ϻ",
	"������ϻ",
	"������ϻ",
	"�׾ѵ�ϻ",
	"���ѵ�ϻ",
	"�Ͼѵ�ϻ",
	"��ǹ˨",
	"��ǹ˨",
	"��ǹ˨",
	"��ǹ��",
	"��ǹ��",
	"��ǹ��",
	"�׾�ǹ��",
	"����ǹ��",
	"�Ͼ�ǹ��",

	"������ѹ",
	"��­��",
	"������",
	"˫�����",
	"",
	"���ǹ��",

	"�����ű�",
	"���տ�Կ��",
	"",
	"���ر�",

	""
};

enum ItemID{
	RED_KRABER = 1,
	MASTIFF = 2,
	GOLD_MASTIFF,
	LSTAR = 7,
	GOLD_LSTAR,
	HAVOC = 12,
	GOLD_HAVOC,
	DEVOTION = 17,
	GOLD_DEVOTION,
	TRIPLE_TAKE = 22,
	GOLD_TRIPLE_TAKE,
	FLATLINE = 27,
	GOLD_FLATLINE,
	HEMLOCK = 32,
	GOLD_HEMLOCK,
	G7_SCOUT = 37,
	GOLD_G7_SCOUT,
	ALTERNATOR = 42,
	GOLD_ALTERNATOR,
	R99 = 47,
	RED_R99,
	PROWLER = 51,
	RED_PROWLER,
	VOLT_SMG = 57,
	LONGBOW = 62,
	GOLD_LONGBOW,
	CHARGE_RIFLE = 67,
	GOLD_CHARGE_RIFLE,
	SPITFIRE = 72,
	GOLD_SPITFIRE,
	R301 = 77,
	GOLD_R301,
	EVA8_AUTO = 82,
	GOLD_EVA8_AUTO,
	PEACEKEEPER = 86,
	RED_PEACEKEEPER,
	MOZAMBIQUE = 88,
	GOLD_MOZAMBIQUE,
	WINGMAN = 93,
	GOLD_WINGMAN,
	P2020 = 98,
	GOLD_P2020,
	RE45 = 103,
	GOLD_RE45,
	SENTINEL = 108,
	GOLD_SENTINEL,

	LIGHT_ROUNDS = 113,
	ENERGY_AMMO,
	SHOTGUN_SHELLS,
	HEAVY_ROUNDS,
	SNIPER_AMMO,

	ULTIMATE_ACCELERANT,
	PHOENIX_KIT,
	MED_KIT,
	SYRINGE,
	SHIELD_BATTERY,
	SHIELD_CELL,

	HELMET_LV1,
	HELMET_LV2,
	HELMET_LV3,
	HELMET_LV4,
	BODY_ARMOR_LV1,
	BODY_ARMOR_LV2,
	BODY_ARMOR_LV3,
	BODY_ARMOR_LV4,
	EVO_SHIELD_LV1 = 133,
	EVO_SHIELD_LV2,
	EVO_SHIELD_LV3,
	EVO_SHIELD_LV4,
	KNOCKDOWN_SHIELD_LV1,
	KNOCKDOWN_SHIELD_LV2,
	KNOCKDOWN_SHIELD_LV3,
	KNOCKDOWN_SHIELD_LV4,
	BACKPACK_LV1,
	BACKPACK_LV2,
	BACKPACK_LV3,
	BACKPACK_LV4,

	THERMITE_GRENADE,
	FRAG_GRENADE,
	ARC_STAR,

	HCOG_CLASSIC,
	HCOG_BRUISER,
	HOLO,
	VARIABLE_HOLO,
	DIGITAL_THREAT,
	HCOG_RANGER,
	VARIABLE_AOG,
	SNIPER,
	VARIABLE_SNIPER,
	DIGITAL_SNIPER_THREAT,

	BARREL_STABILIZER_LV1,
	BARREL_STABILIZER_LV2,
	BARREL_STABILIZER_LV3,
	BARREL_STABILIZER_LV4,
	LIGHT_MAGAZINE_LV1,
	LIGHT_MAGAZINE_LV2,
	LIGHT_MAGAZINE_LV3,
	HEAVY_MAGAZINE_LV1,
	HEAVY_MAGAZINE_LV2,
	HEAVY_MAGAZINE_LV3,
	ENERGY_MAGAZING_LV1,
	ENERGY_MAGAZING_LV2,
	ENERGY_MAGAZING_LV3,
	SNIPER_MAGAZINE_LV1,
	SNIPER_MAGAZINE_LV2,
	SNIPER_MAGAZINE_LV3,
	SHOTGUN_BOLT_LV1,
	SHOTGUN_BOLT_LV2,
	SHOTGUN_BOLT_LV3,
	STANDARD_STOCK_LV1,
	STANDARD_STOCK_LV2,
	STANDARD_STOCK_LV3,
	SNIPER_STOCK_LV1,
	SNIPER_STOCK_LV2,
	SNIPER_STOCK_LV3,

	TURBOCHARGER,//������ѹ��
	SKULLPIERCER_RIFLING,//��­��
	HAMMERPOINT_ROUNDS,//�������ӵ�
	DOUBLE_TAP_TRIGGER,//˫�����
	QUICKDRAW_HOLSTER = 188,//���ǹ��
	
	//ANVIL_RECEIVER//���������
	//SELECTFIRE_RECEIVER,//���ģʽѡ����
	//PRECISION_CHOKE,//��׼������
	//GRAFFITI_MOD,//Ϳѻ��װ						

	MOBILE_RESPAWN_BEACON,
	VAULT_KEY,
	TREASURE_PACK = 192
};

const int itemColor_red[]{
	EVO_SHIELD_LV4,TREASURE_PACK,VAULT_KEY,

	RED_KRABER,RED_R99,RED_PEACEKEEPER,RED_PROWLER
};

const int itemColor_gold[] = {
	GOLD_MASTIFF,GOLD_LSTAR,GOLD_HAVOC,GOLD_TRIPLE_TAKE,
	GOLD_FLATLINE,GOLD_HEMLOCK,GOLD_G7_SCOUT,GOLD_ALTERNATOR,
	GOLD_LONGBOW,GOLD_CHARGE_RIFLE,GOLD_SPITFIRE,GOLD_R301,
	GOLD_EVA8_AUTO,GOLD_MOZAMBIQUE,GOLD_WINGMAN,GOLD_P2020,
	GOLD_RE45,GOLD_SENTINEL,GOLD_DEVOTION,
	
	BARREL_STABILIZER_LV4,
	TURBOCHARGER,SKULLPIERCER_RIFLING,

	HELMET_LV4,BODY_ARMOR_LV4,KNOCKDOWN_SHIELD_LV4,BACKPACK_LV4
};

const int itemColor_purple[]{
	PHOENIX_KIT,HELMET_LV3,BODY_ARMOR_LV3,EVO_SHIELD_LV3,KNOCKDOWN_SHIELD_LV3,
	BACKPACK_LV3,BARREL_STABILIZER_LV3,
	LIGHT_MAGAZINE_LV3,HEAVY_MAGAZINE_LV3,SNIPER_MAGAZINE_LV3,
	SHOTGUN_BOLT_LV3,STANDARD_STOCK_LV3,SNIPER_STOCK_LV3,
	ENERGY_MAGAZING_LV3,QUICKDRAW_HOLSTER,HAMMERPOINT_ROUNDS,DOUBLE_TAP_TRIGGER
};

const int itemColor_blue[]{
	ULTIMATE_ACCELERANT,MED_KIT,SHIELD_BATTERY,HELMET_LV2,BODY_ARMOR_LV2,EVO_SHIELD_LV2,KNOCKDOWN_SHIELD_LV2,
	BACKPACK_LV2,BARREL_STABILIZER_LV2,LIGHT_MAGAZINE_LV2,HEAVY_MAGAZINE_LV2,SNIPER_MAGAZINE_LV2,
	SHOTGUN_BOLT_LV2,STANDARD_STOCK_LV2,SNIPER_STOCK_LV2,ENERGY_MAGAZING_LV2
};

const int itemColor_light[]{
	G7_SCOUT,ALTERNATOR,R99,R301,RE45,
	P2020,LIGHT_ROUNDS
};

const int itemColor_heavy[]{
	FLATLINE,HEMLOCK,PROWLER,SPITFIRE,WINGMAN,
	HEAVY_ROUNDS
};

const int itemColor_shotgun[]{
	MASTIFF,EVA8_AUTO,MOZAMBIQUE,
	SHOTGUN_SHELLS
};

const int itemColor_energy[]{
	LSTAR,HAVOC,DEVOTION,VOLT_SMG,
	ENERGY_AMMO
};

const int itemColor_sniper[]{
	TRIPLE_TAKE,LONGBOW,CHARGE_RIFLE,SENTINEL,
	SNIPER_AMMO
};
