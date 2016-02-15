// FicAUX.cpp: implementation of the CFicAUX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "tomeval.h"
#include "FicAUX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFicAUX::CFicAUX()
{
	m_fVagueLoaded=0;
}

CFicAUX::~CFicAUX()
{
	FreeVague();
}

short CFicAUX::LoadVague(LPCSTR pCheminVague,LPCSTR pCodeVague,HWND hWndMaitre)
{
	m_hWndMaitre=hWndMaitre;
	//	CWaitCursor Wait;
	if(m_fVagueLoaded)FreeVague();
	CStdioFile FicAUX;
	CString NomFic,MsgErr;
	CFileException e;
	short s,j,p,c;
	NomFic.Format("%s\\%s.AUX",pCheminVague,pCodeVague);
	if(!FicAUX.Open(NomFic,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfxMessageBox(MsgErr);
		return(1);
	}

	if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,0,0L);

	FicAUX.Read(&m_NbStation,sizeof(short));
	m_NrStation=new short [m_NbStation];
	m_NbJour=new short [m_NbStation];
	m_CodeJour=new short* [m_NbStation];
	m_NbPlage=new short* [m_NbStation];
	m_HPlage=new short** [m_NbStation];
	m_NbCode=new short** [m_NbStation];
	m_CodeNum=new short*** [m_NbStation];
	m_NbCas=new short** [m_NbStation];
	m_TabProba=new SCouvIndiv*** [m_NbStation];

	for(s=0;s<m_NbStation;s++){
		FicAUX.Read(&m_NrStation[s],sizeof(short));
		FicAUX.Read(&m_NbJour[s],sizeof(short));
		m_CodeJour[s]=new short [m_NbJour[s]];
		m_NbPlage[s]=new short [m_NbJour[s]];
		m_HPlage[s]=new short* [m_NbJour[s]];
		m_NbCode[s]=new short* [m_NbJour[s]];
		m_CodeNum[s]=new short** [m_NbJour[s]];
		m_NbCas[s]=new short* [m_NbJour[s]];
		m_TabProba[s]=new SCouvIndiv** [m_NbJour[s]];

		for(j=0;j<m_NbJour[s];j++){
			FicAUX.Read(&m_CodeJour[s][j],sizeof(short));
			FicAUX.Read(&m_NbPlage[s][j],sizeof(short));
			m_HPlage[s][j]=new short [m_NbPlage[s][j]];
			m_NbCode[s][j]=new short [m_NbPlage[s][j]];
			m_CodeNum[s][j]=new short* [m_NbPlage[s][j]];
			m_NbCas[s][j]=new short [m_NbPlage[s][j]];
			m_TabProba[s][j]=new SCouvIndiv* [m_NbPlage[s][j]];

			for(p=0;p<m_NbPlage[s][j];p++){
				FicAUX.Read(&m_HPlage[s][j][p],sizeof(short));
				FicAUX.Read(&m_NbCode[s][j][p],sizeof(short));
				if(m_NbCode[s][j][p]>0){
					m_CodeNum[s][j][p]=new short [m_NbCode[s][j][p]];
					short z;
					for(z=0;z<m_NbCode[s][j][p];z++){
						FicAUX.Read(&m_CodeNum[s][j][p][z],sizeof(short));
					}
				}
				FicAUX.Read(&m_NbCas[s][j][p],sizeof(short));
				m_TabProba[s][j][p]=new SCouvIndiv [m_NbCas[s][j][p]];
				for(c=0;c<m_NbCas[s][j][p];c++){
					FicAUX.Read(&m_TabProba[s][j][p][c].NrIndiv,sizeof(short));
					FicAUX.Read(&m_TabProba[s][j][p][c].Proba,sizeof(short));
				}
			}
		}
		if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,((s+1)*100)/m_NbStation,0L);
	}

	FicAUX.Close();
	m_fVagueLoaded=1;
	return(0);
}

