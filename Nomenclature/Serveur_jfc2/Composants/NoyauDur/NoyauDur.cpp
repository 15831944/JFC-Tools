// NoyauDur.cpp: implementation of the CNoyauDur class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NoyauDur.h"
#include "JFCFileException.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNoyauDur::CNoyauDur(CString EtudIni, CString Marche, short CodeStation)
{
	CWaitCursor wait;
	RAZMembers();
	if (CodeStation != 3 && CodeStation != 5) return;
	CString msg;
	CFileStatus status;
	m_Init=TRUE;
	m_CodeStation=CodeStation;
	if(!CFile::GetStatus(EtudIni, status))   // static function
    {
		msg.Format("Initialisation du Noyau\nErreur ds le chemin %s.", EtudIni);
		m_Init=false;
    }
	else m_CheminEtudIni=EtudIni;
	m_Marche=Marche;

	InitNoyo();
	if(ChargerUnivers())	m_Init=false;
	if(ChargerMedias())		m_Init=false;
	if(ChargerFamilles())	m_Init=false;
	if(ChargerRegions())	m_Init=false;
	if(ChargerRegies())		m_Init=false;
	if(ChargerStationsMarche())	m_Init=false;
}

CNoyauDur::CNoyauDur()
{
	RAZMembers();
}

CNoyauDur::~CNoyauDur()
{
	LibererMarche();
}

bool CNoyauDur::Initialisation(CString EtudIni, CString Marche, short CodeStation)
{
	if(m_Init)	return m_Init;
	if (CodeStation != 3 && CodeStation != 5) return (false); else m_CodeStation = CodeStation;
	CWaitCursor wait;
	CString msg;
	CFileStatus status;
	if(!CFile::GetStatus(EtudIni, status))   // static function
    {
		msg.Format("Initialisation du Noyau\nErreur ds le chemin %s.", EtudIni);
		return m_Init;
    }
	else m_CheminEtudIni=EtudIni;

	m_Marche=Marche;
	InitNoyo();
	if(ChargerUnivers())	return m_Init;
	if(ChargerMedias())		return m_Init;
	if(ChargerFamilles())	return m_Init;
	if(ChargerRegions())	return m_Init;
	if(ChargerRegies())		return m_Init;
	if(ChargerStationsMarche())	return m_Init;
	m_Init=true;
	return m_Init;
}
/*------------------------------------------------------------------------*/
// Table des univers d'un marché
CTableUniversArray &CNoyauDur::DonneMarche(void)
{
	if(!m_Init)	return m_TableUnivers;
	m_fUniversUnique=false;
	CWaitCursor wait;
	if(!m_Marcheload)
	{
		InitNoyo();
		if(ChargerNoyau(m_Marcheload))	goto badload;
		LibererDicos();
		m_Marcheload=true;
	}
	return m_TableUnivers;
badload:
	LibererDicos();
	return m_TableUnivers;
}

CTableUNIVERS * CNoyauDur::DonneUnivers(int Univers)
{
	if(!m_Init)	return NULL;
	m_fUniversUnique=true;
	CWaitCursor wait;
	m_Univers.Format("U%s0%d", m_Marche, Univers);
	if(!RechercheUniversUnique())	return NULL;
	if(!(m_Universload&(1<<Univers)))
	{
		InitNoyo();
		if(ChargerNoyau(m_Marcheload))	goto badload;
		LibererDicos();
		m_Universload|=(1<<Univers);
	}
	return &m_TableUnivers[m_NrUniversUnique];
badload:
	LibererDicos();
	return NULL;
}

/*------------------------------------------------------------------------*/
CTableFamilleArray &CNoyauDur::DonneTableFamilles(void)
{
	return m_TableFamilles;
}

CTableMediaArray &CNoyauDur::DonneTableMedias(void)
{
	return m_TableMedias;
}

CTableRegionArray &CNoyauDur::DonneTableRegions(void)
{
	return m_TableRegions;
}

CTableRegieArray &CNoyauDur::DonneTableRegies(void)
{
	return m_TableRegies;
}

/*
// Table des stations d'un marché
CTableStationArray ** CNoyauDur::DonneTableStations(int Univers)
{
	CWaitCursor wait;
	m_Univers.Format("U%s0%d", m_Marche, Univers);
	m_fUniversUnique=1;
	if(!m_TableStations)
	{
		InitNoyo();
		if(ChargerStations())	goto badload;
		LibererDicos();
	}
	return m_TableStations;
badload:
	LibererDicos();
	return NULL;
}

CTableSourceArray * CNoyauDur::DonneTableSources(int Univers)
{
	CWaitCursor wait;
	m_Univers.Format("U%s0%d", m_Marche, Univers);
	m_fUniversUnique=1;

	if(!m_TableSources)
	{
		InitNoyo();
		if(!m_TableUnivers.GetSize())
		{
			if(ChargerUnivers())	goto badload;
			RechercheUniversUnique();
		}
		ChargerTabSourcesUnivers();
		LibererDicos();
	}
	return m_TableSources;
badload:
	LibererDicos();
	return NULL;
}

CTableVagueArray ** CNoyauDur::DonneTableVagues(int Univers)
{
	CWaitCursor wait;
	m_Univers.Format("U%s0%d", m_Marche, Univers);
	m_fUniversUnique=1;

	if(!m_TableVagues)
	{
		InitNoyo();
		if(ChargerUnivers())	goto badload;
		RechercheUniversUnique();
		ChargerTabSourcesUnivers();
		ChargerTableVagues();
		LibererDicos();
	}
	return m_TableVagues;
badload:
	LibererDicos();
	return NULL;
}
*/
/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
/*
bool CNoyauDur::VerifMarche(CString M)
{
	if(m_Marche!=_T(""))
	{
		if(M.GetLength()>2)
		{
			if(m_Univers.Find(M)==-1)	return(0);
			else return(1);
		}
		else
			if(M!=m_Marche)	return(0);
			else return(1);
	}
	else	return(1);
}
*/
void CNoyauDur::InitNoyo(void)
{
	// Chemins Data,Univers,Zone,Region
	char * buf=new char[256];
	GetPrivateProfileString(_INI_Local,_INI_Data,"data",buf,256,m_CheminEtudIni);
	m_CheminData.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_Univers,"univers",buf,256,m_CheminEtudIni);
	m_CheminUnivers.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_User,"user",buf,256,m_CheminEtudIni);
	m_CheminUser.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_Zone,"zone",buf,256,m_CheminEtudIni);
	m_CheminZone.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_Source,"source",buf,256,m_CheminEtudIni);
	m_CheminSource.Format("%s", buf);
	memset(buf, 0, sizeof(buf));

	GetPrivateProfileString(_INI_Local,_INI_DataTele,"datatele",buf,256,m_CheminEtudIni);
	m_CheminDataTele.Format("%s", buf);
	memset(buf, 0, sizeof(buf));
	delete buf;

	//chargement de la langue
	if(!m_Dico1)
	{
		CString	dico1;
		dico1.Format("%s\\%snoyau.%s",m_CheminZone,m_Marche,_SUFFIXEDICOFR);
		m_Dico1=new CDictionary(dico1);
		if(m_Marche=="BL" && !m_Dico2)
		{
			CString	dico2;
			dico2.Format("%s\\%sdicsrc.%s",m_CheminSource,m_Marche,_SUFFIXEDICOFR);
			m_Dico2=new CDictionary(dico2);
			if(m_fUniversUnique && !m_Dico3)
			{
				CString	dico3;
				dico3.Format("%s\\%sdic.%s",m_CheminUnivers,m_Univers,_SUFFIXEDICOFR);
				m_Dico3=new CDictionary(dico3);
			}
		}
	}
}

