#pragma once
#include <windows.h>
#include <iostream>
#include <fstream>

#define SERVICENAME L"OS15"
#define TRACEPATH "D:\\Service.log"

VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv);
VOID WINAPI ServiceHandler(DWORD fdwControl);

void trace(const char* msg, int r = std::ofstream::app);