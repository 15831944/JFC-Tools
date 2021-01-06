/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLDimensions
//
//	Descriptif:
//	Table des dimensions
*/


#pragma once
#pragma warning( disable : 4251 )

#include "IElemMap.h"
#include "JDimension.h"


class AFX_EXT_CLASS JTBLDimensions : public IElemMap 
{
public:
	JTBLDimensions();
	virtual ~JTBLDimensions();

	JVoid Import(CStdioFile& file);
};