/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
short CNoyauDur::ChargerNoyau(bool choix)
{
	switch(choix)
	{
	case 0:	//Marche
		if(ChargerStations())	return(1);
		if(ChargerPseudoStations())	return(1);
		ChargerTabSourcesUnivers();
		ChargerTableVagues();
		LierNoyau();
		AroserArbre(); // un peu d'humour...NULLE L'HUMOUR!! QUI C'EST QUI LIT LE CODE??!!:-(#@$
		MajInfoBidon();
		break;
	case 1:	//univers
		TriTableStations();
		if(ChargerPseudoStations())	return(1);
		break;
	}
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerUnivers ID:1
 Purpose:       Chargement de l'univers.
                Fichiers lus :
                UNIVERS\mm_UNIVE.TBL
                UNIVERS\mm_VAGUE.TBL
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerUnivers(void)
{
	CString str;
	CStdioFile Fic;
	CJFCFileException e;
	short u,p,r,m;		// pour boucler:
	short LigneLue=1;
	bool fLireVague=0;
	bool fFausseLigneLue=0;

	str=m_CheminUnivers;
	str.Format("%s\\%s_UNIVE.TBL",m_CheminUnivers,m_Marche);
	if(!Fic.Open(str,CFile::modeRead, &e))
    {
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",str,e.m_cause);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	if(RechercheLigne(LigneLue,str,&Fic,'*')) goto errLectureTableUnivers;
	m_TableUnivers.SetSize((short)atoi(str.Right(str.GetLength()-1)));
	for(u=0;u<m_TableUnivers.GetSize();u++)
	{
		// code de l'univers
		CTableUNIVERS pUnivers;
		if(RechercheLigne(LigneLue,str,&Fic,'#')) goto errLectureTableUnivers;
		pUnivers.m_Code=str.Right(str.GetLength()-1);
		pUnivers.m_Code.TrimRight();

		// Libellé de l'univers
		if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
		pUnivers.m_Libelle=m_BuffLigne;
		pUnivers.m_Libelle.TrimRight();
		// Nombre de périodes ou 0 si DATE
		if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
		pUnivers.m_NbVaguesCalU=(short)atoi(&m_BuffLigne[m_Pos[0]]);
		if(pUnivers.m_NbVaguesCalU){ // si pas daté...
			fLireVague=1;
//			if(pUnivers.m_NbVaguesCalU>TAILLEPOS)goto errLectureTableUnivers;
			pUnivers.m_NomVaguesCalU.SetSize((LIBPERIODEMAX+1)*pUnivers.m_NbVaguesCalU);
			pUnivers.m_DureeVaguesCalU.SetSize((LIBPERIODEMAX+1)*pUnivers.m_NbVaguesCalU);
			// lecture des libellés des périodes du calendrier
			if(LectureLigneV(LigneLue,&Fic,(short)(pUnivers.m_NbVaguesCalU*2))) goto errLectureTableUnivers;
			for(p=0;p<pUnivers.m_NbVaguesCalU;p++){
				if(m_Dico3)
				{
					CString txt;
					txt.Format("VAGUE%03d",p+1);
					pUnivers.m_NomVaguesCalU[p]=m_Dico3->GetDico(txt);
				}
				else {
					pUnivers.m_NomVaguesCalU[p]=&m_BuffLigne[m_Pos[p*2]];
					pUnivers.m_NomVaguesCalU[p].TrimRight();
				}
				pUnivers.m_DureeVaguesCalU[p]=(short)atoi(&m_BuffLigne[m_Pos[1+p*2]]);
			}
		}
		// la liste des médias:
		if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
		pUnivers.m_NbMedia=(short)strlen(m_BuffLigne);
		pUnivers.m_TabMedia=m_BuffLigne[m_Pos[0]];
		pUnivers.m_TabMedia.TrimRight();

		// type de supports de l'univers:
		// Régions en liste ou en arborescence
		// Jour de début
		if(LectureLigneV(LigneLue,&Fic,4)) goto errLectureTableUnivers;
		pUnivers.m_TypeSuppU=(short)atoi(&m_BuffLigne[m_Pos[0]]);
		pUnivers.m_fPresentArbre=(char)atoi(&m_BuffLigne[m_Pos[1]]);
		pUnivers.m_JourDebut=(char)atoi(&m_BuffLigne[m_Pos[2]]);
		pUnivers.m_HeureDebut=(short)atoi(&m_BuffLigne[m_Pos[3]]);


		// format de base de l'univers
		if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
		pUnivers.m_DureeFormatBase=(short)atoi(&m_BuffLigne[m_Pos[0]]);

		// le libellé du niveau mère
		if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
		pUnivers.m_NomNiveauMere=&m_BuffLigne[m_Pos[0]];
		pUnivers.m_NomNiveauMere.TrimRight();
		pUnivers.m_NomNiveauMereLong=&m_BuffLigne[m_Pos[1]];
		pUnivers.m_NomNiveauMereLong.TrimRight();
		pUnivers.m_NomNiveauMereCourt=&m_BuffLigne[m_Pos[1]];
		pUnivers.m_NomNiveauMereCourt.TrimRight();

		// le nombre de régions pour cet univers:
		if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
		pUnivers.m_NbRegion=(short)atoi(&m_BuffLigne[m_Pos[0]]);
		pUnivers.m_CodeZone.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_NrUniqueZone.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_LibRegion.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_LibRegionLong.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_LibRegionCourt.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_LibRegionMini.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_CodeMere.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_Options.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_fGRP.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_TypeSuppR.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_SourceRef.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_SourceEta.SetSize(pUnivers.m_NbRegion);
		pUnivers.m_DescriptionBis.SetSize(pUnivers.m_NbRegion);
		// pour chaque region :
		for(r=0;r<pUnivers.m_NbRegion;r++){
			// code de la zone
			if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
			pUnivers.m_CodeZone[r]=&m_BuffLigne[m_Pos[0]];
			pUnivers.m_CodeZone[r].TrimRight();
			pUnivers.m_NrUniqueZone[r] = (short)atoi((LPCSTR)(pUnivers.m_CodeZone[r])+3);
			// code de la zone mère ou 0 si aucune
			if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
			pUnivers.m_CodeMere[r]=&m_BuffLigne[m_Pos[0]];
			pUnivers.m_CodeMere[r].TrimRight();
			// Options pour cette région
			if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
			pUnivers.m_Options[r]=&m_BuffLigne[m_Pos[0]];
			pUnivers.m_Options[r].TrimRight();
			// Type de supports de cette région:
			if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
			pUnivers.m_TypeSuppR[r]=(short)atoi(&m_BuffLigne[m_Pos[0]]);
			// présence de GRP:
			if(LectureLigneV(LigneLue,&Fic,1)) goto errLectureTableUnivers;
			pUnivers.m_fGRP[r]=&m_BuffLigne[m_Pos[0]];
			pUnivers.m_fGRP[r].TrimRight();
			// si il y a des GRP
			if(pUnivers.m_fGRP[r]){
				// ICI on ne lit pour l'instant qu'une source *******
				// il faudra dès que nécessaire lire la source Bis
				if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
				fFausseLigneLue=0;
				if(m_BuffLigne[m_Pos[0]]=='0'){
					fFausseLigneLue=1;
					if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
					if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
				}
				// La source ETALON ------------
				CSourceDEF Temps;
				Temps.m_Nom=&m_BuffLigne[m_Pos[0]];
				Temps.m_Nom.TrimRight();
				Temps.m_Index=-1; // renseigné + tard (fc LierNoyau())
				if(m_BuffLigne[m_Pos[0]]=='0'){
					AfxMessageBox("Detail des sources à utiliser dans l'univers invalide");
					goto errLectureTableUnivers;
				}
				Temps.m_NbModaGeofrac=(short)atoi(&m_BuffLigne[m_Pos[1]]);
				Temps.m_NrRegionGeofrac=-1;
				if(Temps.m_NbModaGeofrac){
					Temps.m_TabModaGeofrac.SetSize(Temps.m_NbModaGeofrac);
					if(LectureLigneV(LigneLue,&Fic,(short)(1+Temps.m_NbModaGeofrac))) goto errLectureTableUnivers;
					// le Numéro de la Région de la source Géofractionnaire
					Temps.m_NrRegionGeofrac=(short)atoi(&m_BuffLigne[m_Pos[0]]);
					for(m=0;m<Temps.m_NbModaGeofrac;m++){
						Temps.m_TabModaGeofrac[m]=(short)atoi(&m_BuffLigne[m_Pos[1+m]]);
					} // boucle sur les modalités
				} // cas d'une source géofractionnaire
				pUnivers.m_SourceEta.SetAt(r, Temps);
				// lecture des noms de la dll des fonctions à utiliser:
				if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;

				// La source de REFERENCE ----------------
				CSourceDEF Temps2;
				if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
				Temps2.m_Nom=&m_BuffLigne[m_Pos[0]];
				Temps2.m_Nom.TrimRight();
				Temps2.m_Index=-1; // renseigné + tard (fc LierNoyau())
				if(m_BuffLigne[m_Pos[0]]=='0'){
					AfxMessageBox("Detail des sources à utiliser dans l'univers invalide");
					goto errLectureTableUnivers;
				}
				Temps2.m_NbModaGeofrac=(short)atoi(&m_BuffLigne[m_Pos[1]]);
				Temps2.m_NrRegionGeofrac=-1;
				if(Temps2.m_NbModaGeofrac){
					Temps2.m_TabModaGeofrac.SetSize(Temps2.m_NbModaGeofrac);
					if(LectureLigneV(LigneLue,&Fic,(short)(1+Temps2.m_NbModaGeofrac))) goto errLectureTableUnivers;
					// le Numéro de la Région de la source Géofractionnaire
					Temps2.m_NrRegionGeofrac=(short)atoi(&m_BuffLigne[m_Pos[0]]);
					for(m=0;m<Temps2.m_NbModaGeofrac;m++){
						Temps2.m_TabModaGeofrac[m]=(short)atoi(&m_BuffLigne[m_Pos[1+m]]);
					} // boucle sur les modalités
				} // cas d'une source géofractionnaire
				pUnivers.m_SourceRef.SetAt(r, Temps2);
				// lecture des noms de la dll des fonctiond à utiliser:
				if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;

				// on fait semblant de lire la source Bis
				if(fFausseLigneLue==0){
					if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
					if(LectureLigneV(LigneLue,&Fic,2)) goto errLectureTableUnivers;
				}
			} // condition GRP présent pour cette région
		} // boucle par région
		m_TableUnivers.SetAt(u, pUnivers);
	} // boucle par univers
	Fic.Close();

	if(fLireVague){
		// on ouvre aussi le fichier des vagues associées à ce marché ...
		str.Format("%s\\%s_VAGUE.TBL",m_CheminUnivers,m_Marche);
		if(!Fic.Open(str,CFile::modeRead, &e))
		{
			CString MsgErr;
			MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",str,e.m_cause);
			e.AfficheErreur(MsgErr);
			return(1);
		}

		LigneLue=1;
		for(u=0;u<m_TableUnivers.GetSize();u++){
			if(!m_TableUnivers[u].m_NbVaguesCalU) continue;
			if(RechercheLigne(LigneLue,str,&Fic,'#')) goto errLectureTableVagues;
			// on pourrait vérifier ici que l'on est en phase ...
			for(r=0;r<m_TableUnivers[u].m_NbRegion;r++){
				if(!m_TableUnivers[u].m_fGRP[r])continue;
				if(RechercheLigne(LigneLue,str,&Fic,'#')) goto errLectureTableVagues;
				// la source ETALON
				m_TableUnivers[u].m_SourceEta[r].m_Vagues.SetSize(m_TableUnivers[u].m_NbVaguesCalU);
				m_TableUnivers[u].m_SourceEta[r].m_NrIndexVague.SetSize(m_TableUnivers[u].m_NbVaguesCalU);
				if(LectureLigneV(LigneLue,&Fic,m_TableUnivers[u].m_NbVaguesCalU)) goto errLectureTableVagues;
				for(p=0;p<m_TableUnivers[u].m_NbVaguesCalU;p++){
					m_TableUnivers[u].m_SourceEta[r].m_Vagues[p]=&m_BuffLigne[m_Pos[p]];
					m_TableUnivers[u].m_SourceEta[r].m_Vagues[p].TrimRight();
				} // boucle des périodes
				// la source de REFERENCE
				m_TableUnivers[u].m_SourceRef[r].m_Vagues.SetSize(m_TableUnivers[u].m_NbVaguesCalU);
				m_TableUnivers[u].m_SourceRef[r].m_NrIndexVague.SetSize(m_TableUnivers[u].m_NbVaguesCalU);
				if(LectureLigneV(LigneLue,&Fic,m_TableUnivers[u].m_NbVaguesCalU)) goto errLectureTableVagues;
				for(p=0;p<m_TableUnivers[u].m_NbVaguesCalU;p++){
					m_TableUnivers[u].m_SourceRef[r].m_Vagues[p]=&m_BuffLigne[m_Pos[p]];
					m_TableUnivers[u].m_SourceRef[r].m_Vagues[p].TrimRight();
				} // boucle des périodes
			} // boucle par région
		} // boucle par univers
		Fic.Close();
	} // cas où il y a au moins 1 univers non daté
	return(0);

	// en cas de PB:
	errLectureTableUnivers:
	str.Format("Lecture table d'univers stoppée à la ligne %d",LigneLue);
	AfxMessageBox(str);
	Fic.Close();
	return(2);

	// en cas de PB:
	errLectureTableVagues:
	str.Format("Lecture table des vagues stoppée à la ligne %d",LigneLue);
	AfxMessageBox(str);
	Fic.Close();
	return(2);
}

/*------------------------------------------------------------------------
 Procedure:     RechercheUniversUnique ID:1
 Purpose:       Recherche du numéro unique de l'univers d'après son code.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
bool CNoyauDur::RechercheUniversUnique(void)
{
	if(!m_fUniversUnique)return true;
	CTableUNIVERS * sTU=NULL;
	for(short u=0;u<m_TableUnivers.GetSize();u++){
		sTU=&m_TableUnivers[u];
		if(sTU->m_Code==m_Univers)break;
	}
	if(u<m_TableUnivers.GetSize())
		m_NrUniversUnique=u;
	else{
		CString txt;
		txt.Format("Univers %s non trouvé",m_Univers);
		AfxMessageBox(txt);
		m_fUniversUnique=0;
		return false;
	}
	return true;
}

/*------------------------------------------------------------------------
 Procedure:     ChargerMedias ID:1
 Purpose:       Chargement de la table des médias. Fichier DATA\mmMEDIA.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerMedias(void)
{
	CString str;
	CFicTbl fictbl;
	str.Format("%s\\%sMEDIA.TBL",m_CheminData,m_Marche);
	if(fictbl.OpenFile(str))	return(1);
	fictbl.SetLineFormat(1,30, 0);
	m_TableMedias.SetSize(fictbl.FindStar());
	fictbl.FindHash();
	for(short i=0 ; i<m_TableMedias.GetSize() ; i++)
	{
		CTableMEDIA Temps;
		fictbl.ReadLine();
		Temps.m_Nom=fictbl.GetChar(0);
		Temps.m_Libelle=fictbl.GetCharPtr(1);
		m_TableMedias.SetAt(i, Temps);
	}
	fictbl.CloseFile();
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerFamilles ID:1
 Purpose:       Chargement de la table des familles. Fichier
                DATA\mmFAMILL.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerFamilles(void)
{
	CString str;
	CFicTbl fictbl;
	str.Format("%s\\%sFAMILL.TBL",m_CheminData,m_Marche);
	if(fictbl.OpenFile(str))	return(1);
	fictbl.SetLineFormat(3,8,30,0);
	m_TableFamilles.SetSize(fictbl.FindStar());
	fictbl.FindHash();
	for(short i=0 ; i<m_TableFamilles.GetSize() ; i++)
	{
		CTableFAMILLE Temps;
		fictbl.ReadLine();
		Temps.m_Numero=(short)atoi(fictbl.GetCharPtr(0));
		str.Format("FAM_COURT%d",Temps.m_Numero);
		Temps.m_Nom=m_Dico1->GetDico(str);
		str.Format("FAM_LONG%d",Temps.m_Numero);
		Temps.m_Libelle=m_Dico1->GetDico(str);
		m_TableFamilles.SetAt(i, Temps);
	}
	fictbl.CloseFile();
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerRegions ID:1
 Purpose:       Chargement de la table des régions d'un marché. Fichier
                ZONE\mm_REGIO.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerRegions(void)
{
	CFicTbl fictbl;
	CString str;
	str.Format("%s\\%s_regio.TBL",m_CheminZone,m_Marche);
	if(fictbl.OpenFile(str))	return(1);
	fictbl.SetLineFormat(3,5,10,30,8,5,0);
	m_TableRegions.SetSize(fictbl.FindStar());
	fictbl.FindHash();
	for(short i=0 ; i<m_TableRegions.GetSize() ; i++)
	{
		CTableREGION Temps;
		fictbl.ReadLine();
		Temps.m_NrUnique=(short)atoi(fictbl.GetCharPtr(0));
		Temps.m_Code=fictbl.GetCharPtr(1);
		str.Format("REG_MOYEN%d",Temps.m_NrUnique);
		Temps.m_Libelle=m_Dico1->GetDico(str);
		str.Format("REG_LONG%d",Temps.m_NrUnique);
		Temps.m_LibelleLong=m_Dico1->GetDico(str);
		str.Format("REG_COURT%d",Temps.m_NrUnique);
		Temps.m_LibelleCourt=m_Dico1->GetDico(str);
		str.Format("REG_MINI%d",Temps.m_NrUnique);
		Temps.m_LibelleMini=m_Dico1->GetDico(str);
		m_TableRegions.SetAt(i ,Temps);
	}
	fictbl.CloseFile();
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerStations ID:1
 Purpose:       Charge toutes les station des univers des régions d'un marché.
                La donnée est classée par [Univers][Niveau][Région]...
                Retourne un code d'erreur.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerStations(void)
{
//	short FieldNbLien[]={0,4}; // première ligne dans la table des liens
	short FieldNbLien[]={0,m_CodeStation+1}; // le 1/03/2002, passage au code station à 5 caractères
	short NbFieldNbLien=2;// première ligne dans la table des liens
//	short * FieldLien=NULL;
	short NbLienMax; // seconde ligne dans la table des liens
	char fTableLien=0;
	short LigneLue=1;
	CStdioFile Fic;
	CJFCFileException e;
	CFicTbl fictbl;
	CString str;
	short r=0;

	m_NbStation=new CWordArray[m_TableUnivers.GetSize()];
	m_TableStations=new CTableStationArray*[m_TableUnivers.GetSize()];
	for(short u=0;u<m_TableUnivers.GetSize();u++)
	{ //par univers
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		m_NbStation[u].SetSize(m_TableUnivers[u].m_NbRegion);
		m_TableStations[u]=new CTableStationArray[m_TableUnivers[u].m_NbRegion];
		for(r=0;r<m_TableUnivers[u].m_NbRegion;r++)
		{ //par région
			m_NbStation[u][r]=0;
			if(m_TableUnivers[u].m_fGRP[r])
			{
				fTableLien=0;
				str.Format("%s\\%s\\%sSTATIO.TBL",m_CheminZone,m_TableUnivers[u].m_CodeZone[r],m_Marche);
				if(fictbl.OpenFile(str))	continue;
//				fictbl.SetLineFormat(3,5,1,1,2,30,3,5,3,12,3,6,6,5,2,255,0);
				fictbl.SetLineFormat(m_CodeStation,5,1,1,2,30,3,5,3,12,3,6,6,5,2,255,0); // le 1/03/2002, passage au code station à 5 caractères
				m_NbStation[u][r]=fictbl.FindStar();
				m_TableStations[u][r].SetSize(m_NbStation[u][r]);
				fictbl.FindHash();
				for(short s=0;s<m_NbStation[u][r];s++)
				{
					CTableSTATION pStation;
					fictbl.ReadLine();
					pStation.m_NrUnique=(short)atoi(fictbl.GetCharPtr(0));
					pStation.m_Nom=fictbl.GetCharPtr(1);
					// PAS DE TRADUCTION POUR LES NOMS COURTS...
					pStation.m_Media=fictbl.GetChar(2);
					pStation.m_Tarif=fictbl.GetChar(3);
					pStation.m_TypeSuppS=(short)atoi(fictbl.GetCharPtr(4));
					if(m_Marche!="BL")
					{
						str.Format("STA_LONG%d",pStation.m_NrUnique);
						pStation.m_Libelle=m_Dico1->GetDico(str);
					}
					else
					{
						str.Format("STA_LONG%02d%03d",m_TableRegions[r].m_NrUnique,pStation.m_NrUnique);
						pStation.m_Libelle=m_Dico1->GetDico(str);
					}
					pStation.m_NrStationTete=(short)atoi(fictbl.GetCharPtr(6));
					pStation.m_RegionTete=fictbl.GetCharPtr(7);
					pStation.m_NrOrdre=(short)atoi(fictbl.GetCharPtr(8));
					pStation.m_Logo=fictbl.GetCharPtr(9);
					pStation.m_Famille=(short)atoi(fictbl.GetCharPtr(10));
					CString DDMMYY=fictbl.GetCharPtr(11);
					pStation.m_DateDebut.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
					DDMMYY=fictbl.GetCharPtr(12);
					pStation.m_DateFin.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
					pStation.m_NbOption=(short)atoi(fictbl.GetCharPtr(14));
					if(pStation.m_NbOption>0)
					{
						pStation.m_Options.SetSize(pStation.m_NbOption);
						str=fictbl.GetCharPtr(15);
						for(short o=0;o<pStation.m_NbOption;o++)
						{
							if(str.Find('?')>=0){
								while(str.Find('U')>=0){
									CString tmp=str.Mid(str.Find('U')+1,2);
									if(m_TableUnivers[u].m_Code.Find(tmp)>=0)
										pStation.m_Options[o]=str.Mid(str.Find('{')+1, str.Find('}')-str.Find('{')-1);
									if(str.Find(':')<0)	break;
									str=str.Mid(str.Find(':')+1);
								}
							}
							else if(str.Find(' ')<0)
								pStation.m_Options[o]=str;
							else
								pStation.m_Options[o]=GetPtrPWOFic(str, ' ', ' ', 1);
						}
					}
					else pStation.m_Options.RemoveAll();
					m_TableStations[u][r].SetAt(s, pStation);
				}
				fictbl.CloseFile();

				// liens des stations avec les régies *****************************************************
				fTableLien=1;
				str.Format("%s\\%s\\%sLIENS.TBL",m_CheminZone,m_TableUnivers[u].m_CodeZone[r],m_Marche);

				if(!Fic.Open(str,CFile::modeRead, &e))
				{
					CString MsgErr;
					MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",str,e.m_cause);
					e.AfficheErreur(MsgErr);
					return(1);
				}
				if(RechercheLigne(LigneLue,str,&Fic,'*')) goto errLectureTableStations;
				NbLienMax=(short)atoi(str.Right(str.GetLength()-1));
/*				FieldLien=new short[NbLienMax*2];
				for(s=0;s<NbLienMax;s++)
				{// construction du préformat
					FieldLien[s*2]=(short)(s*11);
					FieldLien[s*2+1]=(short)(s*11+7);
				}
*/
				// on se place au début des enregistrements
				if(RechercheLigne(LigneLue,str,&Fic,'#')) goto errLectureTableStations;
				for(s=0;s<m_NbStation[u][r];s++)
				{// pour chaque station:

//					if(LireEnregistrement(LigneLue,&Fic,FieldNbLien,NbFieldNbLien)) goto errLectureTableStations;
					m_Pos.RemoveAll();
					m_Pos.SetSize(2);
					m_Pos[0]=0;
//					m_Pos[1]=4;
					m_Pos[1]=m_CodeStation+1; // le 1/03/2002, passage au code station à 5 caractères
					if(LireEnregistrement(LigneLue,&Fic,NbFieldNbLien)) goto errLectureTableStations;
					if(atoi(&m_BuffLigne[FieldNbLien[0]])!=m_TableStations[u][r][s].m_NrUnique)
					{
						str.Format("Table des liens de la zone %s déphasée",m_TableUnivers[u].m_CodeZone[r]);
						AfxMessageBox(str);
						goto errLectureTableStations;
					}
					m_TableStations[u][r][s].m_NbLien=(short)atoi(&m_BuffLigne[FieldNbLien[1]]);
					m_TableStations[u][r][s].m_DateLien=new COleDateTime[m_TableStations[u][r][s].m_NbLien];
					m_TableStations[u][r][s].m_NrUniqueRegie.SetSize(m_TableStations[u][r][s].m_NbLien);
					m_TableStations[u][r][s].m_NrUniqueRegieValide=-1; // c'est Etudes qui renseignera plus tard
					m_Pos.RemoveAll();
					for(short w=0;w<NbLienMax;w++)
					{// construction du préformat
						m_Pos.Add(w*11);
						m_Pos.Add(w*11+7);
					}
					if(LireEnregistrement(LigneLue,&Fic,(short)(2*m_TableStations[u][r][s].m_NbLien))) goto errLectureTableStations;
					for(short l=0;l<m_TableStations[u][r][s].m_NbLien;l++)
					{
						CString DDMMYY=&m_BuffLigne[m_Pos[l*2]];
						m_TableStations[u][r][s].m_DateLien[l].SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
						m_TableStations[u][r][s].m_NrUniqueRegie[l]=(short)atoi(&m_BuffLigne[m_Pos[l*2+1]]);

					}
				} // boucle station
				Fic.Close();
//				delete FieldLien;
			} // condition si GRP présent
		} // boucle par région
	} // boucle par univers

	// Tri de la table des stations (NrOrdre)
	if(m_fUniversUnique)TriTableStations();
	return(0);


	// en cas de PB:
	errLectureTableStations:
