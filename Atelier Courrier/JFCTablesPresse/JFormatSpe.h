/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JFormatSpe
//
//	Descriptif:
//	Sp�cialisation d'un format
//	Format manuel cr�e par l'utilisateur dans le r�pertoire des supports
*/


#if !defined(AFX_JFORMATSPE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JFORMATSPE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "JFormat.h"


class AFX_EXT_CLASS JFormatSpe : public JFormat  
{

public:
	JFormatSpe();
	JFormatSpe(JUnt32 id, const JChar* lbl, const JChar* lblcourt);
	virtual ~JFormatSpe();
	
	JVoid SetLabel(const JChar* lblLong);
	JVoid SetLabelCourt(const JChar* lblCourt);

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

};


#endif // !defined(AFX_JFORMATSPE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
