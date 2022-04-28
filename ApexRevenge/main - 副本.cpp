#include "main.h"
#include "hwid.h"
#include "entity.h"

#include "SOCKETsys.h"

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_dx9.h"
#include "Imgui\imgui_impl_win32.h"


DWORD32 g_pid = 0;
HWND g_hwnd = 0;
DWORD64 g_add_gamebase = 0, g_add_entitylist = 0, g_add_entitylocal = 0, g_add_nameList;
SOCKET g_sock_1, g_sock_2,g_sock_3;
int g_width, g_height;

bool spoofmachine;
char valid_code[48];
char processdata[48];
int processdatalen = 0;

HWND hwnd_console;

bool debug = false;

int game_scene = 0;
Entity_player gamePlayerList[60];
Entity_player gamelocalEntity;

int gameItemList[10000] = { 0 };
Vector gameItemListPos[10000];
int item_record_count = 0;

HWND ov_hwnd;
IDirect3D9 *d3dObject;
IDirect3DDevice9 *d3dDevice;
LPD3DXFONT dx_font_verda;
LPD3DXFONT dx_font_soti;
LPD3DXLINE dx_boxline;

void initialization();
void WaitingforGame();
void gameLoop_getEntity();
void ClearnAndExit();
void gameLoop_glow();
void gameLoop_aimBot();
void gameLoop_glowItem();
void gameLoop_Overlay();
void Render_showItem();
void RenderThread_getItem();
void fun_behindAlert();
void gameLoop_HotKey();

int GetDrvPort();
bool SetDrvPort(int port);
DWORD GetProcessPid(wchar_t *name);

wchar_t * AnsiToUnicode(const char * szStr);

char * UnicodeToAnsi(const wchar_t * szStr);

DWORD64 entityBaseAddress, entityPlayerAddress;
Matrix ViewMartix;
int g_remainEnemy;
int g_observeCount;

bool g_showMenu = false;

bool hk_glow = false, hk_name = true, hk_distance_weapon = true, hk_hp_ap = true,hk_box = true;
bool hk_aimbotsoomth = true , hk_item = true;
int hk_aimbot;

int code_remaintime = 0;
int code_type = 0;

void loadSetting();
void saveSetting();

void getkilloffset() {
	Entity_player wyq;

	//entityBaseAddress = driver::read<uint64_t>(g_sock_1, g_pid, g_add_entitylist);

	for (int i = 0; i < 60; i++) {
		DWORD64 tmpAddress = g_add_entitylist + (i + 1) * 32;
		tmpAddress = driver::read<uint64_t>(g_sock_1, g_pid, tmpAddress);
		if (!tmpAddress)continue;
		wyq.Address = tmpAddress;
		wyq.GetEntity(g_sock_1);
		wyq.GetDetail(g_sock_1);
		if (strcmp(wyq.name, "ny222hyl") == 0) {
			wyq.Id = i + 1;
			break;
		}
	}

	int offsetlist[4092] = { 0 };
	int cnt = 0;

	int choose;
	int kills = 0;
	int tmpcnt = 0;

	std::cout << wyq.Id << " " << wyq.name << " \n";

	std::cout << "1:清屏\n2:清空结果\n3:新搜索（同时清空结果）\n4:继续搜索\n5:输出结果\n";
	while (true) {
		std::cin >> choose;
		switch (choose) {
		case 1:
			system("cls");
			std::cout << "1:清屏\n2:清空结果\n3:新搜索（同时清空结果）\n4:继续搜索\n5:输出结果\n";
			break;
		case 2:
			memset(offsetlist, 0, sizeof(offsetlist));
			cnt = 0;
			break;
		case 3:
			cnt = 0;
			memset(offsetlist, 0, sizeof(offsetlist));
			wyq.GetEntity(g_sock_1);
			wyq.GetDetail(g_sock_1);
			std::cout << "当前人头:";
			std::cin >> kills;
			for (int i = 0; i < 4092; i++) {
				if (*(int*)(wyq.EntityBufer + i * 4) == kills) {
					offsetlist[cnt++] = i * 4;
				}
			}
			std::cout << "当前结果个数:" << cnt << "\n";
			break;
		case 4:
			tmpcnt = 0;
			wyq.GetEntity(g_sock_1);
			wyq.GetDetail(g_sock_1);
			std::cout << "当前人头:";
			std::cin >> kills;
			std::cout << "搜索内容:" << kills << "\n";
			for (int i = 0; i < cnt; i++) {
				if (*(int*)(wyq.EntityBufer + offsetlist[i]) == kills) {
					offsetlist[tmpcnt++] = offsetlist[i];
				}
			}
			cnt = tmpcnt;
			std::cout << "当前结果个数:" << cnt << "\n";
			break;
		case 5:
			std::cout << "个数：" << cnt <<"\n";
			std::cout << "—————————————\n";
			wyq.GetEntity(g_sock_1);
			wyq.GetDetail(g_sock_1);
			for (int i = 0; i < cnt; i++) {
				std::cout << offsetlist[i];
				std::cout << " " << *(int*)(wyq.EntityBufer + offsetlist[i]) << "\n";

			}
			std::cout << "—————————————\n";
		}
	}


}

int main() {
	VMProtectBegin("main");


	hwnd_console = GetConsoleWindow();

	if (!ConnectToServer()) {
		return 0;
	}
	
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ReceivePacket, 0, 0, 0);


	loadSetting();
	LoadLoginUI();

	if (spoofmachine) {
		SpooferRegister();
		machinespoofer();
	}

	initialization();
	WaitingforGame();

	saveSetting();

	g_add_entitylist = g_add_gamebase + OFFSET_ENTITYLIST;
	g_add_entitylocal = g_add_gamebase + OFFSET_LOCAL_ENT;
	g_add_nameList = g_add_gamebase + OFFSET_PALYER_NAME_LIST;


	

	if (!窗口创建(g_width, g_height, &ov_hwnd, &d3dObject, &d3dDevice)) {
		std::cout << "创建Overlay失败\n";
		Sleep(500);
		ov_hwnd = 0;
	}


	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(ov_hwnd);
	ImGui_ImplDX9_Init(d3dDevice);


	io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 13.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	SetWindowLongPtr(ov_hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);


	D3DXCreateFontA(d3dDevice, 13, 0, FW_BOLD, 0, 0, 5, 0, 4, 0, "Verdana", &dx_font_verda);
	D3DXCreateFontA(d3dDevice, 16, 0, FW_BOLD, 0, 0, 5, 0, 4, 0, "宋体", &dx_font_soti);
	D3DXCreateLine(d3dDevice, &dx_boxline);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_getEntity, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_glow, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_aimBot, NULL, NULL, NULL);

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&RenderThread_getItem, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_Overlay, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_HotKey, NULL, NULL, NULL);

	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&gameLoop_glowItem, NULL, NULL, NULL);

	//gameLoop_Overlay();

	
	//std::cout << driver::read<int>(g_sock_3,g_pid, g_add_gamebase + 0x2A2D508) << "\n"; // entity count

	/*
	uint64_t add_viewRenderer = driver::read<uint64_t>(g_sock_1, g_pid, g_add_gamebase + OFFSET_RENDER);
	uint64_t add_viewMatrix = driver::read<uint64_t>(g_sock_1, g_pid, add_viewRenderer + OFFSET_MATRIX);
	printf("%llX", add_viewMatrix);
*/

	//getkilloffset();

	//void testTime();
	//testTime();

	//Render_showItem();

	/*
	int numset = 0;

	char title[10];
	itoa(numset,title , 10);
	SetConsoleTitleA(title);
	*/

	RECT Rect;
	POINT g_pos;
	while (true) {
		if (!IsIconic(g_hwnd)) {
			
			GetClientRect(g_hwnd, &Rect);
			g_width = Rect.right - Rect.left;
			g_height = Rect.bottom - Rect.top;


			g_pos.x = g_pos.y = 0;
			ClientToScreen(g_hwnd, &g_pos);
			
			SetWindowPos(ov_hwnd , HWND_TOP, g_pos.x, g_pos.y, g_width - Random(4, 8), g_height - Random(2, 5), SWP_SHOWWINDOW);
			
		}
		
		Sleep(10);
	}
	
	VMProtectEnd();

	return 0;
}

