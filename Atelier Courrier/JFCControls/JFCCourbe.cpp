// on inclut les définitions nécessaires
#include "stdafx.h"
#include "math.h"

// on inclut les définitions nécessaires
#include "JFCScheme.h"

// on inclut les définitions nécessaires
#include "JFCCourbe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// on initialise les dimensions des Courbemes
static const long g_BrdH = 4;  //8;		
static const long g_LrgH = 2;  //20;

/////////////////////////////////////////////////////////////////////////////
// le constructeur

JFCCourbe::JFCCourbe()
{
	// on initialise les éléments
	m_pICourbe = 0;

	// on initialise l'acsenseur
	m_pHorzScroll = 0; m_HorzMaster = FALSE;
}

////////////////////////////////////////////
// la fonction pour initialiser les éléments

JFCICourbe* JFCCourbe::SetItems(JFCICourbe* pICourbe, BOOL Update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on mémorise les nouveaux éléments
	JFCICourbe* pOldICourbe = m_pICourbe; m_pICourbe = pICourbe;

	// on met à jour les graduations
	this->OnUpdateGraduations();

	// on met à jour l'ascenseur
	if (Update != FALSE) this->OnSetScrollBar(TRUE);

	// on renvoie les anciens éléments
	return (pOldICourbe);
}

///////////////////////////////////////////////
// la fonction pour définir l'ascenseur externe

void JFCCourbe::SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on réinitialise l'ascenseur courant
	if (m_HorzMaster != FALSE && m_pHorzScroll != NULL) m_pHorzScroll->SetRange(0, 0, 0);

	// on mémorise l'indicateur de maître
	m_HorzMaster = (Master != FALSE && pScroll != NULL) ? TRUE : FALSE;

	// on mémorise l'ascenseur
	m_pHorzScroll = pScroll;

	// on met à jour les ascenseurs
	if (update != FALSE) this->OnSetScrollBar(TRUE);
}

//////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour définir les pas de graduation en X
void JFCCourbe::SetPasGraduationEnX(LONG PasGraduationX)
{
	this->SetPasGraduationX(PasGraduationX); 
}

//////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour définir les pas de graduation en Y
void JFCCourbe::SetPasGraduationEnY(LONG PasGraduationY)
{
	this->SetPasGraduationY(PasGraduationY);
}

///////////////////////////////////////////////////
// les fonctions pour récupérer l'ascenseur externe

JFCScroll* JFCCourbe::GetHorzScroll()
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on renvoie l'ascenseur externe
	return (m_pHorzScroll);
}

/////////////////////////////////////////////////////////////////////////////
// le destructeur

JFCCourbe::~JFCCourbe()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(JFCCourbe, JFCView)
	//{{AFX_MSG_MAP(JFCCourbe)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////
// la fonction pour mettre à jour les graduations

