// Apex-MemReadtest.cpp : 定义控制台应用程序的入口点。
//

#include "server_shared.h"
#include "driver.h"
#include "offsets.h"
#include "game_math.h"
#include "constList.h"

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>
#include <time.h>

using namespace std;

DWORD g_pid = 0;
HWND g_hwnd = 0;
DWORD64 g_baseAdd = 0 , g_entityAdd_list = 0 , g_entityPlayer =0;
SOCKET socket_glow,socket_lock;
int g_width, g_height;
float lock_distant_min = 140.0f * 40, lock_distant_max = 260.0f * 40;
int lockEnemyId = 0 , lock_mode = 0 , lock_nochange = 0;

struct Bone {
	BYTE shit[0xCC];
	float x;
	BYTE shit2[0xC];
	float y;
	BYTE shit3[0xC];
	float z;
};



DWORD GetProcessPid(wchar_t *name);

bool WorldToScreen(Vector from, float* m_vMatrix, int targetWidth, int targetHeight, Vector& to)
{
	float w = m_vMatrix[12] * from.x + m_vMatrix[13] * from.y + m_vMatrix[14] * from.z + m_vMatrix[15];

	if (w < 0.01f) return false;

	to.x = m_vMatrix[0] * from.x + m_vMatrix[1] * from.y + m_vMatrix[2] * from.z + m_vMatrix[3];
	to.y = m_vMatrix[4] * from.x + m_vMatrix[5] * from.y + m_vMatrix[6] * from.z + m_vMatrix[7];

	float invw = 1.0f / w;
	to.x *= invw;
	to.y *= invw;

	float x = targetWidth / 2;
	float y = targetHeight / 2;

	x += 0.5 * to.x * targetWidth + 0.5;
	y -= 0.5 * to.y * targetHeight + 0.5;

	to.x = x;
	to.y = y;
	to.z = 0;
	return true;
}

bool IsSniper(int weaponId) {
	int size = sizeof(weapon_sniper_list) / sizeof(int);
	for (int i = 0; i < size; i++) {
		if (weaponId == weapon_sniper_list[i]) {
			return true;
		}
	}
	return false;
}

int GetPlayerWeaponId(DWORD64 entity_address) {
	DWORD64 entity_weapon_handle = driver::read<uint64_t>(socket_glow, g_pid, entity_address + OFFSET_WEAPON);
	int weapon_entity_Id = entity_weapon_handle & 0xFFFF;

	DWORD64 weapon_entity_address = g_entityAdd_list + weapon_entity_Id * 32;

	weapon_entity_address = driver::read<uint64_t>(socket_glow, g_pid, weapon_entity_address);

	int weaponNameId = driver::read<int>(socket_glow, g_pid, weapon_entity_address + OFFSET_WEAPON_NAME_IDX);
	return weaponNameId;
}

bool CheckAimTargetVaild(int id) {
	if (id == 0) {
		return false;
	}
	DWORD64 entityEnemyAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityAdd_list + id * 32);
	DWORD64 entityPlayerAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityPlayer);
	if (entityEnemyAddress == 0 || entityEnemyAddress == entityPlayerAddress) {
		return false;
	}

	if (driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_LIFE_STATE) > 0)return false;
	if (driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_BLEED_OUT_STATE) > 0)return false;

	
	Vector playerPos = driver::read<Vector>(socket_glow, g_pid, entityPlayerAddress + OFFSET_ORIGIN);
	Vector EnemyPosition = driver::read<Vector>(socket_glow, g_pid, entityEnemyAddress + OFFSET_ORIGIN);
	float now_distant = playerPos.DistTo(EnemyPosition);

	int weapon = GetPlayerWeaponId(entityPlayerAddress);
	if (IsSniper(weapon)) {
		if (now_distant > lock_distant_max) {
			return false;
		}
	}
	else {
		if (now_distant > lock_distant_min) {
			cout << now_distant << endl;
			return false;
		}
	}
	return true;
}


