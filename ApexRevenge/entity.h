#pragma once
#include "main.h"
#include <math.h>
#include <iostream>
#include <Windows.h>


class Entity_player {
	public:
		char EntityTxt[16];
		BYTE EntityBufer[0x3FF0];
		DWORD64 Address;
		int Id;

		bool isPlayer;
		int Team;
		int Health;
		int Shield;
		int MaxShield;
		int kills;
		int Dead;
		int Knocked;
		char name[32] = { 0 };
		Vector Position;
		Vector HeadPosition;
		Vector CameraPos;
		Vector2D ViewAngles;
		//Vector2D DynamicViewAngles;
		int CurrentWeaponId;
		//int WeaponId1;
	//	int WeaponId2;
		int isZoom;
		//int armour_type;
		//int helmet_type;
		float distanceToPlayer;
		float CurrentBulletSpeed;
		float CurrentBulletGravity;
		int observingId;
		int sc_x;
		int sc_y;
		int sc_head_x;
		int sc_head_y;
		float lastvisibletime;
		bool visible;

		void GetEntity(SOCKET gameSocket,int id);
		void GetEntity(SOCKET gameSocket,DWORD64 entityEntity);
		void GetEntity(SOCKET gameSocket);

		void GetDetail(SOCKET gameSocket);
		void Entity_player::GetDetailEx(SOCKET gameSocket);
		void Clean();

		void EnableGlow(SOCKET gameSocket,float* color);

		int GetWeaponInfo(SOCKET gameSocket,int offset,bool handle);

		void GetLocalEntityExtraInfo(SOCKET gameSocket);
		void GetDistanceToPlayer(Entity_player *player);
};

class Entity_Item {
	public:
		char EntityTxt[16];
		DWORD64 Address = 0;
		int itemId;
		Vector Position;
		

		void GetEntity(SOCKET gameSocket);
		void EnableGlow(SOCKET gameSocket);
		//void Clean();
};


class ESP_item {
public:
	int x;
	int y;
	int id;
	int distance;
	int count;
	int arrangement;
	Vector pos;
};