void JFCCourbe::OnUpdateGraduations()
{
	// on initialise la hauteur maximale
	double MaxHeight = 0.0;

	// on vérifie la validité des éléments
	if (m_pICourbe != 0)
	{
		// on récupère le nombre d'éléments en colonne
		long ColCount = m_pICourbe->GetCourbeCount();

		// on récupère le nombre d'éléments en ligne
		long RowCount = m_pICourbe->GetXElmtCount();

		// on boucle sur tous les éléments en ligne
		for (long IdxRow = 0; IdxRow < RowCount; IdxRow += 1)
		{
			// on initialise la hauteur courante
			// double Height = 0.0;
			
			// on boucle sur tous les éléments en colonne
			for (long IdxCol = 0; IdxCol < ColCount; IdxCol += 1)
			{
				// on vérifie la validité de l'élément
				if (m_pICourbe->IsItemValid(IdxRow, IdxCol))
				{
					// on récupère la valeur de l'élément
					bool ItemMarque = false;
					double Value = m_pICourbe->GetValueItem(IdxRow, IdxCol, ItemMarque); if (Value < 0.0) Value = 0.0;

					// on prend la plus grande hauteur
					if (Value > MaxHeight)
						MaxHeight = Value;
				}
			}
		}
	}

	// on met à jour les graduations
	if (MaxHeight)
		m_Graduations.SetRange(0.0, MaxHeight);
	else
		m_Graduations.Reset();
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour configurer les ascenseurs actifs

void JFCCourbe::OnSetScrollBar(BOOL Update)
{
	// on met à jour l'affichage des ascenseurs
	this->ShowScrollBar(SB_HORZ, FALSE);
	this->ShowScrollBar(SB_VERT, FALSE);

	// on teste si on doit configurer l'ascenseur horizontal
	if (m_pHorzScroll != 0 && m_HorzMaster != FALSE)
	{
		// on récupère la dimension de la zone
		long HSize = this->GetHorzSize();

		// on récupère le nombre d'éléments
		long count = 0; if (m_pICourbe != 0) count = m_pICourbe->GetXElmtCount();

		// on calcule le nombre d'éléments par page
		long page = HSize / (2 * g_BrdH + g_LrgH);

		// on configure l'ascenseur externe
		if (count > 0)
			m_pHorzScroll->SetRange(0,count-1, page);
		else
			m_pHorzScroll->SetRange(0,0, page);
	}

	// on met à jour la fenêtre
	if (Update != FALSE) this->InvalidateRect(NULL, FALSE);
}

////////////////////////////////////////////////////////////////////////////
// la fonction pour récupérer la position de l'ascenseur actif

long JFCCourbe::OnGetHorzPosition()
{
	// on initialise la position
	LONG position = 0;

	// on récupère la position de l'ascenseur
	if (m_pHorzScroll != NULL) position = m_pHorzScroll->GetPosition();

	// on renvoie la position
	return (position);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour calculer la position des éléments

long JFCCourbe::OnGetYPosition(long VSize, double YGrdMin, double YGrdMax, double Value)
{
	// on teste la dimension
	if (VSize <= 0) return (0);

	// on vérifie la validité du calcul
	if (YGrdMin == YGrdMax) return (VSize);

	// on calcule la position
	long Pos = (LONG)floor((((Value - YGrdMin) * (double)VSize) / (YGrdMax - YGrdMin)) + 0.05);

	// on renvoie la position
	return (VSize - Pos);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les axes et les Courbemes

void JFCCourbe::OnDrawAxes(CDC & dc, RECT rect)
{
	// on récupère la dimension de la zone
	long VSize = this->GetVertSize();

	// on calcule le dépassement pour dessiner la flèche
	long DepY = (VSize + 8) / 16;

	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(JFCScheme::GetGreyD());

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenB);

	// on modifie la fonte
	CFont* pOldFont = dc.SelectObject(&m_FontP);

	// on dessine l'axe des graduations
	dc.MoveTo(0, -DepY); dc.LineTo(0, VSize);

	// on dessine la flèche de l'axe des graduations
	dc.MoveTo(-1, 1 - DepY); dc.LineTo(-4, 8 - DepY);
	dc.MoveTo( 1, 1 - DepY); dc.LineTo( 4, 8 - DepY);

	// on teste si on a des éléments
	if (m_pICourbe->GetXElmtCount() > 0)
	{
		// on récupère les bornes des graduations
	  	double YGrdMin = 0.0;
	  	double YGrdMax = m_Graduations.GetMax();

		// on récupère le nombre de graduations
		long NbGraduations = m_Graduations.GetNbGraduation();

		// on boucle sur toutes les graduations
		for (long IdxGraduation = 0; IdxGraduation < NbGraduations; IdxGraduation += 1)
		{
			// on récupère la position de la graduation
			long PosY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, m_Graduations.GetGraduation(IdxGraduation));

			// on récupère le libellé de la graduation
			CString Libelle = m_Graduations.GetLibGraduation(IdxGraduation);
			FormatString(Libelle,3);

			// on récupère la taille de la graduation
			CSize SizeY = dc.GetTextExtent(Libelle);

			// on écrit le libellé de la graduation
			//if (IdxGraduation == 0 || (IdxGraduation + 1) % PasGraduation == 0)
			dc.TextOut(-5 - SizeY.cx, PosY - (SizeY.cy / 2), Libelle);

			// on dessine la graduation
			dc.MoveTo(-3, PosY); dc.LineTo(0, PosY);
		}

		// on récupère le libellé de la grandeur utilisée
		const JString Libelle = m_pICourbe->GetLabelResultVert();

		// on récupère la taille de la graduation
		CSize SizeY = dc.GetTextExtent(Libelle.AsJCharPtr());

		// on écrit le libellé de la graduation
		dc.TextOut(10, -DepY - 10, Libelle.AsJCharPtr());


	}

	// on restaure la fonte
	dc.SelectObject(pOldFont);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

void JFCCourbe::OnDrawMoustaches(CDC & dc, RECT rect)
{
	// on récupère les dimensions de la zone
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	// on récupère les bornes des graduations
	double YGrdMin = 0.0;
	double YGrdMax = m_Graduations.GetMax();

	// on initialise la hauteur référence
	long RefY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, 0.0);

	// on initialise la position courante
	long PosX = 0;
	long PosY = RefY;

	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(JFCScheme::GetGreyD());

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenB);

	// on modifie la fonte
	CFont* pOldFont = dc.SelectObject(&m_FontO);

	// on récupère le pinceau
	CBrush* pOldBrush = dc.GetCurrentBrush();

	// on récupère le nombre d'éléments en ligne
	long RowCount = m_pICourbe->GetXElmtCount();

	// on récupère la position de l'ascenceur
	long FrstRow = this->OnGetHorzPosition();
	long LastRow = FrstRow + (HSize / (2 * g_BrdH + g_LrgH)); if (LastRow > RowCount) LastRow = RowCount;

	// on teste si on doit tracer la première graduation
	if (FrstRow < LastRow)
	{
		// on trace la première graduation
		dc.MoveTo(PosX, RefY); dc.LineTo(PosX, RefY + 4);
	}

	// on boucle sur tous les éléments en ligne
	for (long IdxRow = FrstRow; IdxRow < LastRow; IdxRow += 1)
	{
		// on initialise la hauteur courante
		double Height = 0.0;

		// on récupère le libellé de l'élément
		JString Libelle = m_pICourbe->GetLabelItem(IdxRow);

		// on récupère le nombre d'éléments en colonne
		long ColCount = m_pICourbe->GetCourbeCount();

		// Voir si étalonnage à marquer
		bool ItemMarqueHrz = false;

		// on boucle sur tous les éléments en colonne
		for (long IdxCol = 0; IdxCol < ColCount; IdxCol += 1)
		{
			// on vérifie la validité de l'élément
			if (m_pICourbe->IsItemValid(IdxRow, IdxCol))
			{
				// on récupère la valeur de l'élément
				bool ItemMarque = false;
				double Value = m_pICourbe->GetValueItem(IdxRow, IdxCol, ItemMarque); if (Value < 0.0) Value = 0.0;

				// Valide la marquage étallonage horizontal
				if (ItemMarque)
					ItemMarqueHrz = true;

				// on récupère l'indice de la couleur de l'élément
				long IdxColor = m_pICourbe->GetCourbeColor(IdxCol); if (IdxColor < 0) IdxColor = -(IdxColor + 1);

				// on modifie le stylo
				dc.SelectObject(&m_PenG);

				// on modifie le pinceau avec couleur
				//m_Brushes.CreateSolidBrush(RGB(204,204,255));
				m_Brushes.CreateSolidBrush(m_pICourbe->GetColor(IdxColor));
				dc.SelectObject(&m_Brushes);

				// on incrémente la hauteur courante
				Height += Value;

				// on récupère la hauteur de l'Courbeme
				long NewY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, Height);

				// on dessine l'Courbeme
				dc.BeginPath();
				dc.MoveTo(PosX + g_BrdH, PosY);
				dc.LineTo(PosX + g_BrdH, NewY);          dc.LineTo(PosX + g_BrdH + g_LrgH, NewY);
				dc.LineTo(PosX + g_BrdH + g_LrgH, PosY); dc.LineTo(PosX + g_BrdH,          PosY);
				dc.EndPath(); dc.StrokeAndFillPath();

				// on met à jour la position courante
				PosY = NewY;

				m_Brushes.DeleteObject(); 
			}
		}

		if (ItemMarqueHrz)
			// on écrit le libellé de l'élément
			dc.TextOut(PosX + g_BrdH + (g_LrgH / 2), RefY, Libelle.AsJCharPtr());

		// on modifie le stylo
		dc.SelectObject(&m_PenB);

		// on trace l'axe horizontal
		dc.MoveTo(PosX, RefY); dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY); dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY + 4);

		// on incrémente la position
		PosX += 2 * g_BrdH + g_LrgH; PosY = RefY;
	}

	// on calcule le dépassement pour dessiner la flèche
	long DepX = (HSize + 8) / 16;

	// on dessine l'axe des graduations
	dc.MoveTo(PosX, RefY); dc.LineTo(PosX + DepX + 1, RefY);

	// on dessine la flèche de l'axe des graduations
	dc.MoveTo(PosX + DepX - 1, RefY - 1); dc.LineTo(PosX + DepX - 8, RefY - 4);
	dc.MoveTo(PosX + DepX - 1, RefY + 1); dc.LineTo(PosX + DepX - 8, RefY + 4);

	// on restaure le pinceau initial
	dc.SelectObject(pOldBrush);

	// on restaure la fonte initiale
	dc.SelectObject(pOldFont);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Dessin des courbes
