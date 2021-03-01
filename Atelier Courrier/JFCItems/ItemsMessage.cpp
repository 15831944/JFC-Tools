#include "StdAfx.h"
/// <summary>
/// Auto generated ItemsMessage class from C:\Work\AtelierPresse\Version 2.5.1\JFCItems\ItemsLocale.xls
/// </summary>

#include "ItemsMessage.h"

#if defined(BUILD_FOR_ATELIER_INTERNET)

const void ItemsMessage::SetCurrentLanguage(int current)
{
    CurrentLanguage = current;
}

const char* ItemsMessage::GetString(int language, int id)
{
    return MessageMap[language][id];
}

const char* ItemsMessage::GetString(int id)
{
    return MessageMap[CurrentLanguage][id];
}


const char* ItemsMessage::Languages[] = {
    "en", 
    "fr", 
    "it", 
    NULL
};

int ItemsMessage::CurrentLanguage = 0;
const char** ItemsMessage::MessageMap[] = {
    ItemsMessage::Message_en, 
    ItemsMessage::Message_fr, 
    ItemsMessage::Message_it, 
    NULL
};

const char* ItemsMessage::Message_en[] = {
    "Everybody",
    NULL
};

const char* ItemsMessage::Message_fr[] = {
    "Ensemble",
    NULL
};

const char* ItemsMessage::Message_it[] = {
    "Totale individui",
    NULL
};

#endif
