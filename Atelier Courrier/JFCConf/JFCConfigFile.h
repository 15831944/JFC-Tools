/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCConfigFile
//
//	Descriptif:
//	Représentation du fichier de configuration de l'application
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