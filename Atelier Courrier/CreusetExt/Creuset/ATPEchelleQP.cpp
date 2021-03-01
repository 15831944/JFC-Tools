// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelleQP.h"
#include "resource.h"
#include "MsgPrvCreuset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur
IMPLEMENT_DYNCREATE(CATPEchelleQP, JFCSplitterPane)

CATPEchelleQP::CATPEchelleQP()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelleQP::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
	m_B1.SetDocument(pATPDocCreuset);
	m_B2.SetDocument(pATPDocCreuset);
	//m_C1.SetDocument(pATPDocCreuset);
	//m_C2.SetDocument(pATPDocCreuset);
	m_D1.SetDocument(pATPDocCreuset);
	m_D2.SetDocument(pATPDocCreuset);
	m_T1.SetDocument(pATPDocCreuset);

	m_F1.SetDocument(pATPDocCreuset);
	m_F2.SetDocument(pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelleQP::Update()
{
	// on met à jour les composants
	m_A1.Update();
	m_A2.Update();
	m_B1.Update();
	m_B2.Update();
	//m_C1.Update();
	//m_C2.Update();
	m_D1.Update();
	m_D2.Update();
	m_T1.Update();

	m_F1.Update();
	m_F2.Update();

	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelleQP::~CATPEchelleQP()
{
	// on ne fait rien
}

///////////////////////////////////////////////////////////////////
// les fonctions pour récupérer les dimensions minimales du panneau

LONG CATPEchelleQP::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPEchelle_Metriques::CX_Ax() +  CATPEchelle_Metriques::CX_Bx() + 2*CATPEchelle_Metriques::CX_Cx() + CATPEchelle_Metriques::CX_Dx() + CATPEchelle_Metriques::CX_VS();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = (2 * CATPEchelle_Metriques::CX_Border()) + (5 * CATPEchelle_Metriques::CX_Interstice());

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPEchelleQP::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPEchelle_Metriques::CY_T1() + CATPEchelle_Metriques::CY_x1() + CATPEchelle_Metriques::CY_x2() + CATPEchelle_Metriques::CY_x4();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = (2 * CATPEchelle_Metriques::CY_Border()) + (3 * CATPEchelle_Metriques::CY_Interstice());

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPEchelleQP, JFCSplitterPane)
	//{{AFX_MSG_MAP(CATPEchelleQP)
	ON_WM_CREATE()
	ON_BN_CLICKED(MSG_CR_TOUS,		OnBtnTous)
	ON_BN_CLICKED(MSG_CR_AUCUN,		OnBtnAucun)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelleQP::OnUpdate()
{
	// on met à jour le composant
	//if (m_pATPDocCreuset != 0)
	//	this->ShowWindow((m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult()) ? SW_SHOW : SW_HIDE);
}

//////////////////////////////////
// CATPEchelleQP message handlers

int CATPEchelleQP::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCSplitterPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants
	LONG x1_T1 = CATPEchelle_Metriques::CX_Border();				LONG x2_T1 = x1_T1 + CATPEchelle_Metriques::CX_Ax() + CATPEchelle_Metriques::CX_Bx() + 2*CATPEchelle_Metriques::CX_Cx() + CATPEchelle_Metriques::CX_Dx() + (4 * CATPEchelle_Metriques::CX_Interstice());
	LONG y1_T1 = CATPEchelle_Metriques::CY_Border();				LONG y2_T1 = y1_T1 + CATPEchelle_Metriques::CY_T1();
	LONG x1_A1 = x1_T1;                                             LONG x2_A1 = x1_A1 + CATPEchelle_Metriques::CX_Ax();
	LONG y1_A1 = y2_T1 + CATPEchelle_Metriques::CY_Interstice();	LONG y2_A1 = y1_A1 + CATPEchelle_Metriques::CY_x1();
	LONG x1_A2 = x1_A1;                                             LONG x2_A2 = x2_A1;
	LONG y1_A2 = y2_A1 + CATPEchelle_Metriques::CY_Interstice();	LONG y2_A2 = y1_A2 + CATPEchelle_Metriques::CY_x2();

	LONG x1_B1 = x2_A1 + CATPEchelle_Metriques::CX_Interstice();	LONG x2_B1 = x1_B1 + CATPEchelle_Metriques::CX_Bx();
	LONG y1_B1 = y1_A1;                                             LONG y2_B1 = y2_A1;
	LONG x1_B2 = x1_B1;                                             LONG x2_B2 = x2_B1;
	LONG y1_B2 = y1_A2;                                             LONG y2_B2 = y2_A2;

	LONG x1_C1 = x2_B1 + CATPEchelle_Metriques::CX_Interstice();	LONG x2_C1 = x1_C1 + CATPEchelle_Metriques::CX_Cx();
	LONG y1_C1 = y1_B1;                                             LONG y2_C1 = y2_B1;
	LONG x1_C2 = x1_C1;                                             LONG x2_C2 = x2_C1;
	LONG y1_C2 = y1_B2;                                             LONG y2_C2 = y2_B2;

	LONG x1_F1 = x2_C1 + CATPEchelle_Metriques::CX_Interstice();	LONG x2_F1 = x1_F1 + CATPEchelle_Metriques::CX_Cx();
	LONG y1_F1 = y1_C1;                                             LONG y2_F1 = y2_C1;
	LONG x1_F2 = x1_F1;                                             LONG x2_F2 = x2_F1;
	LONG y1_F2 = y1_C2;                                             LONG y2_F2 = y2_C2;

	LONG x1_D1 = x2_F1 + CATPEchelle_Metriques::CX_Interstice();	LONG x2_D1 = x1_D1 + CATPEchelle_Metriques::CX_Dx();
	LONG y1_D1 = y1_F1;                                             LONG y2_D1 = y2_F1;
	LONG x1_D2 = x1_D1;                                             LONG x2_D2 = x2_D1;
	LONG y1_D2 = y1_F2;                                             LONG y2_D2 = y2_F2;

	// Bouton "Tous" et "Aucun"
	LONG x1_E1 = x1_A1;												LONG x2_E1 = x1_E1 + CATPEchelle_Metriques::CX_Ex();
	LONG y1_E1 = y2_A2 + CATPEchelle_Metriques::CY_Interstice();    LONG y2_E1 = y1_E1 + CATPEchelle_Metriques::CY_x4();
	LONG x1_E2 = x2_E1 + CATPEchelle_Metriques::CX_Interstice();    LONG x2_E2 = x1_E2 + CATPEchelle_Metriques::CX_Ex();
	LONG y1_E2 = y1_E1;                                             LONG y2_E2 = y2_E1;

	LONG x1_HS = x1_F2;                                             LONG x2_HS = x2_D2;
	LONG y1_HS = y2_D2 + CATPEchelle_Metriques::CY_Interstice();	LONG y2_HS = y1_HS + CATPEchelle_Metriques::CY_HS();
//	LONG y1_HS = y2_D2 + CATPEchelle_Metriques::CY_Interstice()+ (CATPEchelle_Metriques::CY_x4()-CATPEchelle_Metriques::CY_HS())/2;	LONG y2_HS = y1_HS + CATPEchelle_Metriques::CY_HS();
	LONG x1_VS = x2_D2 + CATPEchelle_Metriques::CX_Interstice();	LONG x2_VS = x1_VS + CATPEchelle_Metriques::CX_VS();
	LONG y1_VS = y1_D2;                                             LONG y2_VS = y2_D2;

	// on crée les composants A1 à T1
	if (m_T1.Create(NULL, WS_VISIBLE, CRect(x1_T1, y1_T1, x2_T1, y2_T1), this, 0) == FALSE) return (-1);
	if (m_A1.Create(NULL, WS_VISIBLE, CRect(x1_A1, y1_A1, x2_A1, y2_A1), this, 0) == FALSE) return (-1);
	if (m_A2.Create(NULL, WS_VISIBLE, CRect(x1_A2, y1_A2, x2_A2, y2_A2), this, 0) == FALSE) return (-1);
	if (m_B1.Create(NULL, WS_VISIBLE, CRect(x1_B1, y1_B1, x2_B1, y2_B1), this, 0) == FALSE) return (-1);
	if (m_B2.Create(NULL, WS_VISIBLE, CRect(x1_B2, y1_B2, x2_B2, y2_B2), this, 0) == FALSE) return (-1);
	//if (m_C1.Create(NULL, WS_VISIBLE, CRect(x1_C1, y1_C1, x2_C1, y2_C1), this, 0) == FALSE) return (-1);
	//if (m_C2.Create(NULL, WS_VISIBLE, CRect(x1_C2, y1_C2, x2_C2, y2_C2), this, 0) == FALSE) return (-1);
	//if (m_F1.Create(NULL, WS_VISIBLE, CRect(x1_F1, y1_F1, x2_F1, y2_F1), this, 0) == FALSE) return (-1);
	//if (m_F2.Create(NULL, WS_VISIBLE, CRect(x1_F2, y1_F2, x2_F2, y2_F2), this, 0) == FALSE) return (-1);
	//if (m_D1.Create(NULL, WS_VISIBLE, CRect(x1_D1, y1_D1, x2_D1, y2_D1), this, 0) == FALSE) return (-1);
	//if (m_D2.Create(NULL, WS_VISIBLE, CRect(x1_D2, y1_D2, x2_D2, y2_D2), this, 0) == FALSE) return (-1);
	if (m_F1.Create(NULL, WS_VISIBLE, CRect(x1_C1, y1_C1, x2_C1, y2_C1), this, 0) == FALSE) return (-1);
	if (m_F2.Create(NULL, WS_VISIBLE, CRect(x1_C2, y1_C2, x2_C2, y2_C2), this, 0) == FALSE) return (-1);
	if (m_D1.Create(NULL, WS_VISIBLE, CRect(x1_F1, y1_D1, x2_D1, y2_D1), this, 0) == FALSE) return (-1);
	if (m_D2.Create(NULL, WS_VISIBLE, CRect(x1_F2, y1_D2, x2_D2, y2_D2), this, 0) == FALSE) return (-1);

	// Bouton "Tous" et "Aucun"
	CString txt;
	txt.LoadString(IDS_CR_TOUS);
	if (m_E1.Create(txt, WS_CHILD|WS_VISIBLE, CRect(x1_E1, y1_E1, x2_E1, y2_E1), this, MSG_CR_TOUS) == FALSE) return (-1);
	txt.LoadString(IDS_CR_AUCUN);
	if (m_E2.Create(txt, WS_CHILD|WS_VISIBLE, CRect(x1_E2, y1_E2, x2_E2, y2_E2), this, MSG_CR_AUCUN) == FALSE) return (-1);

	// on crée les composants HS et VS
	if (m_HS.Create(WS_VISIBLE | SBS_HORZ, CRect(x1_HS, y1_HS, x2_HS, y2_HS), this, 0) == FALSE) return (-1);
	if (m_VS.Create(WS_VISIBLE | SBS_VERT, CRect(x1_VS, y1_VS, x2_VS, y2_VS), this, 0) == FALSE) return (-1);

	// on configure les déplacements
	this->SetMode(&m_T1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_A1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_A2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_B1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_B2, MGER_NONE, MGER_SIZE);
	//this->SetMode(&m_C1, MGER_NONE, MGER_NONE);
	//this->SetMode(&m_C2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_F1, MGER_NONE, MGER_NONE);
	this->SetMode(&m_F2, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_D1, MGER_SIZE, MGER_NONE);
	this->SetMode(&m_D2, MGER_SIZE, MGER_SIZE);

	// Bouton "Tous" et "Aucun"
	this->SetMode(&m_E1, MGER_NONE, MGER_MOVE);
	this->SetMode(&m_E2, MGER_NONE, MGER_MOVE);

	this->SetMode(&m_HS, MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_VS, MGER_MOVE, MGER_SIZE);

	// on configure les ascenseurs
	this->SetScrollBar(&m_A2,  NULL, &m_VS);
	this->SetScrollBar(&m_B2,  NULL, &m_VS);
//	this->SetScrollBar(&m_C2,  NULL, &m_VS);
	this->SetScrollBar(&m_F2,  NULL, &m_VS);
	this->SetScrollBar(&m_D1, &m_HS,  NULL);
	this->SetScrollBar(&m_D2, &m_HS, &m_VS);

	// on lie les ascenseurs
	m_A2.SetVertScroll(&m_VS, TRUE);
	m_B2.SetVertScroll(&m_VS, FALSE);
//	m_C2.SetVertScroll(&m_VS, FALSE);
	m_F2.SetVertScroll(&m_VS, FALSE);
	m_D1.SetHorzScroll(&m_HS, TRUE);
	m_D2.SetVertScroll(&m_VS, FALSE);
	m_D2.SetHorzScroll(&m_HS, FALSE);

	SetColors();
	// on quitte
	return (0);
}

