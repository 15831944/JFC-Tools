#include "stdafx.h"
//#include "tomeval.h"		// prototypes structures et classes


short CTomjob::GetRes(FILE * Fic, char *buff, short lrecl)
{
   long pos;
   short n, i;
   char *tmp;
   pos = ftell(Fic);
   n = fread(buff, 1, lrecl, Fic);
   tmp = strchr(buff, '\r');
   i = n;
   if (!tmp) {
      tmp = strchr(buff, '\n');
   }
   if (tmp)
      i = tmp - buff;
   // for (i = 0; i < n; i++) if (buff[i] == 13) break;
   buff[i] = 0;
   i = min(i + 2, n);
   pos += i;
   fseek(Fic, pos, SEEK_SET);
   if (i == 2) {
      for (i = 0; i < 20; i++)
         buff[i] = ' ';
      buff[i] = 0;
   }
   if (n > 1)
      return (FALSE);   // OK 
   else
      return (TRUE);    // Faux 
}

void CTomjob::AlloueStructTomjob(char fMarginal,bool fCouverture)
{
	T=new STOMJOB;
	T->fJobMarginal=fMarginal;
	if(T->fJobMarginal)T->NbFoisToucheMax=NBFOISTOUCHEMAXMARGINAL;
	else T->NbFoisToucheMax=NBFOISTOUCHEMAXSTD;
	T->fZoomCopied=0;
	m_fQuickJob=fCouverture;
}


#include <share.h>