void loadSetting() {
	char rget[255];
	GetPrivateProfileStringA("Launcher", "CODE", "", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "") == 0) {
		remembercode = false;
	}
	else {
		remembercode = true;
		strcpy(content_codebox, rget);
		len_codebox = strlen(content_codebox);
	}

	GetPrivateProfileStringA("Launcher", "SpoofMachine", "", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		spoofmachine = true;
	}
	else {
		spoofmachine = false;
	}

	GetPrivateProfileStringA("ESP", "glow", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_glow = true;
	}
	else {
		hk_glow = false;
	}
	GetPrivateProfileStringA("ESP", "name", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_name = true;
	}
	else {
		hk_name = false;
	}
	GetPrivateProfileStringA("ESP", "weapon&distance", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_distance_weapon = true;
	}
	else {
		hk_distance_weapon = false;
	}
	GetPrivateProfileStringA("ESP", "health&armour", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_hp_ap = true;
	}
	else {
		hk_hp_ap = false;
	}
	GetPrivateProfileStringA("ESP", "box", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_box = true;
	}
	else {
		hk_box = false;
	}

	GetPrivateProfileStringA("AimBot", "State", "0", rget, 255, ".\\apexRevenge.ini");
	int num = atoi(rget);
	if (num < 0 || num >4) {
		num = 0;
	}
	hk_aimbot = num;
	GetPrivateProfileStringA("AimBot", "soomth", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_aimbotsoomth = true;
	}
	else {
		hk_aimbotsoomth = false;
	}
	GetPrivateProfileStringA("ItemESP", "switch", "true", rget, 255, ".\\apexRevenge.ini");
	if (strcmp(rget, "true") == 0) {
		hk_item = true;
	}
	else {
		hk_item = false;
	}

	hk_item = false;
}

