/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFCMaille
//
//	Descriptif:
//	D�riv�e de JMTFR03Maille pour y acceder � l'exterieur de la DLL
*/

#pragma once

#include ".\MoteurPresse\Include\JMTFR03.h"

class AFX_EXT_CLASS JFCMaille : public JMTFR03Maille
{
public:
	JFCMaille(void);
	~JFCMaille(void);
};
