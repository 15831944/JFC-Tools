// JTBLEchantillons.cpp: implementation of the JTBLEchantillons class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLEchantillons.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTBLEchantillons::JTBLEchantillons()
{
}


JTBLEchantillons::~JTBLEchantillons()
{
}


JVoid JTBLEchantillons::Import(CStdioFile& file)
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
		
		// le libellé
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');

		// on crée un élément
		JEchantillon* filtre = new JEchantillon( id, resToken );
		// on ajoute l'élément
		Add( filtre->GetID() ) = filtre;
	}
}