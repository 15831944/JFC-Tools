/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLEchantillons
//
//	Descriptif:
//	Table des échantillons
*/


#if !defined(AFX_JTBLECHANTILLONS_H__INCLUDED_)
#define AFX_JTBLECHANTILLONS_H__INCLUDED_

#pragma once

#include "IElemMap.h"
#include "JEchantillon.h"


class AFX_EXT_CLASS JTBLEchantillons : public IElemMap 
{
public:
	JTBLEchantillons();
	virtual ~JTBLEchantillons();

	JVoid Import(CStdioFile& file);

};



#endif // !defined(AFX_JTBLECHANTILLONS_H__INCLUDED_)
