
#include <Windows.h>
#include <iostream>

#define SERVICENAME L"OS15"


char* errortxt(const char* mse, int code)
{
    char* buf = new char[512];
    sprintf_s(buf, 512, "%s: error code = %d\n", mse, code);
    return buf;
}


SC_HANDLE schService = NULL, schSCManager = NULL;


int main()
{
    try
    {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
        if (!schSCManager) throw errortxt("OpenSCManager", GetLastError());

        schService = OpenService(schSCManager, SERVICENAME, SERVICE_ALL_ACCESS);
        if (!schService) throw errortxt("OpenService", GetLastError());

        if (!DeleteService(schService)) throw errortxt("DeleteService", GetLastError());
    }
    catch (char* txt) { std::cout << txt << "\n"; }
    if (schSCManager) CloseServiceHandle(schSCManager);
    if (schService) CloseServiceHandle(schService);

    return 0;
}