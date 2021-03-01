/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CATPComm
//
//	Descriptif:
//	Interpr�teur de ligne de commande
*/

#pragma once

// Cible de la commande CATPComm

class CATPComm : public CCommandLineInfo
{
public:
	CATPComm();
	virtual ~CATPComm();

	virtual void ParseParam(const char* pszParam, BOOL bFlag, BOOL bLast);

	enum APPCOM2 {COM2_NOTHING, COM2_ITEM, COM2_ANALYSEUR};
	APPCOM2 m_appToStart;

	enum DICO {DEFAULT, ANG};
	DICO m_dico;
};


