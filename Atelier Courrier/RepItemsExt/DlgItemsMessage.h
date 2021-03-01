#pragma once
#include "StdAfx.h"
#include "..\Config\BuildConfig.h"
#if defined(BUILD_FOR_ATELIER_INTERNET)
/// <summary>
/// Auto generated DlgItemsMessage class from C:\Work\AtelierPresse\Version 2.5.1\RepItemsExt\DlgItemsLocale.xls
/// </summary>
class DlgItemsMessage
{
public:
    static const char* Languages[];
    static const char** MessageMap[];

    static const void SetCurrentLanguage(int current)
    {
        CurrentLanguage = current;
    }

    static const char* GetString(int language, int id)
    {
        return MessageMap[language][id];
    }

    static const char* GetString(int id)
    {
        return MessageMap[CurrentLanguage][id];
    }

private:
    static int CurrentLanguage;
    static const char* Message_en[];
    static const char* Message_fr[];
    static const char* Message_it[];

public:
    static const int MSG_IDS_RI_USER_BLOC = 0;
    static const int MSG_IDS_RI_REPONDANTS = 1;
    static const int MSG_IDS_RI_IMP_ITEM = 2;
    static const int MSG_IDS_RI_EXPORT_ITEM = 3;
    static const int MSG_IDS_RI_TT_ET_S = 4;
    static const int MSG_IDS_RI_TT_OU_S = 5;
    static const int MSG_IDS_RI_TT_BUT_OPE_SEGMENT = 6;
    static const int MSG_IDS_RI_TT_RAZ_S = 7;
    static const int MSG_IDS_RI_TT_BUT_OPE_SELQUESTION = 8;
    static const int MSG_IDS_RI_TT_ET_M = 9;
    static const int MSG_IDS_RI_TT_OU_M = 10;
    static const int MSG_IDS_RI_TT_NI_M = 11;
    static const int MSG_IDS_RI_TT_XOR_M = 12;
    static const int MSG_IDS_RI_TT_BUT_OPE_DIM1 = 13;
    static const int MSG_IDS_RI_ITEM_UP = 14;
    static const int MSG_IDS_RI_ITEM_DOWN = 15;
    static const int MSG_IDS_RI_TT_T_M = 16;
    static const int MSG_IDS_RI_TT_ET_E = 17;
    static const int MSG_IDS_RI_TT_OU_E = 18;
    static const int MSG_IDS_RI_TT_NI_E = 19;
    static const int MSG_IDS_RI_TT_XOR_E = 20;
    static const int MSG_IDS_RI_TT_T_E = 21;
    static const int MSG_IDS_RI_TT_SEGMENT = 22;
    static const int MSG_IDS_RI_TT_SUPPRESSION = 23;
    static const int MSG_IDS_RI_TT_QUESTION = 24;
    static const int MSG_IDS_RI_ET = 25;
    static const int MSG_IDS_RI_OU = 26;
    static const int MSG_IDS_RI_NI = 27;
    static const int MSG_IDS_RI_XOR = 28;
    static const int MSG_IDS_RI_ITEM_LABEL = 29;
    static const int MSG_IDS_RI_MESS_1 = 30;
    static const int MSG_IDS_RI_MULTISEG = 31;
    static const int MSG_IDS_RI_ENSEMBLE = 32;
    static const int MSG_IDS_RI_LBL_PALETTE_COMPO = 33;
    static const int MSG_IDS_RI_LBL_ITEM_CONSTRUCTION = 34;
    static const int MSG_IDS_RI_MESS_2 = 35;
    static const int MSG_IDC_RI_LABEL_POURCENT = 36;
    static const int MSG_IDC_RI_RAD_MODE_LIGNES = 37;
    static const int MSG_IDC_RI_RAD_MODE_COLONNES = 38;
    static const int MSG_IDC_RI_RAD_MODE_PAGES = 39;
    static const int MSG_IDC_RI_LABEL_SAS = 40;
    static const int MSG_IDC_RI_RAD_DIM1_OU = 41;
    static const int MSG_IDC_RI_RAD_DIM1_ET = 42;
    static const int MSG_IDC_RI_RAD_DIM1_XOR = 43;
    static const int MSG_IDC_RI_RAD_DIM1_NI = 44;
    static const int MSG_IDC_RI_RAD_Q_OU = 45;
    static const int MSG_IDC_RI_LABEL_DIMENSION1 = 46;
    static const int MSG_IDC_RI_BUT_DIM1_RAZ = 47;
    static const int MSG_IDC_RI_TREE_THEME = 48;
    static const int MSG_IDC_RI_RAD_S_ET = 49;
    static const int MSG_IDC_RI_RAD_S_OU = 50;
    static const int MSG_IDC_RI_BUT_MENU_OPE_SEGMENT = 51;
    static const int MSG_IDC_RI_BUT_MENU_OPE_QUSEL = 52;
    static const int MSG_IDC_RI_RAD_Q_ET = 53;
    static const int MSG_IDC_RI_BUT_MENU_OPE_DIM2 = 54;
    static const int MSG_IDC_RI_RAD_DIM2_OU = 55;
    static const int MSG_IDC_RI_RAD_DIM2_ET = 56;
    static const int MSG_IDC_RI_RAD_DIM2_XOR = 57;
    static const int MSG_IDC_RI_RAD_DIM2_NI = 58;
    static const int MSG_IDC_RI_BUT_MENU_OPE_DIM1 = 59;
    static const int MSG_IDC_RI_RAD_QUESTIONS = 60;
    static const int MSG_IDC_RI_RAD_LIST_1 = 61;
    static const int MSG_IDC_RI_RAD_LIST_2 = 62;
    static const int MSG_IDC_RI_TAB_SEGMENT = 63;
    static const int MSG_IDC_RI_BUT_DIM2_RAZ = 64;
    static const int MSG_IDC_RI_BUT_ACCEPT = 65;
    static const int MSG_IDC_RI_BUT_GENESE = 66;
    static const int MSG_IDC_RI_LABEL_TITLE = 67;
    static const int MSG_IDC_RI_LABEL_QUESTIONS = 68;
    static const int MSG_IDC_RI_LABEL_NBCAS = 69;
    static const int MSG_IDC_RI_LABEL_EFFECTIF = 70;
    static const int MSG_IDC_RI_LABEL_DIMENSION2 = 71;
    static const int MSG_IDC_RI_CHECK_CALC_SEGMENT = 72;
    static const int MSG_IDC_RI_STATIC_EFF = 73;
    static const int MSG_IDC_RI_STATIC_PERCENT = 74;
    static const int MSG_IDC_RI_STATIC_NBCAS = 75;
    static const int MSG_IDC_RI_LABEL_SOURCE = 76;
    static const int MSG_IDC_RI_BUT_RAZSEARCH = 77;
    static const int MSG_IDC_RI_RAD_DIMENSION1 = 78;
    static const int MSG_IDC_RI_RAD_DIMENSION2 = 79;
    static const int MSG_IDC_RI_PUSH_DIM1_OK = 80;
    static const int MSG_IDC_RI_LABEL_TYPEPOIDS = 81;
    static const int MSG_IDC_RI_BUT_QUESTION_TOUS = 82;
    static const int MSG_IDC_RI_BUT_QFILTER = 83;
    static const int MSG_IDC_RI_BUT_QFILTERRAZ = 84;
    static const int MSG_IDC_RI_LABEL_THEME = 85;
    static const int MSG_IDC_RI_LABEL_DEFITEM = 86;
    static const int MSG_IDC_RI_BUT_SUPPRESSION = 87;
    static const int MSG_IDS_RI_BUT_SUPPRIMER = 88;
    static const int MSG_IDC_RI_LABEL_LIGNES = 89;
    static const int MSG_IDC_RI_LABEL_COLONNES = 90;
    static const int MSG_IDC_RI_LABEL_PAGES = 91;
    static const int MSG_IDC_RI_BTN_SITES_PAGE = 92;
    static const int MSG_IDC_RI_BTN_SITES_COL = 93;
    static const int MSG_IDC_RI_BUT_ALL_COLONNES = 94;
    static const int MSG_IDC_RI_BUT_ALL_PAGES = 95;
    static const int MSG_IDC_RI_BTN_SITES_LIG = 96;
    static const int MSG_IDC_RI_BUT_ALL_LIGNES = 97;
    static const int MSG_IDC_RI_CHK_RUBRIQUE_DEFAUT = 98;
    static const int MSG_IDS_RI_DIM_ERROR = 99;
    static const int MSG_IDS_RI_DIM_SEL_NEW_INGREDIENT = 100;
    static const int MSG_IDS_RI_DIM_DIMENSION = 101;
    static const int MSG_IDS_RI_DIM_SEL_NEW_INGREDIENT_NONVENT = 102;
    static const int MSG_IDS_RI_DIM_SEL = 103;
    static const int MSG_IDS_RI_EN = 104;
    static const int MSG_IDS_RI_ITEMS_CONST = 105;
    static const int MSG_IDS_RI_TT_ET_Q_ITEM = 106;
    static const int MSG_IDS_RI_TT_OU_Q_ITEM = 107;
    static const int MSG_IDS_RI_TT_VALID_ITEM = 108;
    static const int MSG_IDS_RI_ITEMS = 109;
    static const int MSG_IDS_RI_TT_ITEMS = 110;
    static const int MSG_IDS_RI_PALETTES = 111;
    static const int MSG_IDS_RI_TT_PALETTE = 112;
    static const int MSG_IDS_RI_ITEM_DIM = 113;
    static const int MSG_IDS_RI_INDEX_THEMATIQUE = 114;
    static const int MSG_IDS_RI_VERIFIER = 115;
    static const int MSG_IDS_RI_ITEMEXISTS = 116;
    static const int MSG_IDS_RI_PSEUDO = 117;
    static const int MSG_IDS_RI_EXPORTER = 118;
    static const int MSG_IDS_RI_RENOMMER = 119;
    static const int MSG_IDS_RI_SUPPRIMER = 120;
    static const int MSG_IDS_RI_VISUALISER = 121;
    static const int MSG_IDS_RI_LABELITEM_EXIST = 122;
    static const int MSG_IDS_RI_REMPLACER = 123;
    static const int MSG_IDS_RI_TT_ET_Q_PALETTE = 124;
    static const int MSG_IDS_RI_TT_OU_Q_PALETTE = 125;
    static const int MSG_IDS_RI_TT_VALID_PALETTE = 126;
    static const int MSG_IDS_RI_PALETTE_DIM = 127;
    static const int MSG_IDS_RI_FILTRES = 128;
    static const int MSG_IDS_RI_DIM_SEL_VENTILER = 129;
    static const int MSG_IDS_RI_DIM_SEL_REGROUPER = 130;
    static const int MSG_IDS_RI_DIM_DIMENSION_UNIQUE = 131;
    static const int MSG_IDS_RI_COMPOSANTS = 132;
    static const int MSG_IDM_RI_EFFACER_ITEM = 133;
    static const int MSG_IDM_RI_EFFACER_SEGMENT = 134;
    static const int MSG_IDM_RI_EFFACER_TOUT = 135;
    static const int MSG_ID_RI_AJOUTER_RUBRIQUE = 136;
    static const int MSG_ID_RI_SUPPRIMER_RUBRIQUE = 137;
    static const int MSG_ID_RI_RENOMMER_RUBRIQUE = 138;
    static const int MSG_ID_RI_SUPPRIMER_RUBRIQUE_ET_ITEMS = 139;
    static const int MSG_ID_RI_CLASSER_ITEMS = 140;
    static const int MSG_ID_RI_RUBRIQUE_PARDEFAUT = 141;
    static const int MSG_IDD_RI_DLG_ITEMS = 142;
    static const int MSG_IDS_RI_BUT_SEGEMENT = 143;
    static const int MSG_IDS_RI_COL_ITEMS_COMP = 144;
    static const int MSG_IDS_RI_COL_NBCAS = 145;
    static const int MSG_IDS_RI_COL_EFF = 146;
    static const int MSG_IDS_RI_LABEL_SEG = 147;
    static const int MSG_IDS_RI_LABEL_OPSEG = 148;
    static const int MSG_IDS_RI_RUBRIQUE_PALETTES = 149;
    static const int MSG_IDS_RI_RUBRIQUE_ITEMS = 150;
    static const int MSG_IDM_RI_QFQUESTIONS = 151;
    static const int MSG_IDM_RI_QFMODALITES = 152;
    static const int MSG_IDM_RI_QFTOUS = 153;

};

#endif