short CFicAUX::LoadVague(LPCSTR NomFic,HWND hWndMaitre)
{
	m_hWndMaitre=hWndMaitre;
	//	CWaitCursor Wait;
	if(m_fVagueLoaded)FreeVague();
	CStdioFile FicAUX;
	CString MsgErr;
	CFileException e;
	short s,j,p,c;
	if(!FicAUX.Open(NomFic,CFile::modeRead  | CFile::typeBinary, &e ) )
	{
		MsgErr.Format("Erreur à l'ouverture du fichier %s ",NomFic);
		AfxMessageBox(MsgErr);
		return(1);
	}

	if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,0,0L);

	FicAUX.Read(&m_NbStation,sizeof(short));
	m_NrStation=new short [m_NbStation];
	m_NbJour=new short [m_NbStation];
	m_CodeJour=new short* [m_NbStation];
	m_NbPlage=new short* [m_NbStation];
	m_HPlage=new short** [m_NbStation];
	m_NbCode=new short** [m_NbStation];
	m_CodeNum=new short*** [m_NbStation];
	m_NbCas=new short** [m_NbStation];
	m_TabProba=new SCouvIndiv*** [m_NbStation];

	for(s=0;s<m_NbStation;s++){
		long l;
		//FicAUX.Read(&m_NrStation[s],sizeof(short));
		FicAUX.Read(&l,sizeof(long));
		m_NrStation[s]=(short)l;
		FicAUX.Read(&m_NbJour[s],sizeof(short));
		m_CodeJour[s]=new short [m_NbJour[s]];
		m_NbPlage[s]=new short [m_NbJour[s]];
		m_HPlage[s]=new short* [m_NbJour[s]];
		m_NbCode[s]=new short* [m_NbJour[s]];
		m_CodeNum[s]=new short** [m_NbJour[s]];
		m_NbCas[s]=new short* [m_NbJour[s]];
		m_TabProba[s]=new SCouvIndiv** [m_NbJour[s]];

		for(j=0;j<m_NbJour[s];j++){
			FicAUX.Read(&m_CodeJour[s][j],sizeof(short));
			FicAUX.Read(&m_NbPlage[s][j],sizeof(short));
			m_HPlage[s][j]=new short [m_NbPlage[s][j]];
			m_NbCode[s][j]=new short [m_NbPlage[s][j]];
			m_CodeNum[s][j]=new short* [m_NbPlage[s][j]];
			m_NbCas[s][j]=new short [m_NbPlage[s][j]];
			m_TabProba[s][j]=new SCouvIndiv* [m_NbPlage[s][j]];

			for(p=0;p<m_NbPlage[s][j];p++){
//				FicAUX.Read(&m_HPlage[s][j][p],sizeof(short));
				FicAUX.Read(&l,sizeof(long));
				m_HPlage[s][j][p]=(short)l;
				FicAUX.Read(&m_NbCode[s][j][p],sizeof(short));

				if(m_NbCode[s][j][p]>0){
					m_CodeNum[s][j][p]=new short [m_NbCode[s][j][p]];
					short z;
					for(z=0;z<m_NbCode[s][j][p];z++){
						FicAUX.Read(&m_CodeNum[s][j][p][z],sizeof(short));
					}
				}
//				FicAUX.Read(&m_NbCas[s][j][p],sizeof(short));
				FicAUX.Read(&l,sizeof(long));
				m_NbCas[s][j][p]=(short)l;
				m_TabProba[s][j][p]=new SCouvIndiv [m_NbCas[s][j][p]];
				for(c=0;c<m_NbCas[s][j][p];c++){
//					FicAUX.Read(&m_TabProba[s][j][p][c].NrIndiv,sizeof(short));
					FicAUX.Read(&l,sizeof(long));
					m_TabProba[s][j][p][c].NrIndiv=(short)l;
					FicAUX.Read(&m_TabProba[s][j][p][c].Proba,sizeof(short));
				}
			}
		}
		if(m_hWndMaitre)::SendMessage(m_hWndMaitre,MSGTOM_INFO_LOADHABITUDE,((s+1)*100)/m_NbStation,0L);
	}

	FicAUX.Close();
	m_fVagueLoaded=1;
	return(0);
}


