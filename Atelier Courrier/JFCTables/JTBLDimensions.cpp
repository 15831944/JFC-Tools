// JTBLDimensions.cpp: implementation of the JTBLDimensions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLDimensions.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLDimensions::JTBLDimensions()
{
}


JTBLDimensions::~JTBLDimensions()
{
}

JVoid JTBLDimensions::Import(CStdioFile& file)
{
//	Empty();

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
		
		// le libellé
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');

		// on crée un élément
		JDimension* Dimension = new JDimension( id, resToken );
		// on ajoute l'élément
		Add( Dimension->GetID() ) = Dimension;
	}
}