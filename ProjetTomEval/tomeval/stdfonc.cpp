// stdfonc.cpp
#include "stdafx.h"
#include <afxdisp.h>
#include <iostream>
#include <fstream>

using namespace std;


static bool fMessage=true;
int CheckDate(void){
	COleDateTime Djour,Dfin;
	COleDateTimeSpan Duree;
	int NbJour;
	Dfin.SetDate DEADLINE;
	Djour=COleDateTime::GetCurrentTime();
	Duree=Djour-Dfin;
	NbJour=Duree.GetDays();
	if(NbJour<-15)return(1); // *********** AUTORISATION *****************************		
	else return (1);		// Liem 05/05/04 : Il n'y a plus de date validité DEADLINE

	// on va jouer...
	if(NbJour<0 && fMessage==true){
		if(!(rand()%(40-(15+NbJour)))){
			//MessageBox(NULL,"Attention! La licence du moteur arrive à échéance dans quelques jours.\nVeuillez contacter votre prestataire pour vous procurer une nouvelle licence.","Copyright JFC 1998",MB_ICONASTERISK|MB_SYSTEMMODAL);
			fMessage=false;
		}
		return(1);
	}
	if(NbJour<15){
		if(!(rand()%10)){
		//	MessageBox(NULL,"La durée de la licence de ce logiciel est dépassée!\nDans quelques jours vous ne pourrez plus utiliser le moteur.\nVeuillez contacter votre prestataire pour vous procurer une nouvelle licence.","Copyright JFC 1998",MB_ICONWARNING|MB_SYSTEMMODAL);
		}
		return(1);
	}
	//MessageBox(NULL,"La durée de la licence de ce logiciel est dépassée depuis plus de 15 jours!\nL'utilisation de ce logiciel est donc interdite.\nVeuillez contacter votre prestataire pour vous procurer une nouvelle licence.","Copyright JFC 1998",MB_ICONERROR|MB_SYSTEMMODAL);
	return(0);
}

void AfficheMessage(LPCSTR str)
{
	//HWND  hWnd;

	//hWnd = GetActiveWindow();
//	MessageBox(hWnd, str, "Information",MB_OK | MB_ICONINFORMATION|MB_SYSTEMMODAL);
//	_asm{int 3};
}

void AfficheErreur(LPCSTR  str)
{
	
	//HWND  hWnd;

	ofstream lout("tomeval.err", ios::app);
	lout << str << endl;
	lout.close();

	//hWnd = GetActiveWindow();
	//MessageBox(hWnd, str,"Erreur (TomEval)",MB_OK | MB_ICONHAND|MB_SYSTEMMODAL);
//	_asm{int 3};
}