QAngle CalculateBestBoneAim(DWORD64 player_address, DWORD64 enemy_address, float max_fov) {
	Vector EntityPosition = driver::read<Vector>(socket_lock, g_pid, enemy_address + OFFSET_ORIGIN);
	Vector LocalPlayerPosition = driver::read<Vector>(socket_lock, g_pid, player_address + OFFSET_ORIGIN);
	int weaponId = GetPlayerWeaponId( player_address );

	float dist = LocalPlayerPosition.DistTo(EntityPosition);

	int bone = 8;

	if (!IsSniper(weaponId)) {
		if (dist >= 4000) {
			bone = 5;
		}
		EntityPosition.z -= 2.0f;
	}

	max_fov -= ceil(dist / (2000.0f));

	Vector LocalCamera = driver::read<Vector>(socket_lock, g_pid, player_address + OFFSET_CAMERAPOS);


	Vector BonePosition = Vector();
	uintptr_t boneArray = driver::read<uintptr_t>(socket_lock, g_pid, enemy_address + OFFSET_BONES);
	uint32_t boneloc = (8 * 0x30);
	struct Bone entityBone = {};
	entityBone = driver::read<struct Bone>(socket_lock, g_pid, boneArray + boneloc);
	BonePosition.x = EntityPosition.x + entityBone.x;
	BonePosition.y = EntityPosition.y + entityBone.y;
	BonePosition.z = EntityPosition.z + entityBone.z;

	

	QAngle CalculatedAngles = Math::CalcAngle(LocalCamera, BonePosition);
	QAngle ViewAngles = driver::read<QAngle>(socket_lock, g_pid, player_address + OFFSET_VIEWANGLES);
	QAngle Delta = CalculatedAngles - ViewAngles;

	double fov = Math::GetFov(ViewAngles, CalculatedAngles);
	if (fov > max_fov) {
		return QAngle(0, 0, 0);
	}

	if (!IsSniper(weaponId)) {
		QAngle RecoilVec = driver::read<QAngle>(socket_lock, g_pid, player_address + OFFSET_AIMPUNCH);
		if (RecoilVec.x != 0 || RecoilVec.y != 0) {
			Delta -= RecoilVec;
		}
	}
	double smooth = 20.0f;
	//if (spectators > 0) {
	//smooth = 80.0f;
	//}

	Math::NormalizeAngles(Delta);
	if (fov > 2.0f) {
		if (Delta.x > 0.0f) {
			Delta.x /= smooth;
		}
		else {
			Delta.x = ((Delta.x * -1L) / smooth) * -1;
		}

		if (Delta.y > 0.0f) {
			Delta.y /= smooth;
		}
		else {
			Delta.y = ((Delta.y * -1L) / smooth) * -1;
		}
	}
	

	QAngle SmoothedAngles = ViewAngles + Delta;


	Math::NormalizeAngles(SmoothedAngles);

	return SmoothedAngles;
}


void WaitforGame() {
	while (g_pid == 0) {
		g_pid = GetProcessPid(L"r5apex.exe");
		//g_pid = GetProcessPid(L"EasyAntiCheat_launcher.exe");
		Sleep(1000);
	}
	cout << "已找到游戏进程！\n";
	cout << "等待游戏窗口..\n";
	while (!g_hwnd) {
		g_hwnd = FindWindowA("Respawn001", "Apex Legends");
	}
	cout << "游戏窗口已找到  窗口句柄：" << g_hwnd << endl;
	while (IsIconic(g_hwnd)) {
		Sleep(1000);
	}

	RECT Rect;
	GetClientRect(g_hwnd,&Rect);
	g_width = Rect.right - Rect.left;
	g_height = Rect.bottom - Rect.top;
	cout << "游戏分辨率 " << (int)g_width << "x" << (int)g_height << endl;
	g_baseAdd = driver::get_process_base_address(socket_glow,g_pid);
	if (g_baseAdd == 0) {
		cout << "获取游戏进程基地址失败！\n";
		return;
	}
	printf("获取游戏进程基地址成功\nBase Address :%p",(void*)g_baseAdd);
};


void gameGlowLoop() {
	struct Matrix {
		float matrix[16];
	};
	while (true) {
		
		DWORD64 entityPlayerAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityPlayer);
		//lockEnemyId = 0;
		if (entityPlayerAddress !=0) {
			while (true) {
				system("cls");
				DWORD64 entityBaseAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityAdd_list);
				entityPlayerAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityPlayer);
				//lockEnemyId = 0;
				if (entityBaseAddress == 0) {
					break;
				}

				Vector playerPos = driver::read<Vector>(socket_glow, g_pid, entityPlayerAddress + OFFSET_ORIGIN);
				if (playerPos.z > 21000)break;

				int player_team = driver::read<int>(socket_glow, g_pid, entityPlayerAddress + OFFSET_TEAM);
				if (player_team < 0 || player_team >30) {
					break;
				}

				int remain_enemy = 0;
				char EntityTxt[32] = { 0 };
				float closeDistant = 1000000.0f;
				float CrossHairCloseDistant = 1000000.0f;
				int tmp_targetId = 0;

				uint64_t viewRenderer = driver::read<uint64_t>(socket_lock, g_pid, g_baseAdd + OFFSET_RENDER);
				uint64_t viewMatrix = driver::read<uint64_t>(socket_lock, g_pid, viewRenderer + OFFSET_MATRIX);
				Matrix m = driver::read<Matrix>(socket_lock, g_pid, viewMatrix);

				if (!CheckAimTargetVaild(lockEnemyId)) {
					lockEnemyId = 0;
				}

				for (int i = 1; i <= 60; i++) {

					DWORD64 entityEnemyAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityAdd_list + i * 32);

					if (entityEnemyAddress == 0 || entityEnemyAddress == entityPlayerAddress) continue;

					if (driver::read<uint64_t>(socket_glow, g_pid, entityEnemyAddress) == 0) continue;

					if (driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_LIFE_STATE) > 0)continue;
					int entity_health = driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_HEALTH);
					if (entity_health == 0)continue;

					driver::read_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_NAME, (uintptr_t)EntityTxt, sizeof(EntityTxt) - 1);
					if (strcmp(EntityTxt, "player") != 0)continue;

					int enemy_team = driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_TEAM);

					driver::write<float>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_LIFETIME, 100000.0f);
					driver::write<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_FLAGE, 32768);
					driver::write<float>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_NEAR_RANGE, 0.0f);
					driver::write<float>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_FAR_RANGE, 1000000.0f);
					driver::write<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_OUTLINE, 1);
					driver::write<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_CONTEXT, 1);
					driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_FADE, (uintptr_t)glow_fade, sizeof(glow_fade));


					if (enemy_team != player_team) {
						//cout << "entity id:" << i << "\n";
						remain_enemy++;
						if (driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_BLEED_OUT_STATE) > 0) {
							driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_COLORS, (uintptr_t)glow_color_knock_down, sizeof(glow_color_knock_down));
						}
						else {
							driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_COLORS, (uintptr_t)glow_color_enemy, sizeof(glow_color_enemy));
							if (!lock_nochange) {
								Vector EnemyPosition = driver::read<Vector>(socket_lock, g_pid, entityEnemyAddress + OFFSET_ORIGIN);
								float now_distant = playerPos.DistTo(EnemyPosition);

								int lock_distant;
								int weaponId = GetPlayerWeaponId(entityPlayerAddress);
								if (IsSniper(weaponId)) {
									lock_distant = lock_distant_max;
								}
								else {
									lock_distant = lock_distant_min;
								}

								if (now_distant <= lock_distant) {
									if (lockEnemyId == 0 || GetAsyncKeyState(VK_LBUTTON) == 0) {  //lockEnemyId == 0 || s
										Vector BonePosition = Vector();
										uintptr_t boneArray = driver::read<uintptr_t>(socket_lock, g_pid, entityEnemyAddress + OFFSET_BONES);
										uint32_t boneloc = (8 * 0x30);
										struct Bone entityBone = {};
										entityBone = driver::read<struct Bone>(socket_lock, g_pid, boneArray + boneloc);
										BonePosition.x = EnemyPosition.x + entityBone.x;
										BonePosition.y = EnemyPosition.y + entityBone.y;
										BonePosition.z = EnemyPosition.z + entityBone.z;

										Vector bs = Vector();
										WorldToScreen(BonePosition, m.matrix, g_width, g_height, bs);
										if (bs.x > 0 && bs.y > 0) {
											float now_crh_distant = sqrt(pow(fabs(bs.x - g_width / 2), 2) + pow(fabs(bs.y - g_height / 2), 2));
											if ((g_height / 2 - now_crh_distant > 0) && CrossHairCloseDistant - now_crh_distant > 0) { // || now_distant < closeDistant
												CrossHairCloseDistant = now_crh_distant;
												tmp_targetId = i;
											}
										}
									}
								}
							}
							else {
								tmp_targetId = lockEnemyId;
							}
							
							
						}
					}else {
						driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_COLORS, (uintptr_t)glow_color_allied, sizeof(glow_color_allied));
						if (driver::read<int>(socket_glow, g_pid, entityEnemyAddress + OFFSET_BLEED_OUT_STATE) > 0) {
							driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_COLORS, (uintptr_t)glow_color_knock_down, sizeof(glow_color_knock_down));
						}
					}
				}
				if (remain_enemy != 0 || tmp_targetId != 0) {
					lockEnemyId = tmp_targetId;
					DWORD64 entityEnemyAddress = driver::read<uint64_t>(socket_lock, g_pid, g_entityAdd_list + lockEnemyId * 32);
					if (entityEnemyAddress != 0) {
						driver::write_memory(socket_glow, g_pid, entityEnemyAddress + OFFSET_GLOW_COLORS, (uintptr_t)glow_color_aimbot, sizeof(glow_color_aimbot));
					}
				}
				
				char text[8];
				itoa(lockEnemyId, text, 10);
				SetConsoleTitleA(text);
				printf("剩余敌人：%d\n", remain_enemy);
				Sleep(100);
			}
			
		}
		Sleep(1000);
	}
}

void gameLockLoop() {
	while (true) {
		Sleep(1);
		if (lock_mode) {
			if (lockEnemyId != 0) {
				DWORD64 entityEnemyAddress = driver::read<uint64_t>(socket_lock, g_pid, g_entityAdd_list + lockEnemyId * 32);
				if (entityEnemyAddress != 0) {
					if (!CheckAimTargetVaild(lockEnemyId)) {
						lockEnemyId = 0;
						continue;
					}

					DWORD64 entityPlayerAddress = driver::read<uint64_t>(socket_glow, g_pid, g_entityPlayer);
					QAngle Angles = CalculateBestBoneAim(entityPlayerAddress, entityEnemyAddress, 15);
					if (Angles.x != 0 || Angles.y != 0) {
						driver::write<SVector>(socket_lock, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES, SVector(Angles));
					}
				}
			}

		}
		
	}
}


bool LoadDriver() {
	SC_HANDLE schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schManager) {
		return false;
	}
	TCHAR szDriverImagePath[256];
	GetCurrentDirectory(256, szDriverImagePath);
	lstrcatW(szDriverImagePath, L"\\kernelDrv.sys");

	SC_HANDLE schService = OpenService(schManager, L"zyKernelMem", SERVICE_ALL_ACCESS);
	if (schService != NULL) {
		SERVICE_STATUS SvrSta;
		ControlService(schService, SERVICE_CONTROL_STOP, &SvrSta);
		DeleteService(schService);
		CloseServiceHandle(schService);

	}

	 schService = CreateService(schManager, L"zyKernelMem", L"Kernel Driver", SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, szDriverImagePath, NULL, NULL, NULL, NULL,NULL);

	if (schService == NULL) {
		if (ERROR_SERVICE_EXISTS != GetLastError()) {
			cout << "Fail Id = 1\n";
			CloseServiceHandle(schManager);
			return false;
		}
	}
	CloseServiceHandle(schManager);
	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schManager) {
		return false;
	}
	schService = OpenService(schManager, L"zyKernelMem", SERVICE_ALL_ACCESS);
	if (!schService) {
		cout << "Fail Id = 2\n";
		CloseServiceHandle(schManager);
		return false;
	}
	bool r = StartService(schService,0,0);
	if (!r) {
		return false;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);
	return true;
}


void initization() {
	int times = 0;
	char path[] = "SOFTWARE\\zyStudio\\KernelMem";
	char value[] = "Channel";
	unsigned long type = REG_DWORD;
	int port = 0, cb = sizeof(int);
	HKEY mainKeyHandle, subKeyHandle;
	srand(time(NULL));
	if (RegCreateKeyA(HKEY_LOCAL_MACHINE, path, &mainKeyHandle) != ERROR_SUCCESS) {
		printf_s("读取信息失败\n");
		getchar();
		exit(0);
	}
	srand(unsigned(time(0)));

	int minport = 5000, maxport = 60000;

	if (RegQueryValueExA(mainKeyHandle, value, NULL, &type, (LPBYTE)&port, (LPDWORD)&cb) != ERROR_SUCCESS) {
		
		port = rand() % (maxport + 1 - minport) + minport;

		if (RegSetValueExA(mainKeyHandle, value, NULL, REG_DWORD, (LPBYTE)&port, cb) != ERROR_SUCCESS) {
			printf_s("读取信息失败\n");
			getchar();
			exit(0);
		}
	}

	retry:

	server_port = port;
	socket_glow = driver::connect();
	socket_lock = driver::connect();
	if (socket_glow == INVALID_SOCKET || socket_lock == INVALID_SOCKET) {
		times++;
		if (times >= 2) {
			cout << "无法与内核驱动取得通讯！\n";
			getchar();
			exit(0);
		}
		else {
			port = rand() % (maxport + 1 - minport) + minport;
			if (RegSetValueExA(mainKeyHandle, value, NULL, REG_DWORD, (LPBYTE)&port, cb) != ERROR_SUCCESS) {
				printf_s("读取信息失败\n");
				getchar();
				exit(0);
			}
			if (!LoadDriver()) {
				printf_s("驱动加载失败\n");
				getchar();
				exit(0);
			};
			printf_s("驱动加载成功\n");
			goto retry;
		}
		
	}
}


int main()
{
	driver::initialize();
	initization();
	cout << "初始化成功，正在等待游戏进程..\n";

	WaitforGame();
	if (g_baseAdd == 0)return 0;

	cout << "\n游戏进程已找到";

	g_entityAdd_list = g_baseAdd + OFFSET_ENTITYLIST;
	g_entityPlayer = g_baseAdd + OFFSET_LOCAL_ENT;
	
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)&gameGlowLoop,NULL,NULL,NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLockLoop, NULL, NULL, NULL);
	
	
	/*
	while (true) { // entity name
		DWORD64 entityBase = driver::read<uint64_t>(socket_lock, g_pid, g_entityAdd_list);
		
		if (entityBase!=0) {
			for (int i = 0; i <= 200; i++) {
				DWORD64 entityAddress = driver::read<uint64_t>(socket_lock, g_pid, g_entityAdd_list+32*i);
				entityAddress += 0x518;
				char name[32];
				DWORD32 entityNameAdd = driver::read<uint32_t>(socket_lock, g_pid, entityAddress);

				driver::read_memory(socket_lock, g_pid, entityNameAdd,(uintptr_t)name,sizeof(name));
				cout << i << " " << name << endl;
			}
			break;
		}


		Sleep(100);
	}
	

	while (true) {//weaponId
		DWORD64 entityBase = driver::read<uint64_t>(socket_lock, g_pid, g_entityPlayer);
		system("cls");
		if (entityBase != 0) {
			DWORD64 lst_wpn = driver::read<uint64_t>(socket_glow, g_pid, entityBase + OFFSET_WEAPON);
			int weaponId = lst_wpn & 0xFFFF;

			DWORD64 weaponEntity = g_entityAdd_list + weaponId * 32;
			weaponEntity = driver::read<uint64_t>(socket_glow, g_pid, weaponEntity);

			char EntityTxt[32];
			
			DWORD64 nameaddress = driver::read<uint64_t>(socket_glow, g_pid, weaponEntity + OFFSET_SIG_NAME);

			driver::read_memory(socket_glow, g_pid, nameaddress, (uintptr_t)EntityTxt, sizeof(EntityTxt) - 1);
			cout << EntityTxt;

			
			int weaponNameId = driver::read<int>(socket_glow, g_pid, weaponEntity + OFFSET_WEAPON_NAME_IDX);
			char title[12];
			itoa(weaponNameId,title,10);
			SetConsoleTitleA(title);
			
		}
		Sleep(100);
	}
	*/

	int always_lock_mode = 0;

	while (true) {
		
		if (GetForegroundWindow() == g_hwnd) {
			if (GetAsyncKeyState(VK_RBUTTON) != 0) {
				lock_mode = 1;
			}else {
				lock_mode = 0;
			}
			if (GetAsyncKeyState(VK_LSHIFT) != 0) {
				lock_nochange = 1;
			}else {
				lock_nochange = 0;
			}
		}
		else {
			lock_mode = 0;
		}
		if (GetAsyncKeyState(VK_OEM_3) != 0) {
			always_lock_mode = !always_lock_mode;
		}
		if (always_lock_mode) {
			lock_mode = 1;
		}
		Sleep(50);
	}


    return 0;
}


DWORD GetProcessPid(wchar_t *name) {
	HANDLE hld = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hld) {
		return 0;
	}
	PROCESSENTRY32 tProcess;
	tProcess.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hld, &tProcess)) {
		do {
			if (!wcscmp(tProcess.szExeFile, name)) {
				return tProcess.th32ProcessID;
			}

		} while (Process32Next(hld, &tProcess));
		CloseHandle(hld);
	}
	return 0;
}