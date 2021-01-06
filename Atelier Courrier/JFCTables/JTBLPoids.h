/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JTBLPoids
//
//	Descriptif:
//	Table des types de poids
*/


#pragma once
#pragma warning( disable : 4251 )

#include "IElemMap.h"
#include "JPoids.h"

class AFX_EXT_CLASS JTBLPoids : public IElemMap 
{
public:
	JTBLPoids();
	virtual ~JTBLPoids();

	JVoid Import(CStdioFile& file);
};