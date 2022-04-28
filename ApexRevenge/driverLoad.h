#pragma once
#include <Windows.h>

bool LoadDriver(PWCHAR drvServiceName, PWCHAR drvDisplayName, PWCHAR drvFilePath);
bool UnloadDriver(PWCHAR drvServiceName);