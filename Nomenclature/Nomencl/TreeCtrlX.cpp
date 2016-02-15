// TreeCtrlX.cpp : implementation file
//

#include "stdafx.h"
#include "Nomenclature.h"
#include "TreeCtrlX.h"
#include "Noeud.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX

CTreeCtrlX::CTreeCtrlX()
{
}

CTreeCtrlX::~CTreeCtrlX()
{
}


// Chargement des différentes images associées à l'arbre
bool CTreeCtrlX::LoadBitmapNormal(UINT NormalId,int SizeInPixel)
{	
	ImageNormal.Create(NormalId, SizeInPixel, 0, RGB(255,255,255));	
	SetImageList(&(ImageNormal),TVSIL_NORMAL);
	return 0;
}


bool CTreeCtrlX::PresenteArbre(CNoeud *ArbreProduitMem,CNoeud *Arbre,HTREEITEM &m_Tree,CString &NomBranche1,CString &NomBranche2,CString &NomBranche3,HTREEITEM &HItemDossierMarque)

{

	HTREEITEM HItem,HItem2;
	HTREEITEM m_Tree2;
	HTREEITEM HItemProduitMarque;
	HTREEITEM HItemProduitFamille;
	CFileFind Finder;
	CString FileName;
	BOOL Filefind = FALSE ;
	CString TxtMarque;
	CNoeud *pNoeudG;
	CNoeud *pNoeudA;
	CNoeud *pNoeudP;
	int NbNoeud;
	CString DateExercice;
	
	if (Arbre->m_pRacine == NULL) 
	{
		// Les 2 racines Marque et produit
		if (Arbre->m_Libelle == NomBranche1)
		{
			// Arborescence des Marques
			Arbre->m_CodeImage = ImageMarque;
			HItemDossierMarque = InsertItem(Arbre->m_Libelle,ImageClose,ImageOpen);
			SetItemData(HItemDossierMarque,(DWORD)Arbre);
			for (int i = 0;i < Arbre->m_pNoeud.GetSize();i++)
				PresenteArbre(ArbreProduitMem,Arbre->m_pNoeud[i],HItemDossierMarque,NomBranche1,NomBranche2,NomBranche3,HItemDossierMarque);
		}
		else if (Arbre->m_Libelle == NomBranche2)
		{

			// Arborescence des Produits
			Arbre->m_CodeImage = ImageGroupe;
			CString Libel = Arbre->m_Libelle;
			m_Tree2 = InsertItem(Arbre->m_Libelle,ImageClose,ImageOpen);

			SetItemData(m_Tree2,(DWORD)Arbre);
			for (int i = 0;i < Arbre->m_pNoeud.GetSize();i++)
			{				
				CString Libel = Arbre->m_pNoeud[i]->m_Libelle;
				PresenteArbre(ArbreProduitMem,Arbre->m_pNoeud[i],m_Tree2,NomBranche1,NomBranche2,NomBranche3,HItemDossierMarque);
			}
		}

		else if (Arbre->m_Libelle == NomBranche3)
		{

			// Arborescence des Secteurs
			Arbre->m_CodeImage = ImageFamille;
			m_Tree2 = InsertItem(Arbre->m_Libelle,ImageClose,ImageOpen);
			SetItemData(m_Tree2,(DWORD)Arbre);
			for (int i = 0;i < Arbre->m_pNoeud.GetSize();i++)	
				PresenteArbre(ArbreProduitMem,Arbre->m_pNoeud[i],m_Tree2,NomBranche1,NomBranche2,NomBranche3,HItemDossierMarque);
		}


	}

	else
	{
		// Les ss branches avec les fichiers s'ils existent
		if (Arbre->m_Libelle != "") 
	    {
			switch (Arbre->m_Niveau)
			{
				case 1 :
				{
					if(Arbre->m_pRacine->m_Libelle == NomBranche1)

					{	// Branche MARQUE
						HItem = InsertItem(Arbre->m_Alias,ImageMarque,ImageMarque,m_Tree);
						Arbre->m_CodeImage = (ImageMarque * 100) + ImageProduit;
						SetItemData(HItem,(DWORD)Arbre);

						// Ajout Branche Produit et Campagne
						if (ArbreProduitMem != NULL)
						{
							// niveau groupe
							for (int i=0;i<ArbreProduitMem->m_pNoeud.GetSize();i++)
							{

								pNoeudG = ArbreProduitMem->m_pNoeud[i];
								
								// MODIF DATE EXERCICE
								DateExercice = "";
								if (pNoeudG->m_DateExercice != "" && pNoeudG->m_DateExercice != "xxx")  
									DateExercice = pNoeudG->m_DateExercice;

								// niveau annonceur
								for (int j=0;j < pNoeudG->m_pNoeud.GetSize();j++)
								{
									pNoeudA = pNoeudG->m_pNoeud[j]; 

									// MODIF DATE EXERCICE
									if (pNoeudA->m_DateExercice != "" && pNoeudA->m_DateExercice != "xxx" && DateExercice == "")  
										DateExercice = pNoeudA->m_DateExercice;

									// niveau produit
									for (int k=0;k < pNoeudA->m_pNoeud.GetSize();k++)
									{
										pNoeudP = pNoeudA->m_pNoeud[k];
										
										if (pNoeudP->m_Marque == Arbre->m_Alias)
										{	

											// MODIF DATE EXERCICE
											pNoeudP->m_DateExercice = DateExercice;

											// Ajout le produit sur la marque associé
											HItemProduitMarque = AjouterBranche(HItem,pNoeudP->m_Alias,pNoeudP->m_Libelle,pNoeudP->m_Codif,pNoeudP->m_Marque,pNoeudP->m_Famille,pNoeudP->m_Secteur,pNoeudP->m_DateExercice,false);					

											// Affichage des fichiers liés au produit d'il y a lieu
											Filefind = Finder.FindFile(pNoeudP->m_PathFichier + "\\*.cmp");
											while (Filefind)
											{
											  Filefind = Finder.FindNextFile();
											  FileName = Finder.GetFileTitle();	
											  HItem2 = InsertItem(FileName,ImageCampagne,ImageCampagne,HItemProduitMarque);
											  SetItemData(HItem2,0);
											}

										}
									}
								}
							}										
						}

					}

					else if(Arbre->m_pRacine->m_Libelle == NomBranche2)

					{	// Branche PRODUIT/GROUPE
						/*
						if (Arbre->m_Libelle != "groupevide")
						{	
							HItem = InsertItem(Arbre->m_Alias,ImageGroupe,ImageGroupe,m_Tree);
							Arbre->m_CodeImage = (ImageGroupe * 100) + ImageAnnonceur;
							SetItemData(HItem,(DWORD)Arbre);
						}
						else
							HItem = m_Tree;
						*/
						HItem = InsertItem(Arbre->m_Alias,ImageGroupe,ImageGroupe,m_Tree);
						Arbre->m_CodeImage = (ImageGroupe * 100) + ImageAnnonceur;
						SetItemData(HItem,(DWORD)Arbre);
				
					}

					else 

					{	// Branche FAMILLE / SECTEUR
						HItem = InsertItem(Arbre->m_Alias,ImageFamille,ImageFamille,m_Tree);
						Arbre->m_CodeImage = (ImageFamille * 100) + ImageSecteur;
						SetItemData(HItem,(DWORD)Arbre);
					}
					break;

				}
				case 2 :
				{
					if(Arbre->m_pRacine->m_pRacine->m_Libelle == NomBranche2)
					{
						// Branche PRODUIT / GROUPE / ANNONCEUR
						HItem = InsertItem(Arbre->m_Alias,ImageAnnonceur,ImageAnnonceur,m_Tree);
						Arbre->m_CodeImage = (ImageAnnonceur * 100) + ImageProduit;
						SetItemData(HItem,(DWORD)Arbre);
					}

					else if(Arbre->m_pRacine->m_pRacine->m_Libelle == NomBranche3)

					{	// Branche FAMILLE / SECTEUR
						HItem = InsertItem(Arbre->m_Alias,ImageSecteur,ImageSecteur,m_Tree);
						Arbre->m_CodeImage = (ImageSecteur * 100) + ImageProduit;
						SetItemData(HItem,(DWORD)Arbre);

						// Ajout Branche Produit et Campagne
						if (ArbreProduitMem != NULL)
						{
							// niveau groupe
							NbNoeud = ArbreProduitMem->m_pNoeud.GetSize();
							for (int i=0;i<ArbreProduitMem->m_pNoeud.GetSize();i++)
							{
								pNoeudG = ArbreProduitMem->m_pNoeud[i]; 
								// niveau annonceur
								NbNoeud = pNoeudG->m_pNoeud.GetSize();
								for (int j=0;j < pNoeudG->m_pNoeud.GetSize();j++)
								{
									pNoeudA = pNoeudG->m_pNoeud[j]; 


									// Modif Ajout Annonceur dans famille
									if (pNoeudA->m_Famille == Arbre->m_pRacine->m_Alias && pNoeudA->m_Secteur == Arbre->m_Alias)
									{	
										// Ajout l'annonceur sur la famille associé
										HItemProduitFamille = AjouterBranche(HItem,pNoeudA->m_Alias,pNoeudA->m_Libelle,pNoeudA->m_Codif,pNoeudA->m_Marque,pNoeudA->m_Famille,pNoeudA->m_Secteur,pNoeudA->m_DateExercice,true);					

										// Affichage des fichiers liés à l'annonceur
										Filefind = Finder.FindFile(pNoeudA->m_PathFichier + "\\*.cmp");
										while (Filefind)
										{
										  Filefind = Finder.FindNextFile();
										  FileName = Finder.GetFileTitle();	
										  HItem2 = InsertItem(FileName,ImageCampagne,ImageCampagne,HItemProduitFamille);
										  SetItemData(HItem2,0);
										}
									}									
									
									// Nombre de noeud produit
									NbNoeud = pNoeudA->m_pNoeud.GetSize();

									// niveau produit
									for (int k=0;k < pNoeudA->m_pNoeud.GetSize();k++)
									{
										pNoeudP = pNoeudA->m_pNoeud[k];
										
										if (pNoeudP->m_Famille == Arbre->m_pRacine->m_Alias && pNoeudP->m_Secteur == Arbre->m_Alias)
										{	
											// Ajout le produit sur la marque associé
											HItemProduitFamille = AjouterBranche(HItem,pNoeudP->m_Alias,pNoeudP->m_Libelle,pNoeudP->m_Codif,pNoeudP->m_Marque,pNoeudP->m_Famille,pNoeudP->m_Secteur,pNoeudP->m_DateExercice,false);					

											// Affichage des fichiers liés au produit d'il y a lieu
											Filefind = Finder.FindFile(pNoeudP->m_PathFichier + "\\*.cmp");
											while (Filefind)
											{
											  Filefind = Finder.FindNextFile();
											  FileName = Finder.GetFileTitle();	
											  HItem2 = InsertItem(FileName,ImageCampagne,ImageCampagne,HItemProduitFamille);
											  SetItemData(HItem2,0);
											}

										}
									}
								}
							}										
						}

					}



					break;
				}
				case 3 :
				{
					if(Arbre->m_pRacine->m_pRacine->m_pRacine->m_Libelle == NomBranche2)
					{
						// Branche PRODUIT / GROUPE / ANNONCEUR / PRODUIT
						HItem = InsertItem(Arbre->m_Alias,ImageProduit,ImageProduit,m_Tree);
						Arbre->m_CodeImage = (ImageProduit * 100) + ImageCampagne;
						SetItemData(HItem,(DWORD)Arbre);
					}
					break;
				}
			}
			// Affichage des fichiers d'il y a lieu
			Filefind = Finder.FindFile(Arbre->m_PathFichier + "\\*.cmp");
			while (Filefind)
			{
			  Filefind = Finder.FindNextFile();
			  FileName = Finder.GetFileTitle();	
			  HItem2 = InsertItem(FileName,ImageCampagne,ImageCampagne,HItem);
			  SetItemData(HItem2,0);
			}
			
			if (Arbre->m_pRacine->m_Libelle != NomBranche1)
			{
				// Affichage des ss-branches
				for (int i = 0;i < Arbre->m_pNoeud.GetSize();i++)
					PresenteArbre(ArbreProduitMem,Arbre->m_pNoeud[i],HItem,NomBranche1,NomBranche2,NomBranche3,HItemDossierMarque);
			}
		}
	}

	return false;	
}

