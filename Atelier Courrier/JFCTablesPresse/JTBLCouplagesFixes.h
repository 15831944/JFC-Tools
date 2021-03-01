/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLCouplagesFixes
//
//	Descriptif:
//	Table des couplages fixes PLUS UTILISE
*/


#if !defined(AFX_JTBLCOUPLAGESFIXES_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
#define AFX_JTBLCOUPLAGESFIXES_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_

#pragma once

#include "JCouplageFixe.h"
#include "JTitreCouplageFixe.h"
#include "IElemMap.h"
#include "JTBLTitres.h"

class AFX_EXT_CLASS JTBLCouplagesFixes : public IElemMap 
{
public:
	JTBLCouplagesFixes();
	virtual ~JTBLCouplagesFixes();

	JVoid Import(CStdioFile& file, JTBLTitres& tblTitres);
	JVoid Import(CStdioFile& file);
};

#endif // !defined(AFX_JTBLCOUPLAGESFIXES_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