//	if(FieldLien)	delete FieldLien;
	if(fTableLien==1)
	{
		str.Format("Lecture table des liens de la zone %s stoppée à la ligne %d", m_TableUnivers[u].m_CodeZone[r],LigneLue);
		Fic.Close();
	}
	else
	{
		str.Format("Lecture table des stations de la zone %s stoppée à la ligne %d", m_TableUnivers[u].m_CodeZone[r],LigneLue);
		fictbl.CloseFile();
	}
	AfxMessageBox(str);
	return(2);
}

/*------------------------------------------------------------------------
 Procedure:     TriTableStations ID:1
 Purpose:       Tri des stations de chaque région par numéro d'ordre, de 1 à N
                obligatoirement.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CNoyauDur::TriTableStations(void)
{
	if(!m_fUniversUnique)return;

	short u=m_NrUniversUnique;
	CTableStationArray * TableStaTriU=new CTableStationArray[m_TableUnivers[u].m_NbRegion];
	for(short r=0;r<m_TableUnivers[u].m_NbRegion;r++)
	{ //par région
		TableStaTriU[r].Copy(m_TableStations[u][r]);
//		TableStaTriU[r].SetSize(m_NbStation[u][r]);
		for(short s=0;s<m_NbStation[u][r];s++)
		{
			for(short s2=0;s2<m_NbStation[u][r];s2++){
				if(m_TableStations[u][r][s2].m_NrOrdre==s)break;
			}
			if(s2>=m_NbStation[u][r])
			{
				CString str;
				str.Format("PB dans TriTableStations Nr %d de région %d non trouvé",s,r);
				AfxMessageBox(str);
			}
//			else TableStaTriU[r][s]=m_TableStations[u][r][s2];
		}
		// c'est bon pour cette région... on remplace la liste des stations
		m_TableStations[u][r].Copy(TableStaTriU[r]);
	}
	delete [] TableStaTriU;
}

/*------------------------------------------------------------------------
 Procedure:     ChargerPseudoStations ID:1
 Purpose:       Chargement des pseudo station, lecture du fichier
                ZONE\zzzzz\uuuuuPSE.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerPseudoStations(void)
{
	CTableUNIVERS * U;
	short s=0,p=0,i=0;
	short rCherche=0,sCherche=0;
	CString str;
	CFicTbl fictbl;

	if(!m_fUniversUnique)return(0);
	U=&m_TableUnivers[m_NrUniversUnique];

	U->m_NbPseudoStation.SetSize(U->m_NbRegion);
	U->m_NrPseudoStation=new CWordArray[U->m_NbRegion];

	for(short r=0;r<U->m_NbRegion;r++)
	{
		CFileFind File;
		str.Format("%s\\%s\\%sPSE.TBL",m_CheminZone,U->m_CodeZone[r],U->m_Code);
		if(!File.FindFile(str) || fictbl.OpenFile(str))
		{
			U->m_NbPseudoStation[r]=0;
			continue;
		}
//		fictbl.SetLineFormat(2,3); // ajout des régions en Belgique
		fictbl.SetLineFormat(2,m_CodeStation); // le 1/03/2002, passage au code station à 5 caractères
		U->m_NbPseudoStation[r]=fictbl.FindStar();
		fictbl.FindHash();
		U->m_NrPseudoStation[r].SetSize(U->m_NbPseudoStation[r]);
		for(s=0;s<U->m_NbPseudoStation[r];s++)
		{
			fictbl.ReadLine();
			U->m_NrPseudoStation[r][s]=(short)atoi(fictbl.GetCharPtr(1));
		}
		fictbl.CloseFile();
	}

	U->m_NbStationCalcul.SetSize(U->m_NbRegion);
	U->m_NrStationCalcul=new CWordArray[U->m_NbRegion];
	for(r=0;r<U->m_NbRegion;r++){
		U->m_NbStationCalcul[r]=U->m_NbPseudoStation[r]+m_NbStation[m_NrUniversUnique][r];
		U->m_NrStationCalcul[r].SetSize(U->m_NbStationCalcul[r]);
		for(s=0;s<m_NbStation[m_NrUniversUnique][r];s++)
			U->m_NrStationCalcul[r][s]=m_TableStations[m_NrUniversUnique][r][s].m_NrUnique;
		p=s;
		for(s=0;s<U->m_NbPseudoStation[r];s++) {
			for (i=0;i<p;i++)
				if (U->m_NrStationCalcul[r][i]==U->m_NrPseudoStation[r][s]) break;
			if (i<p) continue;
			U->m_NrStationCalcul[r][p]=U->m_NrPseudoStation[r][s];
			p++;
		}
		U->m_NbStationCalcul[r] = p;
	}
	
	if(!m_Marcheload)
	{
		U->m_NbStation.Copy(m_NbStation[m_NrUniversUnique]);
		U->m_TableStations=new CTableStationArray[U->m_NbRegion];
		for(r=0 ; r<U->m_NbRegion ; r++)
		{
			U->m_TableStations[r].SetSize(m_NbStation[m_NrUniversUnique][r]);
			for(short s=0;s<m_NbStation[m_NrUniversUnique][r];s++)
				U->m_TableStations[r][s]=m_TableStations[m_NrUniversUnique][r][s];
		}
	}

	// maintenant on construit la table complète
	U->m_TableStationCalcul=new CTableStationArray[U->m_NbRegion];
	for(r=0;r<U->m_NbRegion;r++){
		U->m_TableStationCalcul[r].SetSize(U->m_NbStationCalcul[r]);
		// on recopie les stations de la région:
		for(s=0;s<U->m_NbStation[r];s++){
			U->m_TableStationCalcul[r][s]=U->m_TableStations[r][s];
		}
		// et on ajoute les pseudo-stations
		for (p=s;p<U->m_NbStationCalcul[r];p++){
			for(rCherche=0;rCherche<U->m_NbRegion;rCherche++){
				for(sCherche=0;sCherche<U->m_NbStation[rCherche];sCherche++){
					if(U->m_TableStations[rCherche][sCherche].m_NrUnique==U->m_NrStationCalcul[r][p])goto finderecherche;
				}
			}
			finderecherche:
			if((rCherche<U->m_NbRegion) && (sCherche<U->m_NbStation[rCherche])){
				U->m_TableStationCalcul[r][p]=U->m_TableStations[rCherche][sCherche];
			}
			else {
				str.Format("PB noyau: pseudo-station %d de région %d non trouvée!",U->m_NrStationCalcul[r][p],r);
				AfxMessageBox(str);
			}
		}
	}
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerRegies ID:1
 Purpose:       Chargement de la table des régies d'un marché. Fichier
                DATA\mm_REGIE.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerRegies(void)
{
	CFicTbl fictbl;
	CString str;

	str.Format("%s\\%s_REGIE.TBL",m_CheminData,m_Marche);
	if(fictbl.OpenFile(str))	return(1);
	fictbl.SetLineFormat(3,5,30,1,3,12,6,6,0);
	m_TableRegies.SetSize(fictbl.FindStar());
	fictbl.FindHash();
	for(short i=0 ; i<m_TableRegies.GetSize() ; i++)
	{
		CTableREGIE Temps;
		fictbl.ReadLine();
		Temps.m_NrUnique=(short)atoi(fictbl.GetCharPtr(0));
		str.Format("RPB_COURT%d",Temps.m_NrUnique);
		Temps.m_Nom=m_Dico1->GetDico(str);
		str.Format("RPB_LONG%d",Temps.m_NrUnique);
		Temps.m_Libelle=m_Dico1->GetDico(str);;
		Temps.m_Remise=(short)atoi(fictbl.GetCharPtr(3));
		Temps.m_NrOrdre=(short)atoi(fictbl.GetCharPtr(4));
		Temps.m_Logo=fictbl.GetCharPtr(5);
		CString DDMMYY=fictbl.GetCharPtr(6);
		Temps.m_DateDebut.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
		DDMMYY=fictbl.GetCharPtr(7);
		Temps.m_DateFin.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
		m_TableRegies.SetAt(i, Temps);
	}
	fictbl.CloseFile();
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerStationsMarche ID:1
 Purpose:       Chargement des stations du marché. Fichier
                ZONE\zzzzz\mmSTATIO.TBL.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerStationsMarche(void)
{
	CFicTbl fictbl;
	CString str;

	m_NbStationMarche.SetSize(m_TableRegions.GetSize());
	m_TableStationsMarche=new CTableStationArray[m_TableRegions.GetSize()];
	for(short r=0;r<m_TableRegions.GetSize();r++)
	{ //par région
		m_NbStationMarche[r]=0;
		str.Format("%s\\%s\\%sSTATIO.TBL",m_CheminZone,m_TableRegions[r].m_Code,m_Marche);
		if(fictbl.OpenFile(str))	continue;
//		fictbl.SetLineFormat(3,5,1,1,2,30,3,5,3,12,3,6,6,5,2,255,0);
		fictbl.SetLineFormat(m_CodeStation,5,1,1,2,30,3,5,3,12,3,6,6,5,2,255,0); // le 1/03/2002, passage au code station à 5 caractères
		m_NbStationMarche[r]=fictbl.FindStar();
		m_TableStationsMarche[r].SetSize(m_NbStationMarche[r]);
		fictbl.FindHash();
		for(short s=0;s<m_NbStationMarche[r];s++)
		{
			CTableSTATION pStation;
			fictbl.ReadLine();
			pStation.m_NrUnique=(short)atoi(fictbl.GetCharPtr(0));
			pStation.m_Nom=fictbl.GetCharPtr(1);
			pStation.m_Media=fictbl.GetChar(2);
			pStation.m_Tarif=fictbl.GetChar(3);
			pStation.m_TypeSuppS=(short)atoi(fictbl.GetCharPtr(4));
			pStation.m_Libelle=fictbl.GetCharPtr(5);
			pStation.m_NrStationTete=(short)atoi(fictbl.GetCharPtr(6));
			pStation.m_RegionTete=fictbl.GetCharPtr(7);
			pStation.m_NrOrdre=(short)atoi(fictbl.GetCharPtr(8));
			pStation.m_Logo=fictbl.GetCharPtr(9);
			pStation.m_Famille=(short)atoi(fictbl.GetCharPtr(10));
			CString DDMMYY=fictbl.GetCharPtr(11);
			pStation.m_DateDebut.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));
			DDMMYY=fictbl.GetCharPtr(12);
			pStation.m_DateFin.SetDate((atoi(DDMMYY.Mid(4, 2))>70)?(atoi(DDMMYY.Mid(4, 2))+1900):(atoi(DDMMYY.Mid(4, 2))+2000), atoi(DDMMYY.Mid(2, 2)), atoi(DDMMYY.Mid(0, 2)));

			// les champs non renseignés:
			pStation.m_NbOption=0;
			pStation.m_NbLien=0;
			if(pStation.m_DateLien)	delete pStation.m_DateLien;
			pStation.m_NrUniqueRegie.RemoveAll();
			pStation.m_NrUniqueRegieValide=-1;
			m_TableStationsMarche[r].SetAt(s, pStation);
		}
		fictbl.CloseFile();
	} // boucle par région
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerTabSourcesUnivers ID:1
 Purpose:       Chargement de la table des sources d'un univers.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerTabSourcesUnivers(void)
{
	CStdioFile Fic;
	CJFCFileException e;
	CString str;
	CTableUNIVERS * sTU;
	CTableSOURCE * LSource;
	m_NbSource.SetSize(m_TableUnivers.GetSize());
	m_TableSources=new CTableSourceArray[m_TableUnivers.GetSize()];
	for(short u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		
		str.Format("%s\\%s%s.csv",m_CheminUnivers,sTU->m_Code,TABLESOURCEUNI);
		if(!(LSource=GetListeSource(str, (short &)m_NbSource[u])))	continue;
		// combien de sources pour ce marché ?
		m_TableSources[u].SetSize(m_NbSource[u]);
		// on copie cette table...
		for(short s=0;s<m_NbSource[u];s++){
			CTableSOURCE Temps;
			Temps.m_Num=LSource[s].m_Num;
			Temps.m_Rep=LSource[s].m_Rep;
			Temps.m_Structure=LSource[s].m_Structure;
			Temps.m_Media=LSource[s].m_Media;
			Temps.m_Role=LSource[s].m_Role;
			Temps.m_Nature=LSource[s].m_Nature;
			Temps.m_Type=LSource[s].m_Type;
			Temps.m_Date=LSource[s].m_Date;
			Temps.m_Grp=LSource[s].m_Grp;

			if(m_Dico2){
				str.Format("SOURCE%03d",s+1);
				Temps.m_NomLong=m_Dico2->GetDico(str);
			}
			else {
				Temps.m_NomLong=LSource[s].m_NomLong;
			}
			Temps.m_NomCourt=LSource[s].m_NomCourt;
			Temps.m_Occupe=LSource[s].m_Occupe;
			m_TableSources[u].SetAt(s, Temps);
		}
		delete [] LSource;
	}
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     GetListeSource ID:1
 Purpose:       Lecture de la table des sources.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
CTableSOURCE * CNoyauDur::GetListeSource(CString FicSource, short & nbsource)
{
	CStdioFile Fic;
	CJFCFileException e;
	CString str;
	CTableSOURCE * LSource;

	if(!Fic.Open(FicSource,CFile::modeRead, &e))
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",str,e.m_cause);
		e.AfficheErreur(MsgErr);
		return(NULL);
	}
	short nb=0;
	while(Fic.ReadString(str))	nb++;
	nbsource=nb;
	Fic.SeekToBegin();
	LSource=new CTableSOURCE[nb];
	for(short i=0 ; i<nb ; i++)
	{
		Fic.ReadString(str);
		LSource[i].m_Num=(short)atoi(GetPtrPWOFic(str, ',', ',', 2));
		LSource[i].m_Rep=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Structure=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Media=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Nature=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Role=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Type=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Date=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_Grp=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_NomLong=GetPtrPWOFic(str, '\"', ',', 2);
		LSource[i].m_NomCourt=GetPtrPWOFic(str, '\"', ',', 1);
		LSource[i].m_Occupe=(short)atoi(str);
	}
	Fic.Close();
	return(LSource);
}

/*------------------------------------------------------------------------
 Procedure:     ChargerTableVagues ID:1
 Purpose:       Construction de la table des vagues.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
short CNoyauDur::ChargerTableVagues(void)
{
	CString str;
	CTableVAGUE * LVague;
	bool FiltreV;
	m_NbVague=new CWordArray[m_TableUnivers.GetSize()];
	m_NbVagueMatrice=new CWordArray[m_TableUnivers.GetSize()];
	m_TableVagues=new CTableVagueArray*[m_TableUnivers.GetSize()];

	for(short u=0;u<m_TableUnivers.GetSize();u++)
	{
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		m_NbVague[u].SetSize(m_NbSource[u]);
		m_NbVagueMatrice[u].SetSize(m_NbSource[u]);
		m_TableVagues[u]=new CTableVagueArray[m_NbSource[u]];
		for(short s=0;s<m_NbSource[u];s++)
		{
			FiltreV = false;
//			for (short r=0;r<m_TableUnivers[u].m_NbRegion;r++) {
//				if(m_TableUnivers[u].m_SourceRef[r].m_Nom==m_TableSources[u][s].m_Rep && m_Marche.Find("FR")>=0){
//				if(m_TableUnivers[u].m_SourceRef[r].m_Nom=="SFR01" && m_Marche.Find("FR")>=0){
//					FiltreV = true;
//					break;
//				}
//			}
			m_NbVague[u][s]=0;
			if(m_TableSources[u][s].m_Rep=="SFR01")	FiltreV = true;
			if(!(LVague=GetListeVagues(m_TableSources[u][s].m_Rep, (short &)m_NbVagueMatrice[u][s], (short &)m_NbVague[u][s], FiltreV)))	continue;
			m_TableVagues[u][s].SetSize(m_NbVague[u][s]);
			for(short v=0 ; v<m_NbVague[u][s] ; v++)
			{
				CTableVAGUE Temps;
				Temps.m_Num=LVague[v].m_Num;
				Temps.m_SourceT=LVague[v].m_SourceT;
				Temps.m_NbInd=LVague[v].m_NbInd;
				Temps.m_NbIndS=LVague[v].m_NbIndS;
				Temps.m_Nom=LVague[v].m_Nom;
				Temps.m_Nom.TrimRight();
				if(m_Dico2){
//					str.Format("VAGUE%02d%03d",s+1,m_TableVagues[u][s][v].m_Num);
					str.Format("VAGUE%02d%03d",s+1,Temps.m_Num);
					Temps.m_Titre=m_Dico2->GetDico(str);
				}
				else {
					Temps.m_Titre=LVague[v].m_Titre;
				}
				Temps.m_Titre.TrimRight();
				Temps.m_Occupe=LVague[v].m_Occupe;
				CString DDMMYY=Temps.m_Nom;
				Temps.m_DateDebut.SetDate((atoi(DDMMYY.Mid(1, 2))>70)?(atoi(DDMMYY.Mid(1, 2))+1900):(atoi(DDMMYY.Mid(3, 2))+2000), (DDMMYY[1]=='A')?11:atoi(DDMMYY.Mid(1, 1)), 01);
				m_TableVagues[u][s].SetAt(v, Temps);
			}
			delete [] LVague;
		}
	}
	return(0);
}

/*------------------------------------------------------------------------
 Procedure:     GetListeVagues ID:1
 Purpose:       Lecture de la liste des vagues.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
CTableVAGUE * CNoyauDur::GetListeVagues(CString NomRep, short & NbVagueOrigine, short & nbvague, bool FiltreV)
{
	CStdioFile Fic;
	CJFCFileException e;
	CString str;
	CTableVAGUE * LVague;
	int NbAuto=0;
	short * TabAuto=NULL;
	CFile FAuto;

	if (FiltreV){
		// Check l'autorisation d'accès
		CString Chemin=m_CheminSource+"\\"+NomRep+"\\VAGUE.COD";
		if(!FAuto.Open(Chemin,CFile::modeRead, &e)){
			CString MsgErr;
			MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",Chemin,e.m_cause);
			e.AfficheErreur(MsgErr);
			FiltreV=false;
		}
		else{
			FAuto.Read(&NbAuto,2);
			TabAuto=new short[NbAuto];
			FAuto.Read((void *)TabAuto,NbAuto*2);
			FAuto.Close();
		}
	}
	CString FicVague;
	FicVague.Format("%s\\%s\\%sVAG.CSV",m_CheminSource,NomRep,NomRep);
	if(!Fic.Open(FicVague,CFile::modeRead, &e))
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s (%d)",FicVague,e.m_cause);
		e.AfficheErreur(MsgErr);
		return(NULL);
	}
	short nb=0;
	while(Fic.ReadString(str))	nb++;
	NbVagueOrigine=nb;
	Fic.SeekToBegin();
	LVague=new CTableVAGUE[nb];
	nbvague=0;
	for(short x=0 ; x<nb ; x++)
	{
		Fic.ReadString(str);
		LVague[nbvague].m_Num=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		LVague[nbvague].m_SourceT=(short)atoi(GetPtrPWOFic(str, ',', ',', 1));
		GetPtrPWOFic(str, ',', ',', 1);
		LVague[nbvague].m_NbInd=(long)atoi(GetPtrPWOFic(str, ',', ',', 1));
		LVague[nbvague].m_NbIndS=(long)atoi(GetPtrPWOFic(str, ',', ',', 2));
		LVague[nbvague].m_Nom=GetPtrPWOFic(str, '\"', ',', 2);
		LVague[nbvague].m_Titre=GetPtrPWOFic(str, '\"', ',', 2);
		GetPtrPWOFic(str, '\"', ',', 2);
		GetPtrPWOFic(str, '\"', ',', 1);
		LVague[nbvague].m_Occupe=(short)atoi(str);
		if(LVague[nbvague].m_Occupe && FiltreV){
			for(short v=0;v<NbAuto;v++){
				if(TabAuto[v]==LVague[nbvague].m_Num) break;
			}
			if (v>=NbAuto) LVague[nbvague].m_Occupe=0;
		}
		if(!LVague[nbvague].m_Occupe) continue;
		nbvague++;
	}
	Fic.Close();
	if(TabAuto)	delete [] TabAuto;
	return(LVague);
}

/*------------------------------------------------------------------------
 Procedure:     LierNoyau ID:1
 Purpose:       Etablir les liens entre tables dans la structure de l'univers.
 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CNoyauDur::LierNoyau(void)
{
	CString str;
	char fNoCodeSource;
	CTableUNIVERS * sTU;
	short iSource;

	if(!m_NbSource.GetSize()) return;

	//LIEN UNIVERS-REGIE
	for(short u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		sTU->m_TableRegies.Copy(m_TableRegies);
	}

	// LIEN UNIVERS-SOURCE par REGION:
	fNoCodeSource=0;
	for(u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		sTU->m_TableSources.Copy(m_TableSources[u]);
		sTU->m_NbVague.Copy(m_NbVague[u]);
		sTU->m_TableVagues=new CTableVagueArray[m_NbSource[u]];
		for(short s=0;s<m_NbSource[u];s++){
			sTU->m_TableVagues[s].Copy(m_TableVagues[u][s]);
/*			sTU->m_TableVagues[s].SetSize(m_NbVague[u][s]);
			for(short v=0;v<m_NbVague[u][s];v++){
				sTU->m_TableVagues[s][v]=m_TableVagues[u][s][v];
			}
*/
		}

		sTU->m_NbVagueMatrice.Copy(m_NbVagueMatrice[u]);
		for(short r=0;r<sTU->m_NbRegion;r++){
			if(!sTU->m_fGRP[r])continue;
			// la source ETALON
			for(short i=0;i<m_NbSource[u];i++){
				if(sTU->m_SourceEta[r].m_Nom==m_TableSources[u][i].m_Rep){
					sTU->m_SourceEta[r].m_Index=i;
					m_TableSources[u][i].m_fTypeUtilisation=1;
					break;
				}
			} // boucle des sources (de TableSources)
			if(i>=m_NbSource[u])fNoCodeSource=1;
			// la source de REFERENCE
			for(i=0;i<m_NbSource[u];i++){
				if(sTU->m_SourceRef[r].m_Nom==m_TableSources[u][i].m_Rep){
					sTU->m_SourceRef[r].m_Index=i;
					m_TableSources[u][i].m_fTypeUtilisation=2;
					break;
				}
			} // boucle des sources (de TableSources)
			if(i>=m_NbSource[u])fNoCodeSource=1;
		} // boucle par région
	} // boucle par univers
	if(fNoCodeSource){
		AfxMessageBox("Liaison TableUnivers-TableSources incomplète");
//		LibererStations(m_TableStations);
		return;
	}


	// LIEN UNIVERS-VAGUE ETALON
	fNoCodeSource=0;
	for(u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		if(sTU->m_NbVaguesCalU<=0)continue;
		for(short r=0;r<sTU->m_NbRegion;r++){
			if(!sTU->m_fGRP[r])continue;
			iSource=sTU->m_SourceEta[r].m_Index;
			if(iSource<0)continue;
			for(short p=0;p<sTU->m_NbVaguesCalU;p++){
				for(short v=0;v<sTU->m_NbVague[iSource];v++){
					if(sTU->m_TableVagues[iSource][v].m_Nom==sTU->m_SourceEta[r].m_Vagues[p])break;
				}// boucle sur les vagues d'une source
				if(v>=sTU->m_NbVague[iSource]){
					str.Format("La vague %s de la source %s du noyau n'est pas trouvée dans la table des vagues en sortie d'affinage",
						sTU->m_SourceEta[r].m_Vagues[p],
						sTU->m_SourceEta[r].m_Nom);
					AfxMessageBox(str);
					fNoCodeSource=1;
				}
				else{
					sTU->m_SourceEta[r].m_NrIndexVague[p]=v;
				}
			} // boucle sur les périodes du calendrier de l'univers
		} // boucle par région
	} // boucle par univers
	if(fNoCodeSource){
		AfxMessageBox("Liaison TableUnivers-TableVagues  incomplète");
//		LibererStations(m_TableStations);
		return;
	}

	// LIEN UNIVERS-REGION:
	fNoCodeSource=0;
	for(u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		for(short r=0;r<sTU->m_NbRegion;r++){
			for(short i=0;i<m_TableRegions.GetSize();i++){
				if(sTU->m_CodeZone[r]==m_TableRegions[i].m_Code)break;
			} // boucle des sources (de TableSources)
			if(i>=m_TableRegions.GetSize()){
				fNoCodeSource=1;
			}
			else{
				sTU->m_LibRegion[r]=m_TableRegions[i].m_Libelle;
				sTU->m_LibRegion[r].TrimRight();
				sTU->m_LibRegionCourt[r]=m_TableRegions[i].m_LibelleCourt;
				sTU->m_LibRegionCourt[r].TrimRight();
				sTU->m_LibRegionMini[r]=m_TableRegions[i].m_LibelleMini;
				sTU->m_LibRegionMini[r].TrimRight();
				sTU->m_LibRegionLong[r]=m_TableRegions[i].m_LibelleLong;
				sTU->m_LibRegionLong[r].TrimRight();
			}
		} // boucle par région
	} // boucle par univers
	if(fNoCodeSource)AfxMessageBox("Liaison TableUnivers-TableRégion incomplète");

	if(m_fUniversUnique) return;
	// LIEN des STATIONS dans la tables UNIVERS
	for(u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		sTU->m_NbStation.Copy(m_NbStation[u]);
		sTU->m_TableStations=new CTableStationArray[sTU->m_NbRegion];
		for(short r=0 ; r<sTU->m_NbRegion ; r++)
			sTU->m_TableStations[r].Copy(m_TableStations[u][r]);
	}
}