void JFCCourbe::OnDrawCourbes(CDC & dc, RECT rect)
{
	// on récupère les dimensions de la zone
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	// on récupère le pas de graduation en X
	long PasGraduationX = this->GetPasGraduationX(); 

	// on récupère les bornes des graduations
	double YGrdMin = 0.0;
	double YGrdMax = m_Graduations.GetMax();

	// on initialise la hauteur référence
	long RefY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, 0.0);

	// on initialise la position courante
	long PosX = 0;
	long PosY = RefY;

	// on modifie la transparence
	long OldBck = dc.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = dc.SetTextColor(JFCScheme::GetGreyD());

	// on modifie le stylo
	CPen* pOldPen = dc.SelectObject(&m_PenB);

	// on modifie la fonte
	// CFont* pOldFont = dc.SelectObject(&m_FontO);
	CFont* pOldFont = dc.SelectObject(&m_FontP);

	// on récupère le pinceau
	CBrush* pOldBrush = dc.GetCurrentBrush();

	// on récupère le nombre d'éléments en ligne
	long RowCount = m_pICourbe->GetXElmtCount();

	// on récupère la position de l'ascenceur
	long FrstRow = this->OnGetHorzPosition();
	long LastRow = FrstRow + (HSize / (2 * g_BrdH + g_LrgH)); if (LastRow > RowCount) LastRow = RowCount;

	// on teste si on doit tracer la première graduation
	if (FrstRow < LastRow)
	{
		// on trace la première graduation
		dc.MoveTo(PosX, RefY); dc.LineTo(PosX, RefY + 4);
	}

	// Récupère départ des courbes (car selon grandeur à évaluer, pas toujours défini au début exemple : coût) 
	JArray <long> TDepRow;
	TDepRow.Reset();
	TDepRow.SetCount(m_pICourbe->GetCourbeCount());
	for (JInt32 InxCrb = 0; InxCrb < m_pICourbe->GetCourbeCount(); InxCrb++)
		// A VOIR
		// TDepRow.Item(InxCrb) = (long)m_pICourbe->GetDepCourbe(InxCrb);
		TDepRow.Item(InxCrb) = (long)0;

	// on boucle sur tous les éléments en ligne
	for (long IdxRow = FrstRow; IdxRow < LastRow; IdxRow += 1)
	{
		// on récupère le nombre d'éléments en colonne
		long ColCount = m_pICourbe->GetCourbeCount();

		// Voir si étalonnage à marquer
		bool ItemMarqueHrz = false;

		// on boucle sur tous les éléments en colonne
		for (long IdxCol = 0; IdxCol < ColCount; IdxCol += 1)
		{
			// on vérifie la validité de l'élément
			if (m_pICourbe->IsItemValid(IdxRow, IdxCol))
			{
				// on récupère la valeur élément précédent et élmt courant
				bool AffichEtal = false;

				double ValuePrec	= m_pICourbe->GetValueItem(IdxRow, IdxCol, AffichEtal); if (ValuePrec < 0.0) ValuePrec = 0.0;
				double Value		= ValuePrec;
				if (IdxRow+1 < LastRow)
				{
					Value	= m_pICourbe->GetValueItem(IdxRow+1, IdxCol, AffichEtal); if (Value < 0.0) Value = 0.0;
				}
				else
					AffichEtal = false;
				

				// Récupére si affichage ou pas libellé jour (selon étalonnage)
				if (AffichEtal || IdxRow == 0)
					ItemMarqueHrz = true;

				// on récupère l'indice de la couleur de l'élément
				long IdxColor = m_pICourbe->GetCourbeColor(IdxCol); if (IdxColor < 0) IdxColor = -(IdxColor + 1);

				// on modifie le stylo
				m_PenColor.CreatePen(PS_SOLID, 0, m_pICourbe->GetColor(IdxColor));
				dc.SelectObject(&m_PenColor);

				// on modifie le pinceau avec couleur
				m_Brushes.CreateSolidBrush(m_pICourbe->GetColor(IdxColor));
				dc.SelectObject(&m_Brushes);

				long NewYPrec = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, ValuePrec);
				long NewY     = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, Value);

				// Tracé si on a dépassé le début courbe prédéfini
				if (IdxRow >= TDepRow.Item(IdxCol))
				{
					if(Value != 0)
					{
						// on dessine l'Courbeme
						dc.BeginPath();
						dc.MoveTo(PosX, NewYPrec);
						dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, NewY);
						dc.EndPath();dc.StrokeAndFillPath();

						// Tracé du symbole insertion si on est sur un point d'étalonnage
						/*
						JUnt32 SizePt = 1;
						dc.MoveTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
						dc.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY + SizePt);
						dc.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY + SizePt);
						dc.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY - SizePt);
						dc.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
						dc.EndPath();dc.StrokeAndFillPath();
						*/

						JUnt32 SizePt = 1;
						if (AffichEtal)
						{
							dc.MoveTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
							dc.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY + SizePt);
							dc.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY + SizePt);
							dc.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY - SizePt);
							dc.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
							dc.EndPath();dc.StrokeAndFillPath();
						}
						// ou bien début de courbe
						else if (IdxRow == TDepRow.Item(IdxCol))
						{
							dc.MoveTo(PosX - SizePt, NewYPrec - SizePt);
							dc.LineTo(PosX - SizePt, NewYPrec + SizePt);
							dc.LineTo(PosX + SizePt, NewYPrec + SizePt);
							dc.LineTo(PosX + SizePt, NewYPrec - SizePt);
							dc.LineTo(PosX - SizePt, NewYPrec - SizePt);
							dc.EndPath();dc.StrokeAndFillPath();
						}
					}

					else if (IdxRow != (LastRow - 1) && ValuePrec != 0)
					{
						// on récupère le libellé de l'élément
						// const JString Libelle = m_pICourbe->GetLabelItem(IdxCol);
						const JString Libelle = m_pICourbe->GetLibCourbeGraph(IdxCol);

						// on écrit le libellé de la courbe en fin de courbe
						dc.TextOut(PosX + 2 * g_LrgH + 10, NewYPrec - 10 , Libelle.AsJCharPtr());
						if (NewY == 0)
							long ValY = NewY;
					}
				}
								
				// on met à jour la position courante
				PosY = NewY;

				// Destruction Brush et Pen
				m_Brushes.DeleteObject(); 
				m_PenColor.DeleteObject(); 

				// on écrit le libellé de la courbe en fin de courbe
				if (IdxRow == LastRow - 1 && ValuePrec != 0)
				{
					// on récupère le libellé de l'élément
					// const JString Libelle = m_pICourbe->GetLabelItem(IdxCol);

					const JString Libelle = m_pICourbe->GetLibCourbeGraph(IdxCol);

                    dc.TextOut(PosX + 2 * g_LrgH + 10 , NewY - 10 , Libelle.AsJCharPtr());
				}
			}

			/*
			if (ItemMarqueHrz && IdxRow != 0)
			{
				// On écrit libellé graduation horizontale
				if (IdxRow == 0 || (IdxRow + 1) % PasGraduationX == 0)
				{
					CString LibGrad;
					LibGrad.Format("%d",IdxRow+1); 
					dc.TextOut(PosX + 3 * g_BrdH / 2 + g_LrgH, RefY + 5, LibGrad);
				}
			}
			*/

			if (IdxRow != 0)
			{
				// On écrit libellé graduation horizontale
				if ((IdxRow + 1) % PasGraduationX == 0)
				{
					CString LibGrad;
					LibGrad.Format("%d",IdxRow+1); 
					dc.TextOut(PosX + 3 * g_BrdH / 2 + g_LrgH, RefY + 5, LibGrad);
				}
			}

		}

		// on modifie le stylo
		dc.SelectObject(&m_PenB);

		// on trace l'axe horizontal
		dc.MoveTo(PosX, RefY); 
		if (ItemMarqueHrz == false || IdxRow == 0)
		{
			// Axe horizontal
			dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY); 
			dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY + 4);
		}
		else
		{
			dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY); 
			dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY - 2); 
			dc.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY + 4);
		}

		// on incrémente la position
		PosX += 2 * g_BrdH + g_LrgH; PosY = RefY;
	}

	// on calcule le dépassement pour dessiner la flèche
	long DepX = (HSize + 8) / 16;

	if (LastRow > FrstRow )
	{
		// on dessine l'axe des graduations
		dc.MoveTo(PosX, RefY); dc.LineTo(PosX + DepX + 1, RefY);

		// on dessine la flèche de l'axe des graduations
		dc.MoveTo(PosX + DepX - 1, RefY - 1); dc.LineTo(PosX + DepX - 8, RefY - 4);
		dc.MoveTo(PosX + DepX - 1, RefY + 1); dc.LineTo(PosX + DepX - 8, RefY + 4);

		// On indique en abscisse le nombre de jours
		const JString LibAxeHorz = m_pICourbe->GetLabelResultHorz();
		dc.TextOut(PosX + 5 , RefY + 10, LibAxeHorz.AsJCharPtr());
	}
	else
	{
		// on dessine l'axe des graduations
		dc.MoveTo(0, RefY); dc.LineTo(HSize - 100, RefY);
		
		// on dessine la flèche de l'axe des graduations
		dc.MoveTo(HSize - 101, RefY - 1); dc.LineTo(HSize - 108, RefY - 4);
		dc.MoveTo(HSize - 101, RefY + 1); dc.LineTo(HSize - 108, RefY + 4);
	}

	// on restaure le pinceau initial
	dc.SelectObject(pOldBrush);

	// on restaure la fonte initiale
	dc.SelectObject(pOldFont);

	// on restaure le stylo initial
	dc.SelectObject(pOldPen);

	// on restaure la couleur du texte
	dc.SetTextColor(OldCol);

	// on restaure la transparence
	dc.SetBkMode(OldBck);
}

