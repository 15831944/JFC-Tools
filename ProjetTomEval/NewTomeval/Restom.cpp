// Restom.cpp: implementation of the CRestom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Restom.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D500D4
CRestom::CRestom()
{
	m_NbVentilation=0;
	m_Couverture_Ventile=NULL;
	m_GRP_Ventile=NULL;
	m_Population_Ventile=NULL;
	m_NbCas_Ventile=NULL;
	m_LibelleModalite=NULL;
}

//##ModelId=3D9AB7D500D5
CRestom::~CRestom()
{
	if(m_Couverture_Ventile)
		delete [] m_Couverture_Ventile;
	if(m_GRP_Ventile)
		delete [] m_GRP_Ventile;
	if(m_Population_Ventile)
		delete [] m_Population_Ventile;
	if(m_NbCas_Ventile)
		delete [] m_NbCas_Ventile;
	if(m_LibelleModalite)
		delete [] m_LibelleModalite;
	m_NbVentilation=0;
	m_Couverture_Ventile=NULL;
	m_GRP_Ventile=NULL;
	m_Population_Ventile=NULL;
	m_NbCas_Ventile=NULL;
	m_LibelleCritere.RemoveAll();
	m_LibelleModalite=NULL;
}

//##ModelId=3D9AB7D50137
void CRestom::InitResultatVentile(int NbVentilation)
{
	m_NbVentilation=NbVentilation;
	m_Couverture_Ventile=new CDoubleArray [NbVentilation];
	m_GRP_Ventile=new CDoubleArray [NbVentilation];
	m_Population_Ventile=new CDoubleArray [NbVentilation];
	m_NbCas_Ventile=new CDoubleArray [NbVentilation];
	m_LibelleCritere.RemoveAll();
	m_LibelleModalite=new CStringArray [NbVentilation];
}

//##ModelId=3D9AB7D500BF
void CRestom::UpdateResultat(InterfaceTomy * pTomy,int Population)
{
#ifdef OPTION_REACH_BELGE
	m_NbInsertion=pTomy->GetNbInsertions();
#else
	m_NbInsertion=pTomy->GetNbSpot();
#endif

#ifdef OPTION_RUSSIE
	m_Population = Population;
#endif

	double couverture=pTomy->GetCouverture();	// Reseultat Se
	m_Audience000=(int)(couverture*Population/100000);
	m_Couverture=couverture;
	m_CouvertureValideBeta=pTomy->GetCouvertureValideBeta();
	m_CouvertureValideXY=pTomy->GetCouvertureValideMinMax();
	m_NbContacts000=(int)(pTomy->GetGRP()*Population/100000);
	m_GRP=pTomy->GetGRP();
	if(couverture>0)m_Repetition=m_GRP/couverture;
	else m_Repetition=0;
	if(m_NbInsertion>0)m_GRPmoyen=pTomy->GetGRP()/m_NbInsertion;
	else m_GRPmoyen=0;

	// Distribution des contacts
	int nbclasse=pTomy->GetNbClasseResultat();
	m_DistributionExacte.SetSize(nbclasse+1);
	m_DistributionCumulee.SetSize(nbclasse+1);
	m_DistributionExacte.SetAt(0,pTomy->GetExactementTouche(0));
	m_DistributionCumulee.SetAt(0,pTomy->GetExactementTouche(0));

#ifdef OPTION_RUSSIE
	// en RUSSIE, on calcule les distributions cumulées de la classe 1 à la classe n
	double cumul=couverture;
	for(int i=1;i<nbclasse;i++)
	{
		double distrib=pTomy->GetExactementTouche(i);
		m_DistributionExacte.SetAt(i,distrib);
		m_DistributionCumulee.SetAt(i,cumul);
		cumul-=distrib;if(cumul<0.0)cumul=0.0;
	}
	m_DistributionExacte.SetAt(nbclasse,cumul);
	m_DistributionCumulee.SetAt(nbclasse,cumul);
#else
	// AILLEURS, on fait comme toujours, de la classe n à la classe 1
	double cumul=0;
	for(int i=nbclasse;i>=1;i--)
	{
		double distrib=pTomy->GetExactementTouche(i);
		cumul+=distrib;
		m_DistributionExacte.SetAt(i,distrib);
		m_DistributionCumulee.SetAt(i,cumul);
	}
#endif
}