void saveSetting() {
	char rget[255];
	if (spoofmachine) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("Launcher", "SpoofMachine", rget, ".\\apexRevenge.ini");

	if (hk_glow) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ESP", "glow",  rget , ".\\apexRevenge.ini");

	if (hk_box) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ESP", "box", rget, ".\\apexRevenge.ini");

	if (hk_name) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ESP", "name", rget, ".\\apexRevenge.ini");

	if (hk_distance_weapon) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ESP", "weapon&distance", rget, ".\\apexRevenge.ini");

	if (hk_hp_ap) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ESP", "health&armour", rget, ".\\apexRevenge.ini");

	itoa(hk_aimbot, rget, 10);
	WritePrivateProfileStringA("AimBot", "State", rget, ".\\apexRevenge.ini");
	if (hk_aimbotsoomth) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("AimBot", "soomth", rget, ".\\apexRevenge.ini");

	
	if (hk_item) {
		strcpy(rget, "true");
	}
	else {
		strcpy(rget, "false");
	}
	WritePrivateProfileStringA("ItemESP", "switch",  rget,".\\apexRevenge.ini");
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

void testTime() {
	int time = GetTickCount();

	float cloest = 100000;
	DWORD64 cloestAddress = 0;

	for (int i = 100; i < 10000; i++) {
		DWORD64 address = g_add_entitylist + i * 32;
		address = driver::read<uint64_t>(g_sock_2, g_pid, address);

		if (!address) {
			//gameItemList[i] = 0;
			continue;
		}

		/*
		if (itemId <= 0 || itemId > 132) {
			//gameItemList[i] = 0;
			continue;
		}*/

		DWORD32 entityNameAdd = driver::read<uint32_t>(g_sock_2, g_pid, address + OFFSET_SIG_NAME);
		if (!entityNameAdd) {
			//gameItemList[i] = 0;
			continue;
		}
		
		char name[16] = { 0 };
		driver::read_memory(g_sock_2, g_pid, entityNameAdd,(uintptr_t)name,16);

		if (strcmp(name,"prop_survival") == 0){
			//BYTE buffer[0x3FF0] = { 0 };
			//driver::read_memory(g_sock_2, g_pid, address, (uintptr_t)buffer, 0x3FF0);
			/*for (int va = 0; va < 0x3FF0; va += 4) {
				if (*(int*)(buffer + va) == 77) {
					printf("%X\n", va);
				}
			}*/


			/*Vector pos = driver::read<Vector>(g_sock_2, g_pid, address + OFFSET_ORIGIN);
			float tmp =  pos.DistTo(gamelocalEntity.Position);
			if (tmp < cloest) {
				cloest = tmp;
				cloestAddress = address;
			}*/
			

			int itemId = driver::read<int>(g_sock_2, g_pid, address + OFFSET_ITEM_ID);
			//cout << i << " " << itemId << " " << address << "\n";
			if (itemId != EVO_SHIELD_LV4) {
				continue;
			}
			cout << i << " " << itemId << " " << address << "\n";
			

			
			/*std::cout << driver::read<int>(g_sock_2, g_pid, address + OFFSET_GLOW_ENABLE) << "\n";
			std::cout << driver::read<int>(g_sock_2, g_pid, address + OFFSET_GLOW_CONTEXT) << "\n";
			std::cout << driver::read<int>(g_sock_2, g_pid, address + OFFSET_GLOW_MAGIC) << "\n";

			std::cout << driver::read<float>(g_sock_2, g_pid, address + OFFSET_GLOW_ITEM_COLOR) << "\n";
			std::cout << driver::read<float>(g_sock_2, g_pid, address + OFFSET_GLOW_ITEM_COLOR + 4) << "\n";
			std::cout << driver::read<float>(g_sock_2, g_pid, address + OFFSET_GLOW_ITEM_COLOR + 8) << "\n";
			*/

			break;
		}

		

		//Vector Position = driver::read<Vector>(g_sock_2, g_pid, address + OFFSET_ORIGIN);

		//gameItemList[i] = itemId;
		//gameItemListPos[i] = Position;

	}
	//printf("%llX \n", cloestAddress);
	time = GetTickCount() - time;
	std::cout << time;
}

void RenderThread_getItem() {
	//VMProtectBegin("RenderThread_getItem");
	while (true) {
		Sleep(1);
		if (entityBaseAddress == 0 || entityPlayerAddress == 0) {
			continue;
		}

		if (!hk_item) {
			continue;
		}

		int tmpcount = 0;

		for (int i = 1; i < 12000; i++) {
			DWORD64 address = g_add_entitylist + i * 32;
			address = driver::read<uint64_t>(g_sock_3, g_pid, address);
			if (!address) {
				continue;
			}

			
			int itemId = driver::read<int>(g_sock_3, g_pid, address + OFFSET_ITEM_ID);
			if (itemId <= 0 || itemId > 255) {
				continue;
			}

			DWORD64 entityNameAdd = driver::read<uint32_t>(g_sock_3, g_pid, address + OFFSET_SIG_NAME);
			if (!entityNameAdd) {
				continue;
			}
			else if(driver::read<byte>(g_sock_3, g_pid, entityNameAdd+12)!='l'){
				continue;
			}

			


			Vector Position = driver::read<Vector>(g_sock_3, g_pid, address + OFFSET_ORIGIN);

			gameItemList[tmpcount] = itemId;
			gameItemListPos[tmpcount] = Position;
			tmpcount++;

			
			
		}
		//std::cout << "\n" << item_record_count << "\n";
		item_record_count = tmpcount;
	}
	//VMProtectEnd();
}


void Render_showItem() {
	//VMProtectBegin("Render_showItem");
	int renderCount = 0;
	ESP_item *itemList = new ESP_item[item_record_count];
	for (int i = 0; i < item_record_count; i++) {
		if (!gameItemList[i]) continue;
		int dist = gameItemListPos[i].DistTo(gamelocalEntity.Position) / 40.0f;
		if (dist > 50) {
			continue;
		}

		Vector ovPos = Vector();
		if (!WorldToScreen(gameItemListPos[i], ViewMartix.matrix, g_width, g_height, ovPos)) {
			continue;
		}

		if (ovPos.x < 0 || ovPos.y < 0) {
			continue;
		}

		if (dist > 5) {
			int l_n = 0;
			for (; l_n < renderCount; l_n++) {
				if (gameItemList[i] == itemList[l_n].id && itemList[l_n].pos.DistTo(gameItemListPos[i]) <= 50 ) {
					itemList[l_n].count++;
					if (itemList[l_n].distance  < dist) {
						itemList[l_n].distance = dist;
						itemList[l_n].x = ovPos.x;
						itemList[l_n].y = ovPos.y;
					}
					break;
				}
			}
			if (renderCount == 0 || l_n == renderCount) {
				goto NewItem;
			}

		}
		else {
			NewItem:
			itemList[renderCount].count = 1;
			itemList[renderCount].pos = gameItemListPos[i];
			itemList[renderCount].x = ovPos.x;
			itemList[renderCount].y = ovPos.y;
			itemList[renderCount].id = gameItemList[i];
			itemList[renderCount].distance = dist;
			itemList[renderCount].arrangement = 0;
			renderCount++;
		}
	}

	for (int i = 0; i < renderCount - 1; i++) {
		int height = 1;
		if (itemList[i].distance <= 8)continue;
		for (int n = i+1; n < renderCount; n++) {
			if (itemList[n].distance <= 8)continue;

			if (itemList[i].arrangement == 0 && itemList[n].arrangement == 0) {
				//if (itemList[i].pos.DistTo(itemList[n].pos) <= 50 && ( fabs(itemList[i].x - itemList[n].x) <= 50 && fabs(itemList[i].y - itemList[n].y) <= 50)) {
				if (fabs(itemList[i].x - itemList[n].x) <= 50 && fabs(itemList[i].y - itemList[n].y) <= 50) {
					itemList[n].x = itemList[i].x;
					itemList[n].y = itemList[i].y + height * 15;
					//itemList[i].arrangement = 1;
					//itemList[n].arrangement = 1;
				}
			}
		}
	}


	for (int i = 0; i < renderCount;i++) {
			D3DCOLOR Textcolor = GetItemColor(itemList[i].id);
			char Text[24] = { 0 };
			
			if (ItemName[itemList[i].id][0] != '\0' && itemList[i].id < sizeof(ItemName) / 16) {
				strcpy(Text, ItemName[itemList[i].id]);
				if (itemList[i].count > 1) {
					char tmpT[8];
					sprintf(tmpT, " X %d", itemList[i].count);
					strcat(Text, tmpT);
				}
			}
			else {
				itoa(itemList[i].id, Text, 10);
			}
			
			if (itemList[i].distance > 15) {
				char tmpT[8];
				sprintf(tmpT, " [%dm]", itemList[i].distance);
				strcat(Text, tmpT);
			}

			if (Textcolor == d3dcolor_rate_red) {
				D3D描边写字(&dx_font_verda, itemList[i].x, itemList[i].y, 13, Textcolor, d3dcolor_White, Text);
			}
			else {
				D3D描边写字(&dx_font_verda, itemList[i].x, itemList[i].y, 13, Textcolor, d3dcolor_Black, Text);
			}
			
			/*
			if (dist > 10) {
				char strDis[10];
				sprintf(strDis, "[%dm]", dist);
				D3D描边写字(&dx_font_verda, ovPos.x, ovPos.y+14, 13, D3DCOLOR_ARGB(255, 191, 192, 253), d3dcolor_Black, strDis);
			}
			*/
		
	}
		delete[] itemList;
	//VMProtectEnd();
}


void gameLoop_HotKey() {
	while (true) {
		Sleep(80);
		if ((GetForegroundWindow() != g_hwnd && GetForegroundWindow() != ov_hwnd ) || IsIconic(g_hwnd))continue;
		if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
			g_showMenu = !g_showMenu;
			if (!g_showMenu) {
				D3D设置窗口穿透(ov_hwnd, true);
				SetForegroundWindow(g_hwnd);
				saveSetting();
			}
			else {
				D3D设置窗口穿透(ov_hwnd, false);
				SetForegroundWindow(ov_hwnd);
			}
		}
	}

}

