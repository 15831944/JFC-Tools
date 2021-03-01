// JTBLUnivRedressement.cpp: implementation of the JTBLUnivRedressement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLUnivRedressement.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLUnivRedressement::JTBLUnivRedressement()
{
}


JTBLUnivRedressement::~JTBLUnivRedressement()
{
}


JVoid JTBLUnivRedressement::Import(CStdioFile& file)
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
		
		// le libell�
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');

		// on cr�e un �l�ment
		JUnivRedressement* filtre = new JUnivRedressement( id, resToken );
		// on ajoute l'�l�ment
		Add( filtre->GetID() ) = filtre;
	}
}