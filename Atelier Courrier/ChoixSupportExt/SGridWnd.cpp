// SGridWnd.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "SGridWnd.h"
#include <afxadv.h>            // For CSharedFile
#include "AllTypeOfIterator.h"
#include "AllTypeOfVisitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CSGridWnd, CGXGridWnd)

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructeur
CSGridWnd::CSGridWnd(void)
{
	// A VOIR ToolTip Grid Analyseur
	this->EnableToolTips(true); 

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;
	m_dwToolTipStyle = TTS_ALWAYSTIP;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destructeur
CSGridWnd::~CSGridWnd(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Init document choix support associé et dialogue
JBool CSGridWnd::OnInitDoc(JATPDocChoixSupport *pDoc, JUnt32 NoColIdSupport)
{
	// Récupére document choix support
	m_pDocChoixSupport = pDoc;

	// Récupére n° colonne id support
	m_NoColIdSupport   = NoColIdSupport;

	// Test si valide
	if (m_pDocChoixSupport->IsValid()) 
		return true;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifie le contenu pour trier une colonne d'un grid
void CSGridWnd::FormatCellTri(JUnt32 NoColonne, JUnt32 NbHeaderRow, JUnt32 NoColDeb, JBool &AffichPrct)
{
	// Delock mode read only
	GetParam()->SetLockReadOnly(FALSE);

	// Balayage de toutes les lignes
	CString Txt;
	for (JUnt32 NoLig = NbHeaderRow; NoLig <= GetRowCount(); NoLig++)
	{
		if (NoColonne > NoColDeb - 1)
		{
			// Récupère le texte résultat
			Txt = GetValueRowCol(NoLig, NoColonne);

			// Attention si %
			if (Txt.Find("%",0) != -1)
			{
				// Indique info %
				AffichPrct = true;

				// Vire % pour le tri
				Txt.Replace("%",""); 
			}

			if (Txt == "*") 
			{	
				Txt = "1";

				// Complete par des blancs pour le tri
				// CompleteString(Txt,15);

				// Remplace momentannément "*" par -1
				Txt = "-" + Txt;
			}	
			else
				// Complete par des blancs pour le tri
				CompleteString(Txt,15);
				
			// Remplace "," par "." pour les tris numériques
			Txt.Replace(",","."); 
			
			// Réinjecte valeur formatée
			SetValueRange(CGXRange(NoLig,NoColonne),Txt); 
		}
	}		
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Remet affichage de base
// enum {FMT_TEXT, FMT_TEXT_MILLIERS, FMT_INT, FMT_FLOAT_1DEC, FMT_FLOAT_2C};
void CSGridWnd::FormatCellFinal(JUnt32 NoColonne, JUnt32 NbHeaderRow, JUnt32 NoColDeb, JBool &AffichPrct)
{
	// Delock mode read only
	GetParam()->SetLockReadOnly(FALSE);

	// Balayage de toutes les lignes
	CString Txt;
	for (JUnt32 NoLig = NbHeaderRow; NoLig <= GetRowCount(); NoLig++)
	{
		if (NoColonne > NoColDeb - 1)
		{
			// Récupère le texte résultat
			Txt = GetValueRowCol(NoLig, NoColonne);

			// Remplace -1 par *
			if (atoi(Txt) < 0) 
				Txt = "*";
			else
			{
				// GROSSE BIDOUILLE SYLVAIN
				JInt32 NbDecimales = 0;
				JInt32 PosDec = Txt.Find(",");
				if (PosDec < 0)
					PosDec = Txt.Find(".");
				if (PosDec >= 0)
				{
					for (JInt32 idx = PosDec + 1; idx < Txt.GetLength(); idx += 1)
						if(Txt.GetAt(idx) >= '0' && Txt.GetAt(idx) <= '9')
							NbDecimales += 1;
				}

				// Reconstruit la chaine par milliers si nécessaires
				RebuildString(Txt, FMT_TEXT_MILLIERS, NbDecimales);
				
				// Remplace "." par "," pour les tris numériques
				// Txt.Replace(".",","); 

				// Affiche prct s'il y a lieu
				if (AffichPrct){
					Txt.TrimRight();
					Txt += "%";
				}
			}

			// Réinjecte valeur formatée
			SetValueRange(CGXRange(NoLig,NoColonne),Txt); 
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Blocage des colonnes normales (en dehors des combobox, editbox etc....)
void CSGridWnd::SetColReadOnly(JUnt32 NbHeaderRow, JUnt32 NoColIdSupport,JUnt32 NoColSupport, JUnt32 NoColPeriodicite)
{
	// Lock mode read only
	GetParam()->SetLockReadOnly(TRUE);

	// NbCol
	JUnt32 NbCol = GetColCount();

	// NbRow
	JUnt32 NbRow = GetRowCount();

	if (NbCol > 0 && NbRow > 0)
	{
		// Toutes les cellules des colonnes sauf colonne format
		SetStyleRange(CGXRange(NbHeaderRow-1, NoColIdSupport-1, NbRow, NoColSupport-1),
					  CGXStyle()
						.SetReadOnly(true));

		SetStyleRange(CGXRange(NbHeaderRow-1, NoColPeriodicite-1, NbRow, NbCol),
					  CGXStyle()
						.SetReadOnly(true));
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Complète chaine numérique avec des caractères devant (pour palier au problème tri)
void CSGridWnd::CompleteString(CString &Txt, JUnt32 NbTotCar)
{
	// Nouvelle chaine
	CString NewTxt = Txt;
	for (int i = Txt.GetLength() ; i < NbTotCar; i++)
		// NewTxt = "0" + NewTxt;
		NewTxt = " " + NewTxt;

	Txt = NewTxt;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Recompose la chaine selon le formatage demandée
void CSGridWnd::RebuildString(CString &Txt, JUnt32 ModeRebuild, JUnt32 NbDecimale)
{
	if (Txt.Find("*", 0) > 0)
	{
		// Traitement spécifique des résultats non déterminés
		Txt = "*";
		return;
	}

	if (ModeRebuild == FMT_TEXT_MILLIERS)
	{
		// Init nouvelle chaine de caractère
		CString NewTxt = "";

		// Vire les espaces
		for (int i = 0; i < Txt.GetLength(); i++)
		{
			if (Txt.Mid(i,1) != " ") 
				NewTxt += Txt.Mid(i,1);
		}
		
		// récupére chaine sans les espaces
		Txt = NewTxt;

		// replace "." par ","
		// Txt.Replace(".",",");
		CString Dec;
		Dec.AppendChar((TCHAR)CFormatStr::GetDecimalPoint());
		Txt.Replace(".",Dec);

		// Prendre la valeur de la chaine
		if (Txt.Find(".",0) > 0 || Txt.Find(",",0) > 0) 
		{
			// Ici chaine numérique avec virgule flottante
			JFlt64 ValTxt = atof(Txt);

			// Refaire la chaine
			if (ValTxt > 0.0)
				if (NbDecimale == 1)
					Txt.Format("%0.1f",ValTxt);
				else
					// Pas plus de 2 décimales
					Txt.Format("%0.2f",ValTxt);
			else
				Txt = "0";

			// Ajoute les espaces
			FormatString(Txt,3);
		}
		else
		{
			// Chaine numérique entière
			JUnt32 ValTxt = atoi(Txt);

			// Refaire la chaine avec les espaces
			if (ValTxt > 0)
				Txt.Format("%d",ValTxt);
			else
				Txt = "0";

			// Ajoute les espaces
			FormatString(Txt,3);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Découpage chaine par milliers
void CSGridWnd::FormatString(CString &Txt, JUnt32 PasDecoup)
{
	// Recupere les decimales
	CString TxtDec = " ";
	int PosDec = 0;
	PosDec = Txt.Find(".");
	if (PosDec < 0)
		PosDec = Txt.Find(",");
	if (PosDec > 0)
	{
		TxtDec = Txt.Mid(PosDec) + " ";
		Txt = Txt.Left(PosDec); 
	}

	// Nouvelle chaine
	CString NewTxt = "";

	// Formatage new chaine
	while (Txt.GetLength() > PasDecoup)
	{
		// Ajoute bloc new chaine
		NewTxt = Txt.Mid(Txt.GetLength() - PasDecoup, PasDecoup) + " " + NewTxt;

		// Enleve bloc old chaine
		Txt.Delete(Txt.GetLength() - PasDecoup, PasDecoup); 
	}

	// S'il reste des caractère, on les ajoute
	if (Txt.GetLength() > 0)
		NewTxt = Txt + " " + NewTxt;

	// Puis on récupère la chaine finale
	Txt = NewTxt;

	// Vire espace si à la fin
	Txt = Txt.TrimRight(); 

	// Ajoute les décimales s'il y a lieu
	Txt += TxtDec;
}

void CSGridWnd::InsertMenu(INode* nd, CMenu & menu, int & i )
{
	IIterateur *ite = nd->CreateIterateur();
	//on ne le fait que si on a un iterateur (node!=Leaf)
	if (ite)
	{
		INode* node = ite->getHead();
		if ((node && node->IsTitre()) || (node == NULL))
		{
			i++;
			m_mapMenu.Add(WM_USER+1+i) =  nd->GetID();
			menu.AppendMenu(MF_STRING, WM_USER+1+i, nd->GetLabel().AsJCharPtr() );
		}
		else
		{
			CMenu menuFils;
			menuFils.CreatePopupMenu();
			while(node != NULL)
			{
				//recursivité
				InsertMenu(node, menuFils, i);
				node = ite->getNext();
			}
			menu.AppendMenu(MF_POPUP, (UINT)menuFils.m_hMenu, nd->GetLabel().AsJCharPtr());
		}
		delete ite;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Gestion Menu Popup Grid Résultat
BEGIN_MESSAGE_MAP(CSGridWnd, CGXGridWnd)
	ON_COMMAND_RANGE(WM_USER + 10, WM_USER + 1000,	OnFamilleCentre)
	ON_COMMAND(WM_USER,								OnInfoTitre)
	ON_COMMAND(WM_USER + 1,							OnCacheSupport)
	ON_COMMAND(WM_USER + 2,							OnAllSupportVisible)
	ON_COMMAND(WM_USER + 3,							OnOnlySupportSelVisible)
	ON_COMMAND(WM_USER + 4,							OnOnlySupportNonSelVisible)
	ON_COMMAND(WM_USER + 5,							OnModifTarif)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage des familles
void CSGridWnd::OnFamilleCentre(UINT nID)
{
	// Test option menu valide
	m_mapMenu.MoveTo(nID);
	if (!m_mapMenu.IsValid())
		return;

	// Regroupement pour les informations famille
	CRegroupements * pReg = CRegroupements::GetInstance();
	CLot * lot = pReg->GetLotByID(m_mapMenu.GetItem());

	// Bouble sur tous les supports sélectionnés
	for(m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// Récupére l'identifiant support
		JUnt32 idSupport = 	m_pDocChoixSupport->m_MapIdSupportsSel.GetKey();
		if (m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() == 1)
		{
			// Récupére élmt support
			IElem* pSupport = pReg->GetIElemByID(idSupport);
			
			// Recherche titre
			if (lot && pSupport && lot->GetFilsByID(idSupport) == NULL && pSupport->IsTitre() && !pSupport->IsCouplageLibre() && !pSupport->IsTitreApparie())
			{
				INode * titre;
				if (pSupport->IsTitreApparente())
					titre = new CTitreApparente(idSupport, pSupport->GetLabel().AsJCharPtr(), pSupport->AsTitreApparente()->GetSupportTuteur(), pSupport->AsTitreApparente()->GetMediaWeight() , lot->GetID() );
				else
					titre = new CTitre(idSupport, pSupport->GetLabel().AsJCharPtr(), lot->GetID() );
				CVisiteurInsert vInsert;
				titre->Accept(&vInsert);
				AfxGetApp()->m_pMainWnd->PostMessage(ATPMSG_SUPPORT_TREE_NEEEDUPD,0,0);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Information Titre
void CSGridWnd::OnInfoTitre()
{
	// Récup ident support en cours
	m_mapMenu.MoveTo(WM_USER);
	if (!m_mapMenu.IsValid())
		return;

	// Récup élément support
	JUnt32 IdSupport = m_mapMenu.GetItem(); 

	// Attention aux lignes spéciales (moyenne etc....)
	if (IdSupport == 0) return;

	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem* pSupport = pReg->GetIElemByID(IdSupport);

	// Affiche les infos supports
	// A COMPLETER en affichant un beau panneau avec Logo Support et Informations générales
	CString TxtInfo;
	TxtInfo = "Information support : ";
	TxtInfo += pSupport->GetLabel().AsJCharPtr();
	AfxMessageBox(TxtInfo);  
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Cache Titre ou paquet de titres
void CSGridWnd::OnCacheSupport()
{
	// Flag pour mise à jour à faire si cache actif
	JBool CacheActif = false;

	// Récup ident support en cours
	m_mapMenu.MoveTo(WM_USER);
	if (!m_mapMenu.IsValid())
		return;

	// Récup élément support
	JUnt32 IdSupport = m_mapMenu.GetItem(); 

	// Puis ajout les autres supports non sélectionnés mais marqués
	CRowColArray TabRowSel;
	this->GetSelectedRows(TabRowSel);

	// Boucle sur les supports marqués
	for (int i = 0; i < TabRowSel.GetCount(); i++)
	{
		// No de la ligne support marqué
		int nRow =TabRowSel.GetAt(i);

		// Récup Ident support
		JUnt32 IdSupport = (JUnt32) atoi(GetValueRowCol(nRow, m_NoColIdSupport-1));

		// Attention aux lignes spéciales (moyenne etc....)
		if (IdSupport == 0) continue;

		// Attention pas de cache pour les supports plateau (sélectionnés pou le plateau)
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
		if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid() && m_pDocChoixSupport->m_MapIdSupportsSel.GetItem())
			continue;

		// Cache le support
		m_pDocChoixSupport->SetSupportVisible(IdSupport, false); 

		// Indique cache actif
		CacheActif = true;
	}

	// Mise à jour de la grille échelle si au moins 1 cache à effectuer
	if (CacheActif)
		m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Désélectionner de toute la grille
	this->SelectRange(CGXRange( ).SetTable( ), FALSE);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Supports tous visibles
void CSGridWnd::OnAllSupportVisible()
{
	// Rend visible tous les supports
	m_pDocChoixSupport->SetAllSupportVisible(true); 

	// Mise à jour des grilles supports
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Désélectionner de toute la grille
	this->SelectRange(CGXRange( ).SetTable( ), FALSE);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Supports seuelemnt les supports sélectionnés visibles
void CSGridWnd::OnOnlySupportSelVisible()
{
	// Rend visible tous les supports
	m_pDocChoixSupport->SetAllSupportSelVisible(true); 

	// Mise à jour de la grille échelle
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Désélectionner de toute la grille
	this->SelectRange(CGXRange( ).SetTable( ), FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Supports seulement les supports non sélectionnés visibles
void CSGridWnd::OnOnlySupportNonSelVisible()
{
	// Rend visible tous les supports
	m_pDocChoixSupport->SetAllSupportNonSelVisible(true); 

	// Mise à jour de la grille échelle
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);

	// Désélectionner de toute la grille
	this->SelectRange(CGXRange( ).SetTable( ), FALSE);
}

///////////////////////////////////////////////////////////////////////////////////////
// Gestion de l'option menu Supports seulement les supports non sélectionnés visibles
void CSGridWnd::OnModifTarif()
{
	// Afficher les bases tarifs utilisateurs
	CString UserPath = AfxGetApp()->GetProfileString("Data", "UserPath", 0);
	
	// Récupération liste des tarifs

	// Liste des bases utilisateurs
	CFile File;
	CString NomFichier;
	
	// Ouverture du fichier
	if (File.Open(NomFichier, CFile::modeRead | CFile::typeText)) 
	{
		try
		{
			// Lecture des différents champs tarif user
			// Creer objet enregistrement tarif user
		}
		catch (CFileException *e)
		{
			AfxMessageBox("Lecture impossible tarifs user");
		}
	}
	else
		AfxMessageBox("Lecture impossible tarifs user");
	
	/*
	if (CreateDirectory())
		AfxMessageBox("Modif Tarif");
	else
		AfxMessageBox("Erreur create directory");
	*/

	// Mise à jour des grilles suite à modif tarif
	/* A FIARE
	m_pDocChoixSupport->UpdateAllViews(UPDATE_MODIF_TARIFS);
	*/
}

///////////////////////////////////////////////////////////////////////////////////////
// Menu popup sans accés familles support
void CSGridWnd::AffichPopupMenu(JUnt32 IdSupport, CPoint pt, JBool AffichPopupFamille)
{
	UINT StyleNormal = 0;
	UINT StyleGrise  = MF_DISABLED | MF_GRAYED;

	// Voir si aucun support sélectionné visible
	JBool AuMoins1SelSuppVisible = false;
	for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// Récupère ident support et état sélection
		JUnt32 IdSupp = m_pDocChoixSupport->m_MapIdSupportsSel.GetKey();
		JBool Select  = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
			
		// Vérifie que si ce support est sélectionné, il est aussi visible
		if (m_pDocChoixSupport->GetSupportVisible(IdSupp) && Select == true)
		{
			// Ok au moins 1 sélectionné de visible
			AuMoins1SelSuppVisible = true;
			break;
		}
	}

	// Comptabilise le nombre de supports visible
	JUnt32 NbSuppVisible = m_pDocChoixSupport->GetNbSupportsVisible(); 

	//si on clik sur un element de plateau sélectionné
	if (AffichPopupFamille)
	{
		// Init Menu Popup
		m_mapMenu.Reset();
		
		// Création ss-menu popup arbre familles
		CRegroupements * pReg = CRegroupements::GetInstance();
		CMenu menuArbre;
		menuArbre.CreatePopupMenu();
		int i = 0;
		for (pReg->GetFamilles()->MoveFirst(); 
				pReg->GetFamilles()->IsValid();
				pReg->GetFamilles()->MoveNext())
				InsertMenu( pReg->GetFamilles()->GetItem(), menuArbre, i );

		// Accrochage ss-menu famille au menu popup général
		CMenu menuParent;
		menuParent.CreatePopupMenu();
		CString str;
		str.LoadString(IDS_CS_MENUFAMILLE);
		menuParent.AppendMenu(MF_POPUP, (UINT)menuArbre.m_hMenu, str);

		// Option Info Titre
		m_mapMenu.Add(WM_USER) =  IdSupport;
		menuParent.AppendMenu(MF_SEPARATOR);
		menuParent.AppendMenu(MF_STRING, WM_USER,"Information Titre");	

		// Séparateur
		menuParent.AppendMenu(MF_SEPARATOR);

		// Option Cacher - Grisé si support sélectionné
		UINT Style = StyleNormal;

		// On Vérifie sélection du support
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
		if (m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() || NbSuppVisible == 1)
		{	
			// Option cache inutile si sélectionné
			Style = StyleGrise;
		}
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 1,"Cacher Support(s)");	
		
		// Option Décacher tous les supports
		Style = StyleNormal;
		if (m_pDocChoixSupport->GetNbSupportsVisible() == m_pDocChoixSupport->m_MapIdSupportsVisible.GetCount())
		{
			// Option restauration globale inutile si tous déjà visibles
			Style = StyleGrise;
		}

		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 2, "Restaurer univers d'analyse");	

		// Option Seulement Supports sélectionnés visibles
		Style = StyleNormal;
		if ((m_pDocChoixSupport->GetNbSupportsSel() == 0) ||
			(m_pDocChoixSupport->GetNbSupportsVisible() == m_pDocChoixSupport->GetNbSupportsSel()) ||
			(m_pDocChoixSupport->GetNbSupportsSel() == m_pDocChoixSupport->m_MapIdSupportsSel.GetCount()) ||
			 AuMoins1SelSuppVisible == false)
		{
			// Option inutile si aucun sélectionné
			Style = StyleGrise;
		}
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 3, "Restreindre aux supports sélectionnés");	

		// Option Seulement Supports non sélectionnés visibles
		Style = StyleNormal;

		
		if ((m_pDocChoixSupport->GetNbSupportsSel() == 0) || 
			(m_pDocChoixSupport->GetNbSupportsSel() == m_pDocChoixSupport->m_MapIdSupportsSel.GetCount()) ||
			AuMoins1SelSuppVisible == false)
		{
			Style = StyleGrise;
		}
        menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 4, "Exclure de l'analyse les supports sélectionnés");

		// Separateur
		menuParent.AppendMenu(MF_SEPARATOR);

		// Option modification tarif - Pour le moment non valide
		Style = StyleGrise;
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 5, "Modification tarif");	

		// Position du popmenu
		menuParent.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	else
	{
		// Reset sélection supports
		m_mapMenu.Reset();
		m_mapMenu.Add(WM_USER) =  IdSupport;

		// Uniquement option Information Titre
		CMenu menuParent;
		menuParent.CreatePopupMenu();
		CString str;
		str.LoadString(IDS_CS_MENUFAMILLE);
		menuParent.AppendMenu(MF_STRING,WM_USER + 10, str);
		menuParent.EnableMenuItem(WM_USER+1, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED); 

		// Option Info Titre
		menuParent.AppendMenu(MF_SEPARATOR);
		menuParent.AppendMenu(MF_STRING, WM_USER,"Information Titre");	

		// Option Cacher si support non sélectionné
		menuParent.AppendMenu(MF_SEPARATOR);

		// Option Cacher - Grisé si support sélectionné
		UINT Style = StyleNormal;

		// On Vérifie sélection du support
		m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdSupport);
		if (m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() || NbSuppVisible == 1)
		{
			// Option cache inutile si sélectionné
			Style = StyleGrise;
		}
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 1,"Cacher Support(s)");	
		
		// Option Décacher tous les supports
		Style = StyleNormal;
		if (m_pDocChoixSupport->GetNbSupportsVisible() == m_pDocChoixSupport->m_MapIdSupportsVisible.GetCount())
		{
			// Option restauration globale inutile si tous déjà visibles
			Style = StyleGrise;
		}
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 2, "Restaurer univers d'analyse");	

		// Option Seulement Supports sélectionnés visibles
		Style = StyleNormal;
		if ((m_pDocChoixSupport->GetNbSupportsSel() == 0) || 
			(m_pDocChoixSupport->GetNbSupportsVisible() == m_pDocChoixSupport->GetNbSupportsSel()) ||
			(m_pDocChoixSupport->GetNbSupportsSel() == m_pDocChoixSupport->m_MapIdSupportsSel.GetCount()) ||
			AuMoins1SelSuppVisible == false)
		{
			// Option inutile si aucun sélectionné
			Style = StyleGrise;
		}
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 3, "Restreindre aux supports sélectionnés");	

		// Option Seulement Supports non sélectionnés visibles
		Style = StyleNormal;

		if  ((m_pDocChoixSupport->GetNbSupportsSel() == 0) ||
			 (m_pDocChoixSupport->GetNbSupportsSel() == m_pDocChoixSupport->m_MapIdSupportsSel.GetCount()) ||
			  AuMoins1SelSuppVisible == false)
		{
			// Option inutile si tous sélectionné
			Style = StyleGrise;
		}
        menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 4, "Exclure de l'analyse les supports sélectionnés");

		// Separateur
		menuParent.AppendMenu(MF_SEPARATOR);

		// Option modification tarif - Pour le moment non valide
		Style = StyleGrise;
		menuParent.AppendMenu(MF_STRING | MF_BYCOMMAND | Style, WM_USER + 5, "Modification tarif");	

		// Position du popmenu
		menuParent.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Affichage informations cellule
void CSGridWnd::AfficheInfoCellule(ROWCOL nRow, ROWCOL nCol, CString TxtToolTip, bool AffichOk)
{
	if (AffichOk)
	{
		// Affichage info supplémentaire cellule (par exemple marge d'erreur réelle)
		this->ActivateTooltip(TRUE);
		this->SetTooltipText(TxtToolTip);
	}
	else
	{
		// Désaffiche le tooltip
		this->ActivateTooltip(FALSE);
	}
}

// Affiche composants famille en mode tooltip
bool CSGridWnd::VisuCompoFamille(JUnt32 IdFamille, ROWCOL nRow, ROWCOL nCol)
{
	// Test si c'est bien une famille ou un support valide
	if (IdFamille != 0)
	{
		// On se positionne sur ce support dans le map des familles
		m_pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(IdFamille);

		//si on clik sur un element de plateau
		if (m_pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// Fabrique liste des supports
			CString StrCompoFamille ="";
			
			// Récupérer la liste des composants
			CRegroupements * pReg = CRegroupements::GetInstance();
			IElem *pElem = pReg->GetIElemByID(IdFamille);

			// Crée liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				// Récupère la liste des titres
				pLot->GetFils(ListSup);

				// Nombre de composants famille
				JUnt32 NbCompo = ListSup.GetCount(); 

				// Fabrique le tooltip text des composants
				for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					// Saut de ligne 
					if (StrCompoFamille != "")
					{
						// Ssi pas trop d'élémnt, sinon on met bout à bout
						if (NbCompo < 30)
							StrCompoFamille += "\n";
						else
							StrCompoFamille += " ,";
					}

					// Récup composant famille
					JUnt32 IdCompo = ListSup.GetItem(); 
					IElem *pCompo  = pReg->GetIElemByID(IdCompo);

					// Recup libellé composant
					JLabel LibCompo = pCompo->GetLabel();

					// Récupére map des supports valides sur les segments en cours
					JMap <JUnt32,JUnt32> MapSuppSegTerr;
					m_pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

					// On vérifie que le support existe sur l'enquête en cours, sinon indique info *
					MapSuppSegTerr.MoveTo(IdCompo); 
					if (!MapSuppSegTerr.IsValid())
					{
						StrCompoFamille += "* ";
					}
					
					// Ajoute à la liste tooltip
					StrCompoFamille += LibCompo.AsJCharPtr();
				}
			}

			// Menu popup avec accés familles support
			if (StrCompoFamille != "")
				this->AfficheInfoCellule(nRow, nCol, StrCompoFamille, true); 
			else
				this->AfficheInfoCellule(nRow, nCol, "Aucun support", true); 

			// C'est bien une famille
			return true;
		}
		else
			// Ce n'est pas une famille
			return false;
	}

	// Ce n'est ni un support, ni une famille 
	return false;
}

void CSGridWnd::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
}

void CSGridWnd::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this, m_dwToolTipStyle);
		// Create inactive
		m_ToolTip.Activate(FALSE);
		// Enable multiline
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		m_ToolTip.SetDelayTime(300);
	} 
}

// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	lpszText
//				Pointer to a null-terminated string containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CSGridWnd::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (lpszText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} 

// This function sets the text to show in the button tooltip.
//
// Parameters:
//		[IN]	nText
//				ID number of the string resource containing the text to show.
//		[IN]	bActivate
//				If TRUE the tooltip will be created active.
//
void CSGridWnd::SetTooltipText(int nText, BOOL bActivate)
{
	CString sText;

	// Load string resource
	sText.LoadString(nText);
	// If string resource is not empty
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
}

BOOL CSGridWnd::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	return CGXGridWnd::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage

//On tool tip test
// int CAnCrossTable::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
int CSGridWnd::OnToolHitTest(CPoint point, TOOLINFO * pTI)
{
	LVHITTESTINFO lvhitTestInfo;
	lvhitTestInfo.pt = point;
		
	this->HitTest(point);

	return -1;
}

//On tooltip text
BOOL CSGridWnd::OnToolTipText(UINT /*id*/, NMHDR *pNMHDR, LRESULT *pResult)
{
	UINT nID = pNMHDR->idFrom;
		
	// check if this is the automatic tooltip of the control
	if (nID == 0) 
		return TRUE;	// do not allow display of automatic tooltip,
						// or our tooltip will disappear

	// handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	*pResult = 0;
	
	// get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT(pMessage);
	CPoint pt;
	pt = pMessage->pt;		// get the point from the message
	ScreenToClient(&pt);	// convert the point's coords to be relative to this control
	
	// A FAIRE
	//
		
	return FALSE;	// we didn't handle the message, let the 
					// framework continue propagating the message
}