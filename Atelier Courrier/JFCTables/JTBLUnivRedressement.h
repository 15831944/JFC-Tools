/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLUnivRedressement
//
//	Descriptif:
//	Table des univers de redressement
*/


#if !defined(AFX_JTBLUNIVREDRESSEMENT_H__INCLUDED_)
#define AFX_JTBLUNIVREDRESSEMENT_H__INCLUDED_

#pragma once

#include "IElemMap.h"
#include "JUnivRedressement.h"

class AFX_EXT_CLASS JTBLUnivRedressement : public IElemMap 
{
public:
	JTBLUnivRedressement();
	virtual ~JTBLUnivRedressement();

	JVoid Import(CStdioFile& file);

};



#endif // !defined(AFX_JTBLUNIVREDRESSEMENT_H__INCLUDED_)