void CFicAUX::FreeVague()
{
	if(!m_fVagueLoaded)return;
//	CWaitCursor Wait;

	short s,j,p;

	for(s=0;s<m_NbStation;s++){
		for(j=0;j<m_NbJour[s];j++){
			for(p=0;p<m_NbPlage[s][j];p++){
				delete [] m_TabProba[s][j][p];
				if(m_NbCode[s][j][p]>0)delete [] m_CodeNum[s][j][p];
			}
			delete [] m_HPlage[s][j];
			delete [] m_NbCode[s][j];
			delete [] m_CodeNum[s][j];
			delete [] m_NbCas[s][j];
			delete [] m_TabProba[s][j];
		}
		delete [] m_HPlage[s];
		delete [] m_CodeJour[s];
		delete [] m_NbPlage[s];
		delete [] m_NbCode[s];
		delete [] m_CodeNum[s];
		delete [] m_NbCas[s];
		delete [] m_TabProba[s];
	}
	delete [] m_NrStation;
	delete [] m_NbJour;
	delete [] m_CodeJour;
	delete [] m_NbPlage;
	delete [] m_NbCode;
	delete [] m_CodeNum;
	delete [] m_NbCas;
	delete [] m_TabProba;
	delete [] m_HPlage;
	
	m_fVagueLoaded=0;
}



short CFicAUX::TrouveProbas(short NrStation,short CodeHoraire,short HorairePassage,short Jour,short&NbCas,SCouvIndiv* &TabProba){

	short s,j,p;

	// recherche de la station
	for(s=0;s<m_NbStation;s++){
		if(m_NrStation[s]==NrStation)break;
	}
	if(s>=m_NbStation){
		AfxMessageBox("ERREUR: Station non trouvée");
		ASSERT(0);
		return(1);
	}

	// recherche du jour
	for(j=0;j<m_NbJour[s];j++){
#ifdef OPTION_ZOOMRADIOTELE
		if(m_CodeJour[s][j] & Jour)break;
#else
		if(m_CodeJour[s][j]==Jour)break;
#endif
	}
	if(j>=m_NbJour[s]){
		AfxMessageBox("ERREUR: Jour non trouvé");
		return(1);
	}

	// recherche par le code
	for(p=0;p<m_NbPlage[s][j];p++){
		short z;
		for(z=0;z<m_NbCode[s][j][p];z++){
			if(m_CodeNum[s][j][p][z]==CodeHoraire)goto OK;
		}
	}
	
	
	// recherche par l'horaire
	for(p=0;p<m_NbPlage[s][j]-1;p++){
		if(m_HPlage[s][j][p+1]>HorairePassage)break;
	}
	
	if(p>=m_NbPlage[s][j]){
		AfxMessageBox("ERREUR: Plage horaire non trouvée");
		return(1);
	}

OK:

	NbCas=m_NbCas[s][j][p];
	TabProba=m_TabProba[s][j][p];

	return(0);
}


void CFicAUX::LoadRatio(long *VCible,short NbIndiv, short *VRatio,short NbRatio)
{
	// Calcul en fc de la cible des ratios d'individus 
	long * Duree,pop,Quota,Used,*C,Ecoute;
	int s,j,p,c,i,q,NrI;
	Duree=new long [NbIndiv];
	memset(Duree,0,sizeof(long)*NbIndiv);
	C=new long [NbIndiv];
	memcpy(C,VCible,sizeof(long)*NbIndiv);

	for(s=0;s<m_NbStation;s++){
		for(j=0;j<m_NbJour[s];j++){
			for(p=0;p<m_NbPlage[s][j];p++){
				for(c=0;c<m_NbCas[s][j][p];c++){
					Duree[m_TabProba[s][j][p][c].NrIndiv]+=m_TabProba[s][j][p][c].Proba;
				}
			}
		}
	}


	for(pop=0,i=0;i<NbIndiv;i++)pop+=VCible[i];
	Used=0;
	for(q=0;q<NbRatio;q++){
		Quota=(pop*(q+1))/NbRatio;
		while(Quota>Used){
			NrI=-1;Ecoute=-1;
			for(i=0;i<NbIndiv;i++){
				if(!C[i])continue;
				if(NrI<0 || Duree[i]<Ecoute){
					NrI=i;
					Ecoute=Duree[i];
				}
			}
			VRatio[NrI]=q;
			Used+=C[NrI];
			C[NrI]=0;
		}
	}
	delete [] Duree;
	delete [] C;
}
