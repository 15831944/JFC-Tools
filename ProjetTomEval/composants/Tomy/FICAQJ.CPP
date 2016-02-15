// FicAQJ.cpp: implementation of the CFicAQJ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FicAQJ.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//##ModelId=3D9AB7D600A3
CFicAQJ::CFicAQJ()
{
	m_FileLoaded=0;

	m_DateDebut=0;
	m_NbJour=0;
	m_NbIndiv=0;
	m_NbStation=0;
}

//##ModelId=3D9AB7D600A4
CFicAQJ::~CFicAQJ()
{
	FreeFile();
}

#ifdef OPTION_DLGPROGRESS
#include "Dlgprogress.h"
#endif

//##ModelId=3D9AB7D60099
LPCSTR CFicAQJ::BindNomFichierUnique(LPCSTR NomFic)
{
	m_NomFic=NomFic;
	m_NomFic+=_T(".AQJ");
	return(m_NomFic);
}
/*
unsigned char *CFicAQJ::GetEcoutesJRS()
{
	return m_EcoutesJRS;
}
*/
//##ModelId=3D9AB7D60090
int CFicAQJ::LoadFile(LPCSTR NomFic,int NbIndiv)
{
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

	BindNomFichierUnique(NomFic);	// ex : Source\SRU10\JFC_S112
	CStdioFile FicAQJ,FicAQP,FicEVR,FicWLY,FicMLY,FicRGN,FicJRS;
	CJFCFileException e;
	short s,j,q,c,i;

#ifdef OPTION_DUREEECOUTE
	short d;
	if(!FicAQJ.Open(m_NomFic,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",m_NomFic);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	CFileStatus filestatus;
	CTime TimeFicAQJ,TimeFicAQP,TimeFicEVR;
	FicAQJ.GetStatus(filestatus);
	TimeFicAQJ=filestatus.m_mtime;

#ifdef OPTION_RUSSIE
	short          NbEcoutes;
	int            NoIndividu;
	unsigned char  ValEcoute;

	FicAQJ.Read(&m_DateDebut, sizeof(long));
	FicAQJ.Read(&m_NbJour,    sizeof(short));
#ifdef OPTION_NOVA_RUSSE
	int NbIndividus;
	FicAQJ.Read(&NbIndividus, sizeof(int));
	m_NbIndiv = NbIndividus;
#elif  OPTION_CIM_RUSSE
	unsigned short NbIndividus;
	FicAQJ.Read(&NbIndividus, sizeof(unsigned short));
	m_NbIndiv = (int)NbIndividus;
#else
	if (NbIndiv >= 65536) {	// #ifdef OPTION_MAROC		// 2**16
		int NbIndividus;
		FicAQJ.Read(&NbIndividus, sizeof(int));
		m_NbIndiv = NbIndividus;
	} else {	// #else
	// ******************** SOLUTION PROVISOIRE si NbIndividus en Short
		unsigned short NbIndividus;
		FicAQJ.Read(&NbIndividus, sizeof(unsigned short));
		m_NbIndiv = (int)NbIndividus;
	}	// #endif
#endif
	FicAQJ.Read(&m_NbStation,sizeof(short));
	m_NrStation=new short [m_NbStation];
	m_EcoutesQH = new CEcouteSupport ** [m_NbStation];
	for(s=0;s<m_NbStation;s++){
		FicAQJ.Read(&m_NrStation[s],sizeof(short));
		m_EcoutesQH[s] = new CEcouteSupport * [m_NbJour];
		for(j=0;j<m_NbJour;j++){
			m_EcoutesQH[s][j] = new CEcouteSupport[96];
			for(q=0;q<96;q++){
				FicAQJ.Read(&NbEcoutes,sizeof(short));
				m_EcoutesQH[s][j][q].SetNbEcoutes((long)NbEcoutes);
				for(c=0;c<m_EcoutesQH[s][j][q].m_NbEcoutes;c++){
#ifdef OPTION_NOVA_RUSSE
					FicAQJ.Read(&NoIndividu,sizeof(int));
#elif OPTION_CIM_RUSSE
					unsigned short NoIndividuS;
					FicAQJ.Read(&NoIndividuS,sizeof(unsigned short));
					NoIndividu = (int)NoIndividuS;
#else
					if (NbIndiv >= 65536) {		// 	#ifdef OPTION_MAROC
						FicAQJ.Read(&NoIndividu,sizeof(int)); 
					} else {			//	#else
					// ******************** SOLUTION PROVISOIRE si NbIndividus en Short
						unsigned short NoIndividuS;
						FicAQJ.Read(&NoIndividuS,sizeof(unsigned short));
						NoIndividu = (int)NoIndividuS;
					}					// #endif
#endif
					FicAQJ.Read(&ValEcoute,sizeof(unsigned char));
					ASSERT(NoIndividu>=0 && NoIndividu<m_NbIndiv);
					ASSERT(ValEcoute>0);
					m_EcoutesQH[s][j][q].m_pNoIndividus[c]=NoIndividu;
					m_EcoutesQH[s][j][q].m_pValEcoutes [c]=ValEcoute;
				}
			}
#ifdef OPTION_DLGPROGRESS
			dlg.m_Progress.SetPos((50*((s*m_NbJour)+j+1))/(m_NbStation*m_NbJour));
#endif
		}
	}
#else
	unsigned short Nr;
	unsigned char proba;
	unsigned short NbIndiv;

	FicAQJ.Read(&m_DateDebut,sizeof(long));
	FicAQJ.Read(&m_NbJour,sizeof(short));
	FicAQJ.Read(&NbIndiv,sizeof(unsigned short));
	m_NbIndiv = NbIndiv;
	FicAQJ.Read(&m_NbStation,sizeof(short));
	m_NrStation=new short [m_NbStation];
	m_DureeQH = new CProbaSupport ** [m_NbStation];
	//CStdioFile TraceFile("c:\\test.txt",CFile::modeCreate   | CFile::modeWrite | CFile::typeText );
	//CString txt;

	for(s=0;s<m_NbStation;s++){
		//txt.Format("STATION %d\n",s);
		//TraceFile.WriteString(txt);
		FicAQJ.Read(&m_NrStation[s],sizeof(short));
		//txt.Format("NUMERO STATION %d\n",m_NrStation[s]);
		//TraceFile.WriteString(txt);
		m_DureeQH[s] = new CProbaSupport * [m_NbJour];
		for(j=0;j<m_NbJour;j++){
		//txt.Format("JOUR %d\n",j);
		//TraceFile.WriteString(txt);
			m_DureeQH[s][j] = new CProbaSupport [96];
			for(q=0;q<96;q++){
				FicAQJ.Read(&Nr,sizeof(short));
				//txt.Format("%02dH%02d : %d individu(s):\n",(q/4)+3,(q%4)*15,Nr);
				//TraceFile.WriteString(txt);
				m_DureeQH[s][j][q].SetNbCas(Nr);
				for(c=0;c<m_DureeQH[s][j][q].m_NbCas;c++){
					FicAQJ.Read(&Nr,sizeof(unsigned short));
					FicAQJ.Read(&proba,sizeof(char)); // Durée d'écoute en 250 ème
					ASSERT(Nr>=0 && Nr<m_NbIndiv);
					ASSERT(proba>0 && proba<=250);
					m_DureeQH[s][j][q].m_NrIndividu[c]=Nr;
					m_DureeQH[s][j][q].m_Proba[c]=((short)proba)*4;
					//TRACE("l'individu %d a une proba de %d\n",Nr,proba);
					//txt.Format("%d\t%d\n",Nr,proba);
					//TraceFile.WriteString(txt);
//					ASSERT(!(s==2 && j==19 && q==63 && Nr==310));
				}
			}
#ifdef OPTION_DLGPROGRESS
			dlg.m_Progress.SetPos((50*((s*m_NbJour)+j+1))/(m_NbStation*m_NbJour));
#endif
		}
	}
#endif
	FicAQJ.Close();
	TRACE("Le fichier AQJ est chargé.\n");
#endif // OPTION_DUREEECOUTE

#ifdef OPTION_NOVA_RUSSE
	CString NomFicWLY=NomFic;
	NomFicWLY+=".WLY";
	if(!FicWLY.Open(NomFicWLY,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicWLY);
		e.AfficheErreur(MsgErr);
		return(1);
	}
	int   NbInd32;
	short NbSta16;
	FicWLY.Read(&NbInd32, sizeof(int));	  ASSERT(NbInd32==m_NbIndiv);
	FicWLY.Read(&NbSta16, sizeof(short)); ASSERT(NbSta16==m_NbStation);
	m_EcoutesWLY = new CEcouteSupport[m_NbStation];
	for(s=0;s<m_NbStation;s++){
		short NoSta;
		int   NbWLY;
		FicWLY.Read(&NoSta, sizeof(short)); ASSERT(NoSta==m_NrStation[s]);
		FicWLY.Read(&NbWLY, sizeof(int));   ASSERT(NbWLY > 0 && NbWLY <= m_NbIndiv);
		m_EcoutesWLY[s].SetNbEcoutes(NbWLY);
		for (int IdxWLY = 0; IdxWLY < NbWLY; IdxWLY += 1)
		{
			int NoIndiv;
			FicWLY.Read(&NoIndiv, sizeof(int)); ASSERT(NoIndiv >= 0 && NoIndiv < m_NbIndiv);
			m_EcoutesWLY[s].m_pNoIndividus[IdxWLY] = NoIndiv;
			m_EcoutesWLY[s].m_pValEcoutes [IdxWLY] = 1;
		}
	}
	FicWLY.Close();
	TRACE("Les fichiers WLY est chargé.\n");

	CString NomFicJRS=NomFic;
	NomFicJRS+=".JRS";
	if(!FicJRS.Open(NomFicJRS,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicJRS);
		e.AfficheErreur(MsgErr);
		return(1);
	}
	m_EcoutesJRS = new unsigned char [m_NbIndiv];

	for (int IdxJRS = 0; IdxJRS < m_NbIndiv; IdxJRS += 1) {
		char Ec;
		FicJRS.Read(&Ec, sizeof(char));

		if (Ec < 1 || Ec > m_NbJour) {
	   	CString txt;
         txt.Format("Le jour dans le fichier JRS est invalide. : %c",Ec);
				::AfxMessageBox(txt);
            ASSERT(0);
         return (1);
		}
		m_EcoutesJRS[IdxJRS] = Ec-1;
	}
	FicJRS.Close();
	TRACE("Le fichier JRS est chargé.\n");


#elif defined OPTION_RUSSIE && !defined(OPTION_CIM_RUSSE)
	CString NomFicWLY=NomFic;
	CString NomFicMLY=NomFic;
	NomFicWLY+=".WLY";
	NomFicMLY+=".MLY";
	if(!FicWLY.Open(NomFicWLY,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicWLY);
		e.AfficheErreur(MsgErr);
		return(1);
	}
	if(!FicMLY.Open(NomFicMLY,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicMLY);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	int   NbInd32;
	short NbSta16;
	FicWLY.Read(&NbInd32, sizeof(int));	  ASSERT(NbInd32==m_NbIndiv);
	FicMLY.Read(&NbInd32, sizeof(int));	  ASSERT(NbInd32==m_NbIndiv);
	FicWLY.Read(&NbSta16, sizeof(short)); ASSERT(NbSta16==m_NbStation);
	FicMLY.Read(&NbSta16, sizeof(short)); ASSERT(NbSta16==m_NbStation);
	m_EcoutesWLY = new CEcouteSupport[m_NbStation];
	m_EcoutesMLY = new CEcouteSupport[m_NbStation];
	for(s=0;s<m_NbStation;s++){
		short NoSta;
		int   NbWLY;
		int   NbMLY;
		FicWLY.Read(&NoSta, sizeof(short)); ASSERT(NoSta==m_NrStation[s]);
		FicMLY.Read(&NoSta, sizeof(short)); ASSERT(NoSta==m_NrStation[s]);
		FicWLY.Read(&NbWLY, sizeof(int));   ASSERT(NbWLY > 0 && NbWLY <= m_NbIndiv);
		FicMLY.Read(&NbMLY, sizeof(int));   ASSERT(NbMLY > 0 && NbMLY <= m_NbIndiv);
		m_EcoutesWLY[s].SetNbEcoutes(NbWLY);
		m_EcoutesMLY[s].SetNbEcoutes(NbMLY);
		for (int IdxWLY = 0; IdxWLY < NbWLY; IdxWLY += 1)
		{
			int NoIndiv;
			FicWLY.Read(&NoIndiv, sizeof(int)); ASSERT(NoIndiv >= 0 && NoIndiv < m_NbIndiv);
			m_EcoutesWLY[s].m_pNoIndividus[IdxWLY] = NoIndiv;
			m_EcoutesWLY[s].m_pValEcoutes [IdxWLY] = 1;
		}
		for (int IdxMLY = 0; IdxMLY < NbMLY; IdxMLY += 1)
		{
			int NoIndiv;
			FicMLY.Read(&NoIndiv, sizeof(int)); ASSERT(NoIndiv >= 0 && NoIndiv < m_NbIndiv);
			m_EcoutesMLY[s].m_pNoIndividus[IdxMLY] = NoIndiv;
			m_EcoutesMLY[s].m_pValEcoutes [IdxMLY] = 1;
		}
	}
	FicWLY.Close();
	FicMLY.Close();
	TRACE("Les fichiers WLY et MLY sont chargés.\n");
#endif

#if defined OPTION_RUSSIE && !defined(OPTION_CIM_RUSSE)
/*
	CString NomFicRGN=NomFic;
	NomFicRGN+=".RGN";
	if(!FicRGN.Open(NomFicRGN,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicRGN);
		e.AfficheErreur(MsgErr);
		return(1);
	}
	m_RegionsIDV = new unsigned char[m_NbIndiv];
	FicRGN.Read((void*)m_RegionsIDV, m_NbIndiv * sizeof(unsigned char));
	FicRGN.Close();
	TRACE("Le fichier RGN est chargé.\n");
*/
#endif

#ifdef OPTION_REACH_BELGE
	CString NomFicEVR=NomFic;
	NomFicEVR+=".EVR";
	if(!FicEVR.Open(NomFicEVR,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite , &e ) )
	{
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicEVR);
		e.AfficheErreur(MsgErr);
		return(1);
	}

	unsigned short NbI,NbS;
	unsigned short NrS,NbC;

	FicEVR.Read(&NbI,sizeof(unsigned short));		// m_NbIndiv
	FicEVR.Read(&NbS,sizeof(unsigned short));		// m_NbStation
	m_Ecoute=new CEverReach [m_NbStation];

	for(s=0;s<NbS;s++){
		FicEVR.Read(&NrS,sizeof(unsigned short));
		FicEVR.Read(&NbC,sizeof(unsigned short));
		for (j=0;j<m_NbStation;j++)
			if (NrS == m_NrStation[j]) break;
		if (j < m_NbStation) {
			m_Ecoute[j].SetNbCas(NbC,NrS);
			for (c=0;c<NbC;c++) {
				FicEVR.Read(&Nr,sizeof(unsigned short));
				m_Ecoute[j].m_NrIndividu[c] = Nr;
			}
		}
		else {
			for (c=0;c<NbC;c++)
				FicEVR.Read(&Nr,sizeof(unsigned short));
		}
	}
	FicEVR.Close();
	TRACE("Le fichier EVR est chargé.\n");
#endif //

#ifdef OPTION_CIMBELGE
	m_FileLoaded=1;
	return 0;
#elif OPTION_RUSSIE
	m_FileLoaded=1;
	return 0;
#endif

	CString NomFicAQP=NomFic;
	NomFicAQP+=".AQP";

	if(!FicAQP.Open(NomFicAQP,CFile::modeRead  | CFile::typeBinary|CFile::shareDenyWrite/*, &e*/ ) )
	{
#ifndef OPTION_DUREEECOUTE
		CString MsgErr;
		MsgErr.Format("Erreur à l'ouverture du fichier %s : ",NomFicAQP);
		e.AfficheErreur(MsgErr);
		return(1);
#else
ProbabilisationForcee:
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Probabilisation du panel constant");
#endif
		TRACE("Probabilisation du fichier AQP\n");
		// Probabilisation des tous les quart d'heure
		long * Duree=new long [m_NbIndiv];
		int SizeOfDuree=sizeof(long)*m_NbIndiv;
		int NbSemaine=m_NbJour/7;
		int Arrondie=NbSemaine/2;
		int NbCas;

		if(m_NbJour%7){
			CString MsgErr;
			MsgErr.Format("BUG: Le fichier %s décrit %d jours (pas divisible par 7!)",m_NomFic,m_NbJour);
			AfxMessageBox(MsgErr);
			return(2);
		}

		if(NbSemaine<=0){
			CString MsgErr;
			MsgErr.Format("BUG: Le fichier %s décrit 0 semaines !",NomFic);
			AfxMessageBox(MsgErr);
			return(2);
		}

		m_ProbaQH = new CProbaSupport ** [m_NbStation];
		for(s=0;s<m_NbStation;s++){
			m_ProbaQH[s] = new CProbaSupport * [7];
			for(j=0;j<7;j++){
				m_ProbaQH[s][j] = new CProbaSupport [96];
				for(q=0;q<96;q++){
					memset(Duree,0,SizeOfDuree);
					// Cumulation des durées d'écoutes
					for(d=j;d<m_NbJour;d+=7){
						for(c=0;c<m_DureeQH[s][d][q].m_NbCas;c++){
							Duree[m_DureeQH[s][d][q].m_NrIndividu[c]]+=m_DureeQH[s][d][q].m_Proba[c];
						}
					}

					// Comptage des individus touchés
					NbCas=0;
					for(i=0;i<m_NbIndiv;i++){
						if(Duree[i]<=0)continue;
						Duree[i]+=Arrondie;
						Duree[i]/=NbSemaine;
						if(Duree[i]<=0)Duree[i]=1;
						ASSERT(Duree[i]>0 && Duree[i]<=1000);
						if(Duree[i]>1000)
						{
							Duree[i]=1000;
							flagpbproba=1;
						}
						NbCas++;
					}

					// Rangement des probabilités
					m_ProbaQH[s][j][q].SetNbCas(NbCas);
					for(i=0,c=0;i<m_NbIndiv;i++){
						if(Duree[i]<=0)continue;
						m_ProbaQH[s][j][q].m_NrIndividu[c]=i;
						m_ProbaQH[s][j][q].m_Proba[c]=(unsigned short)Duree[i];
						ASSERT(Duree[i]>0 && Duree[i]<=1000);
						c++;
					}
				}
#ifdef OPTION_DLGPROGRESS
				dlg.m_Progress.SetPos(50+(40*((s*7)+j+1))/(m_NbStation*7));
#endif
			}
		}
		delete [] Duree;

		TRACE("Ecriture du fichier AQP\n");
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Sauvegarde du panel probabilisé");
#endif
		if(FicAQP.Open(NomFicAQP,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary|CFile::shareExclusive, &e ) ){
			unsigned short Nr;
			unsigned short proba;
			unsigned short NbIndiv;


			// Le format est identique,
			// Mais on n'utilise pas l'info.
			FicAQP.Write(&m_DateDebut,sizeof(long));
			short NbJourProbabilise=7;
			FicAQP.Write(&NbJourProbabilise,sizeof(short));
			FicAQP.Write(&NbIndiv,sizeof(unsigned short));
			m_NbIndiv = NbIndiv;
			FicAQP.Write(&m_NbStation,sizeof(short));
			for(s=0;s<m_NbStation;s++){
				FicAQP.Write(&m_NrStation[s],sizeof(short));
				for(j=0;j<NbJourProbabilise;j++){
					for(q=0;q<96;q++){
						Nr=m_ProbaQH[s][j][q].m_NbCas;
						FicAQP.Write(&Nr,sizeof(short));
						for(c=0;c<m_ProbaQH[s][j][q].m_NbCas;c++){
							Nr=m_ProbaQH[s][j][q].m_NrIndividu[c];
							proba=m_ProbaQH[s][j][q].m_Proba[c];
							FicAQP.Write(&Nr,sizeof(short));
							FicAQP.Write(&proba,sizeof(short)); // Probabilité en 1000 ème
							ASSERT(Nr>=0 && Nr<m_NbIndiv);
							ASSERT(proba>0 && proba<=1000);
						}
					}
#ifdef OPTION_DLGPROGRESS
					dlg.m_Progress.SetPos(90+(10*((s*NbJourProbabilise)+j+1))/(m_NbStation*NbJourProbabilise));
#endif
				}
			}
			FicAQP.Close();
			TRACE("Le fichier AQP est crée.\n");
			CFile::GetStatus(NomFicAQP,filestatus);
			filestatus.m_mtime=TimeFicAQJ;
			CFile::SetStatus(NomFicAQP,filestatus);
		}
		else {
			CString txt;
			txt.Format("Impossible de sauvegarder le panel probabilisé %s : ",NomFicAQP);
			e.AfficheErreur(txt);
		}
#endif // OPTION_DUREEECOUTE
	}
	else {
		TRACE("Lecture du fichier AQP\n");
#ifdef OPTION_DLGPROGRESS
		dlg.SetTitre("Chargement du panel probabilisé");
#endif

#ifdef OPTION_DUREEECOUTE
		// La date est-elle bonne ?
		FicAQP.GetStatus(filestatus);
		TimeFicAQP=filestatus.m_mtime;
		if(TimeFicAQP!=TimeFicAQJ){
			FicAQP.Close();
			goto ProbabilisationForcee;
		}

		// Suite à un bug dans les fichiers QH,
		// On est sûr qu'il faut reprobaliser
		// les fichiers datants d'avant 20/09/00
		CTime date(2000,9,20,0,0,0);
		if(TimeFicAQP<date)
		{
			AfxMessageBox("Ce carnet d'ecoute présente des défauts.\nPour remedier à ce problème, il faut demander à Médiamétrie un nouveau fichier.");
		}

#endif
		unsigned short Nr;
		unsigned short proba;
		long vlong;
		short vshort;
		short NbJourProbabilise;

		// Le format est identique,
		// Mais on n'utilise pas l'info.
		FicAQP.Read(&vlong,sizeof(long));
		FicAQP.Read(&NbJourProbabilise,sizeof(short));
		FicAQP.Read(&vshort,sizeof(short));
		FicAQP.Read(&vshort,sizeof(short));
		m_ProbaQH = new CProbaSupport ** [m_NbStation];
		for(s=0;s<m_NbStation;s++){
			FicAQP.Read(&vshort,sizeof(short));
			m_ProbaQH[s] = new CProbaSupport * [NbJourProbabilise];
			for(j=0;j<NbJourProbabilise;j++){
				m_ProbaQH[s][j] = new CProbaSupport [96];
				for(q=0;q<96;q++){
					FicAQP.Read(&Nr,sizeof(short));
					m_ProbaQH[s][j][q].SetNbCas(Nr);
					for(c=0;c<m_ProbaQH[s][j][q].m_NbCas;c++){
						FicAQP.Read(&Nr,sizeof(short));
						FicAQP.Read(&proba,sizeof(short));
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
		FicAQP.Close();
		TRACE("Le fichier AQP est chargé.\n");
	}

	// Construction des proba neutre
	long * Proba=new long [m_NbIndiv];
	int SizeOfProba=sizeof(long)*m_NbIndiv;
	m_ProbaQHNeutre = new CProbaSupport * [7];
	for(j=0;j<7;j++){
		m_ProbaQHNeutre[j] = new CProbaSupport [96];
		for(q=0;q<96;q++){
			memset(Proba,0,SizeOfProba);
			// Lecture des probas pour toutes les station de
			// ce même quart d'heure, même jour
			for(s=0;s<m_NbStation;s++){
				for(c=0;c<m_ProbaQH[s][j][q].m_NbCas;c++){
					Proba[m_ProbaQH[s][j][q].m_NrIndividu[c]]+=m_ProbaQH[s][j][q].m_Proba[c];
				}
			}

			// Comptage
			int NbProba=0;
			for(i=0;i<m_NbIndiv;i++){
				Proba[i]/=m_NbStation;
				if(Proba[i]>0)NbProba++;
			}
			ASSERT(NbProba);
			m_ProbaQHNeutre[j][q].SetNbCas(NbProba);
			for(i=0,c=0;i<m_NbIndiv;i++){
				if(Proba[i]<=0)continue;
				m_ProbaQHNeutre[j][q].m_NrIndividu[c]=i;
				m_ProbaQHNeutre[j][q].m_Proba[c]=(unsigned short)Proba[i];
				c++;
			}
		}
	}
	delete [] Proba;


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

//##ModelId=3D9AB7D6008F
void CFicAQJ::FreeFile()
{
	if(!m_FileLoaded)return;

#ifdef OPTION_RUSSIE
	short s,j;
	// Libération des durées d'écoute
	for(s=0;s<m_NbStation;s++){
		for(j=0;j<m_NbJour;j++){
			delete [] m_EcoutesQH[s][j];
		}
		delete [] m_EcoutesQH[s];
	}
	delete [] m_EcoutesQH;
#ifdef OPTION_NOVA_RUSSE
	delete [] m_EcoutesJRS;
#endif
#else
	short s,j;
	// Libération des durées d'écoute
	for(s=0;s<m_NbStation;s++){
		for(j=0;j<m_NbJour;j++){
			delete [] m_DureeQH[s][j];
		}
		delete [] m_DureeQH[s];
	}
	delete [] m_DureeQH;
#endif
	delete [] m_NrStation;

	// Libération des probabilités
#if !defined(OPTION_CIMBELGE) && !defined(OPTION_RUSSIE)
	for(s=0;s<m_NbStation;s++){
		for(j=0;j<7;j++){
			delete [] m_ProbaQH[s][j];
		}
		delete [] m_ProbaQH[s];
	}
	delete [] m_ProbaQH;

	// Libération des probabilités neutres
	for(j=0;j<7;j++){
		delete [] m_ProbaQHNeutre[j];
	}
	delete [] m_ProbaQHNeutre;
#endif

#ifdef OPTION_NOVA_RUSSE
	delete [] m_EcoutesWLY;
	delete [] m_EcoutesJRS;

#elif defined(OPTION_RUSSIE) && !defined(OPTION_CIM_RUSSE)
//	delete [] m_RegionsIDV;
	delete [] m_EcoutesWLY;
	delete [] m_EcoutesMLY;
#endif

#ifdef OPTION_REACH_BELGE
	delete [] m_Ecoute;
	m_Ecoute = NULL;
#endif

	m_NrStation=NULL;
	m_ProbaQH=NULL;
	m_DureeQH=NULL;
	m_FileLoaded=0;

	m_DateDebut=0;
	m_NbJour=0;
	m_NbIndiv=0;
	m_NbStation=0;
}
