// Eval.cpp: implementation of the CEval class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "tomeval.h"
#include "Eval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEval::CEval()
{
	m_EtatTom=0; // ready;
	PrepareCalcul();
}

CEval::~CEval()
{
	// on vire tous les calcul CDZ en cours
	CTomjob* Tom;
	short i;

	// wParam est le numéro du lien.
	for(i=0;i<m_TomCDZ.GetSize();i++){
		Tom=m_TomCDZ[i];
		Tom->LibereStructTomjob();
		delete Tom;
	}
	m_TomCDZ.RemoveAll();
	FinPrepareCalcul();
	m_Tom.LibererSource();
}

short CEval::LanceEval(HWND hWnd,HWND hWndMaitre,LPCSTR TOMJOB, LPCSTR CIBJOB,LPCSTR RESTOM)
{
	if(!CheckDate())return(0);		// Autorisation d'acces

	

	// vérification de l'état...
	if(m_EtatTom){
		if(hWndMaitre>0)::PostMessage(hWndMaitre,MSGTOM_RESTOM_READY,m_EtatTom,0L);
		//else {
			/*switch(m_EtatTom){
				case 1: AfficheErreur("Le chargement de la vague est en cours ...");	break;
				case 2: AfficheErreur("Un calcul est en cours ...");	break;
				default:AfficheErreur("m_EtatTom inconnue");	break;*/
			//}
		//}
		return(1);
	}

	m_Tom.m_fBreakDetected=0;
	m_EtatTom=1; // chargement des sources
	m_Tom.AlloueStructTomjob();
	m_Tom.LectureTomjob(TOMJOB, CIBJOB,RESTOM,hWndMaitre?hWndMaitre:hWnd);
	m_Tom.LierSource();
	m_EtatTom=2; // calcul des couvertures

	if(!m_Tom.m_fBreakDetected)m_Tom.EvaluerPlan();
	if(!m_Tom.m_fBreakDetected)m_Tom.EcritureRestom();
	m_Tom.LibereStructTomjob();

	if(m_Tom.m_fBreakDetected){
		//if(hWndMaitre==0)AfficheErreur("Evaluation BREAKED !");	
	}
	else {
		if(hWndMaitre>0){
#ifndef OPTION_CARAT
			::PostMessage(hWndMaitre,MSGTOM_RESTOM_READY,0,0L);
#endif
//			CString txt;
//			txt.Format("MSGTOM_RESTOM_READY->%d",hWndMaitre);
//			AfficheMessage(txt);
		}
		//else AfficheErreur("L'évaluation est terminée !!!!!");	
	}
	if(m_Tom.m_fBreakDetected==2)PostMessage(hWnd,MSGTOM_AUREVOIR,0,0L);
	m_Tom.m_fBreakDetected=0;
	m_EtatTom=0; // ready
	return(0);
}

short CEval::GetNrLien(){
	short r,l,NbLien=m_NrLienAffected.GetSize();

	
	for(r=1;;r++){
		for(l=0;l<NbLien;l++){
			// ce numéro est-il utilisé ?
			if(m_NrLienAffected[l]==r)break;
		}
		if(l<NbLien)continue;
		m_NrLienAffected.Add(r);
		return(r);
	}
}


CTomjob *CEval::GetTomCDZ(short NrLien,bool flagCreate)
{
	CTomjob* Tom;
	short i;

	// wParam est le numéro du lien.
	for(i=0;i<m_TomCDZ.GetSize();i++){
		if(m_TomCDZ[i]->m_NrLien==NrLien)break;
	}
	if(i<m_TomCDZ.GetSize())Tom=m_TomCDZ[i];
	else Tom=NULL;

	if(flagCreate){
		if(Tom==NULL){
			// création du nouveau lien
			Tom=new CTomjob;
			Tom->m_NrLien=NrLien;
			m_TomCDZ.Add(Tom);
		}
		else {
			// on reset le précédent lien
			Tom->LibereStructTomjob();
		}
	}
	
	return(Tom);
}

CTomjob * CEval::DeleteTomCDZ(short NrLien)
{
	CTomjob* Tom;
	short i;

	// wParam est le numéro du lien.
	for(i=0;i<m_TomCDZ.GetSize();i++){
		Tom=m_TomCDZ[i];
		if(Tom->m_NrLien==NrLien){
			m_TomCDZ.RemoveAt(i);
			return(Tom);
		}
	}

	return(NULL);
}

void CEval::DeleteLien(short NrLien)
{
	short i;

	for(i=0;i<m_NrLienAffected.GetSize();i++){
		if(m_NrLienAffected[i]==NrLien){
			m_NrLienAffected.RemoveAt(i);
			return;
		}
	}
}

void CEval::VerifieLeLien(short NrLien)
{
	short i;

	for(i=0;i<m_NrLienAffected.GetSize();i++){
		if(m_NrLienAffected[i]==NrLien)	return;
	}
	//AfficheErreur("Lien non trouvé");
	m_NrLienAffected.Add(NrLien);
}



