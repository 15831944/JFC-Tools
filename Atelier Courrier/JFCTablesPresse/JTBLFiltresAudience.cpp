// JTBLFiltresAudience.cpp: implementation of the JTBLFiltresAudience class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLFiltresAudience.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLFiltresAudience::JTBLFiltresAudience()
{
}


JTBLFiltresAudience::~JTBLFiltresAudience()
{
}


JVoid JTBLFiltresAudience::Import(CStdioFile& file)
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
		JFiltreAudience* filtre = new JFiltreAudience( id, resToken );
		// on ajoute l'�l�ment
		Add( filtre->GetID() ) = filtre;
	}
}