// Tri d'un arbre à partir de sa racine (arbre complet)
void CTreeCtrlX::TrierArbre()
{
	HTREEITEM hItem;
	HTREEITEM hItemRacine;
	CString Txt;

	// Tri des branches principales
	hItem = GetRootItem();
	hItemRacine = GetParentItem(hItem);
	SortChildren(hItemRacine);

	// Tri de de toutes les autres ss-branches
	hItem = GetRootItem();
	while(hItem != NULL)
	{
		TrierBranche(hItem);
		hItem = GetNextItem(hItem,TVGN_NEXT);
	}		
}

// Tri d'un arbre à partir d'une branche
void CTreeCtrlX::TrierBranche(HTREEITEM hBranch)
{
	HTREEITEM hItem;
	CString Txt;

	SortChildren(hBranch);
	if (ItemHasChildren(hBranch))
	{
		// des ss-branches existent
		hItem = GetChildItem(hBranch);
		while (hItem != NULL)
		{
			TrierBranche(hItem);
			hItem = GetNextItem(hItem,TVGN_NEXT);
		}
	}
}



// Mise à jour de l'intitulé marque sur les produits
void CTreeCtrlX::MajMarqueProduit(HTREEITEM HItemProduit,CString ExMarque,CString NvMarque)
{

	CNoeud *pNoeudRacine;
	int NbNoeud;

	pNoeudRacine = (CNoeud*)GetItemData(HItemProduit);

	NbNoeud = pNoeudRacine->m_pNoeud.GetSize();
	for (int i = 0;i<pNoeudRacine->m_pNoeud.GetSize();i++)
	{
		NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();
		for (int j = 0;j<pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();j++)
		{
			NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();
			for (int k = 0;k<pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();k++)
			{
				if (pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Marque == ExMarque)
				{						
					pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Marque = NvMarque;
				}
			}
		}
	}
	
}

// Mise à jour de l'intitulé famille sur les produits
void CTreeCtrlX::MajFamilleProduit(HTREEITEM HItemProduit,CString ExFamille,CString NvFamille)
{
	CNoeud *pNoeudRacine;
	int NbNoeud;

	pNoeudRacine = (CNoeud*)GetItemData(HItemProduit);

	NbNoeud = pNoeudRacine->m_pNoeud.GetSize();
	for (int i = 0;i<pNoeudRacine->m_pNoeud.GetSize();i++)
	{
		NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();
		for (int j = 0;j<pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();j++)
		{
			NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();
			for (int k = 0;k<pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();k++)
			{
				if (pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Famille == ExFamille)
				{						
					pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Famille = NvFamille;
				}
			}
		}
	}
	
}

// Mise à jour de l'intitulé secteur sur les produits
void CTreeCtrlX::MajSecteurProduit(HTREEITEM HItemProduit,CString ExSecteur,CString NvSecteur)
{
	CNoeud *pNoeudRacine;
	int NbNoeud;

	pNoeudRacine = (CNoeud*)GetItemData(HItemProduit);

	NbNoeud = pNoeudRacine->m_pNoeud.GetSize();
	for (int i = 0;i<pNoeudRacine->m_pNoeud.GetSize();i++)
	{
		NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();
		for (int j = 0;j<pNoeudRacine->m_pNoeud[i]->m_pNoeud.GetSize();j++)
		{
			NbNoeud = pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();
			for (int k = 0;k<pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud.GetSize();k++)
			{
				if (pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Secteur == ExSecteur)
				{						
					pNoeudRacine->m_pNoeud[i]->m_pNoeud[j]->m_pNoeud[k]->m_Secteur = NvSecteur;
				}
			}
		}
	}
	
}

// Changement Nom d'un produit, on met à jour dans l'arborescence marque
void CTreeCtrlX::RenommerProduitMarque(HTREEITEM hBrancheMarque,CString Marque,CString ExNomProduit ,CString NvNomProduit)
{

	HTREEITEM hItem;
	HTREEITEM hItemProduit;
	bool MarqueTrouve = false;
	bool ProduitTrouve = false;

	if (hBrancheMarque != NULL)
	{
		if (ItemHasChildren(hBrancheMarque))
		{
			// recherche branche où se trouve le produit
			hItem = GetChildItem(hBrancheMarque);
			while (hItem != NULL && MarqueTrouve == false)
			{
				if (Marque == GetItemText(hItem))
				{
					MarqueTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			// recherche du produit sur ma marque en question
			if (hItem != NULL)
			{
				hItemProduit = GetChildItem(hItem);
				while (hItemProduit != NULL && ProduitTrouve == false)
				{
					if (ExNomProduit == GetItemText(hItemProduit))
					{
						ProduitTrouve = true;
						SetItemText(hItemProduit,NvNomProduit);
						break;
					}
					else
						hItemProduit = GetNextItem(hItemProduit,TVGN_NEXT);			
				}
			}
		}
	}			
}

// Changement Nom d'un produit, on met à jour dans l'arborescence famille
void CTreeCtrlX::RenommerProduitFamille(HTREEITEM hBrancheFamille,CString Famille,CString  Secteur,CString ExNomProduit ,CString NvNomProduit)
{

	HTREEITEM hItem;
	HTREEITEM hItemProduit;
	bool SecteurTrouve = false;
	bool FamilleTrouve = false;
	bool ProduitTrouve = false;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// recherche branche famille où se trouve le produit
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				if (Famille == GetItemText(hItem))
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true)
			{
				if (ItemHasChildren(hItem))
				{
					// recherche branche secteur de cette famille où se trouve le produit
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						if (Secteur == GetItemText(hItem))
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}
				}
			}
			

			// recherche du secteur sur cette famille en question
			if (hItem != NULL)
			{
				hItemProduit = GetChildItem(hItem);
				while (hItemProduit != NULL && ProduitTrouve == false)
				{
					if (ExNomProduit == GetItemText(hItemProduit))
					{
						ProduitTrouve = true;
						SetItemText(hItemProduit,NvNomProduit);
						break;
					}
					else
						hItemProduit = GetNextItem(hItemProduit,TVGN_NEXT);			
				}
			}
		}
	}			
}

