#pragma once

#define INTEROP_EXPORT extern "C" __declspec(dllexport)

INTEROP_EXPORT void SetupIniFile(LPCTSTR iniFile);

INTEROP_EXPORT LPCTSTR GetSourcesPath();

INTEROP_EXPORT LPCTSTR GetConfiguration(LPCTSTR keyVal);

INTEROP_EXPORT LPCTSTR GetIniValue(LPCTSTR sectionVal, LPCTSTR keyVal);

INTEROP_EXPORT void ShowRepItem(int lang, int idSrc, int idTerr, int idPoids);

INTEROP_EXPORT bool LoadTerrain(int idSrc, int idTerr, int idPoids);

INTEROP_EXPORT int GetIndivCount();

INTEROP_EXPORT int GetPoids(double* buffer, int size);

INTEROP_EXPORT void MoveFirstItem();

INTEROP_EXPORT bool IsValidItem();

INTEROP_EXPORT void MoveNextItem();

typedef struct { 
	int id; 
	char * label; 
} Item;

INTEROP_EXPORT void GetItem(Item* item);

INTEROP_EXPORT int GetItemIndivVector(int idItem, int* buffer, int size);

//
//INTEROP_EXPORT LPCTSTR TestString();