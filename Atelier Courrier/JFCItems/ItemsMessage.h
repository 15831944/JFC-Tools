#pragma once
#include "StdAfx.h"
#include "..\Config\BuildConfig.h"
#if defined(BUILD_FOR_ATELIER_INTERNET)
/// <summary>
/// Auto generated ItemsMessage class from C:\Work\AtelierPresse\Version 2.5.1\JFCItems\ItemsLocale.xls
/// </summary>
class AFX_EXT_CLASS ItemsMessage
{
public:
    static const char* Languages[];
    static const char** MessageMap[];

    static const void SetCurrentLanguage(int current);

    static const char* GetString(int language, int id);

    static const char* GetString(int id);

private:
    static int CurrentLanguage;
    static const char* Message_en[];
    static const char* Message_fr[];
    static const char* Message_it[];

public:
    static const int MSG_IDS_ENSEMBLE = 0;

};

#endif