void CATPEchelleQP::SetColors()
{
	// m_T1.SetBorderColor(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	m_E1.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E1.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E1.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E1.SetFontName(CATPColors::GetFontStaticName());
	m_E1.SetFontSize(CATPColors::GetFontStaticSize());
	m_E1.SetFontBold(0);

	m_E2.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E2.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E2.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_E2.SetFontName(CATPColors::GetFontStaticName());
	m_E2.SetFontSize(CATPColors::GetFontStaticSize());
	m_E2.SetFontBold(0);

	Invalidate();
}

void CATPEchelleQP::OnBtnTous()
{
	// Invalide en mode plan étendu
	if (m_pATPDocCreuset->m_AFFPlanEtendu)
		return; 

	// Flag si au moins 1 ajout support dans plateau
	JBool AjoutSupport = false;

	// Validation de tous les élmts de l'offre pour le plateau
	for (JInt32 InxElmt = 0; InxElmt < m_pATPDocCreuset->m_EchelleSupports.GetCount();InxElmt++) 
	{		
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(InxElmt);

		CKeySupport KeySup;
		KeySup.m_IdSupport = IdSupport;

		// on recherche le Support dans le plateau
		JInt32 IdxSupport = m_pATPDocCreuset->m_PlateauSupports.FindItem(KeySup);

		// on teste si support non présent dans plateau
		if (IdxSupport < 0)
		{
			// on teste les exceptions
			try
			{
				// on ajoute le Support au plateau
				m_pATPDocCreuset->m_PlateauSupports.Add(KeySup);
				AjoutSupport = true;
			}
			catch (JException* pException)
			{
				// on affiche le message
				::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
			}
		}
	}

	if (AjoutSupport)
	{
		// on positionne l'indicateur de modification
		m_pATPDocCreuset->SetModifiedFlag(TRUE);

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
	}
}