//##ModelId=3D9AB7D5023C
bool CRestomJFC::EcritureFichier(LPCSTR NomFic,bool flagFormatMarginal/*=false*/)
{
	int p,i;
	CStdioFile f;
	CJFCFileException e;
	CString txt,txt2;

	// Chargement du fichier
	// Note WinTM de Liem a déjà préparer un restom qui ne doit pas être effacé.

	if( !f.Open(NomFic,CFile::modeNoTruncate|CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive, &e ) )   
	{
		txt.Format("Impossible d'ouvrir %s : ",NomFic);
		e.AfficheErreur(txt);
		return (0);
	}
	f.SeekToEnd();

	// Population;Nombre de cas
#if OPTION_CIMBELGE
		txt.Format("%8d%8d\n",m_TabRestom[0].m_Population,m_TabRestom[0].m_NbCas[0]);
#elif OPTION_RUSSIE
		txt.Format("%8d%8d\n",m_TabRestom[0].m_Population,m_TabRestom[0].m_NbCas[0]);
#else
	if(m_TabRestom[0].m_NbCas.GetSize()==2)
		txt.Format("%8d%8d%8d\n",m_TabRestom[0].m_Population,m_TabRestom[0].m_NbCas[0],m_TabRestom[0].m_NbCas[1]);
	else
		txt.Format("%8d%8d\n",m_TabRestom[0].m_Population,m_TabRestom[0].m_NbCas[0]);
#endif
	f.WriteString(txt);

	// Nombre d'individus touchés 1 fois et plus
	txt="";
	for(p=0;p<m_NbRestom;p++)
	{
		txt2.Format("%12.0f",m_TabRestom[p].m_Couverture*m_TabRestom[0].m_Population/100.0);
		txt+=txt2;
	}
	txt+="\n";
	f.WriteString(txt);

	// Nombre de contacts
	txt="";
	for(p=0;p<m_NbRestom;p++)
	{
		txt2.Format("%12.0f",m_TabRestom[p].m_GRP*m_TabRestom[0].m_Population/100.0);
		txt+=txt2;
	}
	txt+="\n";
	f.WriteString(txt);

	// Nombre de spos qui n'ont pas de probabilité:
	if(flagFormatMarginal==false)
		f.WriteString("0\n"); // pas géré en TV.

	// Courbe de réponse
	txt.Format("%4d",m_TabRestom[0].m_CourbeReponse.GetSize());
	for(i=0;i<m_TabRestom[0].m_CourbeReponse.GetSize();i++)
	{
		txt2.Format("%5d",m_TabRestom[0].m_CourbeReponse[i]);
		txt+=txt2;
	}
	txt+="\n";
	f.WriteString(txt);

	// Distribution des contacts	
	for(i=1;i<=m_TabRestom[0].m_CourbeReponse.GetSize();i++)
	{
		txt="";
		for(int p=0;p<m_NbRestom;p++)
		{
			if (flagFormatMarginal == true)
				txt2.Format("%8.2f%8.2f",m_TabRestom[p].m_DistributionExacte[i],m_TabRestom[p].m_DistributionCumulee[i]);
			else
				txt2.Format("%6.2f%6.2f",m_TabRestom[p].m_DistributionExacte[i],m_TabRestom[p].m_DistributionCumulee[i]);
			txt+=txt2;
		}
		txt+="\n";
		f.WriteString(txt);
	}

	// Ventilations*****
	for (int v=0;v<m_TabRestom[0].m_NbVentilation;v++)
	{
		for(int c=0;c<m_TabRestom[0].m_Population_Ventile[v].GetSize();c++)
		{

			// population
			txt.Format("%12.0f\n",m_TabRestom[0].m_Population_Ventile[v][c]);
			f.WriteString(txt);

			// GRP pour chaque plan
			txt="";
			for(p=0;p<m_NbRestom;p++)
			{
				txt2.Format("%9.2f",m_TabRestom[p].m_GRP_Ventile[v][c]);
				txt+=txt2;
			}
			txt+="\n";
			f.WriteString(txt);
			// Couverture pour chaque plan
			txt="";
			for(p=0;p<m_NbRestom;p++)
			{
				txt2.Format("%9.2f",m_TabRestom[p].m_Couverture_Ventile[v][c]);
				txt+=txt2;
			}
			txt+="\n";
			f.WriteString(txt);
		}
	}


	f.WriteString("^\n");// End of result

	f.Close();
	return(1); //OK
}

