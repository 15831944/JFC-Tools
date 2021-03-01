// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPZoomPlan.h"
#include ".\atpzoomplan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "Courbes.h"
//////////////////
// le constructeur

CATPZoomPlan::CATPZoomPlan(CWnd* pParent) : CDialog(CATPZoomPlan::IDD, pParent)
{
	// on initialise le document
	m_pATPDocCreuset = 0;

	m_partBubget = -1.0;
	m_prix = -1.0;
	m_CoefAlpha = -1.0;
	m_partXGRP = -1.0;
	m_pointsXGRP = -1.0;
	m_ApportCouv = -1.0;
	m_ApportRepet = -1.0;
	m_ApportCouvMem = -1.0;

	m_CtGRP = -1.0;
	m_CtInd = -1.0;
	m_CtCts = -1.0;

	m_Numero = 0;

	m_A1.SetParent(this);
	m_A2.SetParent(this);
	m_A3.SetParent(this);

	//{{AFX_DATA_INIT(CATPZoomPlan)
	//}}AFX_DATA_INIT
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPZoomPlan::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on définit le document des composants
	m_A1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);
	m_A3.SetDocument(pATPDocCreuset);

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPZoomPlan::Update()
{
	//si visib on calcul
	m_partBubget = -1.0;
	m_prix = -1.0;
	m_CoefAlpha = -1.0;
	m_partXGRP = -1.0;
	m_pointsXGRP = -1.0;
	m_ApportCouv = -1.0;
	m_ApportRepet = -1.0;
	m_ApportCouvMem = -1.0;

	m_CtGRP = -1.0;
	m_CtInd = -1.0;
	m_CtCts = -1.0;

	m_Numero = 0;

	m_dateD.Reset();

	if (m_pATPDocCreuset && m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
		CalculInfos();


	// on met à jour les composants
	m_A1.Update();
	m_A2.Update();
	m_A3.Update();

	// on met à jour le composant
	this->OnUpdate();
}
void CATPZoomPlan::CalculInfos()
{
	JDate dateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	JDate dateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

	//on se déplace su la bonne hypo
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_AFFZoomPlan.GetIdxHypothese());
	m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_AFFZoomPlan.GetIdxHypothese());


	// on vérifie la validité des identifiants et de la date
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && m_pATPDocCreuset->m_ITRHypotheses.IsValid())
	{
		// on récupère le plan de l'hypothèse courante
		CPlanInsertions  PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();
		JITPDATHypoFormat & datPlanFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();

		if(! datPlanFormat.IsValid())
		{
			m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
						m_pATPDocCreuset->m_KEYPeriodeActionHyp,
						m_pATPDocCreuset->m_KEYPeriodeActionTablier,
						PLNInsertions, datPlanFormat, true );
		}
		const  JITPDATHypothese & datPlan = datPlanFormat.GetDATHypoFormatActif();


		// on récupère l'insertion du ZoomPlan
		const JList<CInsKey> & ListInsK = m_pATPDocCreuset->m_AFFZoomPlan.GetKeyList();
		
		//on les supprime du plan
		CInsKey InsKey;
		CInsertion * pInsertion = NULL;

		JInt32 nbPeriodAlpha = 1;
		JInt32 diviseur = 100;
		JVector<JInt32> vectAlpha;
		vectAlpha.SetCount(nbPeriodAlpha);
		vectAlpha.MoveFirst();
		vectAlpha.GetItem() = 100;

		bool bfirst = true;
		for(ListInsK.MoveFirst() ; ListInsK.IsValid(); ListInsK.MoveNext())
		{
			InsKey = ListInsK.GetItem();
			pInsertion = PLNInsertions.FindInsertion(InsKey); 
			if (bfirst)
			{
  				IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( pInsertion->m_IdSupport );
				if (pElem->IsTitre())
				{
					CKeyTerrain key;
					m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
					key.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
					m_pATPDocCreuset->m_MoteurPresse.GetCoefAlpha(key, pElem->AsITitre()->GetIDAudience(), nbPeriodAlpha, vectAlpha, diviseur);
				} 
				bfirst = false;
			}
			if (pInsertion)
				PLNInsertions.DeleteInsertion(InsKey, dateD, dateF);
		}
	
		vectAlpha.MoveFirst();
		if(!bfirst)
			m_CoefAlpha =  (JFlt64)vectAlpha.GetItem() / (JFlt64)diviseur *100.0;

		//on calcule les infos du plan
		JITPDATHypoFormat datPlanFormatZoom;
		
		m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
								m_pATPDocCreuset->m_KEYPeriodeActionHyp,
								m_pATPDocCreuset->m_KEYPeriodeActionTablier,
								PLNInsertions, datPlanFormatZoom , true);

		const JITPDATHypothese & datPlanZoom = datPlanFormatZoom.GetDATHypoFormatActif();

		// on calcule les valeurs

		//budget
		if (datPlan.m_Budget != 0.0)
		{
			m_prix = datPlan.m_Budget-datPlanZoom.m_Budget;
			m_partBubget = ((m_prix) * 100.0) / datPlan.m_Budget;
		}

		JFlt64 val = 0.0;
		JFlt64 valZoom = 0.0;

		//X-GRP
		if(datPlanFormat.m_Effectif != 0.0)
		{
			val = (100.0 * datPlan.m_VolLecture) / datPlanFormat.m_Effectif;
			valZoom = (100.0 * datPlanZoom.m_VolLecture) / datPlanFormatZoom.m_Effectif;
			if (val != 0.0)
			{
				m_pointsXGRP = val-valZoom;
				m_partXGRP = (m_pointsXGRP * 100.0) / val;
			}

		}

		val = 0.0;
		valZoom = 0.0;
		//Apport Couverture
		if(datPlanFormat.m_Effectif != 0.0)
		{
			// on récupère la couverture cumulée à 1+
			val = datPlan.m_Couverture;
			valZoom = datPlanZoom.m_Couverture;
		
			if(val>0.0)
				m_ApportCouv = 100 * (val-valZoom)/ datPlanFormat.m_Effectif ;
		}

		
		//Apport Repetition
		val = 0.0;
		valZoom = 0.0;
		// on récupère la couverture cumulée à 1+
		val = datPlan.m_Couverture;
		if(val>0.0)
			val = datPlan.m_VolLecture / val;
		
		valZoom = datPlanZoom.m_Couverture;
		if(valZoom>0.0)
			valZoom = datPlanZoom.m_VolLecture / valZoom;

		if(val>0.0)
			m_ApportRepet = val-valZoom;

		//Apport Couverture mem;
		//m_ApportCouvMem;


		int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
		CCourbes * courbes = CCourbes::GetInstance();
		double DistrFreq;
		double DistrFreqZoom;

		// on récupère le nombre de jours
		JInt32 idxD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD()-1;
		JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		val = 0.0;
		valZoom = 0.0;

		for (int k = 0; k<20; k++)
		{
			if( idxD>=0 )
			{
				if (k == 19)
				{
					DistrFreq = datPlan.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlan.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64();
					DistrFreqZoom = datPlanZoom.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlanZoom.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64();
				}
				if (k < 19)
				{
					DistrFreq = (datPlan.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlan.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64())
					- (datPlan.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64() - datPlan.m_DistributionJour.Item(idxD).Item(k+2).AsJFlt64());
					DistrFreqZoom = (datPlanZoom.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlanZoom.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64())
					- (datPlanZoom.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64() - datPlanZoom.m_DistributionJour.Item(idxD).Item(k+2).AsJFlt64());
				}
			}
			else
			{
				if (k == 19)
				{
					DistrFreq = datPlan.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64();
					DistrFreqZoom = datPlanZoom.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64();
				}
				if (k < 19)
				{
					DistrFreq = datPlan.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlan.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64();
					DistrFreqZoom = datPlanZoom.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - datPlanZoom.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64();
				}
			}

			val += DistrFreq * courbes->GetProba(k+1,beta);
			valZoom += DistrFreqZoom * courbes->GetProba(k+1,beta);
		}
		if(val> 0.0 && datPlanFormat.m_Effectif>0.0)
		{
			val = 100.0 * val / datPlanFormat.m_Effectif;
			valZoom = 100.0 * valZoom / datPlanFormatZoom.m_Effectif;
		}
		if(val>0.0)
			m_ApportCouvMem = val-valZoom;



		//cout GRP
		val = 0.0;
		valZoom = 0.0;
		if (m_prix > 0.0)
		{
			val = datPlan.m_AudienceRef - datPlanZoom.m_AudienceRef;
			//val = datPlanEch.m_Audience;
			if (val != 0.0 && datPlanFormat.m_Effectif != 0.0)
				m_CtGRP = (m_prix * datPlanFormat.m_Effectif) / (100.0 * val);
		}

		// cout/1000Ind
		val = 0.0;
		valZoom = 0.0;
		if (m_prix > 0.0)
		{
			val = datPlan.m_Couverture;
			valZoom = datPlanZoom.m_Couverture;
			
			val = val - valZoom;
			if (val != 0.0)
				m_CtInd = (1000.0 * m_prix) / val;
		}

		//cout /1000Cts
		val = 0.0;
		valZoom = 0.0;
		if (m_prix > 0.0)
		{
			val = datPlan.m_VolLecture - datPlanZoom.m_VolLecture;
			if (val != 0.0 && datPlanFormat.m_Effectif != 0.0)
				m_CtCts =  (1000.0 * m_prix) / val;
		}

		//numero

		
		CString buffer;
		if (CTarifs::GetInstance()->GetNumParution(InsKey.m_IdSupport, InsKey.m_DateD, m_Numero))
			buffer.Format("%s", JSRCPresse::GetInstance().m_TBLTitres.GetLabelByID(InsKey.m_IdSupport));
		else
			buffer = "Zoom";
		if (m_Title.CompareNoCase(buffer)!=0)
			m_Ttl.SetText(buffer);
		m_Title = buffer;

		m_dateD = InsKey.m_DateD;

	}
	
}

