// FicAEJ.cpp: implementation of the CFicAEJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FicAEJ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D6022A
CFicAEJ::CFicAEJ()
{
	m_FileLoaded=0;
	m_DateDebut=0;
	m_NbJour=0;
	m_NbIndiv=0;
	m_NbStation=0;
}

//##ModelId=3D9AB7D6022B
CFicAEJ::~CFicAEJ()
{
	FreeFile();

}

#ifdef OPTION_DUREEECOUTE
//##ModelId=3D9AB7D60297
CProbaSupport * CFicAEJ::GetDureeEcoute(int station,int jour,int support)
{
	int e;
	for(e=0;e<m_NbEcran[station][jour];e++){
		if(m_NrEcran[station][jour][e]==support)return(&m_DureeQH[station][jour][e]);
	}
	return(NULL);
}
#endif
//##ModelId=3D9AB7D602A3
CProbaSupport * CFicAEJ::GetProbabilite(int station,int jour,int support)
{
	int e;

	if(GetNrStation(station)>=1000)
	{
		// C'est une station dont le probabilités proviennent de l'étude
		// Médiacabsat qui est en Quart d'heure
		for(e=0;e<m_NbEcranP[station][jour];e++)
		{
			if(m_NrEcranP[station][jour][e]+14>=support)
			{
				return(&m_ProbaQH[station][jour][e]);
			}
		}
	}
	else
	{
		// C'est une station probabilisé par le Médiadata donc des vrai écrans
		for(e=0;e<m_NbEcranP[station][jour];e++)
		{
			if(m_NrEcranP[station][jour][e]>=support)
			{
				// on retourne l'écran probabilisé le plus proche:
				// Si c'est le premier c'est déjà bon
				if(!e)return(&m_ProbaQH[station][jour][e]);
				// On compte les minutes avant et après:
				int NbApres=m_NrEcranP[station][jour][e]-support;
				int NbAvant=support-m_NrEcranP[station][jour][e-1];
				// On retourne la bonne probabilié en privilégiant
				// celle qui siot l'ecran pour favoriser le défaut
				// donné par les code à la place de l'horaire
				if(NbAvant<NbApres)return(&m_ProbaQH[station][jour][e-1]);
				else return(&m_ProbaQH[station][jour][e]);
			}
		}
	}

	return(&m_ProbaQH[station][jour][e-1]);
}

#ifdef OPTION_DLGPROGRESS
#include "Dlgprogress.h"
#endif

//##ModelId=3D9AB7D6023D
LPCSTR CFicAEJ::BindNomFichierUnique(LPCSTR NomFic)
{
	m_NomFic=NomFic;
	m_NomFic+=_T(".AEJ");
	return(m_NomFic);
}