//##ModelId=3D9AB7D5020A
#ifdef OPTION_CARAT
bool CRestomCarat::EcritureFichier(LPCSTR NomFic)
{
	CStdioFile f;
	CJFCFileException e;
	CString txt,txt2;

	// Chargement du fichier
	if( !f.Open(NomFic,CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive, &e ) )   
	{
		txt.Format("Impossible d'ouvrir %s : ",NomFic);
		e.AfficheErreur(txt);
		return (0);
	}

	// Population;Nombre de cas
	txt.Format("%d",m_Population);
	for(int n=0;n<m_NbCas.GetSize();n++)
	{
		txt2.Format(";%d",m_NbCas[n]);
		txt+=txt2;
	}
	txt+="\n";

	f.WriteString(txt);

	f.WriteString("@1\n"); // Résultat global

	txt.Format("%d;%d;%.2f;%.2f;%.2f;%d;%.1f;%.2f;%.1f\n",m_NbInsertion,m_Audience000,m_Couverture,m_CouvertureValideBeta,m_CouvertureValideXY,m_NbContacts000,m_GRP,m_Repetition,m_GRPmoyen);
	f.WriteString(txt);

	// Courbe de réponse
	txt.Format("%d",m_CourbeReponse.GetSize());
	for(int i=0;i<m_CourbeReponse.GetSize();i++)
	{
		txt2.Format(";%d",m_CourbeReponse[i]);
		txt+=txt2;
	}
	txt+="\n";
	f.WriteString(txt);
	

	f.WriteString("@2\n"); // détail des distributions de contacts
	// Les non touchés
	txt.Format("0;%d;%.2f;\n",(int)(m_DistributionExacte[0]*m_Population)/100000,m_DistributionExacte[0]); 
	f.WriteString(txt);

	//Les touches de 1 à n
	for(i=1;i<m_DistributionExacte.GetSize();i++)
	{
		txt.Format("%d;%d;%.2f;%.2f\n",i,(int)(m_DistributionExacte[i]*m_Population)/100000,m_DistributionExacte[i],m_DistributionCumulee[i]);
		f.WriteString(txt);
	}

	f.WriteString("@3\n"); // PMG
	for(i=0;i<3;i++)
	{
		double repet=0;
		double indice=0;
		CString txtPMG;
		switch(i)
		{
			case 0:txtPMG="P(33%)";break;
			case 1:txtPMG="M(33%)";break;
			case 2:txtPMG="G(33%)";break;
			default: ASSERT(0);
		}
		if(m_CouverturePMG[i]>0)repet=m_GRPPMG[i]/m_CouverturePMG[i];
		if(m_GRP>0)indice=100.0*m_GRPPMG[i]/m_GRP;
		txt.Format("%s;%.1f;%.1f;%.1f\n",
			txtPMG,
			m_GRPPMG[i],
			indice,
			repet);
		f.WriteString(txt);
	}

	f.WriteString("@4\n"); // ************ APPORT PAR JOUR ************
	for(int j=0;j<m_TabJour->GetSize();j++)
	{
		double n;
		if(j>0)n=m_TabJour->GetAt(j).m_Individu-m_TabJour->GetAt(j-1).m_Individu;
		else n=m_TabJour->GetAt(j).m_Individu;
		txt.Format("%s;%.0f;%.1f;%.1f\n",
			m_TabJour->GetAt(j).m_Libelle,
			n,
			100.0*n/m_Audience000,
			100000.0*m_TabJour->GetAt(j).m_Individu/m_Population);
		f.WriteString(txt);
	}

	f.WriteString("@5\n"); // ************ APPORT PAR SEMAINE ************
	for(j=0;j<m_TabSemaine->GetSize();j++)
	{
		double n;
		if(j>0)n=m_TabSemaine->GetAt(j).m_Individu-m_TabSemaine->GetAt(j-1).m_Individu;
		else n=m_TabSemaine->GetAt(j).m_Individu;
		txt.Format("%s;%.0f;%.1f;%.1f\n",
			m_TabSemaine->GetAt(j).m_LibJour,
			n,
			100.0*n/m_Audience000,
			100000.0*m_TabSemaine->GetAt(j).m_Individu/m_Population);
		f.WriteString(txt);
	}

	f.WriteString("@6\n"); // ************ APPORT PAR SEMAINE GLISSANTE  ***
	for(j=0;j<m_TabSemaineGlissante->GetSize();j++)
	{
		double n;
		if(j>0)n=m_TabSemaineGlissante->GetAt(j).m_Individu-m_TabSemaineGlissante->GetAt(j-1).m_Individu;
		else n=m_TabSemaineGlissante->GetAt(j).m_Individu;
		txt.Format("%s;%.0f;%.1f;%.1f\n",
			m_TabSemaineGlissante->GetAt(j).m_LibJour,
			n,
			100.0*n/m_Audience000,
			100000.0*m_TabSemaineGlissante->GetAt(j).m_Individu/m_Population);
		f.WriteString(txt);
	}

	f.WriteString("@7\n"); // ************ VENTILATIONS ************
	// Libelle Critère
	// Libelle_Modalité;Effectif;Nombre_de_Cas;GRP;Couverture;Répétition

	for (int v=0;v<m_NbVentilation;v++)
	{
		txt.Format("%s\n",m_LibelleCritere[v]);
		f.WriteString(txt);
		for(int c=0;c<m_Population_Ventile[v].GetSize();c++)
		{
			//Libelle modalité;Effectif;Nombre_de_Cas;GRP;Couverture;Répétition

			double r;
			if(m_Couverture_Ventile[v][c]>0)
				r=m_GRP_Ventile[v][c]/m_Couverture_Ventile[v][c];
			else r=0;
			txt.Format("%s;%.0f;%.0f;%.1f;%.1f;%.1f\n",
				m_LibelleModalite[v][c],
				m_Population_Ventile[v][c],
				m_NbCas_Ventile[v][c],
				m_GRP_Ventile[v][c],
				m_Couverture_Ventile[v][c],
				r);
			f.WriteString(txt);
		}
	}
		
	f.WriteString("@8\n"); // ************ Offre élargie / offre restreinte ************
	
	// offre globale
	txt.Format("%.2f;%.2f;%.1f;%.2f\n",m_GRP,m_Couverture,m_CouvertureValideXY,m_Repetition);
	f.WriteString(txt);

	// offre élargie
	double repet;
	if(m_Couverture_offreEllargie>0)
	{
		repet=m_GRP_offreEllargie/m_Couverture_offreEllargie;
	}
	else repet=0;
	txt.Format("%.2f;%.2f;%.1f;%.2f\n",m_GRP_offreEllargie,m_Couverture_offreEllargie,m_CouvValideXY_offreEllargie,repet);
	f.WriteString(txt);

	// offre restreinte
	if(m_Couverture_offreRestreinte>0)
	{
		repet=m_GRP_offreRestreinte/m_Couverture_offreRestreinte;
	}
	else repet=0;
	txt.Format("%.2f;%.2f;%.1f;%.2f\n",m_GRP_offreRestreinte,m_Couverture_offreRestreinte,m_CouvValideXY_offreRestreinte,repet);
	f.WriteString(txt);

	f.WriteString("@9\n"); // ************ THE END ************


/*
@3 (PMG)
Tercile;GRP;Indice_GRP;Répétition
@4 (Apport par jour)
Date;Apport_en_milliers;Apport%du plan;Apport Couverture
@5 (Apport par semaine)
Semaine(5/98);Apport_en_milliers;Apport%Plan;Apport Couverture
@6 (Ventilation)
Libelle_critère/classe;Effectif;Nombre_de_Cas;GRP;Couverture;Répétition
@7 (fin)
*/


	f.Close();
	return(1); //OK
}
#endif

