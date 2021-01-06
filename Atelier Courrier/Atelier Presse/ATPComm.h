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

	enum APPCOM {COM_NOTHING=0, COM_CHOIXSUPPORT, COM_PLAN, COM_CREUSET, COM_ITEM, COM_SUPPORT, COM_NOMENCLATURE, COM_ANALYSEUR};
	APPCOM m_appToStart;

	enum DICO {DEFAULT, ANG};
	DICO m_dico;
};