// Changement Nom d'un annonceur, on met à jour dans l'arborescence famille
void CTreeCtrlX::RenommerAnnonceurFamille(HTREEITEM hBrancheFamille,CString Famille,CString  Secteur,CString ExNomAnnonceur ,CString NvNomAnnonceur)
{

	HTREEITEM hItem;
	HTREEITEM hItemAnnonceur;
	bool SecteurTrouve = false;
	bool FamilleTrouve = false;
	bool AnnonceurTrouve = false;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// recherche branche famille où se trouve le produit
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				if (Famille == GetItemText(hItem))
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true)
			{
				if (ItemHasChildren(hItem))
				{
					// recherche branche secteur de cette famille où se trouve le produit
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						if (Secteur == GetItemText(hItem))
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}
				}
			}
			

			// recherche du secteur sur cette famille en question
			if (hItem != NULL)
			{
				hItemAnnonceur = GetChildItem(hItem);
				while (hItemAnnonceur != NULL && AnnonceurTrouve == false)
				{
					if (ExNomAnnonceur == GetItemText(hItemAnnonceur))
					{
						AnnonceurTrouve = true;
						SetItemText(hItemAnnonceur,NvNomAnnonceur);
						break;
					}
					else
						hItemAnnonceur = GetNextItem(hItemAnnonceur,TVGN_NEXT);			
				}
			}
		}
	}			
}


// Vire un produit de la branche famille/secteur associée
// Attention on ne vire ici que sur arbre graphique
void CTreeCtrlX::VirerProduitDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasProduit)
{
	HTREEITEM hItem;
	HTREEITEM hItemProduit;
	CNoeud *pNoeud;
	bool SecteurTrouve = false;
	bool FamilleTrouve = false;
	bool ProduitTrouve = false;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// recherche branche famille où se trouve le produit
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				if (Famille == GetItemText(hItem))
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (hItem != NULL && FamilleTrouve == true)
			{
				if (ItemHasChildren(hItem))
				{
					// recherche branche secteur de cette famille où se trouve le produit
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						if (Secteur == GetItemText(hItem))
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (ItemHasChildren(hItem))
					{
						// recherche du produit sur ce secteur en question
						if (hItem != NULL && SecteurTrouve == true)
						{
							hItemProduit = GetChildItem(hItem);
							while (hItemProduit != NULL && ProduitTrouve == false)
							{
								if (AliasProduit == GetItemText(hItemProduit))
								{
									ProduitTrouve = true;
									break;
								}
								else
									hItemProduit = GetNextItem(hItemProduit,TVGN_NEXT);			
							}

							// ici on a l'item produit
							if (hItemProduit != NULL && ProduitTrouve == true)
							{
								// suppression du noeud
								pNoeud = (CNoeud*)GetItemData(hItemProduit);

								// Suppression lien noeud
								SupprimerLienNoeud(pNoeud);
								delete pNoeud;

								// suppression de la branche							
								SupprimerElmt(hItemProduit);
							}
						}
					}
				}
			}			
		}
	}
}

// Vire un annonceur de la branche famille/secteur associée
// Attention on ne vire ici que sur arbre graphique
void CTreeCtrlX::VirerAnnonceurDansFamilleSecteur(HTREEITEM hBrancheFamille,CString Famille,CString Secteur,CString AliasAnnonceur)
{
	HTREEITEM hItem;
	HTREEITEM hItemAnnonceur;
	CNoeud *pNoeud;
	bool SecteurTrouve = false;
	bool FamilleTrouve = false;
	bool AnnonceurTrouve = false;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// recherche branche famille où se trouve le produit
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				if (Famille == GetItemText(hItem))
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (hItem != NULL && FamilleTrouve == true)
			{
				if (ItemHasChildren(hItem))
				{
					// recherche branche secteur de cette famille où se trouve le produit
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						if (Secteur == GetItemText(hItem))
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (ItemHasChildren(hItem))
					{
						// recherche du produit sur ce secteur en question
						if (hItem != NULL && SecteurTrouve == true)
						{
							hItemAnnonceur = GetChildItem(hItem);
							while (hItemAnnonceur != NULL && AnnonceurTrouve == false)
							{
								if (AliasAnnonceur == GetItemText(hItemAnnonceur))
								{
									AnnonceurTrouve = true;
									break;
								}
								else
									hItemAnnonceur = GetNextItem(hItemAnnonceur,TVGN_NEXT);			
							}

							// ici on a l'item annonceur
							if (hItemAnnonceur != NULL && AnnonceurTrouve == true)
							{
								// suppression du noeud
								pNoeud = (CNoeud*)GetItemData(hItemAnnonceur);

								// Suppression lien noeud
								SupprimerLienNoeud(pNoeud);
								delete pNoeud;

								// suppression de la branche							
								SupprimerElmt(hItemAnnonceur);
							}
						}
					}
				}
			}			
		}
	}
}

// Vire un produit de la branche marque associée
// Attention on ne vire ici que sur arbre graphique
void CTreeCtrlX::VirerProduitDansMarque(HTREEITEM hBrancheMarque,CString AliasMarque,CString AliasProduit)
{

	HTREEITEM hItem;
	HTREEITEM hItemProduit;
	CString TxtMarque;
	bool MarqueTrouve = false;
	bool ProduitTrouve = false;
	CNoeud *pNoeud;

	if (hBrancheMarque != NULL)
	{
		if (ItemHasChildren(hBrancheMarque))
		{
			// enfants sur cette branche 
			hItem = GetChildItem(hBrancheMarque);
			while (hItem != NULL && MarqueTrouve == false)
			{
				TxtMarque = GetItemText(hItem);
				if (AliasMarque == TxtMarque)
				{
					MarqueTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}
			// recherche du produit sur ma marque en question
			if (hItem != NULL)
			{
				hItemProduit = GetChildItem(hItem);
				while (hItemProduit != NULL && ProduitTrouve == false)
				{
					if (AliasProduit == GetItemText(hItemProduit))
					{
						ProduitTrouve = true;
						break;
					}
					else
						hItemProduit = GetNextItem(hItemProduit,TVGN_NEXT);			
				}

				// ici on a l'item produit
				if (hItemProduit != NULL)
				{
					// suppression du noeud
					pNoeud = (CNoeud*)GetItemData(hItemProduit);

					// Suppression lien noeud
					SupprimerLienNoeud(pNoeud);
					delete pNoeud;

					// suppression de la branche							
					SupprimerElmt(hItemProduit);

				}

			}
		}
	}			
}

// Ajout d'un produit sur branche famille/secteur
void CTreeCtrlX::AjouterProduitDansFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleProduit,CString SecteurProduit,CString AliasProduit,CString LibelleProduit,CString CodeProduit)
{

	HTREEITEM hItemFamille,hItemSecteur;
	bool FamilleTrouve =false;
	bool SecteurTrouve =false;
	CString TxtFamille,TxtSecteur;
	CString Txt1,Txt2;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// enfants famille sur cette branche 
			hItemFamille = GetChildItem(hBrancheFamille);
			while (hItemFamille != NULL && FamilleTrouve == false)
			{
				TxtFamille = GetItemText(hItemFamille);
				if (FamilleProduit == TxtFamille)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItemFamille = GetNextItem(hItemFamille,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItemFamille != NULL)
			{
				if (ItemHasChildren(hItemFamille))
				{
					// enfants secteur sur cette branche 
					hItemSecteur = GetChildItem(hItemFamille);
					while (hItemSecteur != NULL && SecteurTrouve == false)
					{
						TxtSecteur = GetItemText(hItemSecteur);
						if (SecteurProduit == TxtSecteur)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItemSecteur = GetNextItem(hItemSecteur,TVGN_NEXT);			
					}

				}
			}



			if (SecteurTrouve == true && hItemSecteur != NULL)
			{
				// Branche famille/secteur trouvée on ajoute la branche produit
				AjouterBranche(hItemSecteur,AliasProduit,LibelleProduit,CodeProduit,Txt1,FamilleProduit,SecteurProduit,Txt2,false);
				RedrawWindow();
			}
		}
	}
}

// Ajout d'un annonceur direct sur branche famille/secteur
void CTreeCtrlX::AjouterAnnonceurDansFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleAnnonceur,CString SecteurAnnonceur,CString AliasAnnonceur,CString LibelleAnnonceur,CString CodeAnnonceur,CString DateAnnonceur)
{

	HTREEITEM hItemFamille,hItemSecteur;
	bool FamilleTrouve =false;
	bool SecteurTrouve =false;
	CString TxtFamille,TxtSecteur;
	CString Txt1;

	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// enfants famille sur cette branche 
			hItemFamille = GetChildItem(hBrancheFamille);
			while (hItemFamille != NULL && FamilleTrouve == false)
			{
				TxtFamille = GetItemText(hItemFamille);
				if (FamilleAnnonceur == TxtFamille)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItemFamille = GetNextItem(hItemFamille,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItemFamille != NULL)
			{
				if (ItemHasChildren(hItemFamille))
				{
					// enfants secteur sur cette branche 
					hItemSecteur = GetChildItem(hItemFamille);
					while (hItemSecteur != NULL && SecteurTrouve == false)
					{
						TxtSecteur = GetItemText(hItemSecteur);
						if (SecteurAnnonceur == TxtSecteur)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItemSecteur = GetNextItem(hItemSecteur,TVGN_NEXT);			
					}

				}
			}

			if (SecteurTrouve == true && hItemSecteur != NULL)
			{
				// Branche famille/secteur trouvée on ajoute la branche annonceur
				AjouterBranche(hItemSecteur,AliasAnnonceur,LibelleAnnonceur,CodeAnnonceur,Txt1,FamilleAnnonceur,SecteurAnnonceur,DateAnnonceur,true);
				RedrawWindow();
			}
		}
	}
}