void CATPEchelleQP::OnBtnAucun()
{
	// Invalide en mode plan étendu
	if (m_pATPDocCreuset->m_AFFPlanEtendu)
		return; 
	// Flag si au moins 1 suppression support dans plateau
	JBool SuppressionSupport = false;

	// On test déjà si certains supports qui appartiennent au plateau ont des insertions
	JBool SuppPlateauWithInsert = false;	

	// Suppression de tous les élmts de l'offre pour le plateau
	for (JInt32 InxElmt = 0; InxElmt < m_pATPDocCreuset->m_EchelleSupports.GetCount();InxElmt++) 
	{
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(InxElmt);

		CKeySupport KeySup;
		KeySup.m_IdSupport = IdSupport;

		// on recherche le Support dans le plateau
		JInt32 IdxSupport = m_pATPDocCreuset->m_PlateauSupports.FindItem(KeySup);

		// on teste si on a trouvé le Support
		if (IdxSupport >= 0)
		{
			// on récupère le nombre d'insertions du Support sur période action
			JInt32 NbInsertions = this->OnGetNbInsertionsPA(IdSupport);

			// on teste si le Support porte des insertions
			if (NbInsertions != 0)
			{
				SuppPlateauWithInsert = true;
			}
		}
	}		

	if (SuppPlateauWithInsert)
	{
		// Validation si insertion de la suppression
		SuppPlateauWithInsert = false;

		// effacement après confirmation utilisateur
		CString ttl, qst;
		ttl.LoadString(IDR_CR_ATPFRAME);
		qst.LoadString(IDS_CR_SUPPR_SUPPORTECH);
		if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_YESNO) == IDYES)
			SuppPlateauWithInsert = true;
		else
			return;
	}
	else
		SuppPlateauWithInsert = true;

	// Suppression de tous les élmts de l'offre pour le plateau
	if (SuppPlateauWithInsert)
	{
		for (JInt32 InxElmt = 0; InxElmt < m_pATPDocCreuset->m_EchelleSupports.GetCount();InxElmt++) 
		{
			// on récupère l'identifiant du Support
			const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(InxElmt);

			CKeySupport KeySup;
			KeySup.m_IdSupport = IdSupport;

			// on recherche le Support dans le plateau
			JInt32 IdxSupport = m_pATPDocCreuset->m_PlateauSupports.FindItem(KeySup);

			// on teste si on a trouvé le Support
			if (IdxSupport >= 0)
			{
				// on supprime toutes les insertions de ce support sur toutes les hypotheses
				SuppInsertionSupport(IdxSupport);

				// on teste l'affichage du zoom
				if (m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
					// on met à jour l'affichage du zoom
					m_pATPDocCreuset->m_AFFZoomPlan.Reset();

				// on récupère le nombre d'insertions restantes sur toute l'étude
				//JInt32 NbInsReste = this->OnGetNbInsertionsEtude(IdSupport);

				//if (NbInsReste == 0)
				{
					// on supprime le support du plateau (s'il ne reste plus d'insertion sur toute l'étude)
					m_pATPDocCreuset->m_PlateauSupports.Remove(IdxSupport);
					SuppressionSupport = true;
				}

				// on positionne l'indicateur de modification
				m_pATPDocCreuset->SetModifiedFlag(TRUE);

				// on met à jour les vues du document
				//m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
			}
		}		

		if (SuppPlateauWithInsert || SuppressionSupport)
		{
			// on positionne l'indicateur de modification
			m_pATPDocCreuset->SetModifiedFlag(TRUE);

			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
		}		
	}
}

