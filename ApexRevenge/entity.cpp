#include "entity.h"


void Entity_player::GetEntity(SOCKET gameSocket, DWORD64 entityAddress) {
	driver::read_memory(gameSocket,g_pid, (uintptr_t)entityAddress, (uintptr_t)EntityBufer,sizeof(EntityBufer));
}

void Entity_player::GetEntity(SOCKET gameSocket, int id){
	DWORD64 entityAddress = g_add_entitylist + id * 32;
	entityAddress = driver::read<uint64_t>(gameSocket,g_pid, entityAddress);
	driver::read_memory(gameSocket, g_pid, (uintptr_t)entityAddress, (uintptr_t)EntityBufer, sizeof(EntityBufer));
}

void Entity_player::GetEntity(SOCKET gameSocket) {
	driver::read_memory(gameSocket, g_pid, (uintptr_t)Address, (uintptr_t)EntityBufer, sizeof(EntityBufer));
}

void Entity_player::GetDetail(SOCKET gameSocket) {
	DWORD64 entityNameBase = Address + OFFSET_SIG_NAME;
	DWORD64 entityNameAdd = driver::read<DWORD64>(gameSocket, g_pid, entityNameBase);
	memset(EntityTxt, 0, 16);
	driver::read_memory(gameSocket, g_pid, entityNameAdd, (uintptr_t)EntityTxt, 16);

	//player
	if (EntityTxt[0] == 'p' && EntityTxt[5] == 'r' && EntityTxt[6] == '\0') {
		isPlayer = true;
	}
	else {
		isPlayer = false;
	}

	if (!isPlayer)return;
	//Id = *(int*)(EntityBufer + 0x38);
	Team = *(int*)(EntityBufer + OFFSET_TEAM);
	Health = *(int*)(EntityBufer + OFFSET_HEALTH);
	Shield = *(int*)(EntityBufer + OFFSET_SHIELD);
	MaxShield = *(int*)(EntityBufer + OFFSET_MAX_SHIELD);
	//kills = *(int*)(EntityBufer + OFFSET_KILLCOUNT);
	Dead = *(int*)(EntityBufer + OFFSET_LIFE_STATE);
	Knocked = *(int*)(EntityBufer + OFFSET_BLEED_OUT_STATE);
	Position = *(Vector*)(EntityBufer + OFFSET_ORIGIN);

	float tmpvisibletime = *(float*)(EntityBufer + LAST_VISIBLE_TIME);
	if (tmpvisibletime > lastvisibletime || tmpvisibletime < 0.f && lastvisibletime >0.f) {
		visible = true;
	}
	else {
		visible = false;
	}
	lastvisibletime = tmpvisibletime;

	isZoom = *(int*)(EntityBufer + OFFSET_ZOOMING);
	isZoom = isZoom & 0xff;
	isZoom = isZoom == 0 ? 0:1;
	
}

void Entity_player::GetDetailEx(SOCKET gameSocket) {

	uintptr_t boneArray = *(uintptr_t*)(EntityBufer + OFFSET_BONES) + (8 * 0x30);

	struct Bone entityBone = {};
	entityBone = driver::read<struct Bone>(gameSocket, g_pid, boneArray);
	HeadPosition.x = Position.x + entityBone.x;
	HeadPosition.y = Position.y + entityBone.y;
	HeadPosition.z = Position.z + entityBone.z;

	Vector ov = Vector();
	WorldToScreen(Position, ViewMartix.matrix, g_width, g_height, ov);
	if (ov.x > 0 && ov.y > 0) {
		sc_x = ov.x;
		sc_y = ov.y;
	}
	else {
		sc_x = 0;
		sc_y = 0;
	}

	ov = Vector();
	WorldToScreen(HeadPosition, ViewMartix.matrix, g_width, g_height, ov);
	if (ov.x > 0 && ov.y > 0) {
		sc_head_x = ov.x;
		sc_head_y = ov.y;
	}
	else {
		sc_head_x = 0;
		sc_head_y = 0;
	}

	CurrentWeaponId = GetWeaponInfo(gameSocket, OFFSET_LAST_WEAPON, true);
	observingId = *(int*)(EntityBufer + OFFSET_OBSERVING_TARGET);
	observingId &= 4095;

	if (Id) {
		memset(name, 0, 32);
		uint64_t nameAddress = g_add_nameList + ((Id - 1) << 4);
		nameAddress = driver::read<uint64_t>(gameSocket, g_pid, nameAddress);
		driver::read_memory(gameSocket, g_pid, nameAddress, (uintptr_t)name, sizeof(name) - 1);
		utf8_to_ansi(name);
	}
}