//##ModelId=3D9AB7D500CB
bool CRestom::EffaceFichier(LPCSTR NomFic)
{
	// Note : dans le protocole avc WinTM, il ne faut pas effacer le fichier
	// car plusieur plan peuvent être calculer simultanément

	CFile f;
	CJFCFileException e;
	CString txt;

	// Chargement du fichier
	if( !f.Open(NomFic,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary|CFile::shareExclusive, &e ) )   
	{
		txt.Format("Impossible d'ouvrir %s : ",NomFic);
		e.AfficheErreur(txt);
		return (0);
	}

	f.Close();
	return(1); //OK
}


//##ModelId=3D9AB7D5017E
void CRestomZoom::UpdateResultat(InterfaceTomy * pTomy,CRestom * pRestomAvant,double Population)
{

	// NOTE pour ne pas perdre u temps CPU,
	// on ne met à jour que les variables qui sont utilisés dans EcritureFichier()

	//m_NbInsertion=pTomy->GetNbSpot();
	double couverture=pTomy->GetCouverture();
	m_Population=Population;
	

	//m_Audience000=(int)(couverture*Population/100000);
	m_Couverture=couverture-pRestomAvant->m_Couverture;
	//m_CouvertureValideBeta=pTomy->GetCouvertureValideBeta();
	//m_CouvertureValideXY=pTomy->GetCouvertureValideMinMax();
	//m_NbContacts000=(int)(pTomy->GetGRP()*Population/100000);
	m_GRP=pTomy->GetGRP()-pRestomAvant->m_GRP;
	//if(couverture>0)m_Repetition=m_GRP/couverture;
	//else m_Repetition=0;
	//if(m_NbInsertion>0)m_GRPmoyen=pTomy->GetGRP()/m_NbInsertion;
	//else m_GRPmoyen=0;

	// Distribution des contacts
	int nbclasse=pTomy->GetNbClasseResultat();
	m_DistributionExacte.SetSize(nbclasse+1);
	m_DistributionCumulee.SetSize(nbclasse+1);
	m_DistributionExacte.SetAt(0,pTomy->GetExactementTouche(0)-pRestomAvant->m_DistributionExacte[0]);
	m_DistributionCumulee.SetAt(0,pTomy->GetExactementTouche(0));
	double cumul=0;
	for(int i=nbclasse;i>=1;i--)
	{
		double distrib=pTomy->GetExactementTouche(i)-pRestomAvant->m_DistributionExacte[i];
		cumul+=distrib;
		m_DistributionExacte.SetAt(i,distrib);
		m_DistributionCumulee.SetAt(i,cumul);
	}
}


