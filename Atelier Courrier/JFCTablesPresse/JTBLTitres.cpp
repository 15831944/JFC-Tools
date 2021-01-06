// JTBLTitres.cpp: implementation of the JTBLTitres class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTBLTitres.h"
#include "JTitreCouplageFixe.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

JTBLTitres::JTBLTitres()
{
}

JTBLTitres::~JTBLTitres()
{
}


JUnt32 JTBLTitres::GetPeriodByID(JUnt32 id) const
{
	MoveTo(id);
	if (IsValid())
		return ((JTitre*)GetItem())->GetPeriodicite();
	else
		return 8;
}

JUnt32 JTBLTitres::GetIDAudienceByID(JUnt32 id) const
{
	MoveTo(id);
	if (IsValid())
		return ((JTitre*)GetItem())->GetIDAudience();
	else
		return 0;
}

JVoid JTBLTitres::Import(CStdioFile& file)
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

		// l'identifiant 
		resToken= str.Tokenize(",",curPos);
		JUnt32 id = atoi(resToken);
		
		// l'identifiant audience
		resToken= str.Tokenize(",",curPos);
		JUnt32 idaudience = atoi(resToken);

		// la périodicité
		resToken= str.Tokenize(",",curPos);
		JUnt32 period = atoi(resToken);

		// le libellé
		resToken= str.Tokenize("\"",curPos);
		resToken= str.Tokenize("\"",curPos);
		resToken.Remove('"');
		CString lbl = resToken;

		// la date d
		resToken= str.Tokenize(",",curPos);
		resToken= str.Tokenize(",",curPos);
		CString date_d = resToken;

		// la date f
		resToken= str.Tokenize(",",curPos);
		CString date_f = resToken;

		// le nb option
		resToken= str.Tokenize(",",curPos);
		JUnt32 nbOption = atoi(resToken);

		JBool agregat = false;
		for (int i = 0; i<nbOption; i++)
		{
			// la date f
			resToken= str.Tokenize(",",curPos);
			CString option = resToken;
			if (option.Compare("A") == 0)
				agregat = true;
			if (option.Compare("C") == 0)//couplage fixe
				agregat = agregat;
		}

		// on crée un élément
		JTitre* titre = new JTitre(id, lbl, period, idaudience, agregat);
		// on ajoute l'élément

#ifdef _DEBUG
		//plante qd 2*mm id, c fait pour!
		Add( titre->GetID() ) = titre;
#else
		MoveTo(titre->GetID());
		if (!IsValid())
			Add( titre->GetID() ) = titre;
		else
			delete titre;
#endif

	}

}

JVoid JTBLTitres::ImportCouplagesFixes(CStdioFile& file)
{
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		CString resToken;
		int curPos= 0;
		
		// l'identifiant du couplage
		resToken= str.Tokenize(",",curPos);
		JUnt32 idCpl = atoi(resToken);
		
		// le libellé
		//resToken= str.Tokenize(",",curPos);
		//resToken.Remove('"');
		//CString lbl = resToken;

		//on cree le cpl
		MoveTo(idCpl);
		if (!IsValid())
			continue;
		//GetItem()->AsITitre()->m_cplFixe = new JCouplageFixe(idCpl, GetItem()->GetLabel().AsJCharPtr());
		JCouplageFixe * couplagefixe = new JCouplageFixe(idCpl, GetItem()->GetLabel().AsJCharPtr());
		GetItem()->AsITitre()->m_cplFixe = couplagefixe;

		//on le remplit
		resToken= str.Tokenize(",",curPos);
		JUnt32 nCount = atoi(resToken);
		for (JUnt32 i=0; i<nCount; i++)
		{
			resToken= str.Tokenize(",",curPos);
			JUnt32 id = atoi(resToken);
			
			resToken= str.Tokenize(",",curPos);
			JUnt32 nbInsert = atoi(resToken);

			JTitreCouplageFixe* titrecf = new JTitreCouplageFixe(id, GetLabelByID(id), GetPeriodByID(id), GetIDAudienceByID(id), nbInsert);
			couplagefixe->m_mapIElem.Add( titrecf->GetID() ) = titrecf;

		}
	}
}
