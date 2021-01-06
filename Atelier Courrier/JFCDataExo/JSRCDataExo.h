// Date:    06/05/2005
//
// on v�rifie que le fichier n'a pas d�j� �t� inclus
#ifndef JSRCDataExo_H

// on d�finit la macro pour �viter les inclusions multiples
#define JSRCDataExo_H
#pragma once

class AFX_EXT_CLASS JSRCDataExo
{
private:
	/// le constructeur
	JSRCDataExo();

private:
	/// l'unique instance des sources exog�nes
	static JSRCDataExo m_Instance;

public:
	/// la fonction pour r�cup�rer l'instance des sources exog�nes
	static const JSRCDataExo & GetInstance();

public:
	/// le destructeur
	~JSRCDataExo();

private:
	/// l'op�rateur pour recopier les �l�ments
	JSRCDataExo & operator =(const JSRCDataExo & Source);

	/// le constructeur copie
	JSRCDataExo(const JSRCDataExo & Source);

private:
	// Structure stockage data exo
	struct CDataExo
	{
		// Les valeurs de la data exo
		JMap <JUnt32, JFlt64> m_MapValExo;

		// Nom de la donn�e exog�ne (OJD,....)
		CString m_LabelDataExo;

		// Nom de la donn�e abr�g�e (pour affichage en colonnes)
		CString m_AbrLabelDataExo;

		// Type des valeurs contenues dans la data exo (Chiffre, %, euros...)
		// 1 - Chiffre,  2 - %, 3 - euros
		JUnt32 m_TypeDataExo;

		// Nombre de d�cimales pour les valeurs exo
		JUnt32 m_NbDecExo;

		// Id grandeur de d�marrage
		JUnt32 m_IdDepGrandeur;
	};

	// Chargement d'un fichier exog�ne
	JBool ChargeFichExo(CString FichDataExo, CString &TxtErr);

private:
	// Liste des diff�rentes sources exog�nes
	JList <CString> m_ListNomDataExo;

	// Map des �l�ments exog�nes
	JMap <CString, CDataExo> m_MapDataExo;

	// Pour v�rification non redondance d'identiant grandeur
	JMap <JUnt32, JUnt32> m_MapIdGrandeur;

public:
	// R�cup�ration de la liste des noms date exog�nes disponibles
	const JList <CString> & GetListNomDataExo() const;

	// R�cup�ration valeur d'un titre pour une data exo
	JBool GetValeur(CString NomDataExo, JUnt32 IdSupport, JFlt64 &Valeur) const;

	// R�cup�ration type et nb d�cimal des valeurs inclues dans une data exo
	JBool GetTypeValeur(CString NomDataExo, JUnt32 &Type, JUnt32 &NbDec) const;

	// R�cup�ration 1er Id Grandeur pour chaque data exe
	JBool GetIdDepGrandeur(CString NomDataExo, JUnt32 &IdDepGrandeur) const;

	// R�cup�ration libell� abr�g�
	JBool GetAbrLib(CString NomDataExo, CString &AbrNomDataExo) const;

	// R�cup�ration libell� via ident grandeur
	CString GetLib(JUnt32 IdGrandeur) const;

	// Type de data exog�ne
	enum {Type_Chiffre = 1, Type_Prct = 2, Type_Euro = 3};

};

// fin de l'inclusion conditionnelle
#endif // !defined(JSRCDataExo_H)