/*------------------------------------------------------------------------
 Procedure:     RechercheBranches ID:1
 Purpose:       Fonction récursive de mise à jour d'une branche de l'arbre des
                régions, sans réduction.
 Input:         L'univers et la branche à traiter.
 Output:        La branche est mise à jour.
 Errors:
------------------------------------------------------------------------*/
void CNoyauDur::RechercheBranches(CTableUNIVERS * U,CArbreREGIONS *BrancheM)
{
	// recherche des branches, des branches .....
	short NbBranche=0;
	for(short r=0;r<U->m_NbRegion;r++){
		if(U->m_CodeMere[r]==BrancheM->m_CodeZone)
			NbBranche++;
	}

	if((BrancheM->m_NbBranche=NbBranche)==0){
		// c'est fini pour cette 'feuille ou fruit'...
		BrancheM->m_Branches=NULL;
		return;
	}

	BrancheM->m_Branches=new CArbreREGIONS*[NbBranche];
	short b;
	for(r=0,b=0;b<NbBranche;b++,r++){
		BrancheM->m_Branches[b]=new CArbreREGIONS;
		BrancheM->m_Branches[b]->m_Tronc=BrancheM;
		while(U->m_CodeMere[r]!=BrancheM->m_CodeZone)r++;
		BrancheM->m_Branches[b]->m_LibRegion=U->m_LibRegion[r];
		BrancheM->m_Branches[b]->m_LibRegionLong=U->m_LibRegionLong[r];
		BrancheM->m_Branches[b]->m_LibRegionCourt=U->m_LibRegionCourt[r];
		BrancheM->m_Branches[b]->m_CodeZone=U->m_CodeZone[r];
		BrancheM->m_Branches[b]->m_IndexRegion=r;
		RechercheBranches(U,BrancheM->m_Branches[b]);
	}
}

