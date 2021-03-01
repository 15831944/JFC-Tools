/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CATPComm
//
//	Descriptif:
//	Interpréteur de ligne de commande
*/

#pragma once

// Cible de la commande CATPComm

class CATPComm : public CCommandLineInfo
{
public:
	CATPComm();
	virtual ~CATPComm();

	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);

	enum APPCOM3 {COM3_NOTHING, COM3_ITEM, COM3_ANALYSEUR};
	APPCOM3 m_appToStart;

	enum DICO {DEFAULT, ANG};
	DICO m_dico;
};