void CATPZoomPlan::DoDataExchange(CDataExchange* pDX)
{
	// on appelle le gestionnaire de base
	this->CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CATPZoomPlan)
	DDX_Control(pDX, IDC_CR_A1, m_A1);
	DDX_Control(pDX, IDC_CR_A2, m_A2);
	DDX_Control(pDX, IDC_CR_A3, m_A3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CATPZoomPlan, CDialog)
	//{{AFX_MSG_MAP(CATPZoomPlan)
		ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CR_X, OnBtnX)
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPZoomPlan::OnUpdate()
{
	// on met à jour le composant
	if (m_pATPDocCreuset != 0) this->ShowWindow(m_pATPDocCreuset->m_AFFZoomPlan.IsVisible() ? SW_SHOW : SW_HIDE);
}

///////////////////////////////
// CATPZoomPlan message handlers

BOOL CATPZoomPlan::OnInitDialog() 
{
	// on appelle le gestionnaire de base
	this->CDialog::OnInitDialog();

	SetupControls();
	// on quitte
	return (TRUE);
}

void CATPZoomPlan::OnCancel() 
{
	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on réinitialise le ZoomPlan
		m_pATPDocCreuset->m_AFFZoomPlan.Reset();

		// on met à jour le pupitre
		m_pATPDocCreuset->m_AFFZoomPlan.SetVisible(false);

		// on met à jour les vues du document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_PUPITRE);
	}
	else
	{
		// on met à jour le composant
		this->ShowWindow(SW_HIDE);
	}
}