// Ajout d'un produit sur branche marque
void CTreeCtrlX::AjouterProduitDansMarque(HTREEITEM hBrancheMarque,CString MarqueProduit,CString AliasProduit,CString LibelleProduit,CString CodeProduit)
{

	HTREEITEM hItemMarque;
	bool MarqueTrouve =false;
	CString TxtMarque;
	CString TxtDate ="";
	CString Txt1,Txt2;

	if (hBrancheMarque != NULL)
	{
		if (ItemHasChildren(hBrancheMarque))
		{
			// enfants sur cette branche 
			hItemMarque = GetChildItem(hBrancheMarque);
			while (hItemMarque != NULL && MarqueTrouve == false)
			{
				TxtMarque = GetItemText(hItemMarque);
				if (MarqueProduit == TxtMarque)
				{
					MarqueTrouve =true;
					break;
				}
				else
					hItemMarque = GetNextItem(hItemMarque,TVGN_NEXT);			
			}

			if (hItemMarque != NULL)
			{
				// Branche marque trouvée on ajoute la branche produit
				AjouterBranche(hItemMarque,AliasProduit,LibelleProduit,CodeProduit,MarqueProduit,Txt1,Txt2,TxtDate,false);
				RedrawWindow();
			}

		}
	}

}


// Déplacement d'une campagne produit au niveau de l'arbo marque
void CTreeCtrlX::DeplacerCampagneProduitDansMarque(HTREEITEM hBrancheMarque,CString MarqueExProduit,CString AliasExProduit,CString MarqueNvProduit,CString AliasNvProduit,CString NomCampagne)
{

	// Si MarqueNvProduit ="xxx" on vire la campagne uniquement de l'ancien produit
	if (MarqueExProduit == "")
	{
		// ici campagne ne provenant pas d'un produit
		if (MarqueNvProduit !="") 
		{
			// Insertion campagne sur la branche produit destination
			InsertionCampagneProduitMarque(hBrancheMarque,MarqueNvProduit,AliasNvProduit,NomCampagne);
		}
	}

	else if (MarqueNvProduit =="")
	{
		// ici campagne dirigée vers un élmt <> produit
		if (MarqueExProduit != "")
		{
			// Suppression campagne de la branche produit origine
			SuppressionCampagneProduitMarque(hBrancheMarque,MarqueExProduit,AliasExProduit,NomCampagne);
		}
	}

	else
	{	
		// ici transfert campagne de produit à produit
		InsertionCampagneProduitMarque(hBrancheMarque,MarqueNvProduit,AliasNvProduit,NomCampagne);
		SuppressionCampagneProduitMarque(hBrancheMarque,MarqueExProduit,AliasExProduit,NomCampagne);
	}
}

// Insertion d'une campagne au niveau arborescence Marque/Produit
void CTreeCtrlX::InsertionCampagneProduitMarque(HTREEITEM hBrancheMarque,CString MarqueNvProduit,CString AliasNvProduit,CString NomCampagne)
{

	HTREEITEM hItem,hNvItem;
	CString TxtAlias;
	bool MarqueTrouve = false;
	bool ProduitTrouve = false;

	// et dirigée vers un nouveau produit >> maj arbo marque
	// Recherche branche marque avec produit ou la campagne est à insérer
	if (hBrancheMarque != NULL)
	{
		if (ItemHasChildren(hBrancheMarque))
		{
			// Recherche de la marque du nouveau produit affilié à cette campagne
			hItem = GetChildItem(hBrancheMarque);
			while (hItem != NULL && MarqueTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (MarqueNvProduit == TxtAlias)
				{
					MarqueTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}
			
			if (MarqueTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du produit affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && ProduitTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (AliasNvProduit == TxtAlias)
						{
							ProduitTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}
				}
			}

			// Ajout branche campagne sur produit trouvé
			if (ProduitTrouve == true && hItem != NULL)
				hNvItem = InsertItem(NomCampagne,ImageCampagne,ImageCampagne,hItem);
		}
	}
}

void CTreeCtrlX::SuppressionCampagneProduitMarque(HTREEITEM hBrancheMarque,CString MarqueExProduit,CString AliasExProduit,CString NomCampagne)
{
	HTREEITEM hItem;
	CString TxtAlias;
	bool MarqueTrouve = false;
	bool ProduitTrouve = false;
	bool CampagneTrouve = false;

	// et provenant d'un élmt produit >> suppression de la feuille campagne
	// Recherche branche marque avec produit ou la campagne est à insérer
	if (hBrancheMarque != NULL)
	{
		if (ItemHasChildren(hBrancheMarque))
		{
			// Recherche de la marque du nouveau produit affilié à cette campagne
			hItem = GetChildItem(hBrancheMarque);
			while (hItem != NULL && MarqueTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (MarqueExProduit == TxtAlias)
				{
					MarqueTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}
			
			if (MarqueTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du produit affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && ProduitTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (AliasExProduit == TxtAlias)
						{
							ProduitTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}
				}
			}

			// Suppression branche campagne sur produit trouvé
			if (ProduitTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Puis recherche de la branche campagne à supprimer
					hItem = GetChildItem(hItem);
					while (hItem != NULL && CampagneTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (NomCampagne == TxtAlias)
						{
							CampagneTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (CampagneTrouve == true && hItem != NULL)
						DeleteItem(hItem);
				}
			}
		}
	}
}



// Déplacement d'une campagne produit au niveau de l'arbo famille/secteur
void CTreeCtrlX::DeplacerCampagneProduitDansSecteur(HTREEITEM hBranchFamille,CString FamilleExProduit,CString SecteurExProduit,CString AliasExProduit,
   												                             CString FamilleNvProduit,CString SecteurNvProduit,CString AliasNvProduit,CString NomCampagne)
{
	// Si MarqueNvProduit ="xxx" on vire la campagne uniquement de l'ancien produit
	if (SecteurExProduit == "")
	{
		// ici campagne ne provenant pas d'un produit
		if (SecteurNvProduit !="") 
		{
			// Insertion campagne sur la branche produit destination
			InsertionCampagneProduitFamilleSecteur(hBranchFamille,FamilleNvProduit,SecteurNvProduit,AliasNvProduit,NomCampagne);
		}
	}

	else if (SecteurNvProduit =="")
	{
		// ici campagne dirigée vers un élmt <> produit
		if (SecteurExProduit != "")
		{
			// Suppression campagne de la branche produit origine
			SuppressionCampagneProduitFamilleSecteur(hBranchFamille,FamilleExProduit,SecteurExProduit,AliasExProduit,NomCampagne);
		}
	}

	else
	{	
		// ici transfert campagne de produit à produit
		InsertionCampagneProduitFamilleSecteur(hBranchFamille,FamilleNvProduit,SecteurNvProduit,AliasNvProduit,NomCampagne);
		SuppressionCampagneProduitFamilleSecteur(hBranchFamille,FamilleExProduit,SecteurExProduit,AliasExProduit,NomCampagne);
	}
}

// Insertion d'une campagne au niveau arborescence Famille/Secteur/Produit
void CTreeCtrlX::InsertionCampagneProduitFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleNvProduit,CString SecteurNvProduit,CString AliasNvProduit,CString NomCampagne)
{
	HTREEITEM hItem,hNvItem;
	CString TxtAlias;
	bool ProduitTrouve = false;
	bool FamilleTrouve = false;
	bool SecteurTrouve = false;

	// et dirigée vers un nouveau produit >> maj arbo marque
	// Recherche branche marque avec produit ou la campagne est à insérer
	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// Recherche de la famille du nouveau produit affilié à cette campagne
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (FamilleNvProduit == TxtAlias)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du secteur du produit affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (SecteurNvProduit == TxtAlias)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (SecteurTrouve == true && hItem != NULL)
					{
						if (ItemHasChildren(hItem))
						{
							// Recherche du produit affilié à cette campagne
							hItem = GetChildItem(hItem);
							while (hItem != NULL && ProduitTrouve == false)
							{
								TxtAlias = GetItemText(hItem);
								if (AliasNvProduit == TxtAlias)
								{
									ProduitTrouve =true;
									break;
								}
								else
									hItem = GetNextItem(hItem,TVGN_NEXT);			
							}
						}

						// Ajout branche campagne sur produit trouvé dans famille/secteur
						if (ProduitTrouve == true && hItem != NULL)
							hNvItem = InsertItem(NomCampagne,ImageCampagne,ImageCampagne,hItem);
					}
				}
			}
		}
	}
}

