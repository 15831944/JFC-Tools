// on inclut les d�finitions n�cesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_C3.h"
#include "AjoutInsertion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

//////////////////
// le constructeur

CATPCreusetHypo_C3::CATPCreusetHypo_C3()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPCreusetHypo_C3::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// Ce grid utilisera le fonctionnement editbox
	// this->SetAccesEditBox(true);

	// Ce grid n'utilisera pas la fonction editbox (trop chiant pour r�partir sur les supports)
	this->SetAccesEditBox(false);

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPCreusetHypo_C3::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_C3::~CATPCreusetHypo_C3()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_C3, JFCGridMultiCell)
	//{{AFX_MSG_MAP(CATPCreusetHypo_C3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPCreusetHypo_C3::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG xHyp; 
	LONG yLigne = 1;
	int nbFormat = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// xHyp = m_pATPDocCreuset->m_MaxHypothese; 
		xHyp = m_pATPDocCreuset->m_TblHypotheses.GetCount()-1;
		//nbFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
		m_mapICellFormat.Reset();
		nbFormat = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
			{
				nbFormat++;
				m_mapICellFormat.Add(nbFormat) = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
			}
		}
	}

	//col total
	if(m_pATPDocCreuset->m_AFFColonneTotal)
	{
		nbFormat++;
		m_mapICellFormat.Add(nbFormat) = -1;
	}
	//col total

	this->SetHorzCellCount(nbFormat, FALSE);
	if(nbFormat!=0)
	{
		this->SetHorzCount(xHyp, FALSE);
		this->SetHorzItemSize(nbFormat*CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	}
	else
	{
		this->SetHorzCount(0, FALSE);
		this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	}
	this->SetVertCount(yLigne, TRUE);
}


// la fonction pour dessiner l'int�rieur des cellules
JVoid CATPCreusetHypo_C3::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

	bool bCpl = false;

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on r�cup�re l'hypoth�se courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);

		// on v�rifie la validit� de l'hypoth�se
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{

			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			int id = 0;
			CString Text;

			JUnt32 IdFormatCur = ConvertICellToFormat(ii);

			//CFont* pOldFont = NULL;
			if(IdFormatCur==-1 )
			{
				//LOGFONT lf;
				//dc.GetCurrentFont()->GetLogFont(&lf);
				///*lf.lfWeight = FW_BOLD;*/lf.lfItalic = 1;
				//CFont fontBold;
				//fontBold.CreateFontIndirect(&lf);
				//pOldFont = dc.SelectObject(&fontBold);
				PLNInsertions.GetNbInsert(DateD,DateF, NbInsertions, NbInsertCpl, bCpl);
			}

			if(IdFormatCur!=-1)
				PLNInsertions.GetNbInsert(IdFormatCur,DateD,DateF, NbInsertions, NbInsertCpl, bCpl);

			// on formate le nombre d'insertions
			if(NbInsertions)
			{
				if (bCpl)
					Text.Format(_T("(%d)"), NbInsertions);
				else
				{
					if (NbInsertCpl)
						Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
					else
						Text.Format(_T("%d"), NbInsertions);
				}
			}


			// on �crit le nombre d'insertions
			// si hypoth�se active, couleur diff�rente
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(IdFormatCur);
			if (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif /*&& m_pATPDocCreuset->m_MapIdFormatsSel.IsValid() /*&& m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 3*/)
			{
				// on cr�e le pinceau pour les insertions en arri�re plan
				CBrush BrushB;
				CRect rectTmp = rect;
				if(IdFormatCur==-1)
					BrushB.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
				else
					BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
	

				// on dessine le fond de la cellule
				dc.FillRect(&rect, &BrushB);
			}
			


			dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			//if(pOldFont!=NULL)
			//	dc.SelectObject(pOldFont);
		}
	}
}

////////////////////////////////////////////////////////////////
// Ajout d'insertions sur tous les supports avec formats actifs
JVoid CATPCreusetHypo_C3::OnLButtonDown(LONG i, LONG ii, LONG j)
{
	// Ajoute insertion pour tous les supports
	AjoutInsertionAllSupport(i,ii,1);
}

//////////////////////////////////////////////////////////////////////
// Suppression d'insertions sur tous les supports avec formats actifs
JVoid CATPCreusetHypo_C3::OnRButtonDown(LONG i, LONG ii, LONG j)
{
	// Suppression insertion pour tous les supports
	SuppInsertionAllSupport(i,ii,1);

}

