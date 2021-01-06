// JTBLPeriodicites.cpp: implementation of the JTBLPeriodicites class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLPeriodicites.h"
#include "..\JFCSupports\IDLimits.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLPeriodicites::JTBLPeriodicites()
{
}


JTBLPeriodicites::~JTBLPeriodicites()
{
}

JChar* JTBLPeriodicites::GetLabelCourtByID(JUnt32 id) const
{
	JChar * ret;
	MoveTo(id);
	if (IsValid())
		ret = (JChar*)((JPeriodicite*)GetItem())->GetLabelCourt();
	else
		ret = "TBL Periodicite:GetLabelCourtByID ID inconnu";

	return ret;
}

JVoid JTBLPeriodicites::Import(CStdioFile& file)
{
	Empty();
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		CString resToken;
		int curPos= 0;

		// l'identifiant terrain
		resToken= str.Tokenize(",",curPos);
		JUnt32 id = atoi(resToken);
		
		// le libellé long
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		CString lbl = resToken;

		// le libellé court
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		CString lblcourt = resToken;

		// on crée un élément
		JPeriodicite* periodicite = new JPeriodicite( id, lbl, lblcourt );
		// on ajoute l'élément
		Add( periodicite->GetID() ) = periodicite;
	}

}