short CTomjob::LectureTomjob(LPCSTR NomFicTOMJOB,LPCSTR NomFichCIBJOB, LPCSTR NomFicRESTOM, HWND hWndMaitre,char fCalculMarginal){

	AnnuleMemoZoom();

	m_fMultiMedia=0;
	m_fExtern=0;
	m_fVentilCouv=0;
	m_fCalculCHI=0;
	m_fCalculDupp=0;
	m_fCorrigeCoeffEx=0;
	m_NomFicRESTOM=NomFicRESTOM;
	m_NomFicTOMJOB=NomFicTOMJOB;
	m_NomFicCIBJOB = NomFichCIBJOB;
	m_hWndMaitre=hWndMaitre;
#ifdef OPTION_CARAT
	if(fCalculMarginal){
		m_fCalculRatio=0;
		m_fApportJournalier=0;
	}
	else {
		m_fCalculRatio=3;
		m_fApportJournalier=1;
	}
#else
	m_fCalculRatio=0;
	m_fApportJournalier=0;
#endif

#ifdef OPTION_JFC
	/*if(m_NomFicTOMJOB=="TOMJOB"){
		if(fCalculMarginal){
			m_NomFicTOMJOB.Format("TOMJOB%02d",m_NrLien);
			m_NomFicRESTOM.Format("RESTOM%02d",m_NrLien);
			m_NomFicCIBJOB.Format("CIBJOB%02d",m_NrLien);
		}
		else {
			m_NomFicTOMJOB="DATATM";
			m_NomFicRESTOM="RESTOM";
			m_NomFicCIBJOB="CIBJOB";
		}
	}*/
#endif

#ifdef OPTION_MEDIACONSULT
	if(fCalculMarginal){
		m_NomFicRESTOM.Format("%s%02d",NomFicRESTOM,m_NrLien);
		m_NomFicTOMJOB.Format("%s%02d",NomFicTOMJOB,m_NrLien);
	}
#endif

char fEcraseResrom=1;
#ifdef OPTION_JFC
	if(!T->fJobMarginal)fEcraseResrom=0;
#endif

	if(fEcraseResrom){
		FILE *fp_Restom;
	//	if (NULL==(fp_Restom=_fsopen(theApp.m_NomFicRESTOM,"wb"))) {
		if (NULL==(fp_Restom=_fsopen(m_NomFicRESTOM,"wb",_SH_DENYNO))) {
			CString Error;
			Error.Format("Effacement du fichier de résultats impossible\nerrno: %d %s",errno,strerror(errno));
			AfficheErreur(Error);
		}
		else fclose(fp_Restom);
	}

	FILE *fp_TomJob,*fp_CibJob;
	short Erreur,m,i,NbIns,src,r;
	char *tmp,*Virg,*AptRegion;
	long PosCibJob;
	typedef struct tagSupport {
		struct tagSupport *Next;
		short Station;
		DWORD CodeHoraire;
		short HorairePassage;
		short Jour;
		long AptReg;
		short Option;
		char Prive[30];
		double Grp;
		double **GrpVentil;
		short *Spots;
		double CoefDivGrp;
	} SUPPORTJOB;
	SUPPORTJOB *GSupport,*Support;
	if (NULL==(fp_TomJob=fopen(m_NomFicTOMJOB,"rb"))) {
		AfficheErreur("Le fichier TOMJOB est absent");
		return(1);
	}
	tmp=new char[100];

	while (1) {
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Etude
		if (Erreur) break;
		sscanf(tamponalloc,"%hd,%s",&m,tmp);
		T->NrEtude = m;
		T->LibEtude=tmp;
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Cible
		if (Erreur) break;
#ifdef OPTION_MEDIATOP
		T->LibCible="01";
		T->LibCible.SetAt(0,tamponalloc[0]);
		T->LibCible.SetAt(1,tamponalloc[1]);
		T->LibCible.SetAt(2,0);
		T->NrCible = -1;
#else
		sscanf(tamponalloc,"%hd,%s",&m,tmp);
		T->NrCible = m;
		T->LibCible=tmp;
#endif
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Ligne blanche
		if (Erreur) break;
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// TypesuppU
		if (Erreur) break;
		T->TypeSuppU = atoi(tamponalloc);
		if(T->TypeSuppU>100){
			m_fExtern=1;
			T->TypeSuppU%=100;
		}
		else m_fCorrigeCoeffEx=1; 

/*#ifdef OPTION_MEDIACONSULT
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Ligne blanche
		if (Erreur) break;
#endif*/
		
		// flag de ventilation sur la source de couverture
		if(T->TypeSuppU==1 || T->TypeSuppU==3|| T->TypeSuppU==5)m_fVentilCouv=1;

		if(m_fExtern == 0){
			if (NULL==(fp_CibJob=fopen(m_NomFicCIBJOB,"rb"))) {
				AfficheErreur("Le fichier CIBJOB est absent");
				fclose(fp_TomJob);
				return(1);
			}
		}

#ifndef OPTION_MEDIACONSULT
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Coef beta
		if (Erreur) break;
#ifdef OPTION_CARAT
		sscanf(tamponalloc,"%hd,%hd,%hd",&T->Beta,&T->Mini,&T->Maxi);
#else
		T->Beta = atoi(tamponalloc);
#endif
#else
#ifdef COURBEDEREPONSEIMPOSEE
		int TailleCourbeReponse;
		Erreur = GetRes(fp_TomJob,tamponalloc,1000);	
		if (Erreur) break;
		sscanf(tamponalloc,"%hd",&TailleCourbeReponse);
		/*if(TailleCourbeReponse>T->NbFoisToucheMax){
			AfficheErreur("Taille de la courbe de réponse invalide");
		}*/
		T->CourbeReponseImpose.RemoveAll();
		if (TailleCourbeReponse>0) {
			float f;
			Virg = 1 + strchr(tamponalloc, ',');
			do {
				sscanf(Virg, "%f", &f); 
				T->CourbeReponseImpose.Add(f);
				Virg = strchr(Virg, ',');
				if (Virg) Virg++;
			} while (Virg && T->CourbeReponseImpose.GetSize() < TailleCourbeReponse);
		}

#endif // COURBEDEREPONSEIMPOSEE
#endif //OPTION_MEDIACONSULT

#ifdef OPTION_MEDIACONSULT
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Ligne blanche
		if (Erreur) break;
#endif

		
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nombre de sous plans
		if (Erreur) break;
		T->NbPlan = atoi(tamponalloc);
		if(T->fJobMarginal && T->NbPlan>1){
		 	AfficheErreur("NbPlan>1 dans tomjob marginal");
		}
#ifndef OPTION_MEDIACONSULT
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nombre de sources
		if (Erreur) break;
		T->NbSource = (short) atoi(tamponalloc);
		if(m_fExtern == 0){
			Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Position de debut le fichier CibJob
			if (Erreur) break;
			PosCibJob = atol(tamponalloc);
		}
#endif
#ifdef OPTION_MEDIACONSULT
		T->Beta=-1;
		T->NbSource =1;
#endif
		
		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nombre de ventilation
		if (Erreur) break;
		sscanf(tamponalloc,"%hd",&T->NbVentilation);
		if(T->fJobMarginal && T->NbVentilation>0){
			AfficheErreur("T->NbVentilation dans tomjob marginal");
		}
		if (T->NbVentilation) {
			if(!m_fVentilCouv){ // ventilation sur le GRP
				T->PopGRPclassVent=new double ** [T->NbSource];
				T->NbClasseVent=new short [T->NbVentilation];
				Virg = 1 + strchr(tamponalloc, ',');   // Nb ventilation
				m = 0;
				do {
					sscanf(Virg, "%hd", T->NbClasseVent + m);    // Nb classe
					m++;
					Virg = strchr(Virg, ',');
					if (Virg) Virg++;
				} while (Virg && m < T->NbVentilation);
			}
			else { // ventilation sur la couverture
				T->PopGRPclassVent=new double** [T->NbSource];
				T->NbClasseVent=new short [T->NbVentilation];
				T->NrCritereVent=new short [T->NbVentilation];
				Virg = 1 + strchr(tamponalloc, ',');   // Nb ventilation
				m = 0;
				do {
					sscanf(Virg, "%hd", T->NrCritereVent + m);    // Nr classe
					m++;
					Virg = strchr(Virg, ',');
					if (Virg) Virg++;
				} while (Virg && m < T->NbVentilation);
			}
		}
		T->NrSourceCouv=new short [T->NbSource];
		T->NrVagueCouv=new short [T->NbSource];
		T->FormatSource=new short [T->NbSource];
		T->NomFicCouv=new char* [T->NbSource];
#ifdef OPTION_MEDIACONSULT
		T->NomFicIND=new char* [T->NbSource];
		T->NomFicCOD=new char* [T->NbSource];
		T->NomFicVentil=new char* [T->NbSource];
		T->NomFicPoids=new char* [T->NbSource];
		T->NomFicNbModal=new char* [T->NbSource];
#endif
		T->PopCibleGRP=new long [T->NbSource];
		T->NbIndivC=new short [T->NbSource];
		T->NbRegion=new short [T->NbSource];
		T->NrRegionVect=new short* [T->NbSource];
		T->popGRPregion=new long* [T->NbSource];
		T->popGRPregionVentile=new long*** [T->NbSource];
		T->fIndivRegion=new bool** [T->NbSource];
		T->NrRegionIndiv=new char* [T->NbSource];
		T->CoeffExCible=new long* [T->NbSource];
		T->NrRatio=new short* [T->NbSource];
		T->CoeffExEnsemble=new long* [T->NbSource];
		T->CoeffExRed=new double* [T->NbSource];
		T->NbSupport=new int [T->NbSource];
		T->NbSupportAlloue=new int [T->NbSource];
		T->Support=new SSupport* [T->NbSource];
		if (T->NbVentilation) {
			T->NrClasseVent=new char** [T->NbSource];
		}
		T->CodeVagueCouv=new CString[T->NbSource];

		Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Ligne Blanche debut de la source
		if (Erreur) break;

		if(m_fExtern == 0)fseek(fp_CibJob,PosCibJob, SEEK_SET);
		for (src = 0;src <T->NbSource ;src++) {
#ifdef OPTION_MEDIACONSULT
			// Le fichier .ind
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicIND[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicIND[src],tamponalloc);
			// Le fichier .cod
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicCOD[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicCOD[src],tamponalloc);

			
			// le nom des fichier crc, aux, pdx
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicVentil[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicVentil[src],tamponalloc);
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicCouv[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicCouv[src],tamponalloc);
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicPoids[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicPoids[src],tamponalloc);
			Erreur = GetRes(fp_TomJob,tamponalloc,MAX_PATH);
			if (Erreur) break;
			T->NomFicNbModal[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicNbModal[src],tamponalloc);

			Erreur = GetRes(fp_TomJob,tamponalloc,1000);	// Population de la cible
			if (Erreur) break;
			T->PopCibleGRP[src] = atol(tamponalloc);

			if (T->NbVentilation) T->PopGRPclassVent[src]=new double* [T->NbVentilation];
#else

			Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nr de la source,Nr de la vague
			if (Erreur) break;
			if(m_fExtern){
				char txt[255];
				sscanf(tamponalloc,"%hd,%s,%hd",T->NrSourceCouv+src,txt,T->FormatSource+src);
				txt[4]=0;
				T->CodeVagueCouv[src]=txt;
			}
			else sscanf(tamponalloc,"%hd,%hd,%hd",T->NrSourceCouv+src,T->NrVagueCouv+src,T->FormatSource+src);
			Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nom du fichier de couverture
			if (Erreur) break;
			T->NomFicCouv[src]=new char [strlen(tamponalloc)+1];
			strcpy(T->NomFicCouv[src],tamponalloc);
			Erreur = GetRes(fp_TomJob,tamponalloc,1000);	// Population de la cible
			if (Erreur) break;
			T->PopCibleGRP[src] = atol(tamponalloc);
			Virg = strchr(tamponalloc, ',');
			if (Virg) Virg++;
			if (T->NbVentilation) {
				T->PopGRPclassVent[src]=new double* [T->NbVentilation];
				if(!m_fVentilCouv){
					for (i=0;i<T->NbVentilation;i++){
						T->PopGRPclassVent[src][i]=new double [T->NbClasseVent[i]];
					}
					for (i=0;i<T->NbVentilation;i++) {
						for (m = 0;m<T->NbClasseVent[i] && Virg;m++) {
							long l;
			         		sscanf(Virg, "%ld", &l);
							T->PopGRPclassVent[src][i][m]=l;
							Virg = strchr(Virg, ',');
							if (Virg) Virg++;
						}
					}
				}
			}
			if(m_fExtern == 0){
				Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Nombre d'individus
				if (Erreur) break;
				T->NbIndivC[src] = atoi(tamponalloc);
			}
#endif


			if(T->TypeSuppU==1){ // télé france
				m_fCalculCHI=1;
				T->CoeffCHI=+0.2;
				m_fCalculDupp=1;
				T->CoeffDupp=+0.95;
				if(m_fExtern){
				// !!!!!!!!!!!!!!!!!!!!!!!!!!!
				// Ajustement des coeff en dur
				// liste des cibles enfants
#ifdef OPTION_CHECKUP
					if(T->NrCible==14
						||T->NrCible==15
						||T->NrCible==21
						||T->NrCible==26
						||T->NrCible==29
						||T->NrCible==30
						||T->NrCible==52
						||T->NrCible==60
						||T->NrCible==63
						||T->NrCible==68
						||T->NrCible==69)T->CoeffCHI=+0.6;
#endif
				}
			}
			if(T->TypeSuppU==2){ //en espagne
				m_fCalculCHI=1;
				T->CoeffCHI=+0.3;
			}
#ifndef OPTION_MEDIACONSULT

			Erreur = GetRes(fp_TomJob,tamponalloc,1000);	// Nb de region
			if (Erreur) break;
			T->NbRegion[src] = atoi(tamponalloc);
			if(T->NbRegion[src]==0)T->NbRegion[src]=1;
			T->NrRegionVect[src]=new short [T->NbRegion[src]];
			T->popGRPregion[src]=new long [T->NbRegion[src]];

			// Nr de region
			Erreur = GetRes(fp_TomJob,tamponalloc,1000); // Nr des régions
			if (Erreur) break;
			Virg = tamponalloc;
			i = 0;
			while (Virg && i < T->NbRegion[src]) {
	         	sscanf(Virg, "%hd", T->NrRegionVect[src]+i);
				i++;
				Virg = strchr(Virg, ',');
				if (Virg) Virg++;
			}
			
#else
			T->NbRegion[src]=1;
			T->NrRegionVect[src]=new short [T->NbRegion[src]];
			T->popGRPregion[src]=new long [T->NbRegion[src]];
			T->NrRegionVect[src][0]=1;
#endif

			// Les population en GRP pour chaque régions !
			if(m_fCorrigeCoeffEx){
				Erreur = GetRes(fp_TomJob,tamponalloc,1000); // pop grp de chaque région
				if (Erreur) break;
				Virg = tamponalloc;		i = 0;
				while (Virg && i < T->NbRegion[src]) {
		         	sscanf(Virg,"%ld",T->popGRPregion[src]+i);
					i++; 	Virg = strchr(Virg, ','); 	if (Virg) Virg++;
				}
				
				if (T->NbVentilation && (!m_fVentilCouv) ){
					long l;
					T->popGRPregionVentile[src]=new long** [T->NbRegion[src]];

					for(r=0;r<T->NbRegion[src];r++){
						T->popGRPregionVentile[src][r]=new long* [T->NbVentilation];
						Erreur = GetRes(fp_TomJob,tamponalloc,1000);
						if (Erreur) break;
						Virg = tamponalloc;
						for (i=0;i<T->NbVentilation;i++) {
							T->popGRPregionVentile[src][r][i]=new long [T->NbClasseVent[i]];
							for (m = 0;m<T->NbClasseVent[i];m++) {
								sscanf(Virg,"%ld",&l);
							 	Virg = strchr(Virg, ','); 	
								if (Virg) Virg++;
								T->popGRPregionVentile[src][r][i][m]=l;
							}
						}
					}
				}
			}
				
			if(m_fExtern == 0){
				T->CoeffExCible[src]=new long [T->NbIndivC[src]];
				T->NrRatio[src]=new short [T->NbIndivC[src]];
				T->CoeffExEnsemble[src]=new long [T->NbIndivC[src]];
				T->CoeffExRed[src]=new double [T->NbIndivC[src]];
				fread(T->CoeffExCible[src],sizeof(long),T->NbIndivC[src],fp_CibJob);		// Coefficient d'extrapolation
				if(m_fCorrigeCoeffEx)fread(T->CoeffExEnsemble[src],sizeof(long),T->NbIndivC[src],fp_CibJob);		// Coefficient d'extrapolation
				AptRegion=new char [T->NbIndivC[src]];
				int ret=fread(AptRegion,sizeof(char),T->NbIndivC[src],fp_CibJob); // Appartenance d'individu de la region
				if(ret!=T->NbIndivC[src]){
					AfficheErreur("Le nombre d'octect lu dans le fichier cibjob (zone des régions par individus)");
					memset(AptRegion,0,T->NbIndivC[src]);
					// ExitProcess(-1);
				}
				T->fIndivRegion[src]=new bool* [T->NbRegion[src]+1];
				for (i=0;i <= T->NbRegion[src];i++){
					T->fIndivRegion[src][i]=new bool [T->NbIndivC[src]];
					for (long z=0; z<T->NbIndivC[src];z++)T->fIndivRegion[src][i][z]=false;
				}
				for (i=0;i<T->NbIndivC[src];i++) {
					T->fIndivRegion[src][T->NbRegion[src]][i] = true;
					r = AptRegion[i];
					if (r <T->NbRegion[src])
						T->fIndivRegion[src][r][i] = true;
				}
				//liberation(AptRegion,"LectureTomJob");
				T->NrRegionIndiv[src]=AptRegion;
			}
			else {
			}

			if (T->NbVentilation) {
				T->NrClasseVent[src]=new char* [T->NbVentilation];
				if((m_fExtern == 0) && (!m_fVentilCouv))for (i=0;i<T->NbVentilation;i++) {
					T->NrClasseVent[src][i]=new char [T->NbIndivC[src]];
					fread(T->NrClasseVent[src][i],sizeof(char),T->NbIndivC[src],fp_CibJob);		// Nr de classe ventilée
				}
			}

			if(m_fVentilCouv ==0){
				// MODIF du 14/3/99
				// vérification des NrClasseVent
				// c'est une pure vérification, car le données sont parfois (...)
				// un peu éronnée, donc on vérifie. Dans une monde idéale,
				// ce truc est parfaitement inutile, mais pour liem,
				// Le message permettra de ne pas me retourner le bug
				bool fBugIn_NrClasseVent=false;
				for(short v=0;v<T->NbVentilation;v++){
					for(i=0;i<T->NbIndivC[src];i++){
						if(T->NrClasseVent[src][v][i]>=T->NbClasseVent[v] || T->NrClasseVent[src][v][i]<0){
							T->NrClasseVent[src][v][i]=0;
							fBugIn_NrClasseVent=true;
						}
					}
				}
				if(fBugIn_NrClasseVent)AfficheErreur("Erreur dans les numéros des classes ventilées des individus");
			}


			Erreur = GetRes(fp_TomJob,tamponalloc,100);	// Ligne Blanche
			if (Erreur) break;
			GSupport = NULL;
			NbIns = 0;
			do {
				Erreur = GetRes(fp_TomJob,tamponalloc,10000);	// Les supports
				if (Erreur) break;
	         if (tamponalloc[0] == ' ')
	               break;   // Ligne blanche : Rupture de supports
				if (!GSupport) {
					GSupport=new SUPPORTJOB;
					Support = GSupport;
				}
				else {
					Support->Next=new SUPPORTJOB;
					Support = Support->Next;
				}
				Support->Next = NULL;
				Virg = tamponalloc;
				Support->Station = atoi(Virg);
	         Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
				Support->CodeHoraire = atoi(Virg);
#ifdef HORAIRE_THEORIQUE
	         Virg = strchr(Virg, ',');
			 if (Virg) Virg++;
				Support->HorairePassage = atoi(Virg);
#endif
	         Virg = strchr(Virg, ',');
			 if (Virg) Virg++;
				Support->Jour = atoi(Virg);
	         Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
				Support->AptReg = atol(Virg);
			 Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
				Support->Option = atoi(Virg);
	         Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
				Support->Grp = atof(Virg);
	         Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
			 {
				 short z=0,CoefGrp=100;
				while(Virg[z]!=',' && Virg[z]!='/')Support->Prive[z++]=Virg[z];
				Support->Prive[z]=0;
				if (Virg[z++] =='/') {
					CoefGrp = atoi(Virg+z);	// 0;
					/* while(Virg[z]!=',') {
						CoefGrp = 10*CoefGrp+(Virg[z]-'0');
						z++;
					}*/
				}
				Support->CoefDivGrp = (double)CoefGrp/100.0;
			 }
			 Support->Spots=new short [T->NbPlan];
	         Virg = strchr(Virg, ',');
	         if (Virg) Virg++;
				i = 0;
				while (Virg && i < T->NbPlan) {
	         	sscanf(Virg, "%hd", Support->Spots + i);
	            i++;
	            Virg = strchr(Virg, ',');
	            if (Virg) Virg++;
				}
				if (T->NbVentilation) {
					Support->GrpVentil=new double* [T->NbVentilation];
					if(!m_fVentilCouv){
						for (i=0;i<T->NbVentilation;i++) Support->GrpVentil[i]=new double [T->NbClasseVent[i]];
						for (i=0;i<T->NbVentilation;i++) {
							m = 0;
							if(!m_fVentilCouv)while (Virg && m < T->NbClasseVent[i]) {
				         		sscanf(Virg, "%lf", Support->GrpVentil[i]+m);
								m++;
			        			Virg = strchr(Virg, ',');
				    			if (Virg) Virg++;
							}
						}
					}
				}
				NbIns++;
			}while (1);
			T->NbSupport[src] = NbIns;
			if(T->fJobMarginal)T->NbSupportAlloue[src] = NbIns+NBSPOTPREALLOC;
			else T->NbSupportAlloue[src]=T->NbSupport[src]+1;
			T->Support[src]=new SSupport [T->NbSupportAlloue[src]];
			memset(T->Support[src],0,sizeof(SSupport)*T->NbSupportAlloue[src]);

			for (i=0;i<NbIns;i++) {
				T->Support[src][i].Station = GSupport->Station;

				T->Support[src][i].NrRegionForce=(char)TrouveNrIndexRegion(GSupport->AptReg,src);
				T->Support[src][i].CoefDivGrp = GSupport->CoefDivGrp;


				T->Support[src][i].CodeHoraire = GSupport->CodeHoraire;
#ifdef HORAIRE_THEORIQUE
				T->Support[src][i].HorairePassage = GSupport->HorairePassage;
#else
				T->Support[src][i].HorairePassage = (short)GSupport->CodeHoraire;
#endif
				T->Support[src][i].Jour = GSupport->Jour;
				T->Support[src][i].Option = GSupport->Option;
				T->Support[src][i].GRP = GSupport->Grp;
				T->Support[src][i].NbInsert = GSupport->Spots;
				strcpy(T->Support[src][i].Prive,GSupport->Prive);
				if(T->fJobMarginal){
					T->Support[src][i].Margi_NbInsert = GSupport->Spots[0];
					delete [] T->Support[src][i].NbInsert;
				}
				if (T->NbVentilation)T->Support[src][i].GRPvent = GSupport->GrpVentil;
				Support = GSupport->Next;
				delete [] GSupport;
				GSupport = Support;
			}
		}
		break;
	}
	delete [] tmp;
	fclose(fp_TomJob);
	if(m_fExtern == 0)fclose(fp_CibJob);
//	if(!_CrtCheckMemory())AfficheErreur("CrtCheckMemory error");
	if(T->TypeSuppU==1 || T->TypeSuppU==3)m_fCorrigeCoeffEx=0; 
	return(Erreur);	// 0:OK,1:FALSE
}