short CEval::LanceEvalMarginal(short NrLien,HWND hWndMaitre,LPCSTR TOMJOB,LPCSTR RESTOM,long Option)
{
//	CStdioFile TraceFile;
//	TraceFile.Open("Tracejob.txt",CFile::modeNoTruncate   |CFile::modeCreate   | CFile::modeWrite | CFile::typeText   , NULL )  ;
//	TraceFile.SeekToEnd();
	CTomjob* Tom;

	// le lien existe-il ?
//	TraceFile.WriteString("VerifieLeLien...\n");
	VerifieLeLien(NrLien);
//	TraceFile.WriteString("VerifieLeLien:OK\n");

	// wParam est le numéro du lien recherché
	Tom=GetTomCDZ(NrLien,1);

	// c'est parti pour une évaluation...
//	Tom->m_TraceFile.Open("Tracejob.txt",CFile::modeNoTruncate   |CFile::modeCreate   | CFile::modeWrite | CFile::typeText   , NULL )  ;
//	Tom->m_TraceFile.SeekToEnd();

//	Tom->m_TraceFile.WriteString("AlloueStructTomjob...\n");
	Tom->AlloueStructTomjob(1,Option==1);
//	Tom->m_TraceFile.WriteString("AlloueStructTomjob:OK\n");
	Tom->LectureTomjob(TOMJOB,"CIBJOB",RESTOM,hWndMaitre,1);
//	Tom->m_TraceFile.WriteString("LectureTomjob:OK\n");
	Tom->LierSource();
//	Tom->m_TraceFile.WriteString("LierSource:OK\n");
	Tom->EvaluerPlan();
//	Tom->m_TraceFile.WriteString("EvaluerPlan:OK\n");
	long ret=Tom->EcritureRestom();
//	Tom->m_TraceFile.WriteString("EcritureRestom():OK\n");
//	Tom->m_TraceFile.Close();
//	Tom->LibereStructTomjob();
#ifdef OPTION_CHECKUP
	// spécifique pour Checkup :
	// bien que l'on est en train de traiter un SendMessage() 
	// cela simplifie le traitement pour frédéric
	if(hWndMaitre>0)::PostMessage(hWndMaitre,MSGTOM_RESTOM_READY,0,0L);
#endif
	ReplyMessage(ret);
	return(0);
}

short CEval::LanceEvalMarginal(CTomjob* Tom,bool fCalculMonoSource)
{
	if(fCalculMonoSource){
		Tom->EvaluerPlanMarginal(0,1);
		return(0);
	}

	Tom->LierSource();
	Tom->EvaluerPlan();
	return(0);
}


// wParam est le numéro du lien recherché
short CEval::TermineEvalMarginal(short NrLien)
{
	// wParam est le numéro du lien recherché
	CTomjob* Tom;
	Tom=DeleteTomCDZ(NrLien);
	DeleteLien(NrLien);
	if(Tom==NULL){
//		AfficheErreur("Erreur:Le job de ce lien n'est pas trouvé !");
		ReplyMessage(0);
		return(1);
	}
	Tom->LibereStructTomjob();
	delete Tom;
	ReplyMessage(1);
	return(0);
}

// wParam est le numéro du lien recherché
short CEval::GetRestom(short NrLien)
{
	CTomjob* Tom;

	Tom=GetTomCDZ(NrLien);
	if(Tom==NULL){
		ReplyMessage(0);
		return(0);
	}

	Tom->EcritureRestom();
	ReplyMessage(1);
	return(0);
}


short CEval::AjouteSpotMarginal(short NrLien)
{
	CTomjob* Tom;

	// wParam est le numéro du lien recherché
	Tom=GetTomCDZ(NrLien);
	if(Tom==NULL)return(1);

	// c'est parti pour une évaluation...
	Tom->LectureTomjobMarginal(0);
	if(Tom->InsertNouveauSpot(0))return(1);
	Tom->EvaluerPlanMarginal(0);
	Tom->EcritureRestom();
	return(0);
}

short CEval::AjouteSpotZoom(short NrLien)
{
	CTomjob* Tom;

	// wParam est le numéro du lien recherché
	Tom=GetTomCDZ(NrLien);
	if(Tom==NULL)return(1);

	// c'est parti pour une évaluation...
	Tom->DebutZoom();
	Tom->LectureTomjobMarginal(1);
	Tom->CopieSourceZoom();
	if(Tom->InsertNouveauSpot(1))return(1);
	Tom->EvaluerPlanMarginal(1);
	Tom->EcritureRestom(1);
	Tom->FinZoom();
	return(0);
}

void CEval::BreakCalcul(){
	
	m_Tom.m_fBreakDetected=1;
}

// retourne 1 si on peut fermer l'app
short CEval::TerminerPossible(){
	if(!m_EtatTom)return(1);
	m_Tom.m_fBreakDetected=2;
	return(0);
}

int CEval::GetNbLienAffecte()
{
	return(m_NrLienAffected.GetSize());

}