JInt32 CATPEchelleQP::OnGetNbInsertions(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// on boucle sur toutes les hypothèses
	for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		// on boucle sur toutes les insertions
		for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
		{
			// on récupère l'insertion courante
			const CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

			// on met à jour le compteur
			if (pInsertion->m_IdSupport == IdSupport)
				NbInsertions += 1;
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'insertions d'un Support sur la période d'action uniquement
JInt32 CATPEchelleQP::OnGetNbInsertionsPA(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// on boucle sur toutes les hypothèses
	for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		if (PLNInsertions.GetCount())
		{
			// on crée la clé de l'insertion
			CInsKey Key;
			Key.m_IdSupport 	=   IdSupport;
			Key.m_IdParent		=	0; 
			Key.m_DateD			=	m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
			Key.m_IdFormat		=	0;

			// on cherche insertion à partir du début période étude (si mode hypothèses)
			if (!m_pATPDocCreuset->m_AFFPlanEtendu)
			{
				if(PLNInsertions.FindPAInsertions(Key,
												m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
  												m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
				{
					// 1 insertion trouvé pour ce support
					NbInsertions++;
				}
			}
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

////////////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer le nombre d'insertions d'un Support sur toute l'étude
JInt32 CATPEchelleQP::OnGetNbInsertionsEtude(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// Voir si support sur ce plateau avec des insertions en dehors période action
	if (m_pATPDocCreuset->m_MasterPlan.GetCount())
	{
		// on crée la clé de l'insertion
		CInsKey Key;
		Key.m_IdSupport 	=   IdSupport;
		Key.m_IdParent		=	0; 
		Key.m_DateD			=	m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		Key.m_IdFormat		=	0;

		// on cherche insertion à partir du début période étude (si mode hypothèses)
		if (!m_pATPDocCreuset->m_AFFPlanEtendu)
		{
			if(m_pATPDocCreuset->m_MasterPlan.FindPAInsertions(Key,
											                   m_pATPDocCreuset->m_KEYEtude.GetDateD(),
  											                   m_pATPDocCreuset->m_KEYEtude.GetDateF()))
			{
				// 1 insertion trouvé pour ce support
				NbInsertions++;
			}
		}
	}

	return NbInsertions;
}

JVoid CATPEchelleQP::SuppInsertionSupport(JUnt32 IdxSupport)
{
	// En mode Plan étendu, non actif
	if (!m_pATPDocCreuset->m_AFFPlanEtendu)
	{
		// Suppression sur la période d'action ???? A VOIR
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// on initialise le code
		JInt32 Ret = 0;

		if (IdxSupport >= 0)
		{
			// Récupère le support
			CKeySupport KeySup;
			KeySup = m_pATPDocCreuset->m_PlateauSupports.Item(IdxSupport);

			for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst();
				m_pATPDocCreuset->m_TblHypotheses.IsValid();
				m_pATPDocCreuset->m_TblHypotheses.MoveNext()) 
			{

				if (!KeySup.m_IdParent)
				{
					// Clé insertion
					CInsKeySFD KeySFD;

					// on récupère le plan de l'hypothèse courante
					CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();
					CPlanInsertions Tmp;

					// Nb insertions
					if (PLNInsertions.GetCount())
					{

						// balayage des insertions de cette hypothèse
						for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD();PLNInsertions.MoveNext_SFD())
						{
							// on récupère l'insertion
							CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

							// on teste si l'insertion correspond à notre demande
							if (pInsertion->m_IdSupport	== KeySup.m_IdSupport &&
								pInsertion->m_IdParent	== KeySup.m_IdParent)
							{
								/*
								// Suppression de toutes les insertions à cet endroit 
								while (pInsertion->m_IdSupport == KeySup.m_IdSupport && 
									pInsertion->m_IdParent  == KeySup.m_IdParent  &&
  									pInsertion->HasInsertions()) 
								{
									// on enlève insertion une par une  (à optimiser / A FAIRE) 
									PLNInsertions.DeleteInsertion_SFD(DateD,DateF);
								}
								*/
								// rien à faire
							}
							else
							{
								// Insertion non supprimée , on ajoute au map insertion tmp
								Tmp.AddInsertion(*pInsertion,DateD,DateF);
							}
						}

						// Puis on remet en place la mise à jour plan insertions
						PLNInsertions.Swap(Tmp); 
					}
				}
			}
		}
	}
}
