/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTBLPeriodicites
//
//	Descriptif:
//	Table des périodicitées
*/


#if !defined(AFX_TBLPERIOD_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
#define AFX_TBLPERIOD_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )

#include "IElemMap.h"
#include "JPeriodicite.h"



class AFX_EXT_CLASS JTBLPeriodicites : public IElemMap 
{
public:
	JTBLPeriodicites();
	virtual ~JTBLPeriodicites();

	JChar* GetLabelCourtByID(JUnt32 id) const;
	JVoid Import(CStdioFile& file);

};



#endif // !defined(AFX_TBLPERIOD_H__71064F44_8421_4DC4_BBBA_3BB18191F781__INCLUDED_)
