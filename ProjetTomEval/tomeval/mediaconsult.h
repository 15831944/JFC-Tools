// Dialogue avec l'app TOMEVAL
// le hWnd du APP_MERE est reçu dans la ligne de paramètre

#define MSGTOM_TOMJOB_HIDE WM_USER+4002


// TOMEVAL->APP_MERE lParam contient le hWnd
#define MSGTOM_BONJOUR WM_USER+4000

// APP_MERE->TOMEVAL 
//wParam -> HWND de APP_MERE
#define MSGTOM_TOMJOB_READY WM_USER+4001

// TOMEVAL->APP_MERE
// wParam 0 : OK
// wParam 1 : Chargement de vague en cours
// wParam 2 : Calcul en cours
// wParam 3 : Requette incorrecte
#define MSGTOM_RESTOM_READY WM_USER+4003

// APP_MERE->TOMEVAL 
#define MSGTOM_AUREVOIR WM_USER+4006

// APP_MERE->TOMEVAL 
#define MSGTOM_BREAKCALCUL WM_USER+4007

// TOMEVAL->APP_MERE 
// wParam = NrSouPlan
#define MSGTOM_INFO_PROGRESS WM_USER+4008

// TOMEVAL->APP_MERE 
// wParam = position du chargement en %
#define MSGTOM_INFO_LOADHABITUDE WM_USER+4009



// wParam=hwndMaitre du lien
// retour dans la valeur du replymessage
#define MSGTOM_GETLIEN WM_USER+4010

// en instantannée (Toujours en SendMessage)
// APP_MERE->TOMEVAL 
// wparam NrLien
#define MSGTOM_TOMCDZ_READY WM_USER+4011
#define MSGTOM_TOMCDZ_END WM_USER+4012
#define MSGTOM_TOMCDZ_GETRESTOM WM_USER+4014

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDSPOT WM_USER+4013

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDZOOM WM_USER+4015







