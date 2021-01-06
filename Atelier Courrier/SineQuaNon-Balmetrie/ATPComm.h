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

	enum APPCOM1 {COM1_NOTHING, COM1_ITEM, COM1_ANALYSEUR};
	APPCOM1 m_appToStart;

	enum DICO {DEFAULT, ANG};
	DICO m_dico;
};