short CTomjob::LectureTomjobMarginal(char fZoom){

	if(!fZoom)AnnuleMemoZoom();
	// Effacement du restom
	FILE *fp_Restom;
	if (NULL==(fp_Restom=_fsopen(m_NomFicRESTOM,"wb",_SH_DENYNO))) {
		CString Error;
		Error.Format("Effacement du fichier de résultats impossible\nerrno: %d %s",errno,strerror(errno));
		 AfficheErreur(Error);
	}
	else fclose(fp_Restom);

	// Lecture du TOMJOB
	FILE *fp_TomJob;
	if (NULL==(fp_TomJob=fopen(m_NomFicTOMJOB,"rt"))) {
//	if (NULL==(fp_TomJob=fopen("addspot.wcp","rt"))) {
		AfficheErreur("Le fichier TOMJOB est absent");
		return(1);
	}

	// Lecture de la source
	short i;
	char *ptr;
	fgets(tamponalloc,100,fp_TomJob);
	for(i=0; ;i++){if(tamponalloc[i]<32)tamponalloc[i]=0;break;}
	m_NewSrc=atoi(tamponalloc);

	// lecture des spots
	m_NbNewSup=0;
	while(1){
		i=0;
		fgets(tamponalloc,1000,fp_TomJob);
		ptr=tamponalloc;	while(tamponalloc[i]!=',' && tamponalloc[i]>=32)i++;	tamponalloc[i]=0;
		if(!strcmp(ptr,"EOJ"))break;
		if(m_NbNewSup>=NBSPOTMARGIPLUSMOINS){
#ifdef _DEBUG
//			_asm{int 3};
#endif
			AfficheErreur("Trop de spot en marginal !");
			break;
		}

		// la station
		m_NewStation[m_NbNewSup]=atoi(ptr);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;

		// le code horaire
		m_NewCodeHoraire[m_NbNewSup]=atoi(ptr);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;

#ifdef HORAIRE_THEORIQUE
		// l'horaire de passage
		m_NewHorairePassage[m_NbNewSup]=atoi(ptr);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;
#endif
		// le jour (1:lundi, 2mardi,4:mercredi,8:jeudi,16:vendredi,32:samedi,64:dimanche)
		m_NewJour[m_NbNewSup]=atoi(ptr);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;

		// La région
		short NrUniqueReg=atoi(ptr);
		m_NewRegionForce[m_NbNewSup]=TrouveNrIndexRegion(NrUniqueReg,m_NewSrc);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;

		// l'option
		m_NewOption[m_NbNewSup]=atoi(ptr);
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;

		// le GRP
		m_NewGRP[m_NbNewSup]=atof(ptr);

		// Prive
		ptr=&tamponalloc[i+1];	
		while(tamponalloc[i]!=',')i++;	tamponalloc[i]=0;
		ptr = strchr(ptr,'/');
		if (*ptr) {
			m_NewCoefDivGrp[m_NbNewSup] = (double)(atoi(ptr+1))/100.0;
		}
		
		// le nb d'insertions
		ptr=&tamponalloc[i+1];	while(tamponalloc[i]>=32)i++;	tamponalloc[i]=0;
		m_NewNbInsert[m_NbNewSup]=atoi(ptr);
		m_NbNewSup++;
	}

	
	// Fermeture du Tomjob
	fclose(fp_TomJob);
//	if(!_CrtCheckMemory())AfficheErreur("CrtCheckMemory error");

	// on commence à préparer le terrain de calcul marginal
	memset(T->Margi_Marquage,0,sizeof(char)*T->NbIndivC[m_NewSrc]);



	return(0);	// 0:OK,1:FALSE
}


short CTomjob::TrouveNrIndexRegion(long NrUniqueReg,short src)
{
	short m,r;
	if(NrUniqueReg>=1000) {
		m = (short)(NrUniqueReg%1000L);
		for (r=0;r<T->NbRegion[src];r++){
			if (T->NrRegionVect[src][r] == m) break;
		}
		if(r>=T->NbRegion[src])AfficheErreur("Le numero de la région est invalide");
	}
	else r = T->NbRegion[src];
	return(r);
}


void CTomjob::AnnuleMemoZoom()
{
	short sup;
	if(T->fZoomCopied==0)return;

	
	// il faut liberer les supports qui ont été alloué pour le zoom
	// c'est le cas des ancrages en belgique !
	for(sup=0;sup<T->Zoom_NbSupport;sup++){
		if(T->Zoom_Support[sup].fAlloc==2){
			delete [] T->Zoom_Support[sup].Sup->Indiv;
			delete  T->Zoom_Support[sup].Sup;
		}
	}

	T->fZoomCopied=0;
	return;
}