// Suppression d'une campagne au niveau arborescence Famille/Secteur/Produit
void CTreeCtrlX::SuppressionCampagneProduitFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleExProduit,CString SecteurExProduit,CString AliasExProduit,CString NomCampagne)
{
	HTREEITEM hItem;
	CString TxtAlias;
	bool ProduitTrouve = false;
	bool FamilleTrouve = false;
	bool SecteurTrouve = false;
	bool CampagneTrouve = false;

	// et dirigée vers un nouveau produit >> maj arbo marque
	// Recherche branche marque avec produit ou la campagne est à insérer
	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// Recherche de la famille de l'ancien produit affilié à cette campagne
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (FamilleExProduit == TxtAlias)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du secteur du produit affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (SecteurExProduit == TxtAlias)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (SecteurTrouve == true && hItem != NULL)
					{
						if (ItemHasChildren(hItem))
						{
							// Recherche du produit affilié à cette campagne
							hItem = GetChildItem(hItem);
							while (hItem != NULL && ProduitTrouve == false)
							{
								TxtAlias = GetItemText(hItem);
								if (AliasExProduit == TxtAlias)
								{
									ProduitTrouve =true;
									break;
								}
								else
									hItem = GetNextItem(hItem,TVGN_NEXT);			
							}
						}

						// Suppression branche campagne sur produit trouvé
						if (ProduitTrouve == true && hItem != NULL)
						{
							if (ItemHasChildren(hItem))
							{
								// Recherche de la campagne
								hItem = GetChildItem(hItem);
								while (hItem != NULL && CampagneTrouve == false)
								{
									TxtAlias = GetItemText(hItem);
									if (NomCampagne == TxtAlias)
									{
										CampagneTrouve =true;
										break;
									}
									else
										hItem = GetNextItem(hItem,TVGN_NEXT);			
								}
							}

							if (CampagneTrouve == true && hItem != NULL)
								DeleteItem(hItem);
						}
					}
				}
			}
		}
	}
}


// Insertion d'une campagne au niveau arborescence Famille/Secteur/Annonceur
void CTreeCtrlX::InsertionCampagneAnnonceurFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleNvAnnonceur,CString SecteurNvAnnonceur,CString AliasNvAnnonceur,CString NomCampagne)
{
	HTREEITEM hItem,hNvItem;
	CString TxtAlias;
	bool AnnonceurTrouve = false;
	bool FamilleTrouve = false;
	bool SecteurTrouve = false;

	// et dirigée vers un nouveau annonceur >> maj arbo marque
	// Recherche branche marque avec annonceur ou la campagne est à insérer
	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// Recherche de la famille du nouveau annonceur affilié à cette campagne
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (FamilleNvAnnonceur == TxtAlias)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du secteur annonceur affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (SecteurNvAnnonceur == TxtAlias)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (SecteurTrouve == true && hItem != NULL)
					{
						if (ItemHasChildren(hItem))
						{
							// Recherche Annonceur affilié à cette campagne
							hItem = GetChildItem(hItem);
							while (hItem != NULL && AnnonceurTrouve == false)
							{
								TxtAlias = GetItemText(hItem);
								if (AliasNvAnnonceur == TxtAlias)
								{
									AnnonceurTrouve =true;
									break;
								}
								else
									hItem = GetNextItem(hItem,TVGN_NEXT);			
							}
						}

						// Ajout branche campagne sur annonceur trouvé dans famille/secteur
						if (AnnonceurTrouve == true && hItem != NULL)
							hNvItem = InsertItem(NomCampagne,ImageCampagne,ImageCampagne,hItem);
					}
				}
			}
		}
	}
}


// Suppression d'une campagne au niveau arborescence Famille/Secteur/Annonceur
void CTreeCtrlX::SuppressionCampagneAnnonceurFamilleSecteur(HTREEITEM hBrancheFamille,CString FamilleExAnnonceur,CString SecteurExAnnonceur,CString AliasExAnnonceur,CString NomCampagne)
{
	HTREEITEM hItem;
	CString TxtAlias;
	bool AnnonceurTrouve = false;
	bool FamilleTrouve = false;
	bool SecteurTrouve = false;
	bool CampagneTrouve = false;

	// et dirigée vers un nouveau produit >> maj arbo marque
	// Recherche branche marque avec Annonceur ou la campagne est à insérer
	if (hBrancheFamille != NULL)
	{
		if (ItemHasChildren(hBrancheFamille))
		{
			// Recherche de la famille de l'ancien Annonceur affilié à cette campagne
			hItem = GetChildItem(hBrancheFamille);
			while (hItem != NULL && FamilleTrouve == false)
			{
				TxtAlias = GetItemText(hItem);
				if (FamilleExAnnonceur == TxtAlias)
				{
					FamilleTrouve =true;
					break;
				}
				else
					hItem = GetNextItem(hItem,TVGN_NEXT);			
			}

			if (FamilleTrouve == true && hItem != NULL)
			{
				if (ItemHasChildren(hItem))
				{
					// Recherche du secteur du Annonceur affilié à cette campagne
					hItem = GetChildItem(hItem);
					while (hItem != NULL && SecteurTrouve == false)
					{
						TxtAlias = GetItemText(hItem);
						if (SecteurExAnnonceur == TxtAlias)
						{
							SecteurTrouve =true;
							break;
						}
						else
							hItem = GetNextItem(hItem,TVGN_NEXT);			
					}

					if (SecteurTrouve == true && hItem != NULL)
					{
						if (ItemHasChildren(hItem))
						{
							// Recherche du Annonceur affilié à cette campagne
							hItem = GetChildItem(hItem);
							while (hItem != NULL && AnnonceurTrouve == false)
							{
								TxtAlias = GetItemText(hItem);
								if (AliasExAnnonceur == TxtAlias)
								{
									AnnonceurTrouve =true;
									break;
								}
								else
									hItem = GetNextItem(hItem,TVGN_NEXT);			
							}
						}

						// Suppression branche campagne sur Annonceur trouvé
						if (AnnonceurTrouve == true && hItem != NULL)
						{
							if (ItemHasChildren(hItem))
							{
								// Recherche de la campagne
								hItem = GetChildItem(hItem);
								while (hItem != NULL && CampagneTrouve == false)
								{
									TxtAlias = GetItemText(hItem);
									if (NomCampagne == TxtAlias)
									{
										CampagneTrouve =true;
										break;
									}
									else
										hItem = GetNextItem(hItem,TVGN_NEXT);			
								}
							}

							if (CampagneTrouve == true && hItem != NULL)
								DeleteItem(hItem);
						}
					}
				}
			}
		}
	}
}


// Déplacement d'une campagne annonceur au niveau de l'arbo famille/secteur
void CTreeCtrlX::DeplacerCampagneAnnonceurDansSecteur(HTREEITEM hBranchFamille,CString FamilleExAnnonceur,CString SecteurExAnnonceur,CString AliasExAnnonceur,
   												                             CString FamilleNvAnnonceur,CString SecteurNvAnnonceur,CString AliasNvAnnonceur,CString NomCampagne)
{
	// Si MarqueNvAnnonceur ="xxx" on vire la campagne uniquement de l'ancien annonceur
	if (SecteurExAnnonceur == "")
	{
		// ici campagne ne provenant pas d'un produit
		if (SecteurNvAnnonceur !="") 
		{
			// Insertion campagne sur la branche annonceur destination
			InsertionCampagneAnnonceurFamilleSecteur(hBranchFamille,FamilleNvAnnonceur,SecteurNvAnnonceur,AliasNvAnnonceur,NomCampagne);
		}
	}

	else if (SecteurNvAnnonceur =="")
	{
		// ici campagne dirigée vers un élmt <> Annonceur
		if (SecteurExAnnonceur != "")
		{
			// Suppression campagne de la branche Annonceur origine
			SuppressionCampagneAnnonceurFamilleSecteur(hBranchFamille,FamilleExAnnonceur,SecteurExAnnonceur,AliasExAnnonceur,NomCampagne);
		}
	}

	else
	{	
		// ici transfert campagne de Annonceur à Annonceur
		InsertionCampagneAnnonceurFamilleSecteur(hBranchFamille,FamilleNvAnnonceur,SecteurNvAnnonceur,AliasNvAnnonceur,NomCampagne);
		SuppressionCampagneAnnonceurFamilleSecteur(hBranchFamille,FamilleExAnnonceur,SecteurExAnnonceur,AliasExAnnonceur,NomCampagne);
	}
}


// Test s'il existe au moins 1 campagne au niveau d'une branche
bool CTreeCtrlX:: FichiersCampagnePresent(HTREEITEM hBranche) 
{
	int CodeDessin;
	HTREEITEM hItem;
	bool CampagneTrouve = false;
	
	if (hBranche != NULL && CampagneTrouve == false)
	{
		if (ItemHasChildren(hBranche))
		{
			// recherche campagne dans les branches inférieures
			hItem = GetChildItem(hBranche);
			while (hItem != NULL && CampagneTrouve == false)
			{	
				GetItemImage(hItem,CodeDessin,CodeDessin);
				if (CodeDessin == ImageCampagne)
				{			
					// ici c'est un élmt campagne
					CampagneTrouve = true;
					break;
				}
				else
					CampagneTrouve = FichiersCampagnePresent(hItem);
					hItem = GetNextItem(hItem,TVGN_NEXT);
			}

			return CampagneTrouve;
		}

		else return CampagneTrouve;

	}
	
	else
		// pas de campagne
		return CampagneTrouve;
}