//##ModelId=3D9AB7D60234
int CFicAEJ::LoadFile(LPCSTR NomFic)
{
	unsigned short NbIndiv;
	int id;
	CWaitCursor wait;

	bool flagpbproba=0;

#ifdef OPTION_DLGPROGRESS
	CDlgProgress dlg;
	dlg.Create(IDD_PROGRESS);
	dlg.CenterWindow();
	dlg.ShowWindow(SW_SHOW);
	dlg.UpdateWindow();
#endif

	FreeFile();

	BindNomFichierUnique(NomFic);
	CStdioFile FicJ,FicP;
	CJFCFileException e;
	short s,j,q,c;

#ifdef OPTION_DUREEECOUTE
	short d;
	if(!FicJ.Open(m_NomFic,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",m_NomFic);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	CFileStatus filestatus;
	CTime TimeFicAQJ,TimeFicAQP;
	FicJ.GetStatus(filestatus);
	TimeFicAQJ=filestatus.m_mtime;

	unsigned short Nr;
	unsigned char proba;

	bool pbproba=0;

	FicJ.Read(&m_DateDebut,sizeof(long));
	FicJ.Read(&m_NbJour,sizeof(short));
	FicJ.Read(&NbIndiv,sizeof(unsigned short));
	m_NbIndiv = NbIndiv;
	FicJ.Read(&m_NbStation,sizeof(short));
	m_NrStation=new short [m_NbStation];
	m_DureeQH = new CProbaSupport ** [m_NbStation];
	m_NbEcran = new short * [m_NbStation];
	m_NrEcran = new short ** [m_NbStation];
	for(s=0;s<m_NbStation;s++){
		FicJ.Read(&m_NrStation[s],sizeof(short));
		m_DureeQH[s] = new CProbaSupport * [m_NbJour];
		m_NbEcran[s] = new short [m_NbJour];
		m_NrEcran[s] = new short * [m_NbJour];
		for(j=0;j<m_NbJour;j++){
			FicJ.Read(&m_NbEcran[s][j],sizeof(short));
			if(m_NbEcran[s][j]>0)
			{
				m_NrEcran[s][j] = new short [m_NbEcran[s][j]];
				m_DureeQH[s][j] = new CProbaSupport [m_NbEcran[s][j]];
				for(q=0;q<m_NbEcran[s][j];q++){
					FicJ.Read(&m_NrEcran[s][j][q],sizeof(short));
					FicJ.Read(&Nr,sizeof(short));
					m_DureeQH[s][j][q].SetNbCas(Nr);
					if(Nr<=0)pbproba=1;
					for(c=0;c<m_DureeQH[s][j][q].m_NbCas;c++){
						FicJ.Read(&Nr,sizeof(unsigned short));
						FicJ.Read(&proba,sizeof(char)); // Durée d'écoute en 250 ème
						ASSERT(Nr>=0 && Nr<m_NbIndiv);
						ASSERT(proba>0 && proba<=250);
						m_DureeQH[s][j][q].m_NrIndividu[c]=Nr;
						m_DureeQH[s][j][q].m_Proba[c]=((short)proba)*4;
					}
				}
			}
			else
			{
				m_NrEcran[s][j] = NULL;
				m_DureeQH[s][j] = NULL;
			}
#ifdef OPTION_DLGPROGRESS
			dlg.m_Progress.SetPos((50*((s*m_NbJour)+j+1))/(m_NbStation*m_NbJour));
#endif
		}
	}
	FicJ.Close();
	TRACE("Le fichier AEJ est chargé.");

	if(pbproba)
	{
		AfxMessageBox("Ce carnet d'ecoute présente des défauts.\nPour remedier à ce problème, il faut demander à JFC un nouveau fichier.");
	}
#endif // OPTION_DUREEECOUTE

	CString NomFicAEP=NomFic;
	NomFicAEP+=".AEP";

	if(!FicP.Open(NomFicAEP,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite/*, &e*/ ) )
	{
#ifndef OPTION_DUREEECOUTE
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicAEP);
		e.AfficheErreur(MsgErr);
		return(1);
#else
ProbabilisationForcee:
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Probabilisation du panel constant");
#endif
		TRACE("Probabilisation du fichier AEP");

		// Etablissons la liste des écrans à probabiliser
		#define MAXLIST (3000)
		char Liste[MAXLIST];
		int ecr;

		m_NbEcranP=new short * [m_NbStation];
		m_NrEcranP=new short ** [m_NbStation];
		for(s=0;s<m_NbStation;s++){
			m_NbEcranP[s]=new short [7];
			m_NrEcranP[s]=new short * [7];
			for(int jousem=0;jousem<7;jousem++){
				memset(Liste,0,MAXLIST);
				for(int jouper=jousem;jouper<m_NbJour;jouper+=7){
					for(ecr=0;ecr<m_NbEcran[s][jouper];ecr++){
						Liste[m_NrEcran[s][jouper][ecr]]=1;
					}
				}
				m_NbEcranP[s][jousem]=0;
				for(ecr=0;ecr<MAXLIST;ecr++)if(Liste[ecr])m_NbEcranP[s][jousem]++;
				m_NrEcranP[s][jousem]=new short [m_NbEcranP[s][jousem]];
				int nr=0;
				for(ecr=0;ecr<MAXLIST;ecr++){
					if(Liste[ecr]){
						m_NrEcranP[s][jousem][nr]=ecr;
						nr++;
					}
				}
			}
		}

		// Probabilisation des tous les écrans
		long * Duree=new long [m_NbIndiv];
		int SizeOfDuree=sizeof(long)*m_NbIndiv;
		int NbSemaine=m_NbJour/7;
		int Arrondie;
		int NbCas;

		if(m_NbJour%7){
			CString MsgErr;
			MsgErr.Format("BUG: Le fichier %s décrit %d jours (pas divisible par 7!)",m_NomFic,m_NbJour);
			AfxMessageBox(MsgErr);
			return(2);
		}

		if(NbSemaine<=0){
			CString MsgErr;
			MsgErr.Format("BUG: Le fichier %s décrit 0 semaines !",m_NomFic);
			AfxMessageBox(MsgErr);
			return(2);
		}

		m_ProbaQH = new CProbaSupport ** [m_NbStation];
		for(s=0;s<m_NbStation;s++){
			m_ProbaQH[s] = new CProbaSupport * [7];
			for(j=0;j<7;j++){
				m_ProbaQH[s][j] = new CProbaSupport [m_NbEcranP[s][j]];
				for(q=0;q<m_NbEcranP[s][j];q++){
					memset(Duree,0,SizeOfDuree);
					int NbEcran=0;
					// Cumulation des durées d'écoutes
					for(d=j;d<m_NbJour;d+=7){
						// Recherche de l'écran
						for(ecr=0;ecr<m_NbEcran[s][d];ecr++){
							if(m_NrEcran[s][d][ecr]==m_NrEcranP[s][j][q]){
								for(c=0;c<m_DureeQH[s][d][ecr].m_NbCas;c++){
									Duree[m_DureeQH[s][d][ecr].m_NrIndividu[c]]+=m_DureeQH[s][d][ecr].m_Proba[c];
								}
								NbEcran++;
								break;
							}
						}
					}

					// Comptage des individus touchés
					ASSERT(NbEcran);
					Arrondie=NbEcran/2;
					NbCas=0;
					for(id=0;id<m_NbIndiv;id++){
						if(Duree[id]<=0)continue;
						Duree[id]+=Arrondie;
						Duree[id]/=NbEcran;
						if(Duree[id]<=0)Duree[id]=1;
						ASSERT(Duree[id]>0 && Duree[id]<=1000);
						if(Duree[id]>1000)
						{
							Duree[id]=1000;
							flagpbproba=1;
						}
						NbCas++;
					}

					// Rangement des probabilités
					ASSERT(NbCas>0);
					m_ProbaQH[s][j][q].SetNbCas(NbCas);
					for(id=0,c=0;id<m_NbIndiv;id++){
						if(Duree[id]<=0)continue;
						m_ProbaQH[s][j][q].m_NrIndividu[c]=id;
						m_ProbaQH[s][j][q].m_Proba[c]=(unsigned short)Duree[id];
						ASSERT(Duree[id]>0 && Duree[id]<=1000);
						c++;
					}
				}
#ifdef OPTION_DLGPROGRESS
				dlg.m_Progress.SetPos(50+(40*((s*7)+j+1))/(m_NbStation*7));
#endif
			}
		}
		delete [] Duree;

		TRACE("Ecriture du fichier AEP");
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Sauvegarde du panel probabilisé");
#endif
		if(FicP.Open(NomFicAEP,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary|CFile::shareExclusive, &e ) ){
			unsigned short Nr;
			unsigned short proba;


			// Le format est identique,
			// Mais on n'utilise pas l'info.
			FicP.Write(&m_DateDebut,sizeof(long));
			short NbJourProbabilise=7;
			FicP.Write(&NbJourProbabilise,sizeof(short));
			FicP.Write(&NbIndiv,sizeof(unsigned short));
			m_NbIndiv = NbIndiv;
			FicP.Write(&m_NbStation,sizeof(short));
			for(s=0;s<m_NbStation;s++){
				FicP.Write(&m_NrStation[s],sizeof(short));
				for(j=0;j<NbJourProbabilise;j++){
					FicP.Write(&m_NbEcranP[s][j],sizeof(short));
					for(q=0;q<m_NbEcranP[s][j];q++){
						FicP.Write(&m_NrEcranP[s][j][q],sizeof(short));
						Nr=m_ProbaQH[s][j][q].m_NbCas;
						FicP.Write(&Nr,sizeof(short));
						for(c=0;c<m_ProbaQH[s][j][q].m_NbCas;c++){
							Nr=m_ProbaQH[s][j][q].m_NrIndividu[c];
							proba=m_ProbaQH[s][j][q].m_Proba[c];
							FicP.Write(&Nr,sizeof(unsigned short));
							FicP.Write(&proba,sizeof(short)); // Probabilité en 1000 ème
							ASSERT(Nr>=0 && Nr<m_NbIndiv);
							ASSERT(proba>0 && proba<=1000);
						}
					}
#ifdef OPTION_DLGPROGRESS
					dlg.m_Progress.SetPos(90+(10*((s*NbJourProbabilise)+j+1))/(m_NbStation*NbJourProbabilise));
#endif
				}
			}
			FicP.Close();
			TRACE("Le fichier AEP est crée.");
			CFile::GetStatus(NomFicAEP,filestatus);
			filestatus.m_mtime=TimeFicAQJ;
			CFile::SetStatus(NomFicAEP,filestatus);
		}
		else {
			CString txt;
			txt.Format("Impossible de sauvegarder le panel probabilisé %s : ",NomFicAEP);
			e.AfficheErreur(txt);
		}
#endif // OPTION_DUREEECOUTE
	}
	else {
		TRACE("Lecture du fichier AEP\n");
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Chargement du panel probabilisé");
#endif

#ifdef OPTION_DUREEECOUTE
		// La date est-elle bonne ?
		FicP.GetStatus(filestatus);
		TimeFicAQP=filestatus.m_mtime;
		if(TimeFicAQP!=TimeFicAQJ){
			FicP.Close();
			goto ProbabilisationForcee;
		}

		// Suite à un bug dans les fichiers, et dans la probabilisation
		// On est sûr qu'il faut reprobaliser
		// les fichiers datants d'avant 10/11/00
		CTime date(2000,11,10,0,0,0);
		if(TimeFicAQP<date)
		{
			FicP.Close();
			goto ProbabilisationForcee;
		}

#endif

		unsigned short Nr;
		unsigned short proba;
		long vlong;
#ifdef OPTION_DUREEECOUTE
		short vshort;
#endif
		short NbJourProbabilise;

		// Le format est identique,
		// Mais on n'utilise pas l'info.
		FicP.Read(&vlong,sizeof(long));
		FicP.Read(&NbJourProbabilise,sizeof(short));
#ifndef OPTION_DUREEECOUTE
		FicP.Read(&NbIndiv,sizeof(unsigned short));
		m_NbIndiv = NbIndiv;
		FicP.Read(&m_NbStation,sizeof(short));
		m_NrStation=new short [m_NbStation];
#else
		FicP.Read(&vshort,sizeof(short));
		FicP.Read(&vshort,sizeof(short));
#endif
		m_ProbaQH = new CProbaSupport ** [m_NbStation];
		m_NbEcranP= new short * [m_NbStation];
		m_NrEcranP= new short ** [m_NbStation];
		for(s=0;s<m_NbStation;s++){
#ifndef OPTION_DUREEECOUTE
			FicP.Read(&m_NrStation[s],sizeof(short));
#else
			FicP.Read(&vshort,sizeof(short));
#endif
			m_ProbaQH[s] = new CProbaSupport * [NbJourProbabilise];
			m_NbEcranP[s]= new short [NbJourProbabilise];
			m_NrEcranP[s]= new short * [NbJourProbabilise];
			for(j=0;j<NbJourProbabilise;j++){
				FicP.Read(&m_NbEcranP[s][j],sizeof(short));
				m_NrEcranP[s][j]= new short [m_NbEcranP[s][j]];
				m_ProbaQH[s][j] = new CProbaSupport [m_NbEcranP[s][j]];
				for(q=0;q<m_NbEcranP[s][j];q++){
					FicP.Read(&m_NrEcranP[s][j][q],sizeof(short));
					FicP.Read(&Nr,sizeof(short));
//					ASSERT(Nr); A REMETRE
					m_ProbaQH[s][j][q].SetNbCas(Nr);
					for(c=0;c<m_ProbaQH[s][j][q].m_NbCas;c++){
						FicP.Read(&Nr,sizeof(unsigned short));
						FicP.Read(&proba,sizeof(short));
						ASSERT(Nr>=0 && Nr<m_NbIndiv);
						ASSERT(proba>0 && proba<=1000);
						m_ProbaQH[s][j][q].m_NrIndividu[c]=Nr;
						m_ProbaQH[s][j][q].m_Proba[c]=proba; // Probabilité en 1000 ème
					}
				}
#ifdef OPTION_DLGPROGRESS
				dlg.m_Progress.SetPos(50+(50*((s*NbJourProbabilise)+j+1))/(m_NbStation*NbJourProbabilise));
#endif
			}
		}
		FicP.Close();
		TRACE("Le fichier AEP est chargé.\n");
	}

#ifdef OPTION_DLGPROGRESS
	dlg.DestroyWindow();
#endif
	m_FileLoaded=1;
	ASSERT(_CrtCheckMemory());

	if(flagpbproba)
	{
		AfxMessageBox("Le programme de calcul de probabilités a détecté des erreurs dans le carnet d'écoute.");
	}

	return(0);
}

//##ModelId=3D9AB7D60233
void CFicAEJ::FreeFile()
{
	if(!m_FileLoaded)return;

	short s,j;

	// Libération des durées d'écoute
#ifdef OPTION_DUREEECOUTE
	for(s=0;s<m_NbStation;s++){
		for(j=0;j<m_NbJour;j++){
			delete [] m_DureeQH[s][j];
			delete [] m_NrEcran[s][j];
		}
		delete [] m_DureeQH[s];
		delete [] m_NbEcran[s];
		delete [] m_NrEcran[s];
	}
	delete [] m_DureeQH;
	delete [] m_NbEcran;
	delete [] m_NrEcran;
#endif

	// Libération des probabilités
	for(s=0;s<m_NbStation;s++)
	{
		for(j=0;j<7;j++)
		{
			delete [] m_ProbaQH[s][j];
		}
		delete [] m_ProbaQH[s];
	}
	delete [] m_ProbaQH;
	delete [] m_NrStation;

	// Libération de la liste des écrans probabilisés
	for(s=0;s<m_NbStation;s++){
		for(j=0;j<7;j++){
			delete [] m_NrEcranP[s][j];
		}
		delete [] m_NrEcranP[s];
		delete [] m_NbEcranP[s];
	}
	delete [] m_NrEcranP;
	delete [] m_NbEcranP;



	m_NrStation=NULL;
	m_ProbaQH=NULL;
	m_DureeQH=NULL;
	m_FileLoaded=0;

	m_DateDebut=0;
	m_NbJour=0;
	m_NbIndiv=0;
	m_NbStation=0;
}



// Calcul pour chaque individu à l'appartenance à une strate d'individu.
// Le nombre de strate est déterminé par l'argument
//##ModelId=3D9AB7D6021F
void CFicAEJ::CalculRatioConsomation(int NbRatio)
{
	int i,s;
	CWaitCursor wait;
	m_NrRatioConso.SetSize(m_NbIndiv);


	// 1/ On boucle sur le écoutes (probabilités) pour établir la consomation
	// de chacun des individus dans le média concerné :

	m_Consomation.SetSize(m_NbIndiv);
	for(i=0;i<m_NbIndiv;i++)m_Consomation[i]=0;
	int NbJour=7; // Jour probabilisé
	for(s=0;s<m_NbStation;s++)
	{
		for(int j=0;j<NbJour;j++)
		{
			for(int e=0;e<m_NbEcranP[s][j];e++)
			{
				for(int i=0;i<m_ProbaQH[s][j][e].m_NbCas;i++)
				{
					m_Consomation[m_ProbaQH[s][j][e].m_NrIndividu[i]]+=m_ProbaQH[s][j][e].m_Proba[i];
				}
			}
		}
	}


	// Maintenant on range les résultats pour constituer des strates de consomation
	// Il suffit de boucler autant de fois qu'il y a d'indiv
	CDWordArray Conso;
	Conso.Copy(m_Consomation);
	for(i=0;i<m_NbIndiv;i++)
	{
		int index=-1;
		DWORD mini=0;
		for(int b=0;b<m_NbIndiv;b++)
		{
			if(Conso[b]==-1)continue; // Individu déjà marqué
			if(index<0)
			{
				// On prend le premier
				index=b;
				mini=Conso[b];
			}
			else if(Conso[b]<mini)
			{
				// On le prend car il est pour le moment
				// le plus petit que l'on est trouvé
				index=b;
				mini=Conso[b];
			}
		}

		ASSERT(index>=0 && index<m_NbIndiv);
		m_NrRatioConso[index]=(i*NbRatio)/m_NbIndiv;
		ASSERT(m_NrRatioConso[index]>=0);
		ASSERT(m_NrRatioConso[index]<NbRatio);
		Conso[index]=-1;
	}
}
