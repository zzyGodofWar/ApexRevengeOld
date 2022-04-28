#include "driverLoad.h"

bool LoadDriver(PWCHAR drvServiceName, PWCHAR drvDisplayName, PWCHAR drvFilePath) {
	SC_HANDLE schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schManager) {
		return false;
	}

	SC_HANDLE schService = OpenService(schManager, drvServiceName, SERVICE_ALL_ACCESS);
	if (schService != NULL) {
		SERVICE_STATUS SvrSta;
		ControlService(schService, SERVICE_CONTROL_STOP, &SvrSta);
		DeleteService(schService);
		CloseServiceHandle(schService);

	}

	schService = CreateService(schManager, drvServiceName, drvDisplayName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, drvFilePath, NULL, NULL, NULL, NULL, NULL);

	if (schService == NULL) {
		if (ERROR_SERVICE_EXISTS != GetLastError()) {
			CloseServiceHandle(schManager);
			return false;
		}
	}
	CloseServiceHandle(schManager);
	schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schManager) {
		return false;
	}
	schService = OpenService(schManager, drvServiceName, SERVICE_ALL_ACCESS);
	if (!schService) {
		CloseServiceHandle(schManager);
		return false;
	}

	bool r = StartService(schService, 0, 0);
	if (!r) {
		return false;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(schManager);
	return true;
}

bool UnloadDriver(PWCHAR drvServiceName) {
	SC_HANDLE schManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schManager) {
		return false;
	}
	SC_HANDLE schService = OpenService(schManager, drvServiceName, SERVICE_ALL_ACCESS);
	if (schService == NULL) {
		CloseServiceHandle(schManager);
		return false;
	}
	SERVICE_STATUS SvrSta;
	ControlService(schService, SERVICE_CONTROL_STOP, &SvrSta);
	DeleteService(schService);
	CloseServiceHandle(schManager);
	CloseServiceHandle(schService);
	return true;
}