// CopyBranch           - Copies all items in a branch to a new location
// Returns              - The new branch node
// htiBranch            - The node that starts the branch
// htiNewParent         - Handle of the parent for new branch
// htiAfter             - Item after which the new branch should be created
HTREEITEM CTreeCtrlX::CopierBranche(HTREEITEM hBranch,HTREEITEM hNvParent,HTREEITEM hApres)
{
    HTREEITEM hEnfant;

    HTREEITEM hNvElmt = CopierElmt( hBranch, hNvParent, hApres);
    hEnfant = GetChildItem(hBranch);
    while( hEnfant != NULL)
    {
            // recursively transfer all the items
            CopierBranche(hEnfant, hNvElmt);  
            hEnfant = GetNextSiblingItem(hEnfant);
    }

    return hNvElmt;
}

HTREEITEM CTreeCtrlX::NewCopierBranche(HTREEITEM hBranch,HTREEITEM hNvParent,HTREEITEM hApres)
{
    HTREEITEM hEnfant;
	CString NomNvRep;
	CString NomExRep;
	CString NomExFichier,NomNvFichier;
	int CodeDessin;
	CNoeud *pNoeudParent;
	CNoeud *pNoeud;
	CFile File1,File2;
	bool FileExist=false;

    HTREEITEM hNvElmt = CopierElmt( hBranch, hNvParent, hApres);

	// physiquement on met à jour les répertoires et les fichiers
	GetItemImage(hBranch,CodeDessin,CodeDessin);
	pNoeudParent = (CNoeud*)GetItemData(hNvParent);

	if (CodeDessin == ImageCampagne)
	{
		// copie du fichier campagne sur nouveau répertoire
		NomNvFichier = pNoeudParent->m_PathFichier + "\\"  + GetItemText(hBranch) + ".cmp";
		NomExFichier = pNoeudParent->m_ExPathFichier + "\\" + GetItemText(hBranch) + ".cmp";
		if (CopyFile(NomExFichier,NomNvFichier,FileExist))
		{
			// destruction de l'ancien fichier
			//if (DeleteFile(NomExFichier)== false)
			//{
			//	AfxMessageBox("Pb destruction fichier" + NomExFichier); 
			//}

		}
		
	}

	else
	{
		// ici il s'agit encore d'un répertoire à copier
		pNoeud = (CNoeud*)GetItemData(hBranch);
		NomNvRep = pNoeudParent->m_PathFichier + "\\" + pNoeud->m_Libelle;

		// création nv répertoire associé à cette branche
		CreateDirectory(NomNvRep,NULL);

		// maj Path fichier du noeud enfant / save ancien path rep
		pNoeud->m_ExPathFichier = pNoeud->m_PathFichier;
		pNoeud->m_PathFichier = NomNvRep;
	}

    hEnfant = GetChildItem(hBranch);
    while( hEnfant != NULL)
    {
            // recursively transfer all the items
            NewCopierBranche(hEnfant, hNvElmt);  
            hEnfant = GetNextSiblingItem(hEnfant);
    }

    return hNvElmt;
}


// DeplacerBranche      - Deplacement d'une branche sur une autre branche mère
// Returns              - Handle of the new item
// hItem                - Item to be copied
// htiNewParent         - Handle of the parent for new item
// htiAfter             - Item after which the new item should be created
HTREEITEM CTreeCtrlX:: DeplacerBranche(HTREEITEM hBranchSrc,HTREEITEM hNvParent)
{
	CString MessErr;
	
	// D'abord copie de la branche existante sur nvlle branche mère
	HTREEITEM hNvElmt = CopierBranche(hBranchSrc,hNvParent);
	
	if (CTreeCtrlX::DeleteItem(hBranchSrc) == false)
	{
		MessErr = "Déplacement branche impossible" + CTreeCtrlX::GetItemText(hBranchSrc);
		AfxMessageBox (MessErr);
	}
	return hNvElmt;

}

// Ajouter une branche dans la nomenclature
HTREEITEM CTreeCtrlX :: AjouterBranche(HTREEITEM hBrancheRacine,CString TxtNvItem,CString Libelle,CString Code,CString Marque,CString Famille,CString Secteur,CString DateDeb,bool AnnonceurDirect)
{
	int CodeDessinEnfant;
	HTREEITEM hNvItem;
	CNoeud *pNoeud;
	CNoeud *pNvNoeud;

	if (hBrancheRacine != 0)
	{
		// recup pointeur noeud branche racine
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBrancheRacine);
		CodeDessinEnfant = (pNoeud->m_CodeImage) % 100;

		if (AnnonceurDirect == true && CodeDessinEnfant == CTreeCtrlX::ImageProduit) CodeDessinEnfant = CTreeCtrlX::ImageAnnonceur; 

		// création d'un nouveau noeud
		pNvNoeud=new CNoeud;

		if (CodeDessinEnfant >= ImageOpen || CodeDessinEnfant <= ImageProduit)  

			hNvItem = InsertItem(TxtNvItem,CodeDessinEnfant,CodeDessinEnfant,hBrancheRacine);

			// repositionne le code dessin item racine et enfant
			switch (CodeDessinEnfant)
			{
				case ImageMarque :
				{
					pNvNoeud->m_CodeImage = ImageMarque * 100 + ImageProduit;
					break;
				}

				case ImageFamille :
				{
					pNvNoeud->m_CodeImage = ImageFamille * 100 + ImageSecteur;
					break;
				}

				case ImageSecteur :
				{
					pNvNoeud->m_CodeImage = ImageSecteur * 100 + ImageProduit;
					break;
				}

				case ImageGroupe : 
				{
					pNvNoeud->m_Libelle = Libelle;
					pNvNoeud->m_CodeImage = ImageGroupe * 100 + ImageAnnonceur;
					pNvNoeud->m_DateExercice = DateDeb;
					break;
				}

				case ImageAnnonceur :
				{
					pNvNoeud->m_Libelle = Libelle;
					pNvNoeud->m_CodeImage = ImageAnnonceur * 100 + ImageProduit;
					pNvNoeud->m_Famille = Famille;
					pNvNoeud->m_Secteur = Secteur;
					pNvNoeud->m_DateExercice = DateDeb;
					break;
				}

				case ImageProduit : 
				{
					pNvNoeud->m_Libelle = Libelle;
					pNvNoeud->m_CodeImage = ImageProduit * 100 + ImageCampagne;
					pNvNoeud->m_Marque = Marque;
					pNvNoeud->m_Famille = Famille;
					pNvNoeud->m_Secteur = Secteur;
					// MODIF DATE EXERCICE
					pNvNoeud->m_DateExercice = DateDeb;
					break;
				}

				case ImageCampagne :	
				{
					// Suppression lien noeud
					SupprimerLienNoeud(pNoeud);

					delete pNvNoeud;
					SetItemData(hNvItem,0);		
					break;
				}
			}

			if (CodeDessinEnfant != ImageCampagne)
			{
				SetItemData(hNvItem,(DWORD)pNvNoeud);		
				pNvNoeud->m_Alias = TxtNvItem;
				pNvNoeud->m_Codif = Code;
				pNvNoeud->m_PathFichier= pNoeud->m_PathFichier + "\\" + pNvNoeud->m_Libelle;
				pNvNoeud->m_Niveau = pNoeud->m_Niveau + 1;
				pNvNoeud->m_pRacine=pNoeud;
				pNoeud->m_pNoeud.Add(pNvNoeud);
			}

			/*
			// Cas spécial Produit  / Maj Branche Marques
			if (CodeDessinEnfant == ImageProduit)
			{
				hBrancheMarque = GetRootItem();
				AjouterProduitDansMarque(hBrancheMarque,pNvNoeud->m_Marque,
														pNvNoeud->m_Alias,
														pNvNoeud->m_Libelle,
														pNvNoeud->m_Codif);
			}
			*/

			// puis on se positionne sur cette nouvelle branche
			Expand(hNvItem,TVE_COLLAPSE);   
			return hNvItem;
	}

	else
		// Ajout d'une branche de base
		return 0;

}