void Entity_player::GetLocalEntityExtraInfo(SOCKET gameSocket) {
	//WeaponId1 = GetWeaponInfo(gameSocket, OFFSET_WEAPON1,false);
	//WeaponId1 = GetWeaponInfo(gameSocket, OFFSET_WEAPON2, false);
	//armour_type = driver::read<int>(gameSocket, g_pid, OFFSET_ARMOUR_TYPE);
	//helmet_type = driver::read<int>(gameSocket, g_pid, OFFSET_HELMET_TYPE);
	CameraPos = *(Vector*)(EntityBufer + OFFSET_CAMERAPOS);
	ViewAngles = *(Vector2D*)(EntityBufer + OFFSET_VIEWANGLES);
	//DynamicViewAngles = *(Vector2D*)(EntityBufer + OFFSET_DYNAMIC_VIEWANGLES);
}

void Entity_player::GetDistanceToPlayer(Entity_player *player) {
	distanceToPlayer = Position.DistTo(player->Position);
}

int Entity_player::GetWeaponInfo(SOCKET gameSocket, int offset,bool handle) {
	DWORD entity_weapon_handle = *(DWORD*)(EntityBufer + offset);
	int weapon_entity_Id = entity_weapon_handle & 0xFFFF;
	if (weapon_entity_Id == 65535)return 0;
	DWORD64 weapon_entity_address = g_add_entitylist + weapon_entity_Id * 32;
	weapon_entity_address = driver::read<uint64_t>(gameSocket, g_pid, weapon_entity_address);
	int weaponNameId = driver::read<int>(gameSocket, g_pid, weapon_entity_address + OFFSET_WEAPON_NAME_IDX);
	if (handle) {
		CurrentBulletSpeed = driver::read<int>(gameSocket, g_pid, weapon_entity_address + OFFSET_BULLET_SPEED);
		CurrentBulletGravity = driver::read<int>(gameSocket, g_pid, weapon_entity_address + OFFSET_BULLET_GRAVITY);
		CurrentBulletGravity *= 750.0f;
	}

	return weaponNameId;
}

void Entity_player::Clean() {
	memset(EntityBufer, 0, 0x3FF0);
	isPlayer = false;
	memset(name, 0, sizeof(name));
	sc_x = sc_y = 0;
	sc_head_x = sc_head_y = 0;

}

void Entity_player::EnableGlow(SOCKET gameSocket, float* color) {
	driver::write<int>(gameSocket, g_pid, Address + OFFSET_GLOW_ENABLE, 1);
	driver::write<int>(gameSocket, g_pid, Address + OFFSET_GLOW_CONTEXT, 1);
	driver::write_memory(gameSocket, g_pid, Address + OFFSET_GLOW_FADE, (uintptr_t)glow_fade, sizeof(glow_fade));
	driver::write_memory(gameSocket, g_pid, Address + OFFSET_GLOW_COLORS, (uintptr_t)color, sizeof(float)*3);
}

void Entity_Item::GetEntity(SOCKET gameSocket) {
	
	itemId = driver::read<int>(gameSocket, g_pid, Address + OFFSET_ITEM_ID);
	if (itemId <= 0 || itemId > 132) {
		itemId = -1;
		return;
	}


	DWORD64 entityNameBase = Address + OFFSET_SIG_NAME;
	DWORD64 entityNameAdd = driver::read<DWORD64>(gameSocket, g_pid, entityNameBase);
	memset(EntityTxt, 0, 16);
	driver::read_memory(gameSocket, g_pid, entityNameAdd, (uintptr_t)EntityTxt, 16);

	//strcmp(EntityTxt, "prop_survival") != 0
	if (EntityTxt[0] != 'p' || EntityTxt[5] != 's' || EntityTxt[12] != 'l') {
		itemId = -1;
		return;
	}
	
	Position = driver::read<Vector>(gameSocket, g_pid, Address + OFFSET_ORIGIN);
}

void Entity_Item::EnableGlow(SOCKET gameSocket) {
	//driver::write<int>(gameSocket, g_pid, Address + OFFSET_GLOW_MAGIC, 0x4D407D7E);
	//driver::write_memory(gameSocket, g_pid, Address + OFFSET_GLOW_FADE, (uintptr_t)glow_fade, sizeof(glow_fade));
}

