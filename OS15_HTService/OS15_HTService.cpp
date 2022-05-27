#define _CRT_SECURE_NO_WARNINGS
#define OS13HANDEL void*
#include <Windows.h>
#include <iostream>
#include "../OS13_HTCOM/Element.h"
#include "../OS13_HTCOM/HashTable.h"
#include "../OS13_HTCOMLIB/OS13.h"
#include "Service.h"
using namespace std;

WCHAR ServiceName[] = SERVICENAME;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
SERVICE_STATUS ServiceStatus;
OS13HANDEL h1 = OS13::Init();
HashTable::HTHANDLE* HT = OS13::HashTableLib::Open(h1, "D:\\1\\Lab_15\\Debug\\HashTable.ht");

void trace(const char* msg, int r) { std::ofstream out; out.open(TRACEPATH, r); out << msg << "\n"; out.close(); }

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
	setlocale(LC_ALL, "Russian");
	char temp[121];

	ServiceStatus.dwServiceType = SERVICE_WIN32;
	ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	ServiceStatus.dwWaitHint = 0;

	if (!(hServiceStatusHandle = RegisterServiceCtrlHandler(ServiceName, ServiceHandler)))
	{
		sprintf_s(temp, "RegisterServiceCtrlHandler failed, error code = %d\n", GetLastError());
		trace(temp);
	}
	else
	{
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
		{
			sprintf_s(temp, "\nSetServiceStatus failed, error code = %d\n", GetLastError());
			trace(temp);
		}
		else
		{
			
			
			
			trace(HT->FileName, std::ofstream::out);

			while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
			{
			}
		}
	}
}


VOID WINAPI ServiceHandler(DWORD fdwControl)
{
	setlocale(LC_ALL, "Russian");
	char temp[121];
	switch (fdwControl)
	{
	case SERVICE_CONTROL_STOP:
		
		//if (!OS13::HashTableLib::Snap(h1, HT)) throw "Error snap";
		if (!OS13::HashTableLib::Close(h1, HT)) throw "Error close";
		OS13::Dispose(h1);
		sprintf_s(temp, "service stopped\n");
		trace(temp);
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		ServiceStatus.dwCheckPoint = 0;
		ServiceStatus.dwWaitHint = 0;
		//if (!OS13::HashTableLib::Snap(h1, HT)) throw "Error snap";
		if (!OS13::HashTableLib::Close(h1, HT)) throw "Error close";
		OS13::Dispose(h1);
		sprintf_s(temp, "service stopped\n");
		trace(temp);
		break;
	case SERVICE_CONTROL_PAUSE:
		ServiceStatus.dwCurrentState = SERVICE_PAUSED;
		break;
	case SERVICE_CONTROL_CONTINUE:
		ServiceStatus.dwCurrentState = SERVICE_RUNNING;
		break;
	case SERVICE_CONTROL_INTERROGATE: break;
	default:
		sprintf_s(temp, "Unrecognized opcode %d\n", fdwControl);
		trace(temp);
	};
	if (!SetServiceStatus(hServiceStatusHandle, &ServiceStatus))
	{
		sprintf_s(temp, "\nSetServiceStatus failed, error code = %d\n", GetLastError());
		trace(temp);
	}
}