// Ajout Annonceur (classique)
HTREEITEM CTreeCtrlX ::AjouterBrancheAnnonceur(HTREEITEM hBrancheRacine,CString Alias,CString Libelle,CString Code,
											   CString Marque,CString Famille,CString Secteur,CString DateDeb,
						                       CString m_TypeTiers,CString m_CodeTiers,CString m_TypeCodeTiers,
						                       CString m_NomTiers,CString m_AdresseTiers1,CString m_AdresseTiers2,
						                       CString m_CodePostalTiers,CString m_VilleTiers,CString m_CodePaysTiers,
						                       CString m_NoTelTiers,CString m_NoFaxTiers,
					  	                       CString m_NomContactTiers,CString m_PrenomContactTiers,CString m_AdrMailContactTiers,
 						                       bool AnnonceurIsole)
{
	int CodeDessinEnfant;
	HTREEITEM hNvItem;
	CNoeud *pNoeud;
	CNoeud *pNvNoeud;

	if (hBrancheRacine != 0)
	{
		// recup pointeur noeud branche racine
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBrancheRacine);
		CodeDessinEnfant = (pNoeud->m_CodeImage) % 100;

		// création d'un nouveau noeud
		pNvNoeud=new CNoeud;

		if (CodeDessinEnfant >= ImageOpen || CodeDessinEnfant <= ImageProduit)  

			hNvItem = InsertItem(Alias,CodeDessinEnfant,CodeDessinEnfant,hBrancheRacine);

			// repositionne le code dessin item racine et enfant
			switch (CodeDessinEnfant)
			{

				case ImageAnnonceur :
				{
					pNvNoeud->m_Libelle = Libelle;
					pNvNoeud->m_CodeImage = ImageAnnonceur * 100 + ImageProduit;
					pNvNoeud->m_Famille = Famille;
					pNvNoeud->m_Secteur = Secteur;
					pNvNoeud->m_DateExercice = DateDeb;

					// Les nouvelles infos EDI
					pNvNoeud->m_TypeTiers			= m_TypeTiers;
					pNvNoeud->m_CodeTiers			= m_CodeTiers; 
					pNvNoeud->m_TypeCodeTiers		= m_TypeCodeTiers;
					pNvNoeud->m_NomTiers			= m_NomTiers;
					pNvNoeud->m_AdresseTiers1		= m_AdresseTiers1;
					pNvNoeud->m_AdresseTiers2		= m_AdresseTiers2; 
					pNvNoeud->m_CodePostalTiers		= m_CodePostalTiers;
					pNvNoeud->m_VilleTiers			= m_VilleTiers;
					pNvNoeud->m_CodePaysTiers		= m_CodePaysTiers;		
					pNvNoeud->m_NoTelTiers			= m_NoTelTiers;
					pNvNoeud->m_NoFaxTiers			= m_NoFaxTiers;
					pNvNoeud->m_NomContactTiers		= m_NomContactTiers;
					pNvNoeud->m_PrenomContactTiers  = m_PrenomContactTiers;
					pNvNoeud->m_AdrMailContactTiers = m_AdrMailContactTiers;
					break;
				}

			}

			if (CodeDessinEnfant != ImageCampagne)
			{
				SetItemData(hNvItem,(DWORD)pNvNoeud);		
				pNvNoeud->m_Alias = Alias;
				pNvNoeud->m_Codif = Code;
				pNvNoeud->m_PathFichier= pNoeud->m_PathFichier + "\\" + pNvNoeud->m_Libelle;
				pNvNoeud->m_Niveau = pNoeud->m_Niveau + 1;
				pNvNoeud->m_pRacine=pNoeud;
				pNoeud->m_pNoeud.Add(pNvNoeud);
			}

			// puis on se positionne sur cette nouvelle branche
			Expand(hNvItem,TVE_COLLAPSE);   
			return hNvItem;
	}

	else
		// Ajout d'une branche de base
		return 0;

}

HTREEITEM CTreeCtrlX ::AjouterBrancheAnnonceurDirect(HTREEITEM hBrancheRacine,CString TxtNvItem,CString Libelle,CString Code,CString Marque,CString Famille,CString Secteur,CString DateDeb)
{
	int CodeDessinEnfant;
	HTREEITEM hNvItem;
	CNoeud *pNoeud;
	CNoeud *pNvNoeud;

	if (hBrancheRacine != 0)
	{
		// recup pointeur noeud branche racine
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBrancheRacine);
		CodeDessinEnfant = ImageAnnonceur;

		// création d'un nouveau noeud
		pNvNoeud=new CNoeud;

		if (CodeDessinEnfant >= ImageOpen || CodeDessinEnfant <= ImageProduit)  
		{
			hNvItem = InsertItem(TxtNvItem,CodeDessinEnfant,CodeDessinEnfant,hBrancheRacine);

			pNvNoeud->m_Libelle = Libelle;
			pNvNoeud->m_CodeImage = ImageAnnonceur * 100 + ImageProduit;

			SetItemData(hNvItem,(DWORD)pNvNoeud);		
			pNvNoeud->m_Alias = TxtNvItem;
			pNvNoeud->m_Codif = Code;

			pNvNoeud->m_DateExercice = DateDeb;
			pNvNoeud->m_Famille = Famille;
			pNvNoeud->m_Secteur = Secteur;

			pNvNoeud->m_PathFichier= pNoeud->m_PathFichier + "\\" + pNvNoeud->m_Libelle;
			pNvNoeud->m_Niveau = pNoeud->m_Niveau + 1;

			CString Lib = pNoeud->m_Libelle;
			pNvNoeud->m_pRacine=pNoeud;
			pNoeud->m_pNoeud.Add(pNvNoeud);

			// puis on se positionne sur cette nouvelle branche
			Expand(hNvItem,TVE_COLLAPSE);   
			return hNvItem;
		}
		else
			return 0;
	}

	else
		// Ajout d'une branche de base
		return 0;

}

HTREEITEM CTreeCtrlX :: AjouterBrancheAnnonceurSsGroupe(HTREEITEM hBrancheRacine,CString TxtNvItem,CString Libelle,CString Code,CString DateDeb)
{
	int CodeDessinEnfant;
	HTREEITEM hNvItem;
	CNoeud *pNoeud;
	CNoeud *pNvNoeud;

	if (hBrancheRacine != 0)
	{
		// recup pointeur noeud branche racine
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBrancheRacine);
		CodeDessinEnfant = ImageAnnonceur;

		// création d'un nouveau noeud
		pNvNoeud=new CNoeud;

		if (CodeDessinEnfant >= ImageOpen || CodeDessinEnfant <= ImageProduit)  
		{
			hNvItem = InsertItem(TxtNvItem,CodeDessinEnfant,CodeDessinEnfant,hBrancheRacine);

			pNvNoeud->m_Libelle = Libelle;
			pNvNoeud->m_CodeImage = ImageAnnonceur * 100 + ImageProduit;

			SetItemData(hNvItem,(DWORD)pNvNoeud);		
			pNvNoeud->m_Alias = TxtNvItem;
			pNvNoeud->m_Codif = Code;

			pNvNoeud->m_DateExercice = "xxx";
			pNvNoeud->m_Famille = "xxx";
			pNvNoeud->m_Secteur = "xxx";

			pNvNoeud->m_PathFichier= pNoeud->m_PathFichier + "\\" + pNvNoeud->m_Libelle;
			pNvNoeud->m_Niveau = pNoeud->m_Niveau + 1;
			pNvNoeud->m_pRacine=pNoeud;
			pNoeud->m_pNoeud.Add(pNvNoeud);

			// puis on se positionne sur cette nouvelle branche
			Expand(hNvItem,TVE_COLLAPSE);   
			return hNvItem;
		}
		else
			return 0;
	}

	else
		// Ajout d'une branche de base
		return 0;
}

// Suppression branche y compris maj branche marque et branche famille si suppression Produits
bool CTreeCtrlX :: SupprimerBranche(HTREEITEM hBranch,HTREEITEM hBranchOrg,bool VirerNoeudMem,bool VirerNvPathFichier,HTREEITEM hBranchMarque,HTREEITEM hBranchFamille)
{

	HTREEITEM hItem;
	HTREEITEM hItemParent;
	int CodeDessin;
	CNoeud *pNoeud;
	CString TextItem;
	CString NomFichier;

	pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBranch);

	if (ItemHasChildren(hBranch))
	{
		// enfants sur cette branche
		hItem = GetChildItem(hBranch);
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hItem);
		while (hItem != NULL)
		{
			if (ItemHasChildren(hItem))
			{
				// encore des enfants à ce niveau
				TextItem = GetItemText(hItem);
				SupprimerBranche(hItem,hBranchOrg,VirerNoeudMem,VirerNvPathFichier,hBranchMarque,hBranchFamille);
			}	
			else
			{
				TextItem = GetItemText(hItem);
			    SupprimerBranche(hItem,hBranchOrg,VirerNoeudMem,VirerNvPathFichier,hBranchMarque,hBranchFamille);
			}

			hItem = GetNextItem(hItem,TVGN_NEXT);			
			TextItem = GetItemText(hItem);
			pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBranch);
		}
		TextItem = GetItemText(hBranch);
		SupprimerBranche(hBranch,hBranchOrg,VirerNoeudMem,VirerNvPathFichier,hBranchMarque,hBranchFamille);

	}

	else
	{
		GetItemImage(hBranch,CodeDessin,CodeDessin);
		if (CodeDessin == ImageCampagne)
		{
			// on vire le fichier campagne
			hItemParent = GetParentItem(hBranch);
			pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hItemParent);
			if (VirerNvPathFichier == true)
				NomFichier = pNoeud->m_PathFichier + "\\" + CTreeCtrlX::GetItemText(hBranch) + ".cmp";  
			else
				NomFichier = pNoeud->m_ExPathFichier + "\\" + CTreeCtrlX::GetItemText(hBranch) + ".cmp";  

			if (DeleteFile(NomFichier)== false)
				AfxMessageBox("Pb destruction fichier" + NomFichier); 

			// et on vire la branche
			SupprimerElmt(hBranch);
		}
			
		else
		{
			// pointe sur noeud associé à cette branche
			pNoeud = (CNoeud*)GetItemData(hBranch);
			if (CodeDessin == ImageProduit && (VirerNoeudMem == true))
			{
				// Suppression produit de la marque associé
				VirerProduitDansMarque(hBranchMarque,pNoeud->m_Marque,pNoeud->m_Alias);

				// Suppression produit de la famille/secteur associée
				VirerProduitDansFamilleSecteur(hBranchFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias);
			}

			if (CodeDessin == ImageAnnonceur && (VirerNoeudMem == true))
			{
				// Suppression produit de la famille/secteur associée
				VirerAnnonceurDansFamilleSecteur(hBranchFamille,pNoeud->m_Famille,pNoeud->m_Secteur,pNoeud->m_Alias);
			}


			// Suppression répertoire de la branche
			SupprimerRepBranche(hBranch,VirerNvPathFichier);

			// Détruit en mémoire la branche d'origine
			if ((hBranch == hBranchOrg) && (VirerNoeudMem == true))
			{
				// Suppression lien noeud
				SupprimerLienNoeud(pNoeud);

				// avant on enleve le noeud
				delete pNoeud;
			}
			
			// Suppression de la branche
			SupprimerElmt(hBranch);

		}
	}

	return true;
}

 
// CopyItem             - Copies an item to a new location
// Returns              - Handle of the new item
// hItem                - Item to be copied
// htiNewParent         - Handle of the parent for new item
// htiAfter             - Item after which the new item should be created
HTREEITEM CTreeCtrlX::CopierElmt(HTREEITEM hItem,HTREEITEM hNvParent,HTREEITEM hApres)
{
        TV_INSERTSTRUCT         tvstruct;
        HTREEITEM               hNvElmt;
        CString                 sText;

        // get information of the source item
        tvstruct.item.hItem = hItem;
        tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | 
                                TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        GetItem(&tvstruct.item);  
        sText = GetItemText( hItem );
        
        tvstruct.item.cchTextMax = sText.GetLength();
        tvstruct.item.pszText = sText.LockBuffer();

        // Insert the item at proper location
        tvstruct.hParent = hNvParent;
        tvstruct.hInsertAfter = hApres;
        tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
        hNvElmt = InsertItem(&tvstruct);
        sText.ReleaseBuffer();

        // Now copy item data and item state.
        SetItemData( hNvElmt, GetItemData( hItem ));
        SetItemState( hNvElmt, GetItemState( hItem, TVIS_STATEIMAGEMASK ), 
                                                    TVIS_STATEIMAGEMASK );

        // Call virtual function to allow further processing in derived class
        OnItemCopied( hItem, hNvElmt);

        return hNvElmt;
}

