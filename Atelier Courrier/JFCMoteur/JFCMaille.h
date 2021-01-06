/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCMaille
//
//	Descriptif:
//	Dérivée de JMTFR03Maille pour y acceder à l'exterieur de la DLL
*/

#pragma once

#include ".\MoteurPresse\Include\JMTFR03.h"

class AFX_EXT_CLASS JFCMaille : public JMTFR03Maille
{
public:
	JFCMaille(void);
	~JFCMaille(void);
};
