/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFCDistribution
//
//	Descriptif:
//	D�riv�e de JDistribution pour y acceder � l'exterieur de la DLL
*/

#pragma once
#include ".\MoteurPresse\Include\JDistribution.h"

class AFX_EXT_CLASS JFCDistribution :	public JDistribution
{
public:
	JFCDistribution(void);
	~JFCDistribution(void);
};
