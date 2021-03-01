/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JFCDistribution
//
//	Descriptif:
//	Dérivée de JDistribution pour y acceder à l'exterieur de la DLL
*/

#pragma once
#include ".\MoteurPresse\Include\JDistribution.h"

class AFX_EXT_CLASS JFCDistribution :	public JDistribution
{
public:
	JFCDistribution(void);
	~JFCDistribution(void);
};