void imgui_menu() {
	VMProtectBegin("imgui_menu");
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();


	ImGui::Begin(u8"设置菜单");
	if (ImGui::CollapsingHeader(u8"透视设置")) {

		ImGui::Checkbox(u8"人物描边", &hk_glow);
		ImGui::Checkbox(u8"人物方框", &hk_box);
		ImGui::Checkbox(u8"人物名字", &hk_name);
		ImGui::Checkbox(u8"人物武器与距离", &hk_distance_weapon);
		ImGui::Checkbox(u8"人物血量与护甲", &hk_hp_ap);

	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader(u8"自瞄设置")) {

		ImGui::RadioButton(u8"不自瞄", &hk_aimbot,0);
		ImGui::RadioButton(u8"SHIFT键自瞄", &hk_aimbot,1);
		ImGui::RadioButton(u8"CTRL键自瞄", &hk_aimbot,2);
		ImGui::RadioButton(u8"大锁键自瞄", &hk_aimbot,3);
		ImGui::RadioButton(u8"鼠标右键自瞄", &hk_aimbot, 4);
		ImGui::Checkbox(u8"平滑自瞄", &hk_aimbotsoomth);
	}
	ImGui::Separator();
	
	if (ImGui::CollapsingHeader(u8"物品透视设置")) {
		ImGui::Checkbox(u8"开启物品透视", &hk_item);
	}
	
	ImGui::End();
	ImGui::EndFrame();
	VMProtectEnd();
}

void gameLoop_Overlay() {
	//VMProtectBegin("gameLoop_Overlay");
	if (!ov_hwnd || !d3dDevice) {
		return;
	}
	
	//int timeStamp = time(0);
	while (true) {
		
		//std::cout << "坐标\n" << g_point.x << "\n" << g_point.y << "\n";


		d3dDevice->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, NULL);
		d3dDevice->BeginScene();

		if ((GetForegroundWindow() != g_hwnd && GetForegroundWindow() != ov_hwnd) || IsIconic(g_hwnd)) goto SceneEnd;

		D3D描边写字(&dx_font_verda, 0, 0, 13, d3dcolor_Red, d3dcolor_White, "Fizzy Overlay");
		
		if (!gamelocalEntity.isZoom && hk_item) {
			Render_showItem();
		}
		

		if (g_showMenu) {
			imgui_menu();
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		}

		if (hk_box) {
			for (int i = 0; i < 60; i++) {
				if (gamePlayerList[i].distanceToPlayer <= 10000) {
					int height = gamePlayerList[i].sc_y - gamePlayerList[i].sc_head_y;
					int x1 = gamePlayerList[i].sc_x - height / 4;
					D3D画矩形(&d3dDevice, &dx_boxline, x1, gamePlayerList[i].sc_head_y, x1 + height / 2, gamePlayerList[i].sc_y,true,1,d3dcolor_Blue,false,NULL);
				}
			}
		}
		


		if (g_remainEnemy > 0) {
			char strEm[24] = { 0 };
			sprintf(strEm, "剩余敌人：%d", g_remainEnemy);
			D3D描边写字(&dx_font_soti, g_width * 0.2 , g_height * 0.1 , 16, d3dcolor_Red, d3dcolor_White, strEm);

			sprintf(strEm, "观战人数：%d", g_observeCount);
			D3D描边写字(&dx_font_soti, g_width * 0.2, g_height * 0.1 + 18, 16, d3dcolor_Yellow, d3dcolor_Black, strEm);
		}
	
		fun_behindAlert();

		
		
		SceneEnd:
		d3dDevice->EndScene();
		d3dDevice->Present(NULL, NULL, NULL, NULL);
		Sleep(1);
	}
	//VMProtectEnd();
}

void fun_behindAlert() {
	//VMProtectBegin("fun_behindAlert");

	if (entityBaseAddress == 0 || entityPlayerAddress == 0) {
		g_remainEnemy = 0;
		return;
	}
	//system("cls");
	int fiftyMeters = 0, OneHundredMeters = 0;
	
	for (int i = 0; i < 60; i++) {
		if (!gamePlayerList[i].isPlayer)continue;
		if (gamePlayerList[i].Health <= 0 || gamePlayerList[i].Dead  > 0)continue;

		if (gamePlayerList[i].distanceToPlayer <= 250 * 40 && gamePlayerList[i].Team != gamelocalEntity.Team  && gamePlayerList[i].Knocked <=0) {
			Vector ov = Vector();
			if (WorldToScreen(gamePlayerList[i].Position, ViewMartix.matrix, g_width, g_height, ov)) {
				if (ov.x > 0 && ov.y > 0) {
					char playerInfo[62] = { 0 };
					char wpnName[32] = { 0 };
					if (hk_name) {
						D3D描边写字(&dx_font_verda, ov.x, ov.y, 13, d3dcolor_White, d3dcolor_Black, gamePlayerList[i].name);
						ov.y += 13;
					}
					if (hk_hp_ap) {
						sprintf(playerInfo, "HP:%d AP:%d", gamePlayerList[i].Health, gamePlayerList[i].Shield);
						D3D描边写字(&dx_font_verda, ov.x, ov.y, 13, d3dcolor_rate_blue, d3dcolor_Black, playerInfo);
						ov.y += 13;
					}
					if (hk_distance_weapon) {
						GetWeaponNameInHands(gamePlayerList[i].CurrentWeaponId, wpnName);
						sprintf(playerInfo, "%s %d m", wpnName, (int)(gamePlayerList[i].distanceToPlayer / 40));
						D3D描边写字(&dx_font_verda, ov.x, ov.y, 13, D3DCOLOR_ARGB(255, 255, 105, 180), d3dcolor_Black, playerInfo);
						ov.y += 13;
					}
					/*sprintf(playerInfo, "%d 杀", gamePlayerList[i].kills);
					D3D描边写字(&dx_font_verda, ov.x, ov.y, 13, D3DCOLOR_ARGB(255, 255, 0, 0), d3dcolor_Black, playerInfo);*/
				}
			};
		}
		

		

		if (gamePlayerList[i].Team != gamelocalEntity.Team) {
			QAngle CalculatedAngles = Math::CalcAngle(gamelocalEntity.CameraPos, gamePlayerList[i].Position);
			Math::NormalizeAngles(CalculatedAngles);
			double fov = fabs(CalculatedAngles.y - gamelocalEntity.ViewAngles.y);

			if (fov <= 60 || fov >= 300) {
				continue;
			}

			if (gamePlayerList[i].distanceToPlayer <= 50 * 40) {
				fiftyMeters++;
			}
			else if (gamePlayerList[i].distanceToPlayer <= 100 * 40) {
				OneHundredMeters++;
			}

		}
		
		/*
		Vector ov = Vector();
		if (WorldToScreen(gamePlayerList[i].Position,ViewMartix.matrix,g_width,g_height,ov))continue;

		if (ov.x < 0 || ov.y < 0 || ov.x  > g_width || ov.y > g_height)continue;

		*/
		
		/*
		Vector BonePosition = Vector();
		uintptr_t boneArray = *(uintptr_t*)(gamePlayerList[i].EntityBufer + OFFSET_BONES);
		uint32_t boneloc = (3* 0x30);
		struct Bone entityBone = {};
		entityBone = driver::read<struct Bone>(g_sock_2, g_pid, boneArray + boneloc);

		BonePosition.x = gamePlayerList[i].Position.x + entityBone.x;
		BonePosition.y = gamePlayerList[i].Position.y + entityBone.y;
		BonePosition.z = gamePlayerList[i].Position.z + entityBone.z;

		QAngle CalculatedAngles = Math::CalcAngle(gamelocalEntity.CameraPos, BonePosition);
		
		*/
		//QAngle ViewAngles = driver::read<QAngle>(g_sock_2, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES);
		

		/*
		std::cout << SVector(CalculatedAngles).x << std::endl;
		std::cout << SVector(CalculatedAngles).y << std::endl << std::endl;
		*/

		//driver::write<SVector>(g_sock_2, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES, SVector(CalculatedAngles));

	}
	
	/*
	std::cout << "------------------\n";
	std::cout << gamelocalEntity.ViewAngles.x << std::endl;
	std::cout << gamelocalEntity.ViewAngles.y << std::endl;
	*/
	if (fiftyMeters || OneHundredMeters > 0) {
		char strEm[64] = { 0 };
		sprintf(strEm, "背后敌人数量\n50米内：%d\n100米内：%d", fiftyMeters, OneHundredMeters);
		D3D描边写字(&dx_font_soti, g_width * 0.05 , g_height / 2 , 16, d3dcolor_Red, d3dcolor_White, strEm);
	}
	//VMProtectEnd();
}

