// TableSTATION.h: interface for the CTableSTATION class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLESTATION_H__2113CC04_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
#define AFX_TABLESTATION_H__2113CC04_5C6A_11D2_B3F9_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTableSTATION  
{
public:
	CTableSTATION();
	virtual ~CTableSTATION();
	CTableSTATION & operator=(const CTableSTATION &Data);
	CTableSTATION(const CTableSTATION &Data);

	short m_NrUnique; // numéro unique dans la région
	CString m_Nom;
	char m_Media; // T,R,C...
	short m_TypeSuppS; // 0:Ecran ,15,30...
	short m_TypeMedia; // 0:défaut 1:Télé france
	short m_HeureDebut; // 300 pour 3Heure
	char m_Tarif; // G:Grille E:Ecrans D:Double F:Grille avec écrans
	CString m_Libelle;
	short m_NrStationTete; // NrUnique ou 0 si pas de station tete
	CString m_RegionTete; //$$$$$ si pas de région tête
	short m_NrOrdre;
	CString m_Logo;
	short m_Famille;
	COleDateTime m_DateDebut; // date du début de validité en nb de jours depuis 010180
	COleDateTime m_DateFin;   // date de fin de validité en nb de jours depuis 010180

	// les options des stations:
	// on peut trouver des couplages: C=3:15,7,12
	// on peut trouver des fc spécifiques dispo: D=1:60 (cas de EUR.2)
	short m_NbOption;
	CStringArray m_Options;


	// NOTE pour l'affineur: Tous les champs qui suivent ne sont pas renseignés
	// à la suite d'une demande MSG_TABSTAMARCHE...

	// ********** Pour ETUDE **********************************************
	short m_NbLien;
	COleDateTime * m_DateLien; // par NbLien
//	WordArray m_DateLien; // par NbLien
	CWordArray m_NrUniqueRegie; // par NbLien
	short m_NrUniqueRegieValide; // renseigné uniquement par étude, donne le lien ci-dessus valide
	// dans le noyau réduit uniquement(Univers):
	short m_NbRegionPseudoPresent;
	CWordArray m_NrRegionPseudoPresent; // Nr dynamique, ne pas mémoriser
};

typedef CArray<CTableSTATION,CTableSTATION&> CTableStationArray;
#endif // !defined(AFX_TABLESTATION_H__2113CC04_5C6A_11D2_B3F9_006052017E36__INCLUDED_)