void CATPZoomPlan::OnOK() 
{
	// on ne fait rien
}

void CATPZoomPlan::SetupControls()
{
	m_Backgnd.SubclassDlgItem(IDC_CR_BK, this);
	m_Ttl.SubclassDlgItem(IDC_CR_TTL, this);
	m_Ttl2.SubclassDlgItem(IDC_CR_TTL2, this);
	m_Ttl3.SubclassDlgItem(IDC_CR_TTL3, this);
	m_btnX.SubclassDlgItem(IDC_CR_X, this);
	SetColors();
}

void CATPZoomPlan::SetColors()
{
	m_Backgnd.SetBkColor(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::MEDIUM_DEGRAD);
	m_Backgnd.SetSunken(true);

	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_btnX.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_Ttl2.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl2.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl2.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));

	m_Ttl3.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl3.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl3.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));
}

LRESULT CATPZoomPlan::OnNcHitTest(CPoint point)
{
    UINT hit = CDialog::OnNcHitTest(point);
    if ( hit == HTCLIENT ) 
    {
        return HTCAPTION;
    }
    else
        return hit;
}

void CATPZoomPlan::SetNbFormats(int nbFormats)
{
	// on récupère les dimensions du composant
	LONG siz = m_A2.GetVertItemSize();
	m_A2.SetVertCount(nbFormats);
	RECT Rect;
	RECT Rect2;

	m_A2.GetWindowRect(&Rect);
	m_A2.SetWindowPos(NULL, 0, 0, Rect.right-Rect.left, (nbFormats)*siz , SWP_NOZORDER| SWP_NOMOVE);

	m_Backgnd.GetWindowRect(&Rect);
	m_Ttl2.GetWindowRect(&Rect2);
	int haut = (Rect2.bottom + (nbFormats)*siz + 8) - Rect.top;
	m_Backgnd.SetWindowPos(NULL, 0, 0, Rect.right-Rect.left, haut , SWP_NOZORDER| SWP_NOMOVE);

	GetWindowRect(&Rect);
	haut = (Rect2.bottom + (nbFormats)*siz + 14) - Rect.top;
	SetWindowPos(NULL, 0, 0, Rect.right-Rect.left, haut , SWP_NOZORDER| SWP_NOMOVE);

	CString lbl;
	if(nbFormats<=1)
		lbl.LoadString(IDS_CR_FORMAT);
	else
		lbl.LoadString(IDS_CR_FORMATS);
	m_Ttl2.SetText(lbl);

}

void CATPZoomPlan::OnBtnX()
{
	OnCancel();
}