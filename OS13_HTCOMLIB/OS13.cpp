#pragma once
#include "pch.h"
#include "framework.h"
#include <iostream>
#include "OS13.h"
#include <objbase.h>
#include <tlhelp32.h>

// {0A4873C2-9722-4806-BA51-027A8B893BFA}
static const GUID CLSID_CA =
{ 0xa4873c2, 0x9722, 0x4806, { 0xba, 0x51, 0x2, 0x7a, 0x8b, 0x89, 0x3b, 0xfa } };


// {1340C4DE-7967-4F98-8587-A7AB9A5CD894}
static const GUID IID_IHashTable =
{ 0x1340c4de, 0x7967, 0x4f98, { 0x85, 0x87, 0xa7, 0xab, 0x9a, 0x5c, 0xd8, 0x94 } };

interface IHashTable :IUnknown {

	virtual HRESULT __stdcall  Create   //  ������� HT             
	(
		int	  Capacity,					   // ������� ���������
		int   SecSnapshotInterval,		   // ������������� ���������� � ���.
		int   MaxKeyLength,                // ������������ ������ �����
		int   MaxPayloadLength,            // ������������ ������ ������
		const char HTUsersGroup[512],
		const char  FileName[512],          // ��� ����� 
		OUT HashTable::HTHANDLE* hthandle
	) = 0;

	virtual HRESULT __stdcall  Open(const char FileName[512], const char HTUser[256], const char HTPassword[256], HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Open(const char FileName[512], HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Snap(const HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Close(HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Insert(const HashTable::HTHANDLE* hthandle, const HashTable::Element* element) = 0;

	virtual HRESULT __stdcall  Get(const HashTable::HTHANDLE* hthandle, const HashTable::Element* element, HashTable::Element* retelement) = 0;

	virtual HRESULT __stdcall  Delete(const HashTable::HTHANDLE* hthandle, const HashTable::Element* element) = 0;

	virtual HRESULT __stdcall  Update(const HashTable::HTHANDLE* hthandle, const HashTable::Element* oldelement, const void* newpayload, int newpayloadlength) = 0;

	virtual HRESULT __stdcall  IsProcessPresent(wchar_t* szExe) = 0;

	virtual HRESULT __stdcall GetLastErrorr(HashTable::HTHANDLE* ht) = 0;

};

bool OS13::HashTableLib::IsProcessPresentClose(wchar_t* szExe)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hSnapshot, &pe);

	if (!_wcsicmp((wchar_t*)&pe.szExeFile, szExe))
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID);
		if (TerminateProcess(hProcess, 0)) return true;
		else throw "�� ���� ����� �������";
		return true;
	}

	while (Process32Next(hSnapshot, &pe))
	{
		if (!_wcsicmp((wchar_t*)&pe.szExeFile, szExe))
		{
			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pe.th32ProcessID);
			if (TerminateProcess(hProcess, 0)) return true;
			else throw "�� ���� ����� �������";
			return true;
		}
	}

	return false;
}

void OS13::HashTableLib::print(const HashTable::Element* element)
{
	std::cout << "Key: " << *(int*)element->key << "\n"
		<< "Key lenght: " << element->keylength << "\n"
		<< "Payload: " << *(int*)element->payload << "\n"
		<< "Payload lenght: " << element->payloadlength << "\n";
}

IHashTable* pIHashTable = nullptr;

OS13HANDEL OS13::Init() {
	IUnknown* pIUnknown = NULL;
	CoInitialize(NULL);                        // ������������� ���������� OLE32
	HRESULT hr0 = CoCreateInstance(CLSID_CA, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown, (void**)&pIUnknown);
	if (SUCCEEDED(hr0))
	{
		return pIUnknown;
	}
	else {
		throw (int)hr0;
		return NULL;
	}
}


void OS13::Dispose(OS13HANDEL h) {
	((IUnknown*)h)->Release();
	CoFreeUnusedLibraries();
}


