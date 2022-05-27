#pragma once
#define OUT
#include <objbase.h>
#include "HashTable.h"

#define FNAME L"SDA.OS_14.HTCOM"
#define VINDX L"SDA.OS_14.1"
#define PRGID L"SDA.OS_14"



// {0A4873C2-9722-4806-BA51-027A8B893BFA}
static const GUID CLSID_CA =
{ 0xa4873c2, 0x9722, 0x4806, { 0xba, 0x51, 0x2, 0x7a, 0x8b, 0x89, 0x3b, 0xfa } };





// {1340C4DE-7967-4F98-8587-A7AB9A5CD894}
static const GUID IID_IHashTable =
{ 0x1340c4de, 0x7967, 0x4f98, { 0x85, 0x87, 0xa7, 0xab, 0x9a, 0x5c, 0xd8, 0x94 } };

interface IHashTable :IUnknown {

	virtual HRESULT __stdcall  Create   //  создать HT             
	(
		int	  Capacity,					   // емкость хранилища
		int   SecSnapshotInterval,		   // переодичность сохранения в сек.
		int   MaxKeyLength,                // максимальный размер ключа
		int   MaxPayloadLength,            // максимальный размер данных
		const char HTUsersGroup[512],
		const char  FileName[512],          // имя файла 
		OUT HashTable::HTHANDLE* hthandle
	) = 0;

	virtual HRESULT __stdcall  Open(const char FileName[512], const char HTUser[256], const char HTPassword[256],  HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Open(const char FileName[512], HashTable::HTHANDLE* hthandle) = 0;

	virtual HRESULT __stdcall  Snap(const HashTable::HTHANDLE * hthandle) = 0;

	virtual HRESULT __stdcall  Close(HashTable::HTHANDLE * hthandle) = 0;

	virtual HRESULT __stdcall  Insert(const HashTable::HTHANDLE * hthandle, const HashTable::Element * element ) = 0;

	virtual HRESULT __stdcall  Get(const HashTable::HTHANDLE * hthandle, const HashTable::Element * element, HashTable::Element* retelement) = 0;

	virtual HRESULT __stdcall  Delete(const HashTable::HTHANDLE * hthandle, const HashTable::Element * element) = 0;

	virtual HRESULT __stdcall  Update(const HashTable::HTHANDLE * hthandle, const HashTable::Element * oldelement, const void* newpayload, int newpayloadlength) = 0;

	virtual HRESULT __stdcall  IsProcessPresent(wchar_t* szExe) = 0;

	virtual HRESULT __stdcall GetLastErrorr(HashTable::HTHANDLE * ht) =0;

};