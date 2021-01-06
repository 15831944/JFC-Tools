/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CKeyMoteur
//
//	Descriptif:
//	Clé permettant de paramétrer une requête au moteur
*/

#pragma once

class AFX_EXT_CLASS CKeyMoteur
{
public:
	CKeyMoteur(void);
	CKeyMoteur(JUnt32 IDSource, JUnt32 IDTerrain, JUnt32 IDItem);
	~CKeyMoteur(void);

	void Set (JUnt32 IDSource, JUnt32 IDTerrain, JUnt32 IDItem);
	void SetSource (JUnt32 IDSource);
	void SetTerrain (JUnt32 IDTerrain);
	void SetItem (JUnt32 IDItem);

	bool IsValid();

private:

friend class CMoteurPresse;
	
	JUnt32 m_IDSource;
	JUnt32 m_IDTerrain;
	JUnt32 m_IDItem;
};