/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments

bool JFCCourbe::OnBeginDrawing(CDC & dc)
{
	// on vérifie la validité des éléments
	if (m_pICourbe == 0) return (false);

	// on récupère le résolution de DC
	LONG Reso = dc.GetDeviceCaps(LOGPIXELSY);

	// on crée les fontes
	m_FontP.CreateFont(-MulDiv(8, Reso, 72), 0,    0,    0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	m_FontO.CreateFont(-MulDiv(8, Reso, 72), 0, -250, -250, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");

	// on crée les pinceaux
	/*
	m_Brushes[0].CreateSolidBrush(RGB(204,204,255));
	m_Brushes[1].CreateSolidBrush(RGB(204,255,204));
	m_Brushes[2].CreateSolidBrush(RGB(204,255,255));
	m_Brushes[3].CreateSolidBrush(RGB(255,204,204));
	m_Brushes[4].CreateSolidBrush(RGB(255,204,255));
	m_Brushes[5].CreateSolidBrush(RGB(255,255,204));
	*/

	// on crée les stylos
	m_PenB.CreatePen(PS_SOLID, 0, JFCScheme::GetGreyD());
	m_PenG.CreatePen(PS_SOLID, 0, JFCScheme::GetGreyM());

	// on continue le dessin
	return (true);
}

void JFCCourbe::OnDrawItems(CDC & dc, RECT rect)
{
	// on dessine les axes
	this->OnDrawAxes(dc, rect);

	// on dessine les courbes
	this->OnDrawCourbes(dc, rect);
}

void JFCCourbe::OnEndDrawing(CDC & dc)
{
	// on détruit les fontes
	m_FontO.DeleteObject();
	m_FontP.DeleteObject();

	// on détruit les pinceaux
	m_Brushes.DeleteObject();

	// on détruit le stylo
	m_PenG.DeleteObject();
	m_PenB.DeleteObject();
}

/////////////////////////////////////////////////////////////////////////////
// JFCCourbe message handlers

void JFCCourbe::PreSubclassWindow() 
{
	// on initialise les éléments
	m_pICourbe = 0;

	// on initialise l'acsenseur
	m_pHorzScroll = 0; m_HorzMaster = FALSE;

	// on met à jour les graduations
	this->OnUpdateGraduations();

	// on appelle le gestionnaire de base
	this->JFCView::PreSubclassWindow();

	// on configure les ascenseurs
	this->OnSetScrollBar(FALSE);
}

void JFCCourbe::OnSize(UINT nType, INT cx, INT cy) 
{
	// on appelle le gestionnaire de base
	this->JFCView::OnSize(nType, cx, cy);
	
	// on met à jour les ascenseurs
	this->OnSetScrollBar(FALSE);
}

void JFCCourbe::OnDestroy() 
{
	// on réinitialise les éléments
	m_pICourbe = 0;

	// on réinitialise l'acsenseur
	m_pHorzScroll = 0; m_HorzMaster = FALSE;

	// on met à jour les graduations
	this->OnUpdateGraduations();

	// on appelle le gestionnaire de base
	this->JFCView::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Découpage chaine par milliers
void JFCCourbe::FormatString(CString &Txt, JInt32 PasDecoup)
{
	// Flag découpage effectif
	bool TxtDecoup = false;

	// Nouvelle chaine
	CString NewTxt = "";

	// Garde la partie décimale
	int CarDec = Txt.Find(".",0);
	if (CarDec > 0)
	{
		NewTxt = Txt.Mid(CarDec);
		Txt    = Txt.Left(CarDec);
	}

	// Formatage new chaine
	while (Txt.GetLength() > PasDecoup)
	{
		// Ajoute bloc new chaine
		NewTxt = Txt.Mid(Txt.GetLength() - PasDecoup, PasDecoup) + " " + NewTxt;

		// Enleve bloc old chaine
		Txt.Delete(Txt.GetLength() - PasDecoup, PasDecoup); 

		TxtDecoup = true;
	}

	// S'il reste des caractère, on les ajoute
	if (Txt.GetLength() > 0)
	{
		if (TxtDecoup)
			NewTxt = Txt + " " + NewTxt;
		else
			NewTxt = Txt + NewTxt;
	}	

	// Puis on récupère la chiane finale
	Txt = NewTxt;
}

///////////////////////////////////////////////////////////
// la fonction pour exporter le graphe dde la courbe
JBool JFCCourbe::ExportGraph(LPCTSTR pFilename)
{
	// on vérifie la validité de la fenêtre
	ASSERT(::IsWindow(this->m_hWnd));

	// on récupère le contexte de périphérique
	CDC* pDC = this->GetDC();
	if(pDC == NULL) return false;

    CMetaFileDC MetaDC; // création du MetaFile
	if(!MetaDC.CreateEnhanced(pDC, pFilename, NULL, NULL)) return false;

	// récupération des attributs d'origine
	MetaDC.SetAttribDC(pDC->m_hAttribDC);

	// initialisation des outils de dessin
	if(!this->OnBeginDrawing(MetaDC))
	{
		MetaDC.ReleaseAttribDC();
		MetaDC.CloseEnhanced();
		return false;
	}
	
	///////////////////////////////////////////
	// dessin du titre
	///////////////////////////////////////////

	// on crée la fonte
	CFont FontTitre;
	LOGFONT lfTitre;
	::ZeroMemory (&lfTitre, sizeof (lfTitre));
	lfTitre.lfHeight = CATPColors::GetFontSizeL();
	lfTitre.lfWeight = FW_BOLD;
	lfTitre.lfItalic = FALSE;
	lfTitre.lfUnderline = TRUE;
	::lstrcpy (lfTitre.lfFaceName, CATPColors::GetFontName());
	FontTitre.CreatePointFontIndirect (&lfTitre);

	// on sélectionne la fonte
	CFont* pOldFont = (CFont*)MetaDC.SelectObject(&FontTitre);

	// Passe en mode fond de texte transparent
	LONG OldBack = MetaDC.SetBkMode(TRANSPARENT);

	JInt32 NoMedia = -1;

	// on déclare le titre à afficher
	CString Titre;
	Titre = "Graphique Cumulativité";    // Ajouter les infos nom support etc....

	// on récupère le rectangle de la fenêtre
	// RECT  WindowRect; this->GetWindowRect(&WindowRect); // this->GetClientRect(&WindowRect); 
	RECT  WindowRect; this->GetClientRect(&WindowRect); 

	// Taille texte titre
	CSize SizeTxt = MetaDC.GetTextExtent(Titre);

	// déclaration du rectangle de titre
	RECT RectTitre;
	/*
	RectTitre.top = - SizeTxt.cy;
	RectTitre.left = (WindowRect.right - WindowRect.left - SizeTxt.cx) / 2  ;
	RectTitre.bottom = 0;
	RectTitre.right = RectTitre.left + SizeTxt.cx;
	*/

	int Val = WindowRect.left; 
	RectTitre.top = 0;
	RectTitre.left = WindowRect.left + (WindowRect.right - WindowRect.left - SizeTxt.cx) / 2  ;
	RectTitre.bottom = -SizeTxt.cy;
	RectTitre.right = RectTitre.left + SizeTxt.cx;

	// on crée le pinceau
	CBrush BrushE(CATPColors::GetColorMedium(CATPColors::COLORCHOIXSUPPORTS));

	// on dessine le fond
	// MetaDC.FillRect(&RectTitre, &BrushE);

	// on sélectionne la fonte
	MetaDC.SelectObject(&FontTitre);

	// on écrit le texte
	MetaDC.DrawText(Titre, -1, &RectTitre, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	// on restaure la transparence
	MetaDC.SetBkMode(OldBack);

	///////////////////////////////////////////
	// dessin du graphe 
	///////////////////////////////////////////

	// on dessine les bordures
	this->OnDrawItems(MetaDC, WindowRect);
	//////////////////////////////////////////////////////////

	// libération des outils
	this->OnEndDrawing(MetaDC); MetaDC.ReleaseAttribDC();

	// fermeture du fichier, on garde un handle pour la copie éventuelle dans le Presse-Papiers
	HENHMETAFILE hMetaFile = MetaDC.CloseEnhanced();
	if(hMetaFile == NULL) return false;

	// si un nom de fichier a été spécifié on termine ici avec succès
	if(pFilename == NULL)
	{
		// ouverture du presse-papiers
		if(!OpenClipboard()) return FALSE;

		// vidange du presse-papiers
		if(!EmptyClipboard())
		{
			CloseClipboard();
			return FALSE;
		}
		// envoie les données du MetaFile dans le presse-papiers
		if(SetClipboardData(CF_ENHMETAFILE, hMetaFile) == NULL)
		{
			CloseClipboard();
			return FALSE;
		}
		CloseClipboard();
	}
	else
	{
		// on supprime le métafile
		if (FALSE == ::DeleteEnhMetaFile(hMetaFile)) return (FALSE);
	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Les fonctions dessin via le Metafile
/////////////////////////////////////////////////////////////////////////////
// les fonctions pour dessiner les éléments

bool JFCCourbe::OnBeginDrawing(CMetaFileDC & MetaDC)
{
	// on vérifie la validité des éléments
	if (m_pICourbe == 0) return (false);

	// on récupère le résolution de DC
	LONG Reso = MetaDC.GetDeviceCaps(LOGPIXELSY);

	// on crée les fontes
	m_FontP.CreateFont(-MulDiv(8, Reso, 72), 0,    0,    0, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
	m_FontO.CreateFont(-MulDiv(8, Reso, 72), 0, -250, -250, FW_REGULAR, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");

	// on crée les stylos
	m_PenB.CreatePen(PS_SOLID, 0, JFCScheme::GetGreyD());
	m_PenG.CreatePen(PS_SOLID, 0, JFCScheme::GetGreyM());

	// on continue le dessin
	return (true);
}

void JFCCourbe::OnDrawItems(CMetaFileDC & MetaDC , RECT rect)
{
	// on dessine les axes
	this->OnDrawAxes(MetaDC, rect);

	// on dessine les courbes
	this->OnDrawCourbes(MetaDC, rect);
}

void JFCCourbe::OnEndDrawing(CMetaFileDC & MetaDC)
{
	// on détruit les fontes
	m_FontO.DeleteObject();
	m_FontP.DeleteObject();

	// on détruit les pinceaux
	m_Brushes.DeleteObject();

	// on détruit le stylo
	m_PenG.DeleteObject();
	m_PenB.DeleteObject();
}

void JFCCourbe::OnDrawCourbes(CMetaFileDC &MetaDC, RECT rect)
{
	// on récupère les dimensions de la zone
	long HSize = this->GetHorzSize();
	long VSize = this->GetVertSize();

	// on récupère le pas de graduation en X
	long PasGraduationX = this->GetPasGraduationX(); 

	// on récupère les bornes des graduations
	double YGrdMin = 0.0;
	double YGrdMax = m_Graduations.GetMax();

	// on initialise la hauteur référence
	long RefY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, 0.0);

	// on initialise la position courante
	long PosX = 0;
	long PosY = RefY;

	// on modifie la transparence
	long OldBck = MetaDC.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = MetaDC.SetTextColor(JFCScheme::GetGreyD());

	// on modifie le stylo
	CPen* pOldPen = MetaDC.SelectObject(&m_PenB);

	// on modifie la fonte
	CFont* pOldFont = MetaDC.SelectObject(&m_FontP);

	// on récupère le pinceau
	CBrush* pOldBrush = MetaDC.GetCurrentBrush();

	// on récupère le nombre d'éléments en ligne
	long RowCount = m_pICourbe->GetXElmtCount();

	// on récupère la position de l'ascenceur
	long FrstRow = this->OnGetHorzPosition();
	long LastRow = FrstRow + (HSize / (2 * g_BrdH + g_LrgH)); if (LastRow > RowCount) LastRow = RowCount;

	// on teste si on doit tracer la première graduation
	if (FrstRow < LastRow)
	{
		// on trace la première graduation
		MetaDC.MoveTo(PosX, RefY); MetaDC.LineTo(PosX, RefY + 4);
	}

	// Récupère départ des courbes (car selon grandeur à évaluer, pas toujours défini au début exemple : coût) 
	JArray <long> TDepRow;
	TDepRow.Reset();
	TDepRow.SetCount(m_pICourbe->GetCourbeCount());
	for (JInt32 InxCrb = 0; InxCrb < m_pICourbe->GetCourbeCount(); InxCrb++)
		TDepRow.Item(InxCrb) = (long)0;

	// on boucle sur tous les éléments en ligne
	for (long IdxRow = FrstRow; IdxRow < LastRow; IdxRow += 1)
	{
		// on récupère le nombre d'éléments en colonne
		long ColCount = m_pICourbe->GetCourbeCount();

		// Voir si étalonnage à marquer
		bool ItemMarqueHrz = false;

		// on boucle sur tous les éléments en colonne
		for (long IdxCol = 0; IdxCol < ColCount; IdxCol += 1)
		{
			// on vérifie la validité de l'élément
			if (m_pICourbe->IsItemValid(IdxRow, IdxCol))
			{
				// on récupère la valeur élément précédent et élmt courant
				bool AffichEtal = false;

				double ValuePrec	= m_pICourbe->GetValueItem(IdxRow, IdxCol, AffichEtal); if (ValuePrec < 0.0) ValuePrec = 0.0;
				double Value		= ValuePrec;
				if (IdxRow+1 < LastRow)
				{
					Value	= m_pICourbe->GetValueItem(IdxRow+1, IdxCol, AffichEtal); if (Value < 0.0) Value = 0.0;
				}
				else
					AffichEtal = false;
				

				// Récupére si affichage ou pas libellé jour (selon étalonnage)
				if (AffichEtal || IdxRow == 0)
					ItemMarqueHrz = true;

				// on récupère l'indice de la couleur de l'élément
				long IdxColor = m_pICourbe->GetCourbeColor(IdxCol); if (IdxColor < 0) IdxColor = -(IdxColor + 1);

				// on modifie le stylo
				m_PenColor.CreatePen(PS_SOLID, 0, m_pICourbe->GetColor(IdxColor));
				MetaDC.SelectObject(&m_PenColor);

				// on modifie le pinceau avec couleur
				m_Brushes.CreateSolidBrush(m_pICourbe->GetColor(IdxColor));
				MetaDC.SelectObject(&m_Brushes);

				long NewYPrec = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, ValuePrec);
				long NewY     = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, Value);

				// Tracé si on a dépassé le début courbe prédéfini
				if (IdxRow >= TDepRow.Item(IdxCol))
				{
					if(Value != 0)
					{
						// on dessine l'Courbeme
						MetaDC.BeginPath();
						MetaDC.MoveTo(PosX, NewYPrec);
						MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, NewY);
						MetaDC.EndPath();MetaDC.StrokeAndFillPath();

						JUnt32 SizePt = 1;
						if (AffichEtal)
						{
							MetaDC.MoveTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
							MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY + SizePt);
							MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY + SizePt);
							MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH + SizePt, NewY - SizePt);
							MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH - SizePt, NewY - SizePt);
							MetaDC.EndPath();MetaDC.StrokeAndFillPath();
						}
						// ou bien début de courbe
						else if (IdxRow == TDepRow.Item(IdxCol))
						{
							MetaDC.MoveTo(PosX - SizePt, NewYPrec - SizePt);
							MetaDC.LineTo(PosX - SizePt, NewYPrec + SizePt);
							MetaDC.LineTo(PosX + SizePt, NewYPrec + SizePt);
							MetaDC.LineTo(PosX + SizePt, NewYPrec - SizePt);
							MetaDC.LineTo(PosX - SizePt, NewYPrec - SizePt);
							MetaDC.EndPath();MetaDC.StrokeAndFillPath();
						}
					}

					else if (IdxRow != (LastRow - 1) && ValuePrec != 0)
					{
						// on récupère le libellé de l'élément
						const JString Libelle = m_pICourbe->GetLibCourbeGraph(IdxCol);

						// on écrit le libellé de la courbe en fin de courbe
						MetaDC.TextOut(PosX + 2 * g_LrgH + 10, NewYPrec - 10 , Libelle.AsJCharPtr());
						if (NewY == 0)
							long ValY = NewY;
					}
				}
								
				// on met à jour la position courante
				PosY = NewY;

				// Destruction Brush et Pen
				m_Brushes.DeleteObject(); 
				m_PenColor.DeleteObject(); 

				// on écrit le libellé de la courbe en fin de courbe
				if (IdxRow == LastRow - 1 && ValuePrec != 0)
				{
					// on récupère le libellé de l'élément
					// const JString Libelle = m_pICourbe->GetLabelItem(IdxCol);

					const JString Libelle = m_pICourbe->GetLibCourbeGraph(IdxCol);

                    MetaDC.TextOut(PosX + 2 * g_LrgH + 10 , NewY - 10 , Libelle.AsJCharPtr());
				}
			}

			/* Avant
			if (ItemMarqueHrz && IdxRow != 0)
			{
				// On écrit libellé graduation horizontale
				if (IdxCol == 0 || (IdxCol + 1) % PasGraduationX == 0)
				{
					CString LibGrad;
					LibGrad.Format("%d",IdxRow+1); 
					MetaDC.TextOut(PosX + 3 * g_BrdH / 2 + g_LrgH, RefY + 5, LibGrad);
				}
			}
			*/

			if (IdxRow != 0)
			{
				// On écrit libellé graduation horizontale
				if (IdxRow == 0 || (IdxRow + 1) % PasGraduationX == 0)
				{
					CString LibGrad;
					LibGrad.Format("%d",IdxRow+1); 
					MetaDC.TextOut(PosX + 3 * g_BrdH / 2 + g_LrgH, RefY + 5, LibGrad);
				}
			}
		}

		// on modifie le stylo
		MetaDC.SelectObject(&m_PenB);

		// on trace l'axe horizontal
		MetaDC.MoveTo(PosX, RefY); 
		if (ItemMarqueHrz == false || IdxRow == 0)
		{
			// Axe horizontal
			MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY); 
			MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY + 4);
		}
		else
		{
			MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY); 
			MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY - 2); 
			MetaDC.LineTo(PosX + 2 * g_BrdH + g_LrgH, RefY + 4);
		}

		// on incrémente la position
		PosX += 2 * g_BrdH + g_LrgH; PosY = RefY;
	}

	// on calcule le dépassement pour dessiner la flèche
	long DepX = (HSize + 8) / 16;

	if (LastRow > FrstRow )
	{
		// on dessine l'axe des graduations
		MetaDC.MoveTo(PosX, RefY); MetaDC.LineTo(PosX + DepX + 1, RefY);

		// on dessine la flèche de l'axe des graduations
		MetaDC.MoveTo(PosX + DepX - 1, RefY - 1); MetaDC.LineTo(PosX + DepX - 8, RefY - 4);
		MetaDC.MoveTo(PosX + DepX - 1, RefY + 1); MetaDC.LineTo(PosX + DepX - 8, RefY + 4);

		// On indique en abscisse le nombre de jours
		const JString LibAxeHorz = m_pICourbe->GetLabelResultHorz();
		MetaDC.TextOut(PosX + 5 , RefY + 10, LibAxeHorz.AsJCharPtr());
	}
	else
	{
		// on dessine l'axe des graduations
		MetaDC.MoveTo(0, RefY); MetaDC.LineTo(HSize - 100, RefY);
		
		// on dessine la flèche de l'axe des graduations
		MetaDC.MoveTo(HSize - 101, RefY - 1); MetaDC.LineTo(HSize - 108, RefY - 4);
		MetaDC.MoveTo(HSize - 101, RefY + 1); MetaDC.LineTo(HSize - 108, RefY + 4);
	}

	// on restaure le pinceau initial
	MetaDC.SelectObject(pOldBrush);

	// on restaure la fonte initiale
	MetaDC.SelectObject(pOldFont);

	// on restaure le stylo initial
	MetaDC.SelectObject(pOldPen);

	// on restaure la couleur du texte
	MetaDC.SetTextColor(OldCol);

	// on restaure la transparence
	MetaDC.SetBkMode(OldBck);
}

