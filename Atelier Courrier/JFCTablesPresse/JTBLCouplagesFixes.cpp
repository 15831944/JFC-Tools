// JTBLCouplagesFixes.cpp: implementation of the JTBLCouplagesFixes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLCouplagesFixes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


JTBLCouplagesFixes::JTBLCouplagesFixes()
{
}


JTBLCouplagesFixes::~JTBLCouplagesFixes()
{
}
JVoid JTBLCouplagesFixes::Import(CStdioFile& file)
{
}
JVoid JTBLCouplagesFixes::Import(CStdioFile& file, JTBLTitres& tblTitres)
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
		JUnt32 idCpl = atoi(resToken);
		
		// le libellé
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		CString lbl = resToken;

		//on cree le cpl
		JCouplageFixe * couplagefixe = new JCouplageFixe(idCpl, lbl);

		//on le remplit
		resToken= str.Tokenize(",",curPos);
		JUnt32 nCount = atoi(resToken);
		for (JUnt32 i=0; i<nCount; i++)
		{
			resToken= str.Tokenize(",",curPos);
			JUnt32 id = atoi(resToken);
			
			resToken= str.Tokenize(",",curPos);
			JUnt32 nbInsert = atoi(resToken);

			JTitreCouplageFixe* titrecf = new JTitreCouplageFixe(id, tblTitres.GetLabelByID(id), tblTitres.GetPeriodByID(id), tblTitres.GetIDAudienceByID(id), nbInsert);
			couplagefixe->m_mapIElem.Add( titrecf->GetID() ) = titrecf;

		}
		// on ajoute l'élément
		Add( couplagefixe->GetID() ) = couplagefixe;

	}
}