HashTable::HTHANDLE* OS13::HashTableLib::Create(OS13HANDEL h,
	int	  Capacity,					   // ������� ���������
	int   SecSnapshotInterval,		   // ������������� ���������� � ���.
	int   MaxKeyLength,                // ������������ ������ �����
	int   MaxPayloadLength,            // ������������ ������ ������
	const char HTUsersGroup[512],
	const char  FileName[512]          // ��� ����� 
)
{
	HashTable::HTHANDLE* ht = new HashTable::HTHANDLE();
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Create(Capacity,SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, HTUsersGroup,FileName, ht);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return NULL;
		}
		else {
			pIHashTable->Release();
			return ht;
		}
	}
	else {

		throw (int)hr0;
		return NULL;
	}
}

HashTable::HTHANDLE* OS13::HashTableLib::Open(OS13HANDEL h,
	const char  FileName[512]          // ��� ����� 
)
{
	try
	{
		HashTable::HTHANDLE* ht = new HashTable::HTHANDLE();
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHashTable->Open(FileName, ht);
			if (!SUCCEEDED(hr1)) {
				pIHashTable->Release();
				return ht;
			}
			else {
				pIHashTable->Release();
				return ht;
			}
		}
		else {

			throw (int)hr0;
			return NULL;
		}
	}
	catch (const char* error)
	{
		std::cout << "-- Error: " << error << std::endl;
	}
}

HashTable::HTHANDLE* OS13::HashTableLib::Open(OS13HANDEL h,
	const char  FileName[512],          // ��� ����� 
	const char HTUser[256], 
	const char HTPassword[256]
)
{
	try
	{
		HashTable::HTHANDLE* ht = new HashTable::HTHANDLE();
		HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
		if (SUCCEEDED(hr0))
		{
			HRESULT hr1 = pIHashTable->Open(FileName, HTUser, HTPassword, ht);
			if (!SUCCEEDED(hr1)) {
				pIHashTable->Release();
				HashTable::HTHANDLE* ht1 = new HashTable::HTHANDLE();
				return ht1;
			}
			else {
				pIHashTable->Release();
				
				return ht;
			}
		}
		else {

			return NULL;
		}
	}
	catch (const char* error)
	{
		std::cout << "-- Error: " << error << std::endl;
	}
}

BOOL OS13::HashTableLib::Insert(OS13HANDEL h, const HashTable::HTHANDLE* hthandle, const HashTable::Element* element)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Insert(hthandle, element);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}

HashTable::Element* OS13::HashTableLib::Get(OS13HANDEL h, const HashTable::HTHANDLE* hthandle, const HashTable::Element* element)
{
	HashTable::Element* el = new HashTable::Element();
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Get(hthandle, element, el);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return NULL;
		}
		else {
			pIHashTable->Release();
			return el;
		}
	}
	else {

		throw (int)hr0;
		return NULL;
	}
}

BOOL OS13::HashTableLib::Delete(OS13HANDEL h, const HashTable::HTHANDLE* hthandle, const HashTable::Element* element)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Delete(hthandle, element);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}

BOOL OS13::HashTableLib::Update(OS13HANDEL h, const HashTable::HTHANDLE* hthandle, const HashTable::Element* element, const void* newpayload, int newpayloadlength)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Update(hthandle, element, newpayload, newpayloadlength);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}

BOOL OS13::HashTableLib::Snap(OS13HANDEL h, const HashTable::HTHANDLE* hthandle)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Snap(hthandle);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}

bool OS13::HashTableLib::IsProcessPresent(OS13HANDEL h, wchar_t* szExe)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->IsProcessPresent(szExe);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}

BOOL OS13::HashTableLib::Close(OS13HANDEL h, HashTable::HTHANDLE* hthandle)
{
	HRESULT hr0 = ((IUnknown*)h)->QueryInterface(IID_IHashTable, (void**)&pIHashTable);
	if (SUCCEEDED(hr0))
	{
		HRESULT hr1 = pIHashTable->Close(hthandle);
		if (!SUCCEEDED(hr1)) {
			pIHashTable->Release();
			return false;
		}
		else {
			pIHashTable->Release();
			return true;
		}
	}
	else {

		throw (int)hr0;
		return false;
	}
}