// les fonctions pour dessiner les axes et les Courbes
void JFCCourbe::OnDrawAxes(CMetaFileDC & MetaDC, RECT rect)
{
	// on récupère la dimension de la zone
	long VSize = this->GetVertSize();

	// on calcule le dépassement pour dessiner la flèche
	long DepY = (VSize + 8) / 16;

	// on modifie la transparence
	long OldBck = MetaDC.SetBkMode(TRANSPARENT);

	// on modifie la couleur du texte
	long OldCol = MetaDC.SetTextColor(JFCScheme::GetGreyD());

	// on modifie le stylo
	CPen* pOldPen = MetaDC.SelectObject(&m_PenB);

	// on modifie la fonte
	CFont* pOldFont = MetaDC.SelectObject(&m_FontP);

	// on dessine l'axe des graduations
	MetaDC.MoveTo(0, -DepY); MetaDC.LineTo(0, VSize);

	// on dessine la flèche de l'axe des graduations
	MetaDC.MoveTo(-1, 1 - DepY); MetaDC.LineTo(-4, 8 - DepY);
	MetaDC.MoveTo( 1, 1 - DepY); MetaDC.LineTo( 4, 8 - DepY);

	// on teste si on a des éléments
	if (m_pICourbe->GetXElmtCount() > 0)
	{
		// on récupère les bornes des graduations
	  	double YGrdMin = 0.0;
	  	double YGrdMax = m_Graduations.GetMax();

		// on récupère le nombre de graduations
		long NbGraduations = m_Graduations.GetNbGraduation();

		// on boucle sur toutes les graduations
		for (long IdxGraduation = 0; IdxGraduation < NbGraduations; IdxGraduation += 1)
		{
			// on récupère la position de la graduation
			long PosY = this->OnGetYPosition(VSize, YGrdMin, YGrdMax, m_Graduations.GetGraduation(IdxGraduation));

			// on récupère le libellé de la graduation
			CString Libelle = m_Graduations.GetLibGraduation(IdxGraduation);
			FormatString(Libelle,3);

			// on récupère la taille de la graduation
			CSize SizeY = MetaDC.GetTextExtent(Libelle);

			// on écrit le libellé de la graduation
			MetaDC.TextOut(-5 - SizeY.cx, PosY - (SizeY.cy / 2), Libelle);

			// on dessine la graduation
			MetaDC.MoveTo(-3, PosY); MetaDC.LineTo(0, PosY);
		}

		// on récupère le libellé de la grandeur utilisée
		const JString Libelle = m_pICourbe->GetLabelResultVert();

		// on récupère la taille de la graduation
		CSize SizeY = MetaDC.GetTextExtent(Libelle.AsJCharPtr());

		// on écrit le libellé de la graduation
		MetaDC.TextOut(10, -DepY - 10, Libelle.AsJCharPtr());


	}

	// on restaure la fonte
	MetaDC.SelectObject(pOldFont);

	// on restaure le stylo initial
	MetaDC.SelectObject(pOldPen);

	// on restaure la couleur du texte
	MetaDC.SetTextColor(OldCol);

	// on restaure la transparence
	MetaDC.SetBkMode(OldBck);
}