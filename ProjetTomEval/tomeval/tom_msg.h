// Dialogue avec l'app TOMEVAL
// le hWnd du APP_MERE est re�u dans la ligne de param�tre

#define MSGTOM_TOMJOB_HIDE WM_USER+2002


// TOMEVAL->APP_MERE lParam contient le hWnd
#define MSGTOM_BONJOUR WM_USER+2000

// APP_MERE->TOMEVAL 
#define MSGTOM_TOMJOB_READY WM_USER+2001

// TOMEVAL->APP_MERE
// wParam 0 : OK
// wParam 1 : Chargement de vague en cours
// wParam 2 : Calcul en cours
// wParam 3 : Requette incorrecte
#define MSGTOM_RESTOM_READY WM_USER+2003

// APP_MERE->TOMEVAL 
#define MSGTOM_AUREVOIR WM_USER+2006

// APP_MERE->TOMEVAL 
#define MSGTOM_BREAKCALCUL WM_USER+2007

// TOMEVAL->APP_MERE 
// wParam = NrSouPlan
#define MSGTOM_INFO_PROGRESS WM_USER+2008

// TOMEVAL->APP_MERE 
// wParam = position du chargement en %
#define MSGTOM_INFO_LOADHABITUDE WM_USER+2009



// wParam=hwndMaitre du lien
// retour dans la valeur du replymessage
#define MSGTOM_GETLIEN WM_USER+2010

// en instantann�e (Toujours en SendMessage)
// APP_MERE->TOMEVAL 
// wparam NrLien
#define MSGTOM_TOMCDZ_READY WM_USER+2011
#define MSGTOM_TOMCDZ_END WM_USER+2012
#define MSGTOM_TOMCDZ_GETRESTOM WM_USER+2014

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDSPOT WM_USER+2013

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDZOOM WM_USER+2015







