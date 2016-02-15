//PanelInternet.cpp: implementation of the CPanelInternet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PanelInternet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPanelInternet::CPanelInternet()
{
	m_fVagueLoaded=0;
	m_NbIndiv=0;
}

CPanelInternet::~CPanelInternet()
{
	UnloadPanel();
}

bool CPanelInternet::LoadPanel(LPCSTR NomFic)
{
	CStdioFile FicAUX;
	CString MsgErr;
	CFileException e;
	// Lecture du fichier de paramètre Vague.tbl
	if(!FicAUX.Open(NomFic,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfxMessageBox(MsgErr);
		return(0);
	}


	short NbSite;

	// Nombre de sites
	if(!FicAUX.Read(&NbSite,sizeof(short)))
	{
		MsgErr.Format("Erreur à la lecture du  nombre de sites dans %s",NomFic);
		AfxMessageBox(MsgErr);
		return 0;
	}
	// Boucle sur les stations
	for(int idxsite=0;idxsite<NbSite;idxsite++)
	{	
		short nrsite;
	
		// Lecture du numéro de sites
		if(!FicAUX.Read(&nrsite,sizeof(short)))
		{
			MsgErr.Format("Erreur à la lecture du  numéro de site dans %s",NomFic);
			AfxMessageBox(MsgErr);
			return 0;
		}
		short nbjour;
		// Lecture du nombre de jours
		if(!FicAUX.Read(&nbjour,sizeof(short)))
		{
			MsgErr.Format("Erreur à la lecture du nombre de jours dans %s",NomFic);
			AfxMessageBox(MsgErr);
			return 0;
		}

		// Boucle sur les jours
		for(int idxjour=0;idxjour<nbjour;idxjour++)
		{
			CSupportProba support;
			support.m_NrSite = nrsite;

			// Code jour
			if(!FicAUX.Read(&support.m_Jour,sizeof(short)))
			{
				MsgErr.Format("Erreur à la lecture du code jour dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}

			// Nb de plage
			short nbplage;

			if(!FicAUX.Read(&nbplage,sizeof(short))/*||(nbplage!=1)*/)
			{
				MsgErr.Format("Erreur à la lecture du nombre de plage dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}
			

			// Horaire de début de plage
			short debutplage;

			if(!FicAUX.Read(&debutplage,sizeof(short))/*||(debutplage!=0)*/)
			{
				MsgErr.Format("Erreur à la lecture du début de plage dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}

			short nbecran;

			// Lecture du nb d'écran
			if(!FicAUX.Read(&nbecran,sizeof(short))/*||(nbecran!=1)*/)
			{
				MsgErr.Format("Erreur à la lecture du nombre d'écran dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}

			// Lecture du nb de semaine de l'étude
			if(!FicAUX.Read(&support.m_NbSemaine,sizeof(short)))
			{
				MsgErr.Format("Erreur à la lecture du nombre de semaine de l'étude dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}

			support.m_NbSemaine%=10000;

			// Nombre d'individu du support
			short nbindiv;

			// Nb d'individus
			if(!FicAUX.Read(&nbindiv,sizeof(short)))
			{
				MsgErr.Format("Erreur à la lecture du nombre d'individus dans %s",NomFic);
				AfxMessageBox(MsgErr);
				return 0;
			}

			// Calcul du nb d'individu total
			//m_NbIndiv+=nbindiv;

			// Boucle sur les individus du support
			for(int idxindiv=0;idxindiv<nbindiv;idxindiv++)
			{
				CProbaIndiv proba;

				// Lecture du numéro d'individu
				if(!FicAUX.Read(&proba.m_NrIndiv,sizeof(short)))
				{
					MsgErr.Format("Erreur à la lecture du numéro d'individu dans %s",NomFic);
					AfxMessageBox(MsgErr);
					return 0;
				}

				// Lecture du Quantum de fréquentation
				if(!FicAUX.Read(&proba.m_Quantum,sizeof(short)))
				{
					MsgErr.Format("Erreur à la lecture de la probabilité dans %s",NomFic);
					AfxMessageBox(MsgErr);
					return 0;
				}
				/*if(proba.m_Quantum!=0)AfxMessageBox("quantum");
				proba.m_Quantum=((idxindiv+idxjour*5)%10000);*/

				//if(proba.m_Quantum!=0)AfxMessageBox("Quantum");
				// Ajout de la donnée de l'individu au support
				support.m_Probabilite.Add(proba); 
			}
			// Ajout du support au panel Internet
			m_SupportProba.Add(support);
		}
	}

	
	FicAUX.Close();
	m_fVagueLoaded=1;
	return 1;// OK
}

bool CPanelInternet::LoadPanel(LPCSTR CheminData, LPCSTR CodeVague)
{

	CString NomFic;

	NomFic.Format("%s\\%s.AUX_INTERNET",CheminData,CodeVague);
	m_CodePanel = CodeVague;

	// Lecture du nb individu Panel dans fichier Vague.tbl
	if(!ReadNbIndividuPanel(CheminData))return(0);

	return LoadPanel(NomFic);
}

// Retourne le support proba correspondant au support Internet
CPanelInternet::CSupportProba * CPanelInternet::RechercheSupportProba(CSupportInternet  support)
{
	// Boucle sur les supports probas
	for(int idxsupp=0;idxsupp<m_SupportProba.GetSize();idxsupp++)
	{
		if(support.m_Jour==m_SupportProba[idxsupp].m_Jour && support.m_NrSite==m_SupportProba[idxsupp].m_NrSite)
			return &m_SupportProba[idxsupp];
	}
	AfxMessageBox("Erreur dans les données. Un support internet n'est pas trouvé dans le panel.");
	ASSERT(0);
	return NULL;
}

void CPanelInternet::UnloadPanel()
{
	if(!m_fVagueLoaded)return;

	m_SupportProba.RemoveAll();
}

// Lecture du nb individu Panel dans fichier Vague.tbl
bool CPanelInternet::ReadNbIndividuPanel(LPCSTR CheminData)
{
	CStdioFile Fic;
	CString MsgErr;
	CString line;
	CString NomFic;
	int nblecture=0;

	// Lecture du fichier Vague.tbl
	NomFic.Format("%s\\Vague.tbl",CheminData);

	// Lecture du fichier de paramètre Vague.tbl
	if(!Fic.Open(NomFic,CFile::modeRead  | CFile::typeText) )
	{
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfxMessageBox(MsgErr);
		return(0);
	}
	// Lecture du nb d'individu du panel
	while(nblecture!=3)
	{
		if(!Fic.ReadString(line))
		{
			MsgErr.Format("Erreur à la lecture du fichier %s ",NomFic);
			AfxMessageBox(MsgErr);
			return(0);
		}
		nblecture++;
	}
	nblecture=0;

	while(nblecture!=4)
	{
		int pos = line.Find(" ");
		line = line.Mid(pos);
		line.TrimLeft();
		nblecture++;
	}

	// Nb d'individu Panel
	m_NbIndiv=atoi(line);

	return(1);
}