//8 7 头 6脖子 5 3胸


void gameLoop_glow() {
	VMProtectBegin("gameLoop_glow");
	while (true) {
		Sleep(100);
		system("cls");
		std::cout << "entityBaseAddress = " << entityBaseAddress << "\n";
		std::cout << "entityPlayerAddress = " << entityPlayerAddress << "\n\n";
		if (entityBaseAddress == 0 || entityPlayerAddress == 0) {
			g_remainEnemy = 0;
			continue;
		}
		
		std::cout << "HP:" << gamelocalEntity.Health << " AP:" << gamelocalEntity.Shield << " WeaponId:" << gamelocalEntity.CurrentWeaponId << endl;
		std::cout << "x:" << gamelocalEntity.Position.x << endl;
		std::cout << "y:" << gamelocalEntity.Position.y << endl;
		std::cout << "z:" << gamelocalEntity.Position.z << endl;

		std::cout << "Id:" << gamelocalEntity.Id << " Team:"<< gamelocalEntity.Team <<endl;
		std::cout << "Name:" << gamelocalEntity.name << endl;

		std::cout << "\nscene:" << game_scene << endl;

		if(gamelocalEntity.Position.x == gamelocalEntity.Position.y && gamelocalEntity.Position.x == gamelocalEntity.Position.z)continue;

		//诸王峡谷 21000 //世界尽头 13000


		//if (gamelocalEntity.Position.z> 13000.0f)continue;

		if (game_scene == 2144) {
			if (gamelocalEntity.Position.z> 13000.0f)continue;
		}
		else if (game_scene == 2285) {
			if (gamelocalEntity.Position.z> 21000.0f)continue;
		}

		int ememycnt = 0;
		int observecnt = 0;
	

		for (int i = 0; i < 60; i++) {
			if (!gamePlayerList[i].isPlayer)continue;

			if (gamePlayerList[i].observingId == gamelocalEntity.Id) {
				observecnt++;
			}

			if (gamePlayerList[i].Health <= 0 || gamePlayerList[i].Dead  > 0)continue;

			if (gamePlayerList[i].Team == gamelocalEntity.Team) {
				if (hk_glow) {
					if (gamePlayerList[i].Knocked <= 0) {
						gamePlayerList[i].EnableGlow(g_sock_2, glow_color_allied);
					}
				}
			}else {
				ememycnt++;

				if (hk_glow) {
					if (gamePlayerList[i].Knocked > 0) {
						gamePlayerList[i].EnableGlow(g_sock_2, glow_color_knock_down);
					}
					else {
						if (gamePlayerList[i].Health > 20) {
							gamePlayerList[i].EnableGlow(g_sock_2, glow_color_enemy);
						}
						else {
							gamePlayerList[i].EnableGlow(g_sock_2, glow_color_enemy_healthlow);
						}
					}
				}

			}
		}
		g_remainEnemy = ememycnt;
		g_observeCount = observecnt;
		//std::cout << "剩余敌人:" << ememycnt;
		//gameLoop_glowItem();
	}
	VMProtectEnd();
}

void gameLoop_glowItem() {

	int mark1 = 0;
	for (int i = 0; i <= 10000; i++) {
		char name[32];
		DWORD64 address = g_add_entitylist + i * 32;
		address = driver::read<uint64_t>(g_sock_2, g_pid, address);
		if (!address)continue;

		DWORD32 nameAddr = driver::read<int>(g_sock_2, g_pid, address + OFFSET_SIG_NAME);
		memset(name, 0, sizeof(name));

		driver::read_memory(g_sock_2, g_pid, nameAddr, (uintptr_t)name, sizeof(name));

		

		if (strcmp(name, "prop_survival")!= 0) {
			continue;			

		}

		//std::cout << "i=" << i << " " << driver::read<int>(g_sock_2, g_pid, address + OFFSET_ITEM_ID) << "\n";

		//mark1 = 1;
		//std::cout << i <<"\n";
		//if (driver::read<int>(g_sock_2, g_pid, address + OFFSET_ITEM_ID) != 65)continue;
		//0x4D407D7E
		//driver::write<int>(g_sock_2, g_pid, address + OFFSET_GLOW_ENABLE, 1);
		//driver::write<int>(g_sock_2, g_pid, address + OFFSET_GLOW_CONTEXT, 2);
		//driver::write<int>(g_sock_2, g_pid, address + OFFSET_GLOW_MAGIC, 0x4D407D7E);

		driver::write_memory(g_sock_2, g_pid, address + OFFSET_GLOW_FADE, (uintptr_t)glow_fade, sizeof(glow_fade));
		//driver::write_memory(g_sock_2, g_pid, address + OFFSET_GLOW_ITEM_COLOR , (uintptr_t)glow_color_aimbot, sizeof(float)*3);

		//std::cout << i << " " << << "\n";

	}
}