// Ajoute ou retranche x insertions sur l'hypoth�se courante pour les formats actifs
JBool CATPCreusetHypo_C3::OnInsertionsHyp(JInt32 NbInsertions,JUnt32 NoHypothese, int idFormat)
{

	if(idFormat == -1)
		return false;
	m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(idFormat);

	// Flag test si ajout ou retrait insertions effectu�
	JBool MajInsertion = false;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// Balyage de tous les supports actifs
		JUnt32 j;
		for (j = 0;j < m_pATPDocCreuset->m_PlateauSupports.GetCount();j++)
		{
			// on r�cup�re l'identifiant du Support
			JUnt32 IdSupport;
			if (m_pATPDocCreuset->m_AFFOffreHypos)
			{
				// Mode offre globale
				IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
			}
			else
			{
				// Mode plateau
				const CKeySupport & KeyS = m_pATPDocCreuset->m_PlateauSupports.Item(j);
				if(KeyS.m_IdParent)//il s'agit d'un element d'un couplage
					continue;
				IdSupport = KeyS.m_IdSupport;
			}

			JUnt32 Ret = 0;

			m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypothese);
			if (NbInsertions > 0)
			{
				// Init validit� maj insertions
				JBool OkAjout = true;

				// Boucle pour toutes les insertions � ajouter
				for (JUnt32 Inx = 0; Inx < NbInsertions; Inx++)
				{
				
					// Ajout insertion pour ce format
					if (CAjoutInsertion::AjoutInsertionSuccessif(IdSupport,idFormat,NoHypothese,m_pATPDocCreuset))
					{ 
						// si on ajoute un spot � la derni�re hypoth�se, on en ajoute une nouvelle
						if (NoHypothese <  m_pATPDocCreuset->m_MaxHypothese
							&& NoHypothese  == m_pATPDocCreuset->m_TblHypotheses.GetCount() - 1) 
						{
							m_pATPDocCreuset->AddNewHypo();
						}
					}
					else
						OkAjout = false;
				}
				// R�cup�re validit� insertions successives
				MajInsertion = OkAjout;
			}
			else
			{
				// Init validit� maj insertions
				JBool OkSupp = true;

				// Boucle pour toutes les insertions � supprimer
				for (JUnt32 Inx = NbInsertions; Inx > 0; Inx++)
				{
					// Suppression insertion pour ce format
					if (CAjoutInsertion::SupprLastInsertion(IdSupport,idFormat,NoHypothese,m_pATPDocCreuset))
					{ 
						// on r�cup�re le plan de l'hypoth�se courante apr�s suppression �l�mnt
						m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypothese);
						CPlanInsertions & PLNInsert = m_pATPDocCreuset->m_TblHypotheses.GetItem();

						// Test si encore des insertions pour ce plan
						if (PLNInsert.GetCount() == 0 &&
							m_pATPDocCreuset->m_TblHypotheses.GetCount() > 2 &&
							NoHypothese == m_pATPDocCreuset->m_TblHypotheses.GetCount()-2)
						{
							// Suppression de l'avant derni�re hypoth�se si vide / garde une seule hypoth�se vierge
							m_pATPDocCreuset->UpdateAllViews(UPDATE_SUPP_HYPOTHESECUR);
						}
					}
					else
						// Probl�me suppression insertion
						OkSupp = false;
	
					MajInsertion = OkSupp;
				}
			}
		}
	}

	// retour true si modif insertions effectu�es
	return MajInsertion;
}




/////////////////////////////////////
// CATPCreusetHypo_C3 message handlers

void CATPCreusetHypo_C3::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGridMultiCell::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x3(), TRUE, FALSE);
}

