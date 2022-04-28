#include "other_func.h"
#include "constList.h"

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

	x += 0.5 * to.x * targetWidth;// +0.5;
	y -= 0.5 * to.y * targetHeight;// +0.5;

	to.x = x;
	to.y = y;
	to.z = 0;
	return true;
}

D3DCOLOR GetItemColor(int Id) {
	for (int i = 0; i < sizeof(itemColor_red) / 4; i++) {
		if (Id == itemColor_red[i]) {
			return d3dcolor_rate_red;
		}
	}
	for (int i = 0; i < sizeof(itemColor_gold) / 4; i++) {
		if (Id == itemColor_gold[i]) {
			return d3dcolor_rate_gold;
		}
	}
	for (int i = 0; i < sizeof(itemColor_purple) / 4; i++) {
		if (Id == itemColor_purple[i]) {
			return d3dcolor_rate_purple;
		}
	}
	for (int i = 0; i < sizeof(itemColor_blue) / 4; i++) {
		if (Id == itemColor_blue[i]) {
			return d3dcolor_rate_blue;
		}
	}
	for (int i = 0; i < sizeof(itemColor_light) / 4; i++) {
		if (Id == itemColor_light[i]) {
			return d3dcolor_ammo_light1;
		}
	}
	for (int i = 0; i < sizeof(itemColor_heavy) / 4; i++) {
		if (Id == itemColor_heavy[i]) {
			return d3dcolor_ammo_heavy1;
		}
	}
	for (int i = 0; i < sizeof(itemColor_shotgun) / 4; i++) {
		if (Id == itemColor_shotgun[i]) {
			return d3dcolor_ammo_shotgun1;
		}
	}
	for (int i = 0; i < sizeof(itemColor_energy) / 4; i++) {
		if (Id == itemColor_energy[i]) {
			return d3dcolor_ammo_energy1;
		}
	}
	for (int i = 0; i < sizeof(itemColor_sniper) / 4; i++) {
		if (Id == itemColor_sniper[i]) {
			return d3dcolor_ammo_sniper1;
		}
	}
	return d3dcolor_rate_white;
}

void GetWeaponNameInHands(int id,char *name) {
	switch (id) {
	case HandleWeaponID::BARE_HANDS:
		strcpy(name,"拳头");
		break;
	case HandleWeaponID::HAVOC:
		strcpy(name, "哈沃克");
		break;
	case HandleWeaponID::LSTAR:
		strcpy(name, "L-Star");
		break;
	case HandleWeaponID::KRABER:
		strcpy(name, "克雷贝尔");
		break;
	case HandleWeaponID::DEVOTION:
		strcpy(name, "专注轻机枪");
		break;
	case HandleWeaponID::PEACEKEEPER:
		strcpy(name, "和平捍卫者");
		break;
	case HandleWeaponID::SENTINEL:
		strcpy(name, "哨兵狙击枪");
		break;
	case HandleWeaponID::CHARGE_RIFLE:
		strcpy(name, "充能步枪");
		break;
	case HandleWeaponID::LONGBOW:
		strcpy(name, "长弓");
		break;
	case HandleWeaponID::TRIPLE_TAKE:
		strcpy(name, "三重狙击枪");
		break;
	case HandleWeaponID::REPEATER:
		strcpy(name, "30-30连发枪");
		break;
	case HandleWeaponID::WINGMAN:
		strcpy(name, "小帮手");
		break;
	case HandleWeaponID::SPITFIRE:
		strcpy(name, "喷火轻机枪");
		break;
	case HandleWeaponID::PROWLER:
		strcpy(name, "猎兽冲锋枪");
		break;
	case HandleWeaponID::HEMLOCK:
		strcpy(name, "赫姆洛克");
		break;
	case HandleWeaponID::FLATLINE:
		strcpy(name, "平行步枪");
		break;
	case HandleWeaponID::RE45:
		strcpy(name, "RE-45");
		break;
	case HandleWeaponID::P2020:
		strcpy(name, "P2020");
		break;
	case HandleWeaponID::R301:
		strcpy(name, "R301");
		break;
	case HandleWeaponID::R99:
		strcpy(name, "R99");
		break;
	case HandleWeaponID::ALTERNATOR:
		strcpy(name, "转换者冲锋枪");
		break;
	case HandleWeaponID::G7_SCOUT:
		strcpy(name, "G7侦查枪");
		break;
	case HandleWeaponID::MOZAMBIQUE:
		strcpy(name, "莫桑比克");
		break;
	case HandleWeaponID::EVA8_AUTO:
		strcpy(name, "EVA-8");
		break;
	case HandleWeaponID::MASTIFF:
		strcpy(name, "獒犬");
		break;
	case HandleWeaponID::VOLTSMG:
		strcpy(name, "电能冲锋枪");
		break;
	/*case 23:
		strcpy(name, "毒气罐子");
		break;
	case 25:
		strcpy(name, "防护罩");
		break;
	case 27:
		strcpy(name, "治疗无人机");
		break;
	case 45:
		strcpy(name, "电围栏");
		break;
	case 80:
		strcpy(name, "铝热剂");
		break;
	case 81:
		strcpy(name, "碎片手雷");
		break;
	case 82:
		strcpy(name, "电弧星");
		break;
	case 84:
		strcpy(name, "在打药");
		break;
	case 8:
		strcpy(name, "兰伯特机枪");
		break;
	case 53:
		strcpy(name, "搭建掩体");
		break;
	case 54:
		strcpy(name, "搭建炮台");
		break;
	case 43:
		strcpy(name, "恶灵传家宝");
		break;
	case 18:
		strcpy(name, "班加罗尔绝招");
		break;*/
	default:
		strcpy(name, "未知 ");
		char tid[8];
		itoa(id, tid, 10);
		strcat(name, tid);
		break;
	}

}