/*------------------------------------------------------------------------
 Procedure:     AroserArbre ID:1
 Purpose:       Construction de l'arbre des régions. La région mère n'est pas
                toujours celle de niveau le plus élevé.
 Input:         L'univers réduit et le nom de la région mère.
 Output:        L'univers réduit est mis à jour.
 Errors:
------------------------------------------------------------------------*/
void CNoyauDur::AroserArbre(void)
{
	CTableUNIVERS * sTU=NULL;
	for(short u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		// Tronc de l'arbre:
		CArbreREGIONS Temps;
		Temps.m_Tronc=NULL;
		Temps.m_LibRegion=sTU->m_NomNiveauMere;
		Temps.m_LibRegion.TrimRight();
		Temps.m_LibRegionLong=sTU->m_NomNiveauMereLong;
		Temps.m_LibRegionCourt=sTU->m_NomNiveauMereCourt;
		Temps.m_LibRegionCourt.TrimRight();
		Temps.m_CodeZone.Empty();
		Temps.m_IndexRegion=-1;
		RechercheBranches(sTU,&Temps);
		sTU->m_ArbreRegions.Add(Temps);
	}
}

/*------------------------------------------------------------------------
 Procedure:     MajInfoBidon ID:1
 Purpose:       Suite à séance de travail avec pwo, on évolue vers des
                structures mais pour l'instant on ne change pas les tables du
                noyau dur...
                En attendant on met des valeurs dans ces nouveaux champs.

 Input:
 Output:
 Errors:
------------------------------------------------------------------------*/
void CNoyauDur::MajInfoBidon(void)
{
	CTableUNIVERS * sTU=NULL;
	for(short u=0;u<m_TableUnivers.GetSize();u++){
		if(m_fUniversUnique && m_NrUniversUnique!=u)continue;
		sTU=&m_TableUnivers[u];
		for(short r=0;r<sTU->m_NbRegion;r++)sTU->m_DescriptionBis[r]=-1;
		sTU->m_SourceEtaBis.RemoveAll();
		sTU->m_SourceRefBis.RemoveAll();
		if(sTU->m_NbVaguesCalU<=0)sTU->m_NbVaguesCalU=-1;

		if(sTU->m_Code=="UFR01"){
			for(short r=0;r<sTU->m_NbRegion;r++){
				if (!sTU->m_NbStation.GetSize()) continue; // bug 32 bits
				for(short s=0;s<sTU->m_NbStation[r];s++){
					if (sTU->m_TableStations) {
					sTU->m_TableStations[r][s].m_TypeMedia=1;
					sTU->m_TableStations[r][s].m_HeureDebut=sTU->m_HeureDebut;
					}// bug 32 bits
				}
			}
		}
	}
}

