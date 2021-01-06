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

	enum APPCOM {COM_NOTHING=0, COM_CHOIXSUPPORT, COM_PLAN, COM_CREUSET, COM_ITEM, COM_SUPPORT, COM_NOMENCLATURE, COM_ANALYSEUR};
	APPCOM m_appToStart;

	enum DICO {DEFAULT, ANG};
	DICO m_dico;
};


