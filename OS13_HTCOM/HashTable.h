#pragma once
#include <Windows.h>
#include "Element.h"

namespace HashTable
{
	__declspec(dllexport) struct HTData
	{
		HTData();
		HTData(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char HTUsersGroup[512]);
		int Capacity;               // ������� ��������� � ���������� ��������� 
		int SecSnapshotInterval;    // ������������� ���������� � ���. 
		int MaxKeyLength;           // ������������ ����� �����
		int MaxPayloadLength;       // ������������ ����� ������
		char HTUsersGroup[512];
	};

	__declspec(dllexport) struct HTHANDLE    // ���� ���������� HT
	{
		HTHANDLE();
		HTHANDLE(int Capacity, int SecSnapshotInterval, int MaxKeyLength, int MaxPayloadLength, const char HTUsersGroup[512], const char FileName[512]);
		HTHANDLE(HTData* pHTData, const char FileName[512]);
		HANDLE hSnapThread;
		bool alive = true;
		HTData Data;
		int MaxContentSize;
		char    FileName[512];          // ��� ����� 
		HANDLE  File;                   // File HANDLE != 0, ���� ���� ������
		HANDLE  FileMapping;            // Mapping File HANDLE != 0, ���� mapping ������  
		LPVOID  Addr;                   // Addr != NULL, ���� mapview ��������  
		char    LastErrorMessage[512];  // ��������� �� ��������� ������ ��� 0x00  
		time_t  lastsnaptime;           // ���� ���������� snap'a (time()) 
		LPVOID keyAddr(int n) const;
		LPVOID payloadAddr(int n) const;
	};

};