BOOL IsWin10System()
{
	//string与CString转换
	//string sPath = (LPCSTR)(CStringA)(strPath);
	//std::string vname;
	// 先判断是否为win8.1或win10
	typedef void(__stdcall*NTPROC)(DWORD*, DWORD*, DWORD*);
	HINSTANCE hinst = GetModuleHandleA("ntdll.dll");
	if (!hinst) {
		hinst = LoadLibraryA("ntdll.dll");
	}
	DWORD dwMajor, dwMinor, dwBuildNumber;
	NTPROC proc = (NTPROC)GetProcAddress(hinst, "RtlGetNtVersionNumbers");
	proc(&dwMajor, &dwMinor, &dwBuildNumber);

	if (dwMajor == 10 && dwMinor == 0)
	{
		return TRUE;
	}

	return FALSE;
}

char* RC4(char* C, char* key, int textlen)
{
	int S[256];
	int T[256];

	int  count = 0;
	count = strlen(key);
	int length = 0;
	length = textlen;

	int i, j, m = 0;

	for (i = 0; i < 256; i++)
	{
		S[i] = i;
		int tmp = i % count;
		T[i] = key[tmp];
	}

	for (i = 0; i < 256; i++)
	{
		m = (m + S[i] + T[i]) % 256;
		int tmp;
		tmp = S[m];
		S[m] = S[i];
		S[i] = tmp;
	}

	i = 0, j = 0;

	for (int p = 0; p < length; p++)
	{
		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;

		int k = S[(S[i] + S[j]) % 256];
		C[p] = C[p] ^ k;
	}

	return C;
}

char* utf8_to_ansi(char* strUTF8) {
	UINT nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, NULL, NULL);
	WCHAR *wszBuffer = new WCHAR[nLen + 1];

	nLen = MultiByteToWideChar(CP_UTF8, NULL, strUTF8, -1, wszBuffer, nLen);
	wszBuffer[nLen] = 0;
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, NULL, NULL, NULL, NULL);
	CHAR *szBuffer = new CHAR[nLen + 1];
	nLen = WideCharToMultiByte(936, NULL, wszBuffer, -1, szBuffer, nLen, NULL, NULL);
	szBuffer[nLen] = 0;
	strcpy(strUTF8, szBuffer);
	
	delete[]szBuffer;
	delete[]wszBuffer;
	return strUTF8;
}