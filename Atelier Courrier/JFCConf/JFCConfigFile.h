/*
//	JFC Informatique et M�dia
//	2005
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFCConfigFile
//
//	Descriptif:
//	Repr�sentation du fichier de configuration de l'application
*/


#pragma once


class AFX_EXT_CLASS JFCConfigFile
{
public:
	JFCConfigFile();
	virtual ~JFCConfigFile();

	bool Import(CStdioFile& file);

	bool m_NameTitresApp;
	int m_Type_moteurDef;

};