/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLFiltresAudience
//
//	Descriptif:
//	Table des filtres d'audience
*/


#if !defined(AFX_JTBLFILTRESAUDIENCE_H__INCLUDED_)
#define AFX_JTBLFILTRESAUDIENCE_H__INCLUDED_

#pragma once

#include "IElemMap.h"
#include "JFiltreAudience.h"


class AFX_EXT_CLASS JTBLFiltresAudience : public IElemMap 
{
public:
	JTBLFiltresAudience();
	virtual ~JTBLFiltresAudience();

	JVoid Import(CStdioFile& file);

};



#endif // !defined(AFX_JTBLFILTRESAUDIENCE_H__INCLUDED_)