//##ModelId=3D9AB7D50188
bool CRestomZoom::EcritureFichier(LPCSTR NomFic)
{
	CStdioFile f;
	CJFCFileException e;
	CString txt,txt2;

	// Chargement du fichier
	if( !f.Open(NomFic,CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive, &e ) )   
	{
		txt.Format("Impossible d'ouvrir %s : ",NomFic);
		e.AfficheErreur(txt);
		return (0);
	}


	//NOTE: Les 4 premères lignes ne sont pas lues par l'Atelier Radio

	f.WriteString("\n"); //Population et nombre de cas
	
	// Nombre d'individus touchés 1 fois et plus
	txt.Format("%12.0f\n",m_Couverture*m_Population/100.0);
	f.WriteString(txt);

	// Nombre de contacts
	txt.Format("%12.0f\n",m_GRP*m_Population/100.0);
	f.WriteString(txt);

	f.WriteString("\n"); //Courbe de réponse

	// Distribution des contacts	
	for(int i=1;i<m_DistributionExacte.GetSize();i++)
	{
		txt.Format("%8.4f%8.4f\n",m_DistributionExacte[i],m_DistributionCumulee[i]);
		f.WriteString(txt);
	}
	f.WriteString("^\n");// End of result
	f.Close();
	return(1); //OK
}

#ifdef OPTION_CARAT
void CRestomCarat::UpdateResultat(CTomyConsolideMediadataMediacabsat * pTomy,int Population)
{
	CRestom::UpdateResultat(pTomy,Population);

	// Offre élargie et offre restreinte
	m_GRP_offreEllargie=pTomy->GetGRPOffreElargie();
	m_GRP_offreRestreinte=pTomy->GetGRPOffreRestreinte();
	m_Couverture_offreEllargie=pTomy->GetCouvertureOffreElargie();
	m_Couverture_offreRestreinte=pTomy->GetCouvertureOffreRestreinte();
	m_CouvValideXY_offreEllargie=pTomy->GetCouvertureValideMinMaxOffreElargie();
	m_CouvValideXY_offreRestreinte=pTomy->GetCouvertureValideMinMaxOffreRestreinte();

}
#endif