/*------------------------------------------------------------------------
------------------------------------------------------------------------*/
void CNoyauDur::LibererTousUnivers(CTableUniversArray & M)
{
	for(short u=0 ; u<M.GetSize() ; u++){
		if(m_Marcheload || (m_Universload & (1<<(u+1)))){
			CTableUNIVERS pUnivers=M.GetAt(u);
			LibererUnivers(pUnivers);
		}
	}
}

void CNoyauDur::LibererUnivers(CTableUNIVERS & U)
{
	if(U.m_TableStations){
		delete [] U.m_TableStations;
		U.m_TableStations=NULL;
	}
	if(U.m_TableVagues){
		delete [] U.m_TableVagues;
		U.m_TableVagues=NULL;
	}
	if(U.m_NrPseudoStation){
		delete [] U.m_NrPseudoStation;
		U.m_NrPseudoStation=NULL;
	}
	if(U.m_NrStationCalcul){
		delete [] U.m_NrStationCalcul;
		U.m_NrStationCalcul=NULL;
	}
	if(U.m_TableStationCalcul){
		delete [] U.m_TableStationCalcul;
		U.m_TableStationCalcul=NULL;
	}
}

void CNoyauDur::LibererStations(CTableStationArray ** S)
{
	if(S)
	{
		for(short u=0 ; u<m_TableUnivers.GetSize() ; u++){
			if(m_Marcheload || (m_Universload & (1<<(u+1)))){
				for(short r=0 ; r<m_NbStation[u].GetSize() ; r++){
					for(short s=0;s<m_NbStation[u][r];s++){
						if(S[u][r][s].m_DateLien) delete [] S[u][r][s].m_DateLien;
//						CTableSTATION pStation=S[u][r].GetAt(s);
//						if(pStation.m_DateLien)	delete [] pStation.m_DateLien;
					}
				}
				if(S[u]){
					delete [] S[u];
				}
			}
		}
		delete [] S;
	}
}