int CATPCreusetHypo_C3::ConvertICellToFormat(int iCell)
{
	m_mapICellFormat.MoveTo(iCell);
	if(m_mapICellFormat.IsValid())
		return m_mapICellFormat.GetItem();

	return -1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Activation de la boite �dition cellule
bool CATPCreusetHypo_C3::OnBeginEditing(CString &CurString, LONG i, LONG j)
{
	// Pour le moment pas d'utilisation edit box
	return false;

	/* Si on voulait utiliser l'�ditbox de JFCGrid
	// R�cup indice cellule
	m_RowCellEdit = j;
	m_ColCellEdit = i;

	// Sur quel support
	JUnt32 IdSupport;
	const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	IdSupport = KeySupp.m_IdSupport; 

	// R�cup�re le nombre d'insertion courant sur ce support
	// LONG NoColFormat = GetNoColFormat();
	m_NoColFormatCur = GetNoColFormat();
	LONG NoColFormat = m_NoColFormatCur;

	// R�cup le nombre d'insertions pr�c�dentes
	m_NbInsCurEdit = GetNbInsertion(i, NoColFormat);

	// Met � jour le texte par d�faut de la boite d'�dition
	CurString.Format("%d", m_NbInsCurEdit); 

	// on active l'�dition des �l�ments
	return (true);
	*/
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Fin �dition cellule
void CATPCreusetHypo_C3::OnEndEditing()
{
	// R�init indice cellule �dition
	m_RowCellEdit = -1;
	m_ColCellEdit = -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// R�cup�re valeur �dition
bool CATPCreusetHypo_C3::OnEditItem(const TCHAR* pString, LONG i, LONG j)
{
	// Mise � jour cellule avec nb insertions
	// LONG NoFormat = GetNoColFormat();
	LONG NoFormat = m_NoColFormatCur;

	// Nouveau nombre d'insertions
	int NbInsEdit = atoi(pString);

	// Test non d�passement maximum insertion
	if (NbInsEdit < MaxInsertion)
	{
		// Il faut ajouter des insertions
		// JUnt32 NbAjout = NbInsEdit - m_NbInsCurEdit; 
		JUnt32 NbAjout = NbInsEdit; 
		AjoutInsertionAllSupport(m_ColCellEdit, NoFormat, NbAjout);

		/*
		}
		else
		{
			// Il faut supprimer des insertions
			JUnt32 NbSupp = m_NbInsCurEdit - NbInsEdit; 
			SuppInsertionAllSupport(m_ColCellEdit, NoFormat, NbSupp);
		}
		*/
	}
	else
	{
		// Maximum insertion atteint
		CString TxtMess;
		TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
		AfxMessageBox(TxtMess,MB_ICONINFORMATION);
	}

	return (true);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Annulation �dition
void CATPCreusetHypo_C3::OnCancelEditing()
{
	// R�init indice cellule �dition
	m_RowCellEdit = -1;
	m_ColCellEdit = -1;
}

///////////////////////////////////////////////////////////////////////////////
// R�cup�re le nombre d'insertion total sur hypothese
JUnt32 CATPCreusetHypo_C3::GetNbInsertion(LONG NoHypo, LONG NoFormat)
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

	bool bCpl = false;

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on r�cup�re l'hypoth�se courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypo + 1);

		// on v�rifie la validit� de l'hypoth�se
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{

			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			int id = 0;
			CString Text;

			JUnt32 IdFormatCur = ConvertICellToFormat(NoFormat);

			//CFont* pOldFont = NULL;
			if(IdFormatCur==-1 )
			{
				PLNInsertions.GetNbInsert(DateD,DateF, NbInsertions, NbInsertCpl, bCpl);
			}

			if(IdFormatCur!=-1)
				PLNInsertions.GetNbInsert(IdFormatCur,DateD,DateF, NbInsertions, NbInsertCpl, bCpl);
		}
	}

	// Renvoi le nombre total d'insertions sur l'hypoth�se en cours
	return NbInsertions;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajoute nb insertions pour tous les supports
void CATPCreusetHypo_C3::AjoutInsertionAllSupport(LONG NoHypo, LONG NoFormat, int NbAjoutIns)
{
	// Si nouvelle hypoth�se active, on l'active
	JUnt32 IdHypothese = NoHypo + 1;
	if (m_pATPDocCreuset->m_IdxHypotheseActif != IdHypothese 
		&& IdHypothese <= m_pATPDocCreuset->m_TblHypotheses.GetCount() )
	{
		// On change l'hypoth�se active
		m_pATPDocCreuset->m_IdxHypotheseActif	= IdHypothese;

		// Mise � jour composant
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
	}

	// Puis on ajoute 1 insertion sur chacun des supports pour les formats actifs
	m_pATPDocCreuset->m_IdxHypotheseCourant = NoHypo + 1;
	
	if (OnInsertionsHyp(NbAjoutIns,m_pATPDocCreuset->m_IdxHypotheseActif, ConvertICellToFormat(NoFormat)))
	{
		// Si insertion(s) effectu�e(s) on met � jour le document
		m_pATPDocCreuset->SetModifiedFlag(TRUE); 
		m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
		m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Suppression nb insertion pour tous les supports
void CATPCreusetHypo_C3::SuppInsertionAllSupport(LONG NoHypo, LONG NoFormat, int NbSuppIns)
{
	// Si nouvelle hypoth�se active, on l'active
	JUnt32 IdHypothese = NoHypo + 1;
	if (m_pATPDocCreuset->m_IdxHypotheseActif != IdHypothese 
		&& IdHypothese <= m_pATPDocCreuset->m_TblHypotheses.GetCount() )
	{
		// On change l'hypoth�se active
		m_pATPDocCreuset->m_IdxHypotheseActif	= IdHypothese;

		// Mise � jour composant
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
	}

	// Puis on supprime 1 insertion sur chacun des supports pour les formats actifs
	m_pATPDocCreuset->m_IdxHypotheseCourant = NoHypo + 1;
	NbSuppIns = -NbSuppIns;
	if (OnInsertionsHyp(NbSuppIns,m_pATPDocCreuset->m_IdxHypotheseActif, ConvertICellToFormat(NoFormat)))
	{
		// Si insertion(s) effectu�e(s) on met � jour le document
		m_pATPDocCreuset->SetModifiedFlag(TRUE); 
		m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
		m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	}
}