// Déplacement d'un élément sur une autre branche racine
HTREEITEM CTreeCtrlX::DeplacerElmt(HTREEITEM hElmtSrc,HTREEITEM hNvParent,HTREEITEM hApres)
{
	CString MessErr;

	HTREEITEM hNvElmt = CopierElmt( hElmtSrc, hNvParent, hApres);
	if (CTreeCtrlX::DeleteItem(hElmtSrc) == false)
	{
		MessErr = "Déplacement élément " + CTreeCtrlX::GetItemText(hElmtSrc) + "  impossible";
		AfxMessageBox (MessErr);
	}

	return hNvElmt;

}

// Suppression d'un elmt de la nomenclature
bool CTreeCtrlX :: SupprimerElmt(HTREEITEM hElmt)
{

	return CTreeCtrlX::DeleteItem(hElmt);
}

// Suppression du répertoire lié à une branche
bool CTreeCtrlX ::SupprimerRepBranche(HTREEITEM hBranche,bool VirerNvPathFichier)
{
	CNoeud *pNoeud;
	
	if (hBranche != NULL)
	{
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBranche);
		if (pNoeud != NULL)
		{
			// Suppression d'un répertoire pointé par Branche HItem	
			if (VirerNvPathFichier == true)
				RemoveDirectory(pNoeud->m_PathFichier);
			else
				RemoveDirectory(pNoeud->m_ExPathFichier);
			return true;
		}
		else
			return false;
		
	}
	else
		return false;
}

// Gestion des répertoires -- Ajout Nouveau Repertoire Branche
bool CTreeCtrlX :: AjoutRepBranche(CString RepACreer)
{
	CString Buf;

	if (CreateDirectory(RepACreer,NULL) == false)
	{
		if (RepACreer.Find('\\') != -1 ||
			RepACreer.Find('/') != -1  || 
			RepACreer.Find(':') != -1  ||
			RepACreer.Find('*') != -1  ||
			RepACreer.Find('?') != -1  ||
			RepACreer.Find('!') != -1  ||
			RepACreer.Find('"') != -1  ||
			RepACreer.Find('<') != -1  ||
			RepACreer.Find('>') != -1  ||
			RepACreer.Find('|') != -1) 
			Buf = "Problème certains caractères (\\,/,:,*,?,!,'""',<,>,|) nom répertoire non autorisés ";
		else
			Buf = "Problème création répertoire déjà existant " + RepACreer;
		AfxMessageBox (Buf);
		return false;
	}

	else 
		return true;
}


bool CTreeCtrlX :: ReconstituerRepBranche(HTREEITEM hBranche)
{
	CNoeud *pNoeud;
	CNoeud *pNoeudParent;
	HTREEITEM hBranchParent;
	HTREEITEM hSsBranch;
	int CodeDessin;

	if (hBranche != NULL)
	{
		// Creation répertoire associé à cette branche
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hBranche);
		hBranchParent = GetParentItem(hBranche);
		pNoeudParent = (CNoeud*)CTreeCtrlX::GetItemData(hBranchParent);
		if (pNoeud != NULL && pNoeudParent != NULL)
		{
			// attention si campagne ce sont des fichiers sinon répertoire
			GetItemImage(hBranche,CodeDessin,CodeDessin);
			
			if (CodeDessin != ImageCampagne)
			{
				// remet le bon path fichier
				pNoeud->m_PathFichier = pNoeudParent->m_PathFichier + "\\" + pNoeud->m_Libelle;
				if (AjoutRepBranche(pNoeud->m_PathFichier)== true)
				{
					// enfants sur cette branche
					if (ItemHasChildren(hBranche))
					{
						hSsBranch = GetChildItem(hBranche);
						while (hSsBranch != NULL)
						{
							ReconstituerRepBranche(hSsBranch);
							hSsBranch = GetNextItem(hSsBranch,TVGN_NEXT);			
						}

					}
				}

				else
					return false;
			}


		}
		
		else
			return false;
	}	

	else
		return false;

	return false;
}


void CTreeCtrlX::SupprimerLienNoeud(CNoeud *pNoeud)
{
	int NbSsNoeud;

	if(pNoeud->m_pRacine != NULL)
	{
		NbSsNoeud = pNoeud->m_pRacine->m_pNoeud.GetSize();
		if (NbSsNoeud != 0)
		{
			for (int i=0;i<NbSsNoeud;i++)
			{	
				if (pNoeud->m_pRacine->m_pNoeud[i] == pNoeud)
				{
					pNoeud->m_pRacine->m_pNoeud.RemoveAt(i);
					break;
				}
			}
				
		}

	}
}

HTREEITEM CTreeCtrlX :: TypeSsBranche(HTREEITEM hItem)
{
	HTREEITEM hExItem;
	HTREEITEM hItemRoot;

	hItemRoot = GetRootItem();
	if(hItem!=NULL)
	{
		while (hItem != NULL)
		{
			hExItem = hItem;
			hItem = GetParentItem(hItem);
		}
	}

	return hExItem;

}

// Récupération de la date d'exercice à tous les niveaux
CString CTreeCtrlX ::DateExerciceBranche(HTREEITEM hItem)
{
	CString StrDate ="";
	HTREEITEM hRootItem = NULL;
	CNoeud *pNoeud;
	CNoeud *pNoeudParent;
	CNoeud *pNoeudGrandParent;
	
	// Recup Racine Arbre
	hRootItem = GetRootItem();

	// Récup le noeud associé
	if (hItem!=NULL && hItem != hRootItem)
	{
		pNoeud = (CNoeud*)CTreeCtrlX::GetItemData(hItem);

		if (pNoeud != NULL)
		{
			if (pNoeud->m_DateExercice != "" && pNoeud->m_DateExercice != "xxx")
				// ici info direct au niveau du noeud
				return 	pNoeud->m_DateExercice;
			
			else
			{
				hItem = GetParentItem(hItem);
				if (hItem!=NULL && hItem != hRootItem)
				{			
					pNoeudParent = (CNoeud*)CTreeCtrlX::GetItemData(hItem);
					if (pNoeudParent != NULL)
					{
						if (pNoeudParent->m_DateExercice != "" && pNoeudParent->m_DateExercice != "xxx")
							// ici info direct au niveau du noeud
							return 	pNoeudParent->m_DateExercice;
						else
						{
							hItem = GetParentItem(hItem);
							if (hItem!=NULL && hItem != hRootItem)
							{			
								pNoeudGrandParent = (CNoeud*)CTreeCtrlX::GetItemData(hItem);
								if (pNoeudGrandParent != NULL)
								{
									if (pNoeudGrandParent->m_DateExercice != "" && pNoeudGrandParent->m_DateExercice != "xxx")
										// ici info direct au niveau du noeud
										return 	pNoeudGrandParent->m_DateExercice;

								}
							}
						}
					}
				}
			}
		}
	}

	// Ici date non trouvé on renvoie chaine vide
	return StrDate;

}


void CTreeCtrlX::OnItemCopied(HTREEITEM /*hItem*/, HTREEITEM /*hNewItem*/ )
{
        // Virtual function 
}

BEGIN_MESSAGE_MAP(CTreeCtrlX, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeCtrlX)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeCtrlX message handlers



void CTreeCtrlX::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