void CNoyauDur::LibererSources(CTableSourceArray * Source)
{
	if(Source)	delete [] Source;
}

void CNoyauDur::LibererVagues(CTableVagueArray ** Vagues)
{
	if(Vagues)
	{
		for(short u=0 ; u<m_TableUnivers.GetSize() ; u++){
			if(m_Marcheload || (m_Universload & (1<<(u+1))))
				if(Vagues[u]){
					delete [] Vagues[u];
				}
		}
		delete [] Vagues;
	}
}

void CNoyauDur::LibererDicos(void)
{
	if(m_Dico1)		delete m_Dico1;
	if(m_Dico2)		delete m_Dico2;
	if(m_Dico3)		delete m_Dico3;
	m_Dico1=NULL;
	m_Dico2=NULL;
	m_Dico3=NULL;
}

void CNoyauDur::LibererMarche(void)
{
	LibererDicos();
	LibererStations(m_TableStations);
	LibererSources(m_TableSources);
	LibererVagues(m_TableVagues);
	LibererTousUnivers(m_TableUnivers);
	if(m_NbStation)		delete [] m_NbStation;
	if(m_NbVague)		delete [] m_NbVague;
	if(m_NbVagueMatrice)delete [] m_NbVagueMatrice;
	if(m_TableStationsMarche)	delete [] m_TableStationsMarche;
	RAZMembers();
	m_Init=false;
}

void CNoyauDur::RAZMembers(void)
{
	m_NbStation=NULL;
	m_NbVague=NULL;
	m_NbVagueMatrice=NULL;
	m_NrUniversUnique=0;
	m_Marcheload=false;
	m_Universload=0;
	m_CodeStation=5;
	m_Init=false;

	m_Marche=_T("");
	m_Univers=_T("");
	m_CheminData=_T("");
	m_CheminSource=_T("");
	m_CheminUnivers=_T("");
	m_CheminUser=_T("");
	m_CheminZone=_T("");
	
	m_fUniversUnique=false;
	
	m_TableRegies.RemoveAll();
	m_TableRegions.RemoveAll();
	m_TableMedias.RemoveAll();
	m_TableFamilles.RemoveAll();
	m_TableStations=NULL;
	m_TableStationsMarche=NULL;
	m_TableSources=NULL;
	m_TableVagues=NULL;
	m_TableUnivers.RemoveAll();
	m_Dico1=NULL;
	m_Dico2=NULL;
	m_Dico3=NULL;
}
