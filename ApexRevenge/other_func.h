#pragma once
#include "vector.h"
#include "constList.h"
#include "d3d9.h"
#include <math.h>

bool WorldToScreen(Vector from, float * m_vMatrix, int targetWidth, int targetHeight, Vector & to);

D3DCOLOR GetItemColor(int Id);

void GetWeaponNameInHands(int id, char * name);

BOOL IsWin10System();

char * RC4(char * C, char * key, int textlen);

char * utf8_to_ansi(char * strUTF8);