short CTomjob::CopieSourceZoom(){
	short sup,ind,z,z2;

	AnnuleMemoZoom();
	T->Zoom_NbSupport=T->NbSupport[m_NewSrc];
	for(sup=0;sup<T->Zoom_NbSupport;sup++){
		T->Zoom_Support[sup]=T->Support[m_NewSrc][sup];
		T->Zoom_MW[sup]=T->Margi_MW[m_NewSrc][sup];
#ifndef OPTION_POLOGNE
		T->Zoom_MWW[sup]=T->Margi_MWW[m_NewSrc][sup];
#endif
	}
	for(ind=0;ind<T->NbIndivC[m_NewSrc];ind++){
		//memcpy(T->Zoom_VB[ind],T->Margi_VB[m_NewSrc][ind],TailleVB);
		for(z=0;z<=NBFOISTOUCHEMAXZOOM;z++)T->Zoom_VB[ind][z]=T->Margi_VB[m_NewSrc][ind][z];
		for(z2=z;z2<=NBFOISTOUCHEMAXMARGINAL;z2++)T->Zoom_VB[ind][z-1]+=T->Margi_VB[m_NewSrc][ind][z2];

		T->Zoom_P[ind]=T->Margi_P[m_NewSrc][ind];
		T->Zoom_PP[ind]=T->Margi_PP[m_NewSrc][ind];
#ifndef OPTION_POLOGNE
		T->Zoom_PPP[ind]=T->Margi_PPP[m_NewSrc][ind];
#endif
	}
	T->fZoomCopied=1;
	return(0);
}

short CTomjob::InsertNouveauSpot(char fZoom)
{
	short Sup;

	for(Sup=0;Sup<m_NbNewSup;Sup++){
		if(InsertNewSup(Sup,fZoom))return(1);
	}
	return(0);
}


short CTomjob::InsertNewSup(short sup,char fZoom)
{
	int s,*pNbSupport;
	SSupport *Support;
	if(fZoom){
		Support=T->Zoom_Support;
		pNbSupport=&T->Zoom_NbSupport;
	}
	else {
		Support=T->Support[m_NewSrc];
		pNbSupport=&T->NbSupport[m_NewSrc];
	}

	int NbInsert=m_NewNbInsert[sup];
	for(s=0;s<*pNbSupport;s++){
		if(Support[s].Station!=m_NewStation[sup])continue;
		if(Support[s].Jour!=m_NewJour[sup])continue;
		if(Support[s].CodeHoraire!=m_NewCodeHoraire[sup])continue;
#ifdef HORAIRE_THEORIQUE
		if(Support[s].HorairePassage!=m_NewHorairePassage[sup])continue;
#endif
		if(Support[s].Option!=m_NewOption[sup])continue;
		if(Support[s].GRP!=m_NewGRP[sup])continue;
		if(Support[s].NrRegionForce!=m_NewRegionForce[sup])continue;

		m_NewSup[sup]=s;

		// on ajoute le nombre d'insertion (qui peut être négatif)
		Support[m_NewSup[sup]].Margi_NbInsert+=NbInsert;

		if(Support[m_NewSup[sup]].Margi_NbInsert<0){
			// si il est négatif, il faut le distribuer sur un autre
			NbInsert=Support[m_NewSup[sup]].Margi_NbInsert;
			Support[m_NewSup[sup]].Margi_NbInsert=0;
#ifndef OPTION_MEDIACONSULT
			MarqueLesIndividus(m_NewSrc,m_NewSup[sup],fZoom);
#endif
			continue;
		}
		else {
			// on marque les individus de ce support
#ifndef OPTION_MEDIACONSULT
			MarqueLesIndividus(m_NewSrc,m_NewSup[sup],fZoom);
#endif
			return(0);
		}
	}
	if(m_NewNbInsert[sup]<0){
//(Liem 25/11/04)		AfficheErreur("Erreur dans DelSpot: spot non trouvé, ou nb d'insertion à retrancher trop grand");
		return(1);
	}
		
	// on ajoute dans la liste ce nouveau support

	// variables pour la réalloc des supportsReallocation du nb de support
	SSupport * ptrSupport;
	double * ptrDouble;

	if(!fZoom){
		if(*pNbSupport>=T->NbSupportAlloue[m_NewSrc]){

			T->NbSupportAlloue[m_NewSrc]+=NBSPOTPREALLOC;

			// reallocation des supports
			ptrSupport=T->Support[m_NewSrc];
			Support=T->Support[m_NewSrc]=new SSupport[T->NbSupportAlloue[m_NewSrc]];
			memset(T->Support[m_NewSrc],0,sizeof(SSupport)*T->NbSupportAlloue[m_NewSrc]);
			for(s=0;s<T->NbSupport[m_NewSrc];s++)T->Support[m_NewSrc][s]=ptrSupport[s];
			delete [] ptrSupport;

			// realloc de:Margi_MW
			ptrDouble=T->Margi_MW[m_NewSrc];
			T->Margi_MW[m_NewSrc]=new double[T->NbSupportAlloue[m_NewSrc]];
			memcpy(T->Margi_MW[m_NewSrc],ptrDouble,T->NbSupport[m_NewSrc]*sizeof(double));
			delete [] ptrDouble;

#ifndef OPTION_POLOGNE
			// realloc de:Margi_MWW
			ptrDouble=T->Margi_MWW[m_NewSrc];
			T->Margi_MWW[m_NewSrc]=new double[T->NbSupportAlloue[m_NewSrc]];
			memcpy(T->Margi_MWW[m_NewSrc],ptrDouble,T->NbSupport[m_NewSrc]*sizeof(double));
			delete [] ptrDouble;
#endif
		}
	}
	else { // si zoom
		// realloc éventuelle pour le zoom
		if(*pNbSupport>=T->Zoom_NbSupportAlloue){

			T->Zoom_NbSupportAlloue+=NBSPOTPREALLOC;

			// reallocation des supports
			ptrSupport=T->Zoom_Support;
			Support=T->Zoom_Support=new SSupport [T->Zoom_NbSupportAlloue];
			memset(T->Zoom_Support,0,sizeof(SSupport)*T->Zoom_NbSupportAlloue);
			for(s=0;s<T->Zoom_NbSupport;s++)T->Zoom_Support[s]=ptrSupport[s];
			delete [] ptrSupport;

			// realloc de:Zoom_MW
			ptrDouble=T->Zoom_MW;
			T->Zoom_MW=new double [T->Zoom_NbSupportAlloue];
			memcpy(T->Zoom_MW,ptrDouble,T->Zoom_NbSupport*sizeof(double));
			delete [] ptrDouble;

#ifndef OPTION_POLOGNE
			// realloc de:Zoom_MWW
			ptrDouble=T->Zoom_MWW;
			T->Zoom_MWW=new double [T->Zoom_NbSupportAlloue];
			memcpy(T->Zoom_MWW,ptrDouble,T->Zoom_NbSupport*sizeof(double));
			delete [] ptrDouble;
#endif
		}
	}
	m_NewSup[sup]=s=(*pNbSupport)++;
	Support[m_NewSup[sup]].Station=m_NewStation[sup];
	Support[m_NewSup[sup]].Jour=m_NewJour[sup];
	Support[m_NewSup[sup]].CodeHoraire=m_NewCodeHoraire[sup];
#ifdef HORAIRE_THEORIQUE
	Support[m_NewSup[sup]].HorairePassage=m_NewHorairePassage[sup];
#endif
	Support[m_NewSup[sup]].Option=m_NewOption[sup];
	Support[m_NewSup[sup]].GRP=m_NewGRP[sup];
	Support[m_NewSup[sup]].CoefDivGrp=m_NewCoefDivGrp[sup];
	Support[m_NewSup[sup]].NrRegionForce=(char)m_NewRegionForce[sup];
	Support[m_NewSup[sup]].NbInsert=NULL;
	Support[m_NewSup[sup]].Margi_NbInsert=m_NewNbInsert[sup];
	Support[m_NewSup[sup]].fAlloc=0;

	LierNouveauSpot(m_NewSrc,m_NewSup[sup],fZoom);
	CalculMediaWeightNouveauSpot(m_NewSrc,m_NewSup[sup],fZoom);
	MarqueLesIndividus(m_NewSrc,m_NewSup[sup],fZoom);
	return(0);
}

