// Dialogue avec l'app TOMEVAL
// le hWnd du APP_MERE est reçu dans la ligne de paramètre



// TOMEVAL->APP_MERE lParam contient le hWnd
#define MSGTOM_BONJOUR 3124

#define MSGTOM_TOMJOB_HIDE 3126

// APP_MERE->TOMEVAL 
#define MSGTOM_TOMJOB_READY 3125

// TOMEVAL->APP_MERE
// wParam 0 : OK
// wParam 1 : Chargement de vague en cours
// wParam 2 : Calcul en cours
// wParam 3 : Requette incorrecte
#define MSGTOM_RESTOM_READY 3127

// APP_MERE->TOMEVAL 
#define MSGTOM_AUREVOIR 3130

// APP_MERE->TOMEVAL 
#define MSGTOM_BREAKCALCUL 3131

// TOMEVAL->APP_MERE 
// wParam = NrSouPlan
#define MSGTOM_INFO_PROGRESS 3132

// TOMEVAL->APP_MERE 
// wParam = position du chargement en %
#define MSGTOM_INFO_LOADHABITUDE 3133



// wParam=hwndMaitre du lien
// retour dans la valeur du replymessage
#define MSGTOM_GETLIEN 3134

// en instantannée (Toujours en SendMessage)
// APP_MERE->TOMEVAL 
// wparam NrLien
#define MSGTOM_TOMCDZ_READY 3135
#define MSGTOM_TOMCDZ_END 3136
#define MSGTOM_TOMCDZ_GETRESTOM 3138

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDSPOT 3137

// pour le marginal: (Toujours en SendMessage)
#define MSGTOM_ADDZOOM 3139







