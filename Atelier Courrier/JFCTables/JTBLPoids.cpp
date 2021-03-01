// JTBLPoids.cpp: implementation of the JTBLPoids class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLPoids.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLPoids::JTBLPoids()
{
}


JTBLPoids::~JTBLPoids()
{
}

JVoid JTBLPoids::Import(CStdioFile& file)
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
		JPoids* Poids = new JPoids( id, resToken );
		// on ajoute l'�l�ment
		Add( Poids->GetID() ) = Poids;
	}
}