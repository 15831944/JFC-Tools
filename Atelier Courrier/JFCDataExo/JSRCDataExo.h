// Date:    06/05/2005
//
// on vérifie que le fichier n'a pas déjà été inclus
#ifndef JSRCDataExo_H

// on définit la macro pour éviter les inclusions multiples
#define JSRCDataExo_H
#pragma once

class AFX_EXT_CLASS JSRCDataExo
{
private:
	/// le constructeur
	JSRCDataExo();

private:
	/// l'unique instance des sources exogènes
	static JSRCDataExo m_Instance;

public:
	/// la fonction pour récupérer l'instance des sources exogènes
	static const JSRCDataExo & GetInstance();

public:
	/// le destructeur
	~JSRCDataExo();

private:
	/// l'opérateur pour recopier les éléments
	JSRCDataExo & operator =(const JSRCDataExo & Source);

	/// le constructeur copie
	JSRCDataExo(const JSRCDataExo & Source);

private:
	// Structure stockage data exo
	struct CDataExo
	{
		// Les valeurs de la data exo
		JMap <JUnt32, JFlt64> m_MapValExo;

		// Nom de la donnée exogène (OJD,....)
		CString m_LabelDataExo;

		// Nom de la donnée abrégée (pour affichage en colonnes)
		CString m_AbrLabelDataExo;

		// Type des valeurs contenues dans la data exo (Chiffre, %, euros...)
		// 1 - Chiffre,  2 - %, 3 - euros
		JUnt32 m_TypeDataExo;

		// Nombre de décimales pour les valeurs exo
		JUnt32 m_NbDecExo;

		// Id grandeur de démarrage
		JUnt32 m_IdDepGrandeur;
	};

	// Chargement d'un fichier exogène
	JBool ChargeFichExo(CString FichDataExo, CString &TxtErr);

private:
	// Liste des différentes sources exogènes
	JList <CString> m_ListNomDataExo;

	// Map des éléments exogènes
	JMap <CString, CDataExo> m_MapDataExo;

	// Pour vérification non redondance d'identiant grandeur
	JMap <JUnt32, JUnt32> m_MapIdGrandeur;

public:
	// Récupèration de la liste des noms date exogènes disponibles
	const JList <CString> & GetListNomDataExo() const;

	// Récupération valeur d'un titre pour une data exo
	JBool GetValeur(CString NomDataExo, JUnt32 IdSupport, JFlt64 &Valeur) const;

	// Récupération type et nb décimal des valeurs inclues dans une data exo
	JBool GetTypeValeur(CString NomDataExo, JUnt32 &Type, JUnt32 &NbDec) const;

	// Récupération 1er Id Grandeur pour chaque data exe
	JBool GetIdDepGrandeur(CString NomDataExo, JUnt32 &IdDepGrandeur) const;

	// Récupération libellé abrégé
	JBool GetAbrLib(CString NomDataExo, CString &AbrNomDataExo) const;

	// Récupération libellé via ident grandeur
	CString GetLib(JUnt32 IdGrandeur) const;

	// Type de data exogène
	enum {Type_Chiffre = 1, Type_Prct = 2, Type_Euro = 3};

};

// fin de l'inclusion conditionnelle
#endif // !defined(JSRCDataExo_H)