long CTomjob::EcritureRestom(char fZoom)
{

#ifdef OPTION_CARAT
	return(EcritureRestom_Carat(fZoom));
#endif
#ifdef OPTION_MEDIATOP
	return(EcritureRestom_Mediatop(fZoom));
#endif
#ifdef OPTION_MEDIACONSULT
	return(EcritureRestom_MediaConsultant(fZoom));
#endif

	if(m_fQuickJob){
		if((!T->fJobMarginal) || fZoom){
			AfficheErreur("Ce cas n'est pas encore prévu.");
			return(1);
		}
		float f=(float)(100.0*T->NbIndivT[0]/T->PopCibTotalGRP);
		return((long)f);
	}

	
	FILE *fp_Restom;
	short i,j,p,n;

char fEcraseResrom=1;
#ifdef OPTION_JFC
	if(!T->fJobMarginal)fEcraseResrom=0;
#endif
	if(fEcraseResrom)fp_Restom=_fsopen(m_NomFicRESTOM,"wb",_SH_DENYNO);
	else fp_Restom=_fsopen(m_NomFicRESTOM,"ab",_SH_DENYNO);
//	if (NULL==(fp_Restom=fopen(theApp.m_NomFicRESTOM,"ab"))) {
	if (NULL==fp_Restom) {
		CString Error;
		Error.Format("La création du fichier de résultat est impossible\nerrno: %d %s",errno,strerror(errno));
		 AfficheErreur(Error);
		return(0);
	}

	
	// les résultats
	j = sprintf(tamponalloc,"%.0lf %d\r\n",T->PopCibTotalGRP,T->NbCasTotal);
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%11.0lf.",T->Zoom_NbIndivT);
	else for(i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%11.0lf.",T->NbIndivT[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%11.0lf.",T->Zoom_NbContact);
	else for (i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%11.0lf.",T->NbContact[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

#ifdef COURBEDEREPONSEIMPOSEE
	j=0;
	if(!fZoom){
		for (i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%11.0lf.",T->CouvertureValide[i] * T->PopCibTotalGRP / 100.0);
		strcat(tamponalloc,"\r\n");j+=2;
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
#endif

#ifdef OPTION_JFC
	if((!T->fJobMarginal) && (!fZoom)){
		j = sprintf(tamponalloc,"%d\r\n",T->m_StatProbaDef_NbSpot);
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
#endif

	j = sprintf(tamponalloc,"%4d",T->NbClasseContact);
	for (i=0;i<T->NbClasseContact;i++)j += sprintf(tamponalloc+j,"%4.0f.",100.0*T->CourbeReponse[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	for (n=0;n<T->NbClasseContact;n++) {
		j=0;

#ifdef OPTION_MEDIACONSULT
		if(fZoom)j += sprintf(tamponalloc+j,"%9.5lf%9.5lf",T->Zoom_DistribExact[n],T->Zoom_DistribCumulee[n]);
		else for (i=0;i<T->NbPlan;i++) {
			if(T->fJobMarginal)j += sprintf(tamponalloc+j,"%9.5lf%9.5lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
			else j += sprintf(tamponalloc+j,"%9.5lf%9.5lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
		}
#else 
#ifdef OPTION_JFC
		if(fZoom)j += sprintf(tamponalloc+j,"%8.4lf%8.4lf",T->Zoom_DistribExact[n],T->Zoom_DistribCumulee[n]);
		else for (i=0;i<T->NbPlan;i++) {
			if(T->fJobMarginal)j += sprintf(tamponalloc+j,"%8.4lf%8.4lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
			else j += sprintf(tamponalloc+j,"%6.2lf%6.2lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
		}
#else 
		if(fZoom)j += sprintf(tamponalloc+j,"%6.2lf%6.2lf",T->Zoom_DistribExact[n],T->Zoom_DistribCumulee[n]);
		else for (i=0;i<T->NbPlan;i++) {
			if(T->fJobMarginal)j += sprintf(tamponalloc+j,"%6.2lf%6.2lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
			else j += sprintf(tamponalloc+j,"%6.2lf%6.2lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
		}
#endif
#endif
		strcat(tamponalloc,"\r\n");j+=2;
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
	if (T->NbVentilation) {
		for (i=0;i<T->NbVentilation;i++)
			for (n=0;n<T->NbClasseVent[i];n++) {
				// Population de classe ventile
				// j = sprintf(tamponalloc,"%12ld\r\n",T->PopventCouv[i][n]);
#ifdef OPTION_CHECKUP
				j = sprintf(tamponalloc,"%12.0f %5d\r\n",T->PopventGRP[i][n],T->NbCasVentilation[i][n]);
#else
				j = sprintf(tamponalloc,"%12.0f\r\n",T->PopventGRP[i][n]);
#endif
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// GRP Ventile
				j = 0;
				for (p=0;p<T->NbPlan;p++)
#ifdef OPTION_MEDIACONSULT
					j += sprintf(tamponalloc+j,"%12.5lf",T->resGRPvent[p][i][n]);
#else
					j += sprintf(tamponalloc+j,"%9.2lf",T->resGRPvent[p][i][n]);
#endif
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// Couverture ventile
				j = 0;
				for (p=0;p<T->NbPlan;p++)
#ifdef OPTION_MEDIACONSULT
					j += sprintf(tamponalloc+j,"%12.5lf",T->resCOUVvent[p][i][n]);
#else
					j += sprintf(tamponalloc+j,"%9.2lf",T->resCOUVvent[p][i][n]);
#endif
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
			}
	}
	if (T->Beta >= 0) {
		j=0;
		for (i=0;i<T->NbPlan;i++) {
			j += sprintf(tamponalloc+j,"%9.1lf",T->CouvertureValide[i]);
		}
		strcat(tamponalloc,"\r\n");j+=2;
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
	strcpy(tamponalloc," ^\r\n");
	fwrite(tamponalloc,sizeof(char),4,fp_Restom);
	fclose(fp_Restom);
	return(1);
}



#ifdef OPTION_MEDIACONSULT
long CTomjob::EcritureRestom_MediaConsultant(char fZoom)
{
	FILE *fp_Restom;
	short i,j,p,n;

	fp_Restom=_fsopen(m_NomFicRESTOM,"wb",_SH_DENYNO);
	if (NULL==fp_Restom) {
		CString Error;
		Error.Format("La création du fichier de résultat est impossible\nerrno: %d %s",errno,strerror(errno));
		 AfficheErreur(Error);
		return(0);
	}

	// les résultats
	j = sprintf(tamponalloc,"%.0lf,%d\r\n",T->PopCibTotalGRP,T->NbCasTotal);
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	// Nombre d'individus touchés
	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%.0lf",T->Zoom_NbIndivT);
	else for(i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%.0lf,",T->NbIndivT[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	// Nombre de conatacts
	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%.0lf",T->Zoom_NbContact);
	else for (i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%.0lf,",T->NbContact[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	// Couverture validée
	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%.0lf",T->Zoom_CouvertureValide);
	else for (i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%.0lf,",T->CouvertureValide[i] );
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	
	
	// Nombre de contacts validés
	j=0;
	if(fZoom)j += sprintf(tamponalloc+j,"%.0lf",T->Zoom_NbContactValide);
	else for (i=0;i<T->NbPlan;i++)j += sprintf(tamponalloc+j,"%.0lf,",T->NbContactValide[i] );
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);

	
	j = sprintf(tamponalloc,"%d,",T->NbClasseContact);
	for (i=0;i<T->NbClasseContact;i++)j += sprintf(tamponalloc+j,"%.0f,",100.0*T->CourbeReponse[i]);
	strcat(tamponalloc,"\r\n");j+=2;
	fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	for (n=0;n<T->NbClasseContact;n++) {
		j=0;

		if(fZoom)j += sprintf(tamponalloc+j,"%.0lf,%.0lf",T->Zoom_DistribExact[n],T->Zoom_DistribCumulee[n]);
		else for (i=0;i<T->NbPlan;i++) {
			if(T->fJobMarginal)j += sprintf(tamponalloc+j,"%.0lf,%.0lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
			else j += sprintf(tamponalloc+j,"%.0lf,%.0lf",T->DistribExact[i][n],T->DistribCumulee[i][n]);
		}
		strcat(tamponalloc,"\r\n");j+=2;
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
	if (T->NbVentilation) {
		for (i=0;i<T->NbVentilation;i++){
			j = sprintf(tamponalloc,"\r\n");
			fwrite(tamponalloc,sizeof(char),j,fp_Restom);
			for (n=0;n<T->NbClasseVent[i];n++) {
				// Population de classe ventile
				// j = sprintf(tamponalloc,"%12ld\r\n",T->PopventCouv[i][n]);
				j = sprintf(tamponalloc,"%.0f\r\n",T->PopventGRP[i][n]);
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// GRP Ventile
				j = 0;
				for (p=0;p<T->NbPlan;p++)
					j += sprintf(tamponalloc+j,"%.0lf,",T->resGRPvent[p][i][n]);
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// GRP Ventile validé
				j = 0;
				for (p=0;p<T->NbPlan;p++)
					j += sprintf(tamponalloc+j,"%.0lf,",T->resGRPventValide[p][i][n]);
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// Couverture ventile
				j = 0;
				for (p=0;p<T->NbPlan;p++)
					j += sprintf(tamponalloc+j,"%.0lf,",T->resCOUVvent[p][i][n]);
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
				// Couverture ventile validé
				j = 0;
				for (p=0;p<T->NbPlan;p++)
					j += sprintf(tamponalloc+j,"%.0lf,",T->resCOUVventValide[p][i][n]);
				strcat(tamponalloc,"\r\n");j+=2;
				fwrite(tamponalloc,sizeof(char),j,fp_Restom);
			}
		}
	}
	if (T->Beta >= 0) {
		j=0;
		for (i=0;i<T->NbPlan;i++) {
			j += sprintf(tamponalloc+j,"%9.1lf",T->CouvertureValide[i]);
		}
		strcat(tamponalloc,"\r\n");j+=2;
		fwrite(tamponalloc,sizeof(char),j,fp_Restom);
	}
	strcpy(tamponalloc,"^\r\n");
	fwrite(tamponalloc,sizeof(char),4,fp_Restom);
	fclose(fp_Restom);
	return(1);
}
#endif




long CTomjob::EcritureRestom_Carat(char fZoom)
{

	CStdioFile f;
	short i,n,r,j;
	CFileException e;
	CString txt,lig;

	if( !f.Open(m_NomFicRESTOM, CFile::modeCreate | CFile::modeWrite | CFile::typeText   , &e ) )   {
#ifdef _DEBUG   
		afxDump << "File could not be opened " << e.m_cause << "\n";
#else
		CString Error;
		Error.Format("La création du fichier de résultat est impossible\nerrno: %ld",e.m_cause);
		AfficheErreur(Error);
#endif   
		return(0);
	}

	// les résultats
	// population de référence, nb de cas de référence
	double pop100=(double)T->PopCibTotalGRP/100.0;
	sprintf(tamponalloc,"%.0lf;%d\n",T->PopCibTotalGRP,T->NbCasTotal);
	f.WriteString(tamponalloc);
	
	int Plan,NbPlan=1;
	for(Plan=0;Plan<T->NbPlan;Plan++){
		CString txt="";
		f.WriteString("@1\n"); // ***************************************
		int NbInsert=0;
		{ //Calcul du nb d'insert
			short src,sup;
			for(src=0;src<T->NbSource;src++){
				for(sup=0;sup<T->NbSupport[src];sup++){
					if(T->fJobMarginal){
						if(fZoom)NbInsert+=T->Zoom_Support[sup].Margi_NbInsert;
						else NbInsert+=T->Support[src][sup].Margi_NbInsert;
					}
					else NbInsert+=T->Support[src][sup].NbInsert[Plan];
				}
			}
		}

		// Nb d'insertions
		sprintf(tamponalloc,"%d;",NbInsert);
		txt+=tamponalloc;

		// Nb d'individus touché
		if(fZoom)sprintf(tamponalloc,"%.0lf;",T->Zoom_NbIndivT/1000.0);
		else sprintf(tamponalloc,"%.0lf;",T->NbIndivT[Plan]/1000.0);
		txt+=tamponalloc;

		// Couverture
		if(fZoom)sprintf(tamponalloc,"%.1lf;",T->Zoom_DistribCumulee[0]);
		else sprintf(tamponalloc,"%.1lf;",T->DistribCumulee[Plan][0]);
		txt+=tamponalloc;

		// Couverture valide
		if(fZoom)sprintf(tamponalloc,"%.1lf;",0.0);
		else sprintf(tamponalloc,"%.1lf;",T->CouvertureValide[Plan]);
		txt+=tamponalloc;

		// Couverture entre x et y
		{
			double d=+0.0;
			short c;
			if(fZoom)for(c=T->Mini;c<=T->Maxi;c++)d+=T->Zoom_DistribExact[c-1];
			else for(c=T->Mini;c<=T->Maxi;c++)d+=T->DistribExact[Plan][c-1];
			sprintf(tamponalloc,"%.1lf;",d);
			txt+=tamponalloc;
		}

		// Nb de contacts
		if(fZoom)sprintf(tamponalloc,"%.0lf;",T->Zoom_NbContact/1000.0);
		else sprintf(tamponalloc,"%.0lf;",T->NbContact[Plan]/1000.0);
		txt+=tamponalloc;

		// GRP
		double GRP;
		if(fZoom)GRP=T->Zoom_NbContact/pop100;
		else GRP=T->NbContact[Plan]/pop100;
		sprintf(tamponalloc,"%.1lf;",GRP);
		txt+=tamponalloc;

		// Répétition
		if(fZoom)sprintf(tamponalloc,"%.1lf;",0/*T->Zoom_NbContact/(T->Zoom_DistribCumulee[0]*pop100)*/);
		else sprintf(tamponalloc,"%.1lf;",T->NbContact[Plan]/(T->DistribCumulee[Plan][0]*pop100));
		txt+=tamponalloc;

		// GRP moyen
		sprintf(tamponalloc,"%.1lf",GRP/NbInsert);
		txt+=tamponalloc;
		txt+="\n";
		f.WriteString(txt);

		// Nb de classes  et le courbe de réponse
		txt="";
		sprintf(tamponalloc,"%d;",T->NbClasseContact);
		txt+=tamponalloc;
		for (i=0;i<T->NbClasseContact;i++){
			sprintf(tamponalloc,"%.0f;",100.0*T->CourbeReponse[i]);
			txt+=tamponalloc;
		}
		txt+="\n";
		f.WriteString(txt);

		txt="";
		f.WriteString("@2\n"); // ********** TOUCHE 1-20 ***************
		// Nb d'individus non touché
		if(fZoom){
			txt.Format("0;%.0f;%.1f;\n",
				(T->PopCibTotalGRP-T->Zoom_NbIndivT)/1000,
				(T->PopCibTotalGRP-T->Zoom_NbIndivT)/pop100);
			f.WriteString(txt);
			// individus touché de 1 à n
			for (n=0;n<T->NbClasseContact;n++) {
				txt.Format("%d;%.0f;%.1f;%.1f\n",
					n+1,
					T->Zoom_DistribExact[n]*pop100/1000,
					T->Zoom_DistribExact[n],
					T->Zoom_DistribCumulee[n]);
				f.WriteString(txt);
			}
		}
		else { // Cas sans le zoom
			txt.Format("0;%.0f;%.1f;\n",
				(T->PopCibTotalGRP-T->NbIndivT[Plan])/1000,
				(T->PopCibTotalGRP-T->NbIndivT[Plan])/pop100);
			f.WriteString(txt);
			// individus touché de 1 à n
			for (n=0;n<T->NbClasseContact;n++) {
				txt.Format("%d;%.0f;%.1f;%.1f\n",
					n+1,
					T->DistribExact[Plan][n]*pop100/1000,
					T->DistribExact[Plan][n],
					T->DistribCumulee[Plan][n]);
				f.WriteString(txt);
			}
		}
		f.WriteString("@3\n"); // **************** PMG *******************
		for (r=0;r<m_fCalculRatio;r++) {
			double repet=0;
			CString txtPMG;
			switch(r){
				case 0:txtPMG="P(33%)";break;
				case 1:txtPMG="M(33%)";break;
				case 2:txtPMG="G(33%)";break;
			}
			if(T->IndividuRatio[Plan][r]>0)repet=T->ContactRatio[Plan][r]/T->IndividuRatio[Plan][r];
			txt.Format("%s;%.1f;%.1f;%.1f\n",
				txtPMG,//100.0/m_fCalculRatio,
				T->ContactRatio[Plan][r]/pop100,
				100.0*T->ContactRatio[Plan][r]/T->NbContact[Plan],
				repet);
			f.WriteString(txt);
		}
		f.WriteString("@4\n"); // ************ APPORT PAR JOUR ************
		for(j=0;j<T->Jour.GetSize();j++){
			double n;
			if(j>0)n=T->Jour[j].Individu-T->Jour[j-1].Individu;
			else n=T->Jour[j].Individu;
			txt.Format("%s;%.0f;%.1f;%.1f\n",
				T->Jour[j].Libelle,
				n/1000,
				100.0*n/T->NbIndivT[0],
				T->Jour[j].Individu/pop100);
			f.WriteString(txt);
		}

		f.WriteString("@5\n"); // ************ APPORT PAR SEMAINE ************
		for(j=0;j<T->Semaine.GetSize();j++){
			double n;
			if(j>0)n=T->Semaine[j].Individu-T->Semaine[j-1].Individu;
			else n=T->Semaine[j].Individu;
			txt.Format("%s;%.0f;%.1f;%.1f\n",
				T->Semaine[j].LibJour,
				n/1000,
				100.0*n/T->NbIndivT[0],
				T->Semaine[j].Individu/pop100);
			f.WriteString(txt);
		}

		f.WriteString("@6\n"); // ************ APPORT PAR SEMAINE GLISSANTE  ***
		for(j=0;j<T->SemaineGliss.GetSize();j++){
			double n;
			if(j>0)n=T->SemaineGliss[j].Individu-T->SemaineGliss[j-1].Individu;
			else n=T->SemaineGliss[j].Individu;
			txt.Format("%s;%.0f;%.1f;%.1f\n",
				T->SemaineGliss[j].LibJour,
				n/1000,
				100.0*n/T->NbIndivT[0],
				T->SemaineGliss[j].Individu/pop100);
			f.WriteString(txt);
		}

		f.WriteString("@7\n"); // ************ VENTILATIONS ************
		for(i=0;i<T->NbVentilation;i++){
			txt.Format("%s\n",m_FicLBX->GetLibCritere(T->NrCritereVent[i]));
			f.WriteString(txt);
			for(n=0;n<T->NbClasseVent[i];n++) {
				double r;
				if(T->resCOUVvent[Plan][i][n]>0)
					r=T->resGRPvent[Plan][i][n]/T->resCOUVvent[Plan][i][n];
				else r=0;
				txt.Format("%s;%.0f;%d;%.1f;%.1f;%.1f\n",
					m_FicLBX->GetLibModalite(T->NrCritereVent[i],n),
					T->PopventGRP[i][n],
					T->NbCasVentilation[i][n],
					T->resGRPvent[Plan][i][n],
					T->resCOUVvent[Plan][i][n],
					r);
				f.WriteString(txt);
			}
		}
		f.WriteString("@8\n"); // ************ THE END ************
	}
	f.Close();
	return(0);
}

void CTomjob::LibereStructTomjob(void){
	short src,s,v,r;

	if(m_fMultiMedia){
		int nb=	nb=T->NbFoisToucheMax+1;
		for(int n1=0;n1<nb;n1++){
			delete [] m_TabCroiseValide[n1];
			delete [] m_TabCroise[n1];
			delete [] m_TabBeta[n1];
		}
		delete [] m_Quartiles;
		delete [] m_GRPQuartiles;
		delete [] m_TabCroiseValide;
		delete [] m_TabBeta;
		delete [] m_TabCroise;
		if(m_CoeffExComp)delete [] m_CoeffExComp;
	}

	DelierSource();
	LibereCalcul();
	if (T->NbVentilation) {
		if(m_fCorrigeCoeffEx){
			for (src=0;src<T->NbSource;src++) {
				for(r=0;r<T->NbRegion[src];r++){
					for (s=0;s<T->NbVentilation;s++) {
						delete [] T->popGRPregionVentile[src][r][s];
					}
					delete [] T->popGRPregionVentile[src][r];
				}
				delete [] T->popGRPregionVentile[src];
			}
		}

		for (src=0;src<T->NbSource;src++) {
			for (s=0;s<T->NbVentilation;s++) {
				delete [] T->NrClasseVent[src][s];
				delete [] T->PopGRPclassVent[src][s];
			}
			delete [] T->NrClasseVent[src];
			delete [] T->PopGRPclassVent[src];
		}
		delete [] T->NrClasseVent;
		for (src=0;src<T->NbSource;src++) {
			for(s=0;s<T->NbSupport[src];s++) {
				for (v=0;v<T->NbVentilation;v++) {
					delete [] T->Support[src][s].GRPvent[v];
				}
				delete [] T->Support[src][s].GRPvent;
			}
		}
		delete [] T->NbClasseVent;
		if(m_fExtern)delete [] T->NrCritereVent;
		delete [] T->PopGRPclassVent;
	}
	for (src=0;src<T->NbSource;src++) {
		for (s=0;s<=T->NbRegion[src];s++) {
			delete [] T->fIndivRegion[src][s];
		}
		delete [] T->fIndivRegion[src];
		delete [] T->NrRegionVect[src];
		delete [] T->popGRPregion[src];
		if(!m_fExtern)delete [] T->NrRegionIndiv[src];
		
		if(!T->fJobMarginal)for (s=0;s<T->NbSupport[src];s++){
			delete [] T->Support[src][s].NbInsert;
		}
		delete [] T->NomFicCouv[src];
#ifdef OPTION_MEDIACONSULT
		delete [] T->NomFicIND[src];
		delete [] T->NomFicCOD[src];
		delete [] T->NomFicVentil[src];
		delete [] T->NomFicNbModal[src];
		delete [] T->NomFicPoids[src];
#endif
		delete [] T->CoeffExCible[src];
		delete [] T->NrRatio[src];
		delete [] T->CoeffExRed[src];
		if(!m_fExtern)delete [] T->CoeffExEnsemble[src];
		delete [] T->Support[src];
	}
	delete [] T->NrSourceCouv;
	delete [] T->NrVagueCouv;
	delete [] T->FormatSource;
	delete [] T->NomFicCouv;
#ifdef OPTION_MEDIACONSULT
	delete [] T->NomFicIND;
	delete [] T->NomFicCOD;
	delete [] T->NomFicVentil;
	delete [] T->NomFicNbModal;
	delete [] T->NomFicPoids;
#endif
	delete [] T->PopCibleGRP;
	delete [] T->NbIndivC;
	delete [] T->CoeffExCible;
	delete [] T->NrRatio;
	delete [] T->CoeffExEnsemble;
	delete [] T->CoeffExRed;
	delete [] T->NbRegion;
	delete [] T->NrRegionVect;
	delete [] T->popGRPregion;
	delete [] T->popGRPregionVentile;
	delete [] T->fIndivRegion;
	if(!m_fExtern)	delete [] T->NrRegionIndiv;
	delete [] T->NbSupport;
	delete [] T->NbSupportAlloue;

	delete [] T->CodeVagueCouv;
	delete [] T->Support;
	delete T;
	m_fSourceAllocated=0;
}

CTomjob::CTomjob()
{
	tamponalloc=new char [10000];
//	PrepareCalcul();
	m_fBreakDetected=0;
	m_fQuickJob=0;
	m_fSourceAllocated=0;


#ifdef OPTION_MEDIACONSULT
	m_FicInd=NULL;
	m_FicAuxItaly=NULL;
	m_FicCod=NULL;
#endif

}

CTomjob::~CTomjob()
{
	delete [] tamponalloc;
//	FinPrepareCalcul();
//	LibererSource();
}


void CTomjob::MarqueLesIndividus(short src,short sup,char fZoom)
{
	short i,NrI;

//	short *pNbSupport;
	SSupport *Support;
	if(fZoom){
		Support=T->Zoom_Support;
//		pNbSupport=&T->Zoom_NbSupport;
	}
	else {
		Support=T->Support[m_NewSrc];
//		pNbSupport=&T->NbSupport[m_NewSrc];
	}

	for(i=0;i<Support[sup].Sup->NbCas;i++){
		NrI=Support[sup].Sup->Indiv[i].NrIndiv;
		T->Margi_Marquage[NrI]=1;
	}
}

#include <afxdisp.h>

void CTomjob::EvaluerApportSemaine()
{
	CString OldLib="",NewLib,LibEnJour;
	COleDateTime D;
	SJour S;
	for(int j=0;j<T->Jour.GetSize();j++){
		// Ce jour fait parti de quelle semaine ?
		D.ParseDateTime(T->Jour[j].Libelle,VAR_DATEVALUEONLY);
	//	D.SetTime(12,1,1);
		int NoSem=atoi(D.Format("%W"))+1;
		int Year=D.GetYear();
		NewLib.Format("Semaine %d/%d",NoSem,Year);

		// Est-ce une nouvelle semaine ?
		if(NewLib!=OldLib){
			COleDateTimeSpan Duree;
			COleDateTime Lun,Dim;

			OldLib=NewLib;
			T->Semaine.Add(S);
			// On en profite pour créer le libelle
			int day=D.GetDayOfWeek()-2;
			if(day<0)day=6;
			Duree.SetDateTimeSpan(day,0,0,0);
			Lun=D-Duree;
			Duree.SetDateTimeSpan(6,0,0,0);
			Dim=Lun+Duree;
			LibEnJour.Format("du %s au %s",Lun.Format("%d/%m/%y"),Dim.Format("%d/%m/%y"));
		}
		strcpy(S.Libelle,NewLib);
		strcpy(S.LibJour,LibEnJour);
		S.Individu=T->Jour[j].Individu;
		T->Semaine[T->Semaine.GetSize()-1]=S;
	}
}

void CTomjob::EvaluerApportSemaineGlissante()
{
	COleDateTimeSpan Dgliss; 
	int DayGliss;
	CString OldLib="",NewLib,LibEnJour;
	COleDateTime D;
	SJour S;
	
	// On calcul le décalage entre le jour de début et le lundi
	if(T->Jour.GetSize()<=0)return;
	D.ParseDateTime(T->Jour[0].Libelle,VAR_DATEVALUEONLY);
	DayGliss=D.GetDayOfWeek()-2;
	if(DayGliss<0)DayGliss=6;
	Dgliss.SetDateTimeSpan(DayGliss,0,0,0);

	for(int j=0;j<T->Jour.GetSize();j++){
		// Ce jour fait parti de quelle semaine ?
		D.ParseDateTime(T->Jour[j].Libelle,VAR_DATEVALUEONLY);
	//	D.SetTime(12,1,1);
		D=D-Dgliss;
		int NoSem=atoi(D.Format("%W"))+1;
		int Year=D.GetYear();
		NewLib.Format("Semaine %d/%d",NoSem,Year);

		// Est-ce une nouvelle semaine ?
		if(NewLib!=OldLib){
			COleDateTimeSpan Duree;
			COleDateTime Lun,Dim;

			OldLib=NewLib;
			T->SemaineGliss.Add(S);
			// On en profite pour créer le libelle
			int day=D.GetDayOfWeek()-2;
			if(day<0)day=6;
			day-=DayGliss;
			Duree.SetDateTimeSpan(day,0,0,0);
			Lun=D-Duree;
			Duree.SetDateTimeSpan(6,0,0,0);
			Dim=Lun+Duree;
			LibEnJour.Format("du %s au %s",Lun.Format("%d/%m/%y"),Dim.Format("%d/%m/%y"));
		}
		strcpy(S.Libelle,NewLib);
		strcpy(S.LibJour,LibEnJour);
		S.Individu=T->Jour[j].Individu;
		T->SemaineGliss[T->SemaineGliss.GetSize()-1]=S;
	}
}

long CTomjob::EcritureRestom_Mediatop(char fZoom)
{

	CStdioFile f;
	short i,n;
	CFileException e;
	CString txt,lig;

	if( !f.Open(m_NomFicRESTOM, CFile::modeCreate | CFile::modeWrite | CFile::typeText   , &e ) )   {
#ifdef _DEBUG   
		afxDump << "File could not be opened " << e.m_cause << "\n";
#else
		CString Error;
		Error.Format("La création du fichier de résultat est impossible\nerrno: %ld",e.m_cause);
		AfficheErreur(Error);
#endif   
		return(0);
	}

	// les résultats
	// population de référence, nb de cas de référence
	double pop100=(double)T->PopCibTotalGRP/100.0;
	sprintf(tamponalloc,"%.0lf %d\n",T->PopCibTotalGRP,T->NbCasTotal);
	f.WriteString(tamponalloc);
	
	int Plan,NbPlan=1;
	if(fZoom){
		f.WriteString("Format de fichier à définir");
	}
	else for(Plan=0;Plan<T->NbPlan;Plan++){
		CString txt="";
		f.WriteString("#1 NbInsert GRP Couverture Couverture_Validée Répétition\n"); // ***************************************
		int NbInsert=0;
		{ //Calcul du nb d'insert
			short src,sup;
			for(src=0;src<T->NbSource;src++){
				for(sup=0;sup<T->NbSupport[src];sup++){
					NbInsert+=T->Support[src][sup].NbInsert[Plan];
				}
			}
		}

		// Nb d'insertions
		sprintf(tamponalloc,"%d ",NbInsert);
		txt+=tamponalloc;

		// GRP
		double GRP=T->NbContact[Plan]/pop100;
		sprintf(tamponalloc,"%.2lf ",GRP);
		txt+=tamponalloc;


		// Couverture
		sprintf(tamponalloc,"%.2lf ",T->DistribCumulee[Plan][0]);
		txt+=tamponalloc;

		// Couverture valide
		sprintf(tamponalloc,"%.2lf ",T->CouvertureValide[Plan]);
		txt+=tamponalloc;

		// Répétition
		sprintf(tamponalloc,"%.2lf\n",T->NbContact[Plan]/(T->DistribCumulee[Plan][0]*pop100));
		txt+=tamponalloc;
		f.WriteString(txt);

		// *************************** DISTRIBUTION *************
		f.WriteString("#2 Classe Courbe_de_réponse Couverture_exacte Couverture_cumulée \n"); // ********** TOUCHE 1-20 ***************
		txt.Format("%d\n",T->NbClasseContact);
		f.WriteString(txt);
		for (n=0;n<T->NbClasseContact;n++) {
			txt.Format("%d %.0f %.2f %.2f\n",
				n+1,
				100.0*T->CourbeReponse[n],
				T->DistribExact[Plan][n],
				T->DistribCumulee[Plan][n]);
			f.WriteString(txt);
		}
		for(i=0;i<T->NbVentilation;i++){
		f.WriteString("#3 Ventilation\n"); // ************ VENTILATIONS ************
			txt.Format("\"%s\"\n",m_FicLBX->GetLibCritere(T->NrCritereVent[i]));
			f.WriteString(txt);
			for(n=0;n<T->NbClasseVent[i];n++) {
				double r;
				if(T->resCOUVvent[Plan][i][n]>0)
					r=T->resGRPvent[Plan][i][n]/T->resCOUVvent[Plan][i][n];
				else r=0;
				txt.Format("\"%s\" %.0f %.2f %.2f %.2f\n",
					m_FicLBX->GetLibModalite(T->NrCritereVent[i],n),
					T->PopventGRP[i][n],
					//T->NbCasVentilation[i][n],
					T->resGRPvent[Plan][i][n],
					T->resCOUVvent[Plan][i][n],
					r);
				f.WriteString(txt);
			}
		}
		f.WriteString("## fin de ce sous-plan\n");
	}
	f.WriteString("### fin\n"); // ************ THE END ************
	f.Close();
	return(0);
}

void CTomjob::SetJob0_New(HWND hWndMaitre,char fCalculMarginal){
	AlloueStructTomjob(fCalculMarginal);

	AnnuleMemoZoom();
	m_fExtern=0;
	m_fVentilCouv=0;
	m_fCalculCHI=0;
	m_fCalculDupp=0;
	m_fCorrigeCoeffEx=0;
	m_NomFicRESTOM="";
	m_NomFicTOMJOB="";
	m_hWndMaitre=hWndMaitre;
	m_fCalculRatio=0;
	m_fApportJournalier=0;
	m_fMultiMedia=0;
}

void CTomjob::SetJob1_Etude(int Nr,LPCSTR Libelle){
	T->NrEtude = Nr;
	T->LibEtude=Libelle;
}

void CTomjob::SetJob2_Cible(int Nr,LPCSTR Libelle){
	T->NrCible = Nr;
	T->LibCible=Libelle;
}

void CTomjob::SetJob3_TypeSuppU(int Nr){
	T->TypeSuppU = Nr;
	if(T->TypeSuppU>100){
		m_fExtern=1;
		T->TypeSuppU%=100;
	}
	else m_fCorrigeCoeffEx=1; 
	// flag de ventilation sur la source de couverture
	if(T->TypeSuppU==1 || T->TypeSuppU==3|| T->TypeSuppU==5)m_fVentilCouv=1;
	if(T->TypeSuppU==1 || T->TypeSuppU==3)m_fCorrigeCoeffEx=0; 

#ifdef OPTION_ZOOMRADIOTELE
	m_fCorrigeCoeffEx=0;
	m_fMultiMedia=1;
#endif

#ifdef OPTION_TOPCHRONO
	m_fCorrigeCoeffEx=0;
	m_fMultiMedia=0;
#endif

	if(m_fMultiMedia){
		int nb=	nb=T->NbFoisToucheMax+1;
		m_TabCroise=new double*[nb];
		m_TabCroiseValide=new double*[nb];
		m_TabBeta=new double*[nb];
		m_Quartiles=new double[5];
		m_GRPQuartiles=new double[5];
		for(int n1=0;n1<nb;n1++){
			m_TabCroise[n1]=new double[nb];
			m_TabCroiseValide[n1]=new double[nb];
			m_TabBeta[n1]=new double[nb];
		}
		m_CoeffExComp=NULL; // Alloué + loin
	}
}

void CTomjob::SetJob4_Beta(int Nr){
	// TODO:T->Mini T->Maxi
	T->Beta = Nr;
}

#include "math.h"
void CTomjob::SetJob4_Beta(int BetaMedia1,int BetaMedia2,int VisualTransfert){
	// C'est pour du multimedia
	if(!m_fMultiMedia){
		AfficheErreur("SetJob4_Beta(int BetaMedia1,int BetaMedia2): Réservé au plan multimédia");
		return;
	}
	T->Beta=BetaMedia1;
	if(T->Beta<=0)return;

	int n,n1,n2,nb=T->NbFoisToucheMax;

	m_TabBeta[0][0]=0;
	double Beta0=(double)BetaMedia1/100.0;
	for(n=1;n<=nb;n++)m_TabBeta[n][0]=1-pow(1-Beta0,n);
	Beta0=(double)BetaMedia2/100.0;
	for(n=1;n<=nb;n++)m_TabBeta[0][n]=1-pow(1-Beta0,n);
	if(VisualTransfert<=0){
		for(n1=1;n1<=nb;n1++){
			for(n2=1;n2<=nb;n2++){
				m_TabBeta[n1][n2]=m_TabBeta[n1][0]+(1-m_TabBeta[n1][0])*m_TabBeta[0][n2];
			}
		}
	}
	else{
		//calcul de k
		double BetaRT=(double)VisualTransfert/100.0;
		double BetaR=(double)BetaMedia1/100.0;
		double BetaT=(double)BetaMedia2/100.0;
		double k=1-sqrt( (1-BetaRT)/((1-BetaR)*(1-BetaT)) );

		for(n1=1;n1<=nb;n1++){
			for(n2=1;n2<=nb;n2++){
				m_TabBeta[n1][n2]=1-pow(1-BetaR,n1)*pow(1-BetaT,n2)*pow(1-k,n1+n2) ;
			}
		}
	}
}

void CTomjob::SetJob5_NbPlan(int Nr){
	T->NbPlan = Nr;
	if(T->fJobMarginal && T->NbPlan>1){
	 	AfficheErreur("NbPlan>1 dans tomjob marginal");
	}
}

void CTomjob::SetJob6_NbSource(int Nr){
	T->NbSource =Nr;
}

void CTomjob::SetJob7_Ventilation(int Nb,int * Nr){
	if(Nb){
		AfficheErreur("Ventilations à faire...");
		//TODO: Ventilations à faire...
	}
	T->NbVentilation=0; // Nb;
	if(T->fJobMarginal && T->NbVentilation>0){
		AfficheErreur("T->NbVentilation dans tomjob marginal");
	}
}

void CTomjob::SetSource1_Definition(int NrSrc,LPCSTR LibVague,int Format){


	AlloueJob();

	if(m_fExtern){
		T->NrSourceCouv[NrSrc]=NrSrc;
		T->FormatSource[NrSrc]=Format;
		T->CodeVagueCouv[NrSrc]=LibVague;
	}
	else {
		T->NrSourceCouv[NrSrc]=NrSrc;
		T->FormatSource[NrSrc]=Format;
		T->NrVagueCouv=0; // TODO 
	}
}

void CTomjob::SetSource2_Repertoire(int NrSrc,LPCSTR Directory){
	T->NomFicCouv[NrSrc]=new char [strlen(Directory)+1];
	strcpy(T->NomFicCouv[NrSrc],Directory);
}

void CTomjob::SetSource3_Population(int NrSrc,long Population){
	T->PopCibleGRP[NrSrc]=Population;
	//TODO	if(!fExtern...T->NbIndivC[src]
}

void CTomjob::SetSource4_Region(int NrSrc,int Nb,int * Nr){
	T->NbRegion[NrSrc] = Nb;
	if(T->NbRegion[NrSrc]==0)T->NbRegion[NrSrc]=1;
	T->NrRegionVect[NrSrc]=new short [T->NbRegion[NrSrc]];
	T->popGRPregion[NrSrc]=new long [T->NbRegion[NrSrc]];
	for(int r=0;r<T->NbRegion[NrSrc];r++)T->NrRegionVect[NrSrc][r]=Nr[r];
}

void CTomjob::SetSource5_Support(int NrSrc,int Nb,bool fCalculMonoSource){
	if(fCalculMonoSource){
		for(int sup=0;sup<T->NbSupport[NrSrc];sup++)
			delete [] T->Support[NrSrc][sup].Sup;
		delete [] T->Support[NrSrc];
		m_NewSrc=NrSrc;
	}

	T->NbSupport[NrSrc]=Nb;
	if(T->fJobMarginal)T->NbSupportAlloue[NrSrc] = Nb+NBSPOTPREALLOC;
	else T->NbSupportAlloue[NrSrc]=T->NbSupport[NrSrc]+1;
	T->Support[NrSrc]=new SSupport [T->NbSupportAlloue[NrSrc]];
	memset(T->Support[NrSrc],0,sizeof(SSupport)*T->NbSupportAlloue[NrSrc]);
}

void CTomjob::SetSupport1_Definition(int NrSrc,int NrSup,int NrStation,int Horaire,int Jour,int NrRegion,int Option,double GRP,LPCSTR Prive,int * NbInsert,double * GRP_ventile,bool fCalculMonoSource){
	T->Support[NrSrc][NrSup].Station = NrStation;

	T->Support[NrSrc][NrSup].NrRegionForce=1; // TODO (char)TrouveNrIndexRegion(GSupport->AptReg,src);
	T->Support[NrSrc][NrSup].CodeHoraire = Horaire;
#ifdef HORAIRE_THEORIQUE
	// TODO
	//	T->Support[src][i].HorairePassage = GSupport->HorairePassage;
#else
	T->Support[NrSrc][NrSup].HorairePassage = Horaire;
#endif
	T->Support[NrSrc][NrSup].NrRegionForce=(char)TrouveNrIndexRegion(NrRegion,NrSrc);
	T->Support[NrSrc][NrSup].Jour = Jour;
	T->Support[NrSrc][NrSup].Option = Option;
	T->Support[NrSrc][NrSup].GRP = GRP;
	if(T->fJobMarginal){
		T->Support[NrSrc][NrSup].Margi_NbInsert =NbInsert[0];
		if(fCalculMonoSource){
			LierNouveauSpot(NrSrc,NrSup,0);
			CalculMediaWeightNouveauSpot(NrSrc,NrSup,0);
		}
	}
	else{
		// TODO: T->Support[NrSrc][NrSup].NbInsert = new ...
	}
	// TODO : GRP_ventile
}

void CTomjob::SetMode_QuickJob(bool flag)
{
	m_fQuickJob=flag;
}

void CTomjob::AlloueJob(){
	if(m_fSourceAllocated)return;


	T->NrSourceCouv=new short [T->NbSource];
	T->NrVagueCouv=new short [T->NbSource];
	T->FormatSource=new short [T->NbSource];
	T->NomFicCouv=new char* [T->NbSource];
#ifdef OPTION_MEDIACONSULT
	T->NomFicIND=new char* [T->NbSource];
	T->NomFicCOD=new char* [T->NbSource];
	T->NomFicVentil=new char* [T->NbSource];
	T->NomFicPoids=new char* [T->NbSource];
	T->NomFicNbModal=new char* [T->NbSource];
#endif
	T->PopCibleGRP=new long [T->NbSource];
	T->NbIndivC=new short [T->NbSource];
	T->NbRegion=new short [T->NbSource];
	T->NrRegionVect=new short* [T->NbSource];
	T->popGRPregion=new long* [T->NbSource];
	T->popGRPregionVentile=new long*** [T->NbSource];
	T->fIndivRegion=new bool** [T->NbSource];
	T->NrRegionIndiv=new char* [T->NbSource];
	T->CoeffExCible=new long* [T->NbSource];
	T->NrRatio=new short* [T->NbSource];
	T->CoeffExEnsemble=new long* [T->NbSource];
	T->CoeffExRed=new double* [T->NbSource];
	T->NbSupport=new int [T->NbSource];
	T->NbSupportAlloue=new int [T->NbSource];
	T->Support=new SSupport* [T->NbSource];;
	if (T->NbVentilation) {
		T->NrClasseVent=new char** [T->NbSource];
	}
	T->CodeVagueCouv=new CString[T->NbSource];

	m_fSourceAllocated=1;
}

double CTomjob::GetResultat_Couverture(){
	double f=100.0*T->NbIndivT[0]/ T->PopCibTotalGRP;
	return(f);
}

#ifdef CALCULASYMPTOTE
double CTomjob::GetResultat_Assymptote(){
	double f=100.0*T->NbIndivAssymptote[0]/ T->PopCibTotalGRP;
	return(f);
}
#endif

double ** CTomjob::GetResultat_TableauCroise()
{
	return m_TabCroise;
}

double ** CTomjob::GetResultat_TableauCroiseValide()
{
	return m_TabCroiseValide;
}

int CTomjob::GetResultat_NbClasseContact()
{
	return T->NbFoisToucheMax;
}

double * CTomjob::GetResultat_Quartiles(int NrSource,bool fCalcul)
{
	if(fCalcul)CalculQuartiles(NrSource);
	return m_Quartiles;
}

double * CTomjob::GetResultat_GRPQuartiles()
{
	return m_GRPQuartiles;
}

double CTomjob::GetResultat_ApportScanner()
{
	return m_ApportScanner;
}


long CTomjob::GetResultat_NbCasCible()
{
	return(T->NbCasTotal);
}

long CTomjob::GetResultat_PopulationCible()
{
	return((long)T->PopCibTotalGRP); //T->PopCibTotalCOUV
}
