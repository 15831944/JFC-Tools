// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPResultHypo_T1.h"
#include "ItemsConstruitsManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_T1::CATPResultHypo_T1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPResultHypo_T1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPResultHypo_T1::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_T1::~CATPResultHypo_T1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_T1, JFCTitle)
	//{{AFX_MSG_MAP(CATPResultHypo_T1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'�l�ment

JVoid CATPResultHypo_T1::OnDrawItem(CDC & dc, RECT rect)
{
	CString Text = "";

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on r�cup�re libell� filtre d'audience
		CString TxtFiltre = "";
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
		if(m_pATPDocCreuset->GetIdFiltreAudienceSel() != 1)
		{
			TxtFiltre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(m_pATPDocCreuset->m_IdFiltreAudienceSel);
			Text += TxtFiltre;
			Text += " - ";
		}

		/*
		// on r�cup�re libell� de la source
		if (m_pATPDocCreuset && m_pATPDocCreuset->m_IdSrcAudienceSel)
		{
			// R�cup Table
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// R�cup la source audience courante
			JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

			// R�cup le nouveau source
			const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

			// Recuperer dans table source
			Text += pt->m_Libelle.AsJCharPtr();
		}

		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 0)
		{
			if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 1)
			{
				// Multi-terrains
				Text += " ";
				Text += "Multi-terrains";
			}
			else
			{
				// Mono-terrain
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();

				// Ident terrain s�lectionn�
				JUnt32 IdTerrainSel = m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey();

				// Test si terrain valide
				JBool IdTerrainValid = false;
				for (m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
					m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid();
					m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
				{
					if (IdTerrainSel == m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem())
					{
						IdTerrainValid = true;
						break;
					}
				}

				if (IdTerrainValid)
				{
					// R�cup Table
					const JSRCTables  & srcTables	= JSRCTables::GetInstance();

					// R�cup la source audience courante
					JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

					// R�cup le nouveau terrain
					const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,IdTerrainSel);

					// Recuperer dans table terrain
					Text += " ";
					Text += pt->m_Libelle.AsJCharPtr();
				}
			}
		}
		*/

		// Cas d'une cible normale s�lectionn�e
		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// on recup�re libell� de la cible courante
			m_pATPDocCreuset->m_MAPpItems.MoveTo(m_pATPDocCreuset->m_IdItemSel);
			if(m_pATPDocCreuset->m_MAPpItems.IsValid())
			{
				// R�cup id cible
				JUnt32 Id = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
				if(Id)
				{
					// R�cup Table des cibles
					// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

					// R�cup cible correspondant � l'indice
					// IItem * pItem = pItemConst->GetItemConstruit(Id);
					IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(Id);

					// Si cible trouv�, on retourne son libell�
					if(pItem)
					{
						// Recuperer libell� cible en cours
						Text += pItem->GetLabel().AsJCharPtr();

						// R�cup�re effectif cible courante
						JFlt64 EffectifCible = 0.0;
						
						// on cr�e la cl� des effectifs
						CKeyMoteur keyMoteur;
						// on initialise la cl�
						// A FAIRE JEREMY
						m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
						if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
							keyMoteur.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
						// on initialise l'effectif
		
						EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);
		
						// si effectif <> 0 ajoute info effectif
						if (EffectifCible > 0)
						{
							CString TxtEffectif;
							Text += " (";

							TxtEffectif.Format("%.0f",EffectifCible); 
							CString Fmt = _T("### ### ### ### ###");
							CFormatStr::FormatNum(Fmt, TxtEffectif);
							Text += TxtEffectif;

							Text += " ";
							//on ajoute le libell� du type de poids s�lectionn�
							const JSRCTables  & srcTables	= JSRCTables::GetInstance();
							Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);
							Text += ")";
						}

					}
				}
			}
		}
		else
		{
			// On recup�re libell� de la cible pond�r�e courante
			m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
			if(m_pATPDocCreuset->m_MAPpItemsPonderes.IsValid())
			{
				// R�cup id cible pond�r�e
				JUnt32 Id = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();
				if(Id)
				{
					// R�cup Table des cibles
					// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

					// R�cup cible correspondant � l'indice
					// IItem * pItem = pItemConst->GetItemPondere(Id);
					IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(Id);

					// Si cible trouv�, on retourne son libell�
					if(pItem)
					{
						// Recuperer libell� cible en cours
						Text += pItem->GetLabel().AsJCharPtr();


						// A FAIRE ALAIN - HK
						// R�cup�re effectif cible courante
						JFlt64 EffectifCible = 0.0;

						// on cr�e la cl� des effectifs
						CKeyMoteur keyMoteur;
						// on initialise la cl�
						// A FAIRE JEREMY
						m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
						if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
							keyMoteur.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
						// on initialise l'effectif

						EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

						// si effectif <> 0 ajoute info effectif
						if (EffectifCible > 0)
						{
							CString TxtEffectif;
							Text += " (";

							TxtEffectif.Format("%.0f",EffectifCible); 
							CString Fmt = _T("### ### ### ### ###");
							CFormatStr::FormatNum(Fmt, TxtEffectif);
							Text += TxtEffectif;

							Text += " ";
							//on ajoute le libell� du type de poids s�lectionn�
							const JSRCTables  & srcTables	= JSRCTables::GetInstance();
							Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);
							Text += ")";
						}
					}
				}
			}
		}

		// Les segments
		// + nom segment (si +, seg.multiples)
		if (m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetCount() > 1)
			// Ici indique plusieurs segments
			Text.Append(" - Multi Segments");
		else
		{
			// Source Table
			const JSRCTables & Sources = JSRCTables::GetInstance ();

			// Id Segment de ciblage
			JUnt32 IdSeg;
			m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
			IdSeg = m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetItem();

			// Affiche ssi segment diff�rent de ensemble
			if (IdSeg != 1)
			{
				const JSegment* Seg = Sources.m_TBLSegments.GetItem(IdSeg);
				Text += " - Segment : ";
				Text += Seg->m_Libelle.AsJCharPtr();
			}
		}
	}

	if (Text=="")
		Text.LoadString(IDS_CR_RESULTATS);

	// Font sp�cifique (bold et taille moyenne 12)
	LOGFONT lf;
	dc.GetCurrentFont()->GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	// lf.lfHeight = -16;
	CFont fontBold;
	fontBold.CreateFontIndirect(&lf);

	// Sauve ancienne font
	CFont* pOldFont = dc.SelectObject(&fontBold);

	// Ecriture texte avec font courante
	dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	// Remet ancienne Font
	dc.SelectObject(pOldFont);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPResultHypo_T1::OnUpdate()
{
	// on redessine la fen�tre
	this->InvalidateRect(NULL, FALSE);
}

///////////////////////////////////////
// CATPResultHypo_T1 message handlers

void CATPResultHypo_T1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