bool CheckAimTargetValid(int id) {
	if (id < 0 || id >= 60) {
		return false;
	}

	if (!gamePlayerList[id].isPlayer)return false;

	if (gamePlayerList[id].distanceToPlayer > 250 * 40) {
		return false;
	}

	if (gamePlayerList[id].Team == gamelocalEntity.Team)return false;

	if (gamePlayerList[id].Health <= 0 || gamePlayerList[id].Dead > 0 || gamePlayerList[id].Knocked > 0) return false;

	int i;
	for (i = 0; i < sizeof(weapon_sniper_list)/4; i++) {
		if (weapon_sniper_list[i] == gamelocalEntity.CurrentWeaponId) {
			i = 1;
			break;
		}
	}
	if (i) {
		return true;
	}
	else {
		if (gamelocalEntity.CurrentWeaponId == HandleWeaponID::G7_SCOUT || gamelocalEntity.CurrentWeaponId == HandleWeaponID::WINGMAN) {
			if (gamePlayerList[id].distanceToPlayer > 200 * 40) {
				return false;
			}
			else {
				return true;
			}
		}
		else {
			if (gamePlayerList[id].distanceToPlayer > 150 * 40) {
				return false;
			}
			else {
				return true;
			}
		}
	}
}

void gameLoop_aimBot() {
	//VMProtectBegin("gameLoop_aimBot");
	int aimtarget = -1;
	short keylist[] = { VK_LSHIFT , VK_LCONTROL , VK_CAPITAL,VK_RBUTTON};
	while (true) {
		//system("cls");
		Sleep(1);

		if (entityBaseAddress == 0 || entityPlayerAddress == 0 || hk_aimbot == 0) {
			aimtarget = -1;
			continue;
		}
		
		if (gamelocalEntity.Position.z> 13000.0f)continue;

		/*if (game_scene == 2144) {
			if (gamelocalEntity.Position.z> 21000.0f)continue;
		}
		else if (game_scene >= 1900 && game_scene <= 1907) {
			if (gamelocalEntity.Position.z> 13000.0f)continue;
		}*/

		if (aimtarget != -1) {
			gamePlayerList[aimtarget].EnableGlow(g_sock_2,glow_color_aimbot);
		}

		if ((GetAsyncKeyState(keylist[hk_aimbot-1])& 0x8000) && aimtarget != -1) {
			if (!CheckAimTargetValid(aimtarget)) {
				aimtarget = -1;
				goto getAimTarget;
			}

			int i = 0;
			for (i = 0; i < sizeof(weapon_sniper_list) / 4; i++) {
				if (gamelocalEntity.CurrentWeaponId == weapon_sniper_list[i]) {
					i = 1;
					break;
				}
			}

			Vector BonePosition = Vector();
			uintptr_t boneArray = *(uintptr_t*)(gamePlayerList[aimtarget].EntityBufer + OFFSET_BONES);
			uint32_t boneloc;
			if (gamelocalEntity.isZoom) {
				if (i == 1) {
					boneloc = (8 * 0x30);
				}
				else {
					boneloc = (7 * 0x30);
				}
				
			}
			else {
				boneloc = (3 * 0x30);
			}
			 
			struct Bone entityBone = {};
			entityBone = driver::read<struct Bone>(g_sock_2, g_pid, boneArray + boneloc);

			BonePosition.x = gamePlayerList[aimtarget].Position.x + entityBone.x;
			BonePosition.y = gamePlayerList[aimtarget].Position.y + entityBone.y*0.5f;
			BonePosition.z = gamePlayerList[aimtarget].Position.z + entityBone.z;

			/*std::cout << gamelocalEntity.CurrentBulletSpeed << "\n";
			std::cout << gamelocalEntity.CurrentBulletGravity << "\n";*/
			
			if (i == 1 || gamelocalEntity.CurrentWeaponId == HandleWeaponID::G7_SCOUT || gamelocalEntity.CurrentWeaponId == HandleWeaponID::WINGMAN) {

				if (gamelocalEntity.CurrentBulletSpeed > 1.f)
				{
					float bulletTime = gamePlayerList[aimtarget].distanceToPlayer / gamelocalEntity.CurrentBulletSpeed;
					BonePosition.z += (gamelocalEntity.CurrentBulletGravity * 0.5f) * (bulletTime * bulletTime);
				}
			}

			QAngle CalculatedAngles = Math::CalcAngle(gamelocalEntity.CameraPos, BonePosition);

			QAngle Delta = CalculatedAngles;

			//std::cout << Delta.x << " " << Delta.y << "\n";

			Delta.x -= gamelocalEntity.ViewAngles.x;
			Delta.y -= gamelocalEntity.ViewAngles.y;


			Math::NormalizeAngles(CalculatedAngles);
			double fov = fabs(CalculatedAngles.y - gamelocalEntity.ViewAngles.y);
			double fov_vertical = fabs(CalculatedAngles.x - gamelocalEntity.ViewAngles.x);

			bool aim = true;
			if (gamePlayerList[aimtarget].distanceToPlayer > 150 * 40) {
				if (fov > 5 && fov < 355) {
					aim = false;
				}
				if (fov_vertical > 5) {
					aim = false;
				}
			}
			else if (gamePlayerList[aimtarget].distanceToPlayer > 100*40) {
				if (fov > 10 && fov < 350) {
					aim = false;
				}
				if (fov_vertical > 8) {
					aim = false;
				}
			}else if (gamePlayerList[aimtarget].distanceToPlayer > 50 * 40) {
				if (fov > 20 && fov < 340) {
					aim = false;
				}
				if (fov_vertical > 10) {
					aim = false;
				}
			}
			else {
				if (fov > 30 && fov < 330) {
					aim = false;
				}
				if (fov_vertical > 20) {
					aim = false;
				}
			}

			Vector2D RecoilVec = *(Vector2D*)(gamelocalEntity.EntityBufer + OFFSET_AIMPUNCH);
			if (i!=1) {
				/*std::cout << RecoilVec.x << "\n";
				std::cout << RecoilVec.y << "\n";*/
				if (RecoilVec.x != 0 || RecoilVec.y != 0) {
					Delta.x -= RecoilVec.x;
					Delta.y -= RecoilVec.y;
				}
			}
			else {
				if (RecoilVec.x != 0 || RecoilVec.y != 0) {
					Delta.x -= RecoilVec.x*0.05f;
					Delta.y -= RecoilVec.y*0.05f;
				}
			}
			
			

			Math::NormalizeAngles(Delta);
			double smooth = 30.0f;

			if ((fov > 15 && fov < 345)|| fov_vertical > 15) {
				smooth = 10.0f;
			}
			else if ((fov > 5 && fov < 355) || fov_vertical > 5) {
				smooth = 20.0f;
			}

			if (gamePlayerList[aimtarget].distanceToPlayer <= 30 * 40) {
				smooth = 8.0f;
			}

			if (gamePlayerList[aimtarget].distanceToPlayer > 10 * 40 && hk_aimbotsoomth) {
				if ((fov > 2 && fov < 358) || fov_vertical > 2) {

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
			}


			Delta.x += gamelocalEntity.ViewAngles.x;
			Delta.y += gamelocalEntity.ViewAngles.y;

			Math::NormalizeAngles(Delta);
			if (aim) {
				/*if (gamelocalEntity.isZoom && i == 1) {
					Vector2D DynamicViewAngle = *(Vector2D*)(gamelocalEntity.EntityBufer + OFFSET_DYNAMIC_VIEWANGLES);
					if (fabs(DynamicViewAngle.x - Delta.x)< 1 && fabs(DynamicViewAngle.y - Delta.y)<1) {
						Delta.x -= (DynamicViewAngle.x - Delta.x)*0.8f;
						Delta.y -= (DynamicViewAngle.y - Delta.y)*0.8f;
						Math::NormalizeAngles(Delta);
					}
					std::cout << Delta.x << " " << Delta.y << "\n";
					driver::write<SVector>(g_sock_2, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES, SVector(Delta));
				}
				else {*/
					
					driver::write<SVector>(g_sock_2, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES, SVector(Delta));
				//}
				//driver::write<SVector>(g_sock_2, g_pid, entityPlayerAddress + OFFSET_VIEWANGLES, SVector(Delta));
			}
			
		}
		else {
		getAimTarget:
			int closest_to_crosshair = -1;
			float closet_distance = g_width*g_width;
			int closest_player = -1;
			float closet_pdis = 100000;
			for (int i = 0; i < 60; i++) {
				if (!CheckAimTargetValid(i))continue;

				Vector BonePosition = Vector();
				uintptr_t boneArray = *(uintptr_t*)(gamePlayerList[i].EntityBufer + OFFSET_BONES);
				uint32_t boneloc = (5 * 0x30);
				struct Bone entityBone = {};
				entityBone = driver::read<struct Bone>(g_sock_2, g_pid, boneArray + boneloc);

				BonePosition.x = gamePlayerList[i].Position.x + entityBone.x;
				BonePosition.y = gamePlayerList[i].Position.y + entityBone.y;
				BonePosition.z = gamePlayerList[i].Position.z + entityBone.z;

				Vector sc = Vector();
				WorldToScreen(BonePosition,ViewMartix.matrix,g_width,g_height,sc);

				/*std::cout << BonePosition.x << "\n";
				std::cout << BonePosition.y << "\n";
				std::cout << BonePosition.z << "\n";*/

				if (sc.x <= 0 || sc.y <=0) {
					continue;
				}

				if (gamePlayerList[i].distanceToPlayer < closet_pdis) {
					closet_pdis = gamePlayerList[i].distanceToPlayer;
					closest_player = i;
				}

				float nowdistance = pow(fabs(sc.x - g_width / 2), 2) + pow(fabs(sc.y - g_height / 2), 2);

				if (nowdistance > pow(g_height / 2, 2))continue;

				if (nowdistance < closet_distance) {
					closet_distance = nowdistance;
					closest_to_crosshair = i;
				}
				
			}
			if (closet_pdis < 400) {
				aimtarget = closest_player;
			}
			else {
				aimtarget = closest_to_crosshair;
			}
		}

	}
	//VMProtectEnd();
}

void gameLoop_getEntity() {
	//VMProtectBegin("gameLoop_getEntity");

	bool sendname = false;
	memset(gamelocalEntity.name, 0, 32);

	while (true) {
		Sleep(1);
		//system("cls");
		game_scene = driver::read<int>(g_sock_1, g_pid, g_add_gamebase + OFFSET_GAME_SCENE);


		if (!game_scene || game_scene == 13 ) {
			continue;
		}

		uint64_t add_viewRenderer = driver::read<uint64_t>(g_sock_1, g_pid, g_add_gamebase + OFFSET_RENDER);
		uint64_t add_viewMatrix = driver::read<uint64_t>(g_sock_1, g_pid, add_viewRenderer + OFFSET_MATRIX);
		ViewMartix = driver::read<Matrix>(g_sock_1, g_pid, add_viewMatrix);

		entityBaseAddress = driver::read<uint64_t>(g_sock_1, g_pid, g_add_entitylist);
		entityPlayerAddress = driver::read<uint64_t>(g_sock_1, g_pid, g_add_entitylocal);
		if (entityBaseAddress == 0 || entityPlayerAddress == 0) {
			continue;
		}
		gamelocalEntity.Address = entityPlayerAddress;
		gamelocalEntity.GetEntity(g_sock_1);
		gamelocalEntity.GetDetail(g_sock_1);
		gamelocalEntity.GetDetailEx(g_sock_1);
		gamelocalEntity.GetLocalEntityExtraInfo(g_sock_1);
		
		if (!sendname && gamelocalEntity.name[0]) {
			sendname = true;
			char buf[32] = "[01]";
			strcat(buf, gamelocalEntity.name);
			send(valid_server, buf,strlen(buf),0);
		}

		//std::cout << "InGetEntity \n HP:" << gamelocalEntity.Health << " AP:" << gamelocalEntity.Shield << "\n";
		for (int i = 0; i < 60;i++) {
			gamePlayerList[i].Id = i+1;
			DWORD64 tmpAddress = g_add_entitylist + (i + 1) * 32;
			tmpAddress = driver::read<uint64_t>(g_sock_1, g_pid, tmpAddress);
			if (tmpAddress == 0) {
				gamePlayerList[i].Clean();
				continue;
			}
			gamePlayerList[i].Address = tmpAddress;

			if (gamePlayerList[i].Address == entityPlayerAddress) {
				gamelocalEntity.Id = i+1;
				gamePlayerList[i].Clean();
				continue;
			}
			gamePlayerList[i].GetEntity(g_sock_1);
			gamePlayerList[i].GetDetail(g_sock_1);
			if (!gamePlayerList[i].isPlayer) {
				gamePlayerList[i].Clean();
				continue;
			}
			gamePlayerList[i].GetDistanceToPlayer(&gamelocalEntity);
			if (gamePlayerList[i].distanceToPlayer <= 10000) {
				gamePlayerList[i].GetDetailEx(g_sock_1);
			}
			
		}
	}
	//VMProtectEnd();
}

void ClearnAndExit() {
	UnloadDriver(L"IntelSCSIDisk");
	RestoreRegister();

	DestroyWindow(hwnd_loginui);
	DestroyWindow(ov_hwnd);

	closesocket(valid_server);
	WSACleanup();

	ExitProcess(0);
}

void WaitingforGame() {
	VMProtectBegin("WaitingforGame");

	char decrytcode[7] = { 0 };
	memmove(decrytcode, valid_code,6);
	RC4(processdata, decrytcode, processdatalen-1);

	wchar_t *targetprocess = AnsiToUnicode(processdata);

	cout << "正在等待游戏进程...\n";
	while (g_pid == 0) {
		if (debug) {
			g_pid = GetProcessPid(L"EasyAntiCheat_launcher.exe");
		}
		else {
			//g_pid = GetProcessPid(L"r5apex.exe");
			g_pid = GetProcessPid(targetprocess);
		}
		Sleep(500);
	}
	delete[]targetprocess;
	cout << "已找到游戏进程！\n";
	cout << "正在等待游戏窗口...\n";
	while (!g_hwnd) {
		g_hwnd = FindWindowA("Respawn001", "Apex Legends");
	}
	cout << "游戏窗口已找到！\n窗口句柄：" << g_hwnd << endl;
	while (IsIconic(g_hwnd)) {
		Sleep(500);
	}

	RECT Rect;
	GetClientRect(g_hwnd, &Rect);
	g_width = Rect.right - Rect.left;
	g_height = Rect.bottom - Rect.top;
	cout << "游戏分辨率 " << (int)g_width << "x" << (int)g_height << endl;
	g_add_gamebase = driver::get_process_base_address(g_sock_1, g_pid);
	if (g_add_gamebase == 0) {
		cout << "获取游戏进程基址失败！\n";
		cout << "...按回车键退出...";
		getchar();
		exit(0);
	}
	printf("获取游戏进程基地址成功\nBase Address :%p\n", (void*)g_add_gamebase);

	VMProtectEnd();
}



void initialization() {
	VMProtectBegin("initialization");

	driver::initialize();
	int times = 0;
	int n_regport = GetDrvPort();
	switch (n_regport) {
	case -1:
		cout << "获取信息失败\n";
		cout << "...按回车键退出...";
		getchar();
		exit(0);
		break;
	case -2:
		retry:
		srand(unsigned(time(0)));
		int minport = 5000, maxport = 60000;
		n_regport = rand() % (maxport + 1 - minport) + minport;
		if (!SetDrvPort(n_regport)) {
			cout << "设置信息失败\n";
			cout << "...按回车键退出...";
			getchar();
			exit(0);
		}
		/*WCHAR drvBinaryPath[256];
		GetCurrentDirectory(256, drvBinaryPath);
		wcscat(drvBinaryPath, L"\\SOCKET.sys");*/

		WCHAR drvBinaryPath[MAX_PATH];
		int Strlen = GetTempPath(MAX_PATH, drvBinaryPath);
		if (Strlen < 1) {
			cout << "设置信息失败2\n";
			cout << "...按回车键退出...";
			getchar();
			exit(0);
		}

		WCHAR drv_file_name[16] = { 0 };
		GetRandomString(12, drv_file_name);

		wcscat(drvBinaryPath, drv_file_name);
		HANDLE file = CreateFile(drvBinaryPath, GENERIC_ALL, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
		if (file == INVALID_HANDLE_VALUE) {
			file = CreateFile(drvBinaryPath, GENERIC_ALL, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_SYSTEM, NULL);
			if (file == INVALID_HANDLE_VALUE) {
				cout << "设置信息失败3\n";
				cout << "...按回车键退出...";
				getchar();
				exit(0);
			}

			DWORD sizeWriten, dwSize = sizeof(drvBuffer);
			BYTE* pByte = new BYTE[dwSize + 1];
			memcpy(pByte, drvBuffer, dwSize);

			BYTE *tmpBuf = pByte;
			do {
				WriteFile(file, tmpBuf, dwSize, &sizeWriten, NULL);
				dwSize -= sizeWriten;
				tmpBuf += sizeWriten;

			} while (sizeWriten>0);
			delete[] pByte;

		}

		CloseHandle(file);

		WCHAR drv_srv_name[20] = { 'Z' }, drv_srv_dname[20] = { 'Y' };
		GetRandomString(16, drv_srv_name+1);
		GetRandomString(12, drv_srv_dname+1);

		if (!LoadDriver(drv_srv_name, drv_srv_dname, drvBinaryPath)) {
			cout << "驱动加载失败！\n";
			cout << "...按回车键退出...";
			getchar();
			exit(0);
		}
		UnloadDriver(drv_srv_name);
		DeleteFile(drvBinaryPath);
		cout << "驱动加载成功！\n";
		break;
	}
	server_port = n_regport;

	g_sock_1 = driver::connect();
	g_sock_2 = driver::connect();
	g_sock_3 = driver::connect();
	times++;
	if (g_sock_1 == INVALID_SOCKET || g_sock_2 == INVALID_SOCKET || g_sock_3 == INVALID_SOCKET) {
		if (times < 2) {
			goto retry;
		}
		cout << "无法与内核驱动取得通讯！\n";
		cout << "...按回车键退出...";
		getchar();
		exit(0);
	}
	cout << "连接驱动成功！\n";

	VMProtectEnd();
}

int GetDrvPort() {

	char path[] = "SOFTWARE\\zyStudio\\KernelMem";
	char value[] = "Channel";
	unsigned long type = REG_DWORD;
	int cb = sizeof(int),port;
	HKEY mainKeyHandle;
	if (RegCreateKeyA(HKEY_LOCAL_MACHINE, path, &mainKeyHandle) != ERROR_SUCCESS) {
		return -1;
	}

	if (RegQueryValueExA(mainKeyHandle, value, NULL, &type, (LPBYTE)&port, (LPDWORD)&cb) != ERROR_SUCCESS) {
		return -2;
	}
	RegCloseKey(mainKeyHandle);
	return port;
}

bool SetDrvPort(int port) {
	char path[] = "SOFTWARE\\zyStudio\\KernelMem";
	char value[] = "Channel";
	int cb = sizeof(int);
	HKEY mainKeyHandle;
	if (RegCreateKeyA(HKEY_LOCAL_MACHINE, path, &mainKeyHandle) != ERROR_SUCCESS) {
		return false;
	}

	if (RegSetValueExA(mainKeyHandle, value, NULL, REG_DWORD, (LPBYTE)&port, cb) != ERROR_SUCCESS) {
		return false;
	}
	RegCloseKey(mainKeyHandle);
	return true;
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

//将单字节char*转化为宽字节wchar_t*  
inline wchar_t* AnsiToUnicode(const char* szStr)
{
	int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
	if (nLen == 0)
	{
		return NULL;
	}
	wchar_t* pResult = new wchar_t[nLen];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
	return pResult;
}

//将宽字节wchar_t*转化为单字节char*  
inline char* UnicodeToAnsi(const wchar_t* szStr)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
	if (nLen == 0)
	{
		return NULL;
	}
	char* pResult = new char[nLen];
	WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
	return pResult;
}