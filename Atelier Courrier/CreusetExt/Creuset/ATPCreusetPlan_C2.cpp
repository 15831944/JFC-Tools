// on inclut les définitions nécesaires
#include "stdafx.h"
#include "Resource.h"
#include "ATPCreusetPlan_C2.h"
#include "ATPCreusetPlan.h"
#include ".\atpcreusetplan_c2.h"
#include "AjoutInsertion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPCreusetPlan_C2::CATPCreusetPlan_C2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
	m_indCplEdited = -1;
	AllowSelection(false);
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetPlan_C2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetPlan_C2::SetParent(CATPCreusetPlan * pATPCreusetPlan)
{
	m_pATPCreusetPlan = pATPCreusetPlan; 
}
//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetPlan_C2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetPlan_C2::~CATPCreusetPlan_C2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetPlan_C2, JFCWaveSelect)
	//{{AFX_MSG_MAP(CATPCreusetPlan_C2)
	
	//}}AFX_MSG_MAP
	ON_COMMAND(IDM_CR_POPUPGRILLE_ITEM0, OnCrPopupgrilleItem0)
	ON_COMMAND(IDM_CR_POPUPGRILLE_ITEM1, OnCrPopupgrilleItem1)
	ON_COMMAND(IDM_CR_POPUPGRILLE_ITEM2, OnCrPopupgrilleItem2)
	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

/////////////////////////////////////////////////
// la fonction pour récupérer l'état des éléments

LONG CATPCreusetPlan_C2::OnGetItemState(LONG indice, JDate date)
{
	LONG Etat = 0;

	int NbInsertions = 0;

	// on récupère l'identifiant du Support
	CKeySupport Support;
	Support.m_IdSupport = 0;
	if (m_pATPDocCreuset->m_AFFPlateauGrille)
	{
		// Mode affichage plateau (tous les supports sélectionnées)
		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
			// Récupère identificateur support sur le plateau plan étendu
			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(indice);
		else
			// Récupère identificateur support sur le plateau hypothèses
			Support = m_pATPDocCreuset->m_PlateauSupports.Item(indice);
	}
	else
	{
		// Récupère index plateau
		JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(indice);
		if (InxPlateau >=0)
			if (m_pATPDocCreuset->m_AFFPlanEtendu) 
				// Récupère identificateur support sur le plateau plan étendu
				Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
			else
				// Récupère identificateur support sur le plateau
				Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
	}

	// on récupère l'hypothèse courante
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

	if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
	{
		// On boucle sur tous les formats actifs (il n'y en a qu'un) 
		for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			// Récupére état du format
			JUnt32 EtatF = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
			
			if(!(EtatF & 0x0003))
				continue;

			// on récupère le plan de l'hypothèse courante
			CPlanInsertions * pPLNInsertions = &m_pATPDocCreuset->m_TblHypotheses.GetItem();

			// Id format courant
			JUnt32 IdFormatCur = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();

			//on initialise le fond
			if (EtatF & 0x0003)
			{
				// Indication zone tarifaire  A FAIRE Légende
				//
				// NOTRF  / JFCWAVE_FOND_RED   : pas de tarif	= fond rose
				// TRFOUT / JFCWAVE_FOND_GRIS  : hors période	= fond gris
				// TRFMAN / JFCWAVE_FOND_TRF1  : tarif manuel	= "JFCWAVE_FOND_TRF1"
				// TRFPAR / JFCWAVE_FOND_TRF2  : tarif coiplage = "JFCWAVE_FOND_TRF2"  
				//        / JFCWAVE_FOND_RED2  : ????
				//        / JFCWAVE_FOND_MASK  : ????
				//        / JFCWAVE_FOND_SEL   : ???? 

				CTarifs* pTarif = CTarifs::GetInstance();
	
				//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
				RETTRF ret2 = pTarif->GetStatutTarifNew(Support.m_IdSupport, IdFormatCur, date, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());

				if(ret2 == NOTRF)
					//pas de trf
					Etat |= JFCWAVE_FOND_RED;

				if (ret2 == TRFOUT)
					// hors periode
					Etat |= JFCWAVE_FOND_GRIS;

				if (ret2 == TRFMAN)
					// tarif manuel
					Etat |= JFCWAVE_FOND_TRF1;

				if (ret2 == TRFPAR)
					// tarif couplage
					Etat |= JFCWAVE_FOND_TRF2;

			}

			if (m_indCplEdited!= -1 &&  indice != m_indCplEdited)
			{
				Etat &= JFCWAVE_FOND_MASK;
				Etat |= JFCWAVE_FOND_GRIS;
			}
			if(indice == m_indCplEdited	&& (date < m_dateCplMin || m_dateCplMax < date))
			{
				Etat &= JFCWAVE_FOND_MASK;
				Etat |= JFCWAVE_FOND_GRIS;
			}


			// on crée la clé de l'insertion
			CInsKeySFD KeySFD;
			KeySFD.m_IdFormat  = IdFormatCur;
			KeySFD.m_DateD     = date;


			if(Support.m_IdParent)
			{
				KeySFD.m_IdSupport	= Support.m_IdParent;
				KeySFD.m_IdParent	= 0;

				// on recherche l'insertion
				pPLNInsertions->MoveTo_SFD(KeySFD,-1);

				// on teste si on a trouvé une insertion
				if (!pPLNInsertions->MoveTo_SFD(KeySFD,-1))
					continue;

				pPLNInsertions = &(pPLNInsertions->GetItem_SFD()->GetCouplageInsertions());
			}

			// on initialise la clé
			KeySFD.m_IdSupport	= Support.m_IdSupport;
			KeySFD.m_IdParent	= Support.m_IdParent;

			// on recherche l'insertion
			pPLNInsertions->MoveTo_SFD(KeySFD,-1); //  -1);

			// on teste si on a trouvé une insertion
			if (pPLNInsertions->IsValid_SFD())
			{
				// on récupère l'insertion
				const CInsertion * pInsertion = pPLNInsertions->GetItem_SFD();

				// on récupère la durée de l'insertion
				JInt32 Duree	= pInsertion->m_Duree;
				JDate Fin		= pInsertion->m_DateD+Duree-1;

				// on teste si l'insertion correspond à notre demande
				if (pInsertion->m_IdSupport	== KeySFD.m_IdSupport &&
					pInsertion->m_IdParent	== KeySFD.m_IdParent &&
					pInsertion->m_IdFormat	== KeySFD.m_IdFormat &&
					!(date < pInsertion->m_DateD || date > Fin))
				{
					if (EtatF & 0x0003)
					{
						// on comptabilise les insertions seulement sur les formats affichés
						NbInsertions += pInsertion->m_NbInsertions; 

						// on vérifie la durée
						if (Duree == 1)//quotidien
						{
							if(date == pInsertion->m_DateD)
							{
								Etat &= JFCWAVE_ELEM_MASK;
								if (EtatF == 2 && NbInsertions<=1)
									Etat |= JFCWAVE_ELEM_QTD_BORD;
								else
									Etat |= JFCWAVE_ELEM_QTD;
								
								if(m_pATPDocCreuset->m_AFFStatutInsert)
								{
									Etat &= JFCWAVE_STATE_MASK;
									if(pInsertion->m_Data.m_status == JATPDATInsertion::STATUT1)
										Etat |= JFCWAVE_STATE_STATUS1;
									if(pInsertion->m_Data.m_status == JATPDATInsertion::STATUT2)
										Etat |= JFCWAVE_STATE_STATUS2;
								}

							}
						}//Duree == 1
						
						//autres périodicités
						if (Duree>1)
						{
							JInt32 Delta = (date - pInsertion->m_DateD) +1;
							Etat &= JFCWAVE_ELEM_MASK;
							if(Delta == 1)//1er jour
							{
								if (EtatF == 2 && NbInsertions<=1)
									Etat |= JFCWAVE_ELEM_START_BORD;
								else
									Etat |= JFCWAVE_ELEM_START;
							}
							if(Delta == Duree)//Dernier jour
							{
								if (EtatF == 2 && NbInsertions<=1)
									Etat |= JFCWAVE_ELEM_STOP_BORD;
								else
									Etat |= JFCWAVE_ELEM_STOP;
							}
							if(Delta >1 && Delta < Duree )//interieur
							{
								if (EtatF == 2 && NbInsertions<=1)
									Etat |= JFCWAVE_ELEM_ELEM_BORD;
								else
									Etat |= JFCWAVE_ELEM_ELEM;
							}
							if(Delta == 2 && m_pATPDocCreuset->m_AFFChromie)
							{
								Etat &= JFCWAVE_STATE_MASK;
								switch(pInsertion->m_IdFormat%4)
								{
								case 1://Noir
									Etat |= JFCWAVE_STATE_CHROMIE1;
									break;
								case 2://Bichro
									Etat |= JFCWAVE_STATE_CHROMIE2;
									break;
								case 3://Trichro
									Etat |= JFCWAVE_STATE_CHROMIE3;
									break;
								case 0://Quadri
									Etat |= JFCWAVE_STATE_CHROMIE4;
									break;
								}
							}

							if(Delta == 3 && m_pATPDocCreuset->m_AFFStatutInsert)
							{
								Etat &= JFCWAVE_STATE_MASK;
								if(pInsertion->m_Data.m_status == JATPDATInsertion::STATUT1)
									Etat |= JFCWAVE_STATE_STATUS1;
								if(pInsertion->m_Data.m_status == JATPDATInsertion::STATUT2)
									Etat |= JFCWAVE_STATE_STATUS2;
							}

						}//Duree > 1
					}
				}//validité insrt trouvée
			}//validité insrt trouvée

		}//for formats


		if(NbInsertions>1)
		{
			Etat &= JFCWAVE_STATE_MASK;
			Etat |= JFCWAVE_STATE_SUPERPOSE;
		}
	}


	JDate first, last;
	m_pATPCreusetPlan->m_C1.GetSelRange(first, last);
	if ((first.IsValid() && last.IsValid())
	&& (first <= date && date <= last))
	{
		Etat &= JFCWAVE_FOND_MASK;
		Etat |= JFCWAVE_FOND_SEL;
	}


	return Etat;
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
JVoid CATPCreusetPlan_C2::OnLButtonDown(LONG indice, JDate date)
{
}

JVoid CATPCreusetPlan_C2::OnLButtonUp(LONG indice, JDate date)
{
	//Test lors de l'édition d'un couplage
	if (m_indCplEdited!= -1 &&  indice != m_indCplEdited)
		return;
	
	//Menu contextuel
	if(HasSelection())
	{
		CMenu menu;
		CPoint pt;
		pt = m_ptFirst;
		ClientToScreen(&pt);
		menu.LoadMenu(IDR_CR_GRILLE);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		return;
	}

	// Ajout de l'insertion sur support indicé et à la date en cours
	JDate DateFinIns;
	short StateCtrl  = GetAsyncKeyState(VK_CONTROL);
	short StateShift = GetAsyncKeyState(VK_SHIFT);
	AjouterInsertion(indice, date, DateFinIns, StateCtrl, StateShift);
}

////////////////////////////////////////////////////////////////////////////////
// Ajout de l'insertion sur support indicé et à la date en cours
JBool CATPCreusetPlan_C2::AjouterInsertion(LONG indice, JDate date, JDate &DateFinIns, short StateCtrl, short StateShift)
{
	// Si plan master actif / rien à faire
	if (!m_pATPDocCreuset->m_IdxHypotheseCourant)
	{
		CString ttl, qst;
		ttl.LoadString(IDR_CR_ATPFRAME);
		qst.LoadString(IDS_CR_NOINSPM);
		MessageBox(qst, ttl, MB_OK|MB_ICONINFORMATION);
		return false;
	}

	// Invalide en mode plan étendu
	if (m_pATPDocCreuset->m_AFFPlanEtendu) 
		return false; 

	// on initialise le code de retour
	JInt32 Ret = 0;

	// on récupère l'identifiant du Support
	JInt32 IndicePlateau = indice;
	if (m_pATPDocCreuset->m_AFFPlateauGrille == 0) 
		IndicePlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(indice);

	// Récupération du support
	const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(IndicePlateau);

	// Récupération de la périodicité du support
	CRegroupements* regroup = CRegroupements::GetInstance();
	IElem * pElem = regroup->GetIElemByID(Support.m_IdSupport);
	JUnt32 IdPeriod_Support = 0;
	if(pElem && pElem->IsTitre() && !pElem->IsCouplage())
		IdPeriod_Support =  pElem->AsITitre()->GetPeriodicite();

	// Est-ce un quotidien ?
	// Aurélien : est-ce une erreur????? a mon avis oui!!
	// avavnt : JBool SupportQuotidien = (IdPeriod_Support = 1);
	// maintenant :
	JBool SupportQuotidien = (IdPeriod_Support == 1);

	// on vérifie la validité de l'hypothèse
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

	//insertion dans un titre (ie pas un couplage)
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && !Support.m_IdParent)
	{
		// Flag suppression insertion
		JBool FlagSuppInsertion = false;

		// Clé insertion
		CInsKeySFD KeySFD;

		// on récupère le plan de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		// On boucle sur tous les formats actifs
		for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			// Récupére état du format
			JUnt32 Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();

			if (Etat == 3)
			{
				// Id format courant
				JUnt32 IdFormatCur = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();

				// Format sélectionné
				// on initialise la clé (Id Supp + Id Parent + Id Format + Id Date
				KeySFD.m_IdSupport			= Support.m_IdSupport;
				KeySFD.m_IdParent			= Support.m_IdParent; 
				KeySFD.m_IdFormat			= IdFormatCur;
				KeySFD.m_DateD				= date;

				// on recherche l'insertion et on teste si on a trouvé une insertion
				if (PLNInsertions.MoveTo_SFD(KeySFD, -1))
				// while (CptInsertion > 0 && PLNInsertions.MoveTo_SFD(KeySFD, -1))	
				{
					// on récupère l'insertion
					CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

					// on teste si l'insertion correspond à notre demande
					if (pInsertion->m_IdSupport	== KeySFD.m_IdSupport &&
						pInsertion->m_IdParent	== KeySFD.m_IdParent)
					{
						// on récupère la durée de l'insertion
						JInt32 Duree = pInsertion->m_Duree;

						// On teste si on est bien sur les bornes intervalle date insertion
						if (date >= pInsertion->m_DateD && 
							date <= (pInsertion->m_DateD + Duree-1))
						{
							// on calcule le delta de la date courante
							JInt32 Delta = (date - pInsertion->m_DateD) + 1;

							// on modifie le code
							Ret = 1;

							// on teste si on est dans la période
							if (Delta >= 1 && Delta <= Duree)
							{
								// Si même format
								if (pInsertion->m_IdFormat == KeySFD.m_IdFormat)
								{
									// Increment the insertions
									if(StateCtrl<0 || StateShift<0)
									{
										// on recopie l'insertion courante
										CInsertion Insertion = *pInsertion;
										// on ne rajoute qu'une seule insertion
										Insertion.m_NbInsertions = 1;
										// Ajout insertion
										PLNInsertions.AddInsertion(Insertion,m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																             m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());

										// Positionne date fin insertion (pour insertion suivante)
										DateFinIns = pInsertion->m_DateD + Duree-1;
									}
									else
									{
										// Suppression insertion
										PLNInsertions.DeleteInsertion_SFD(m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																          m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
										FlagSuppInsertion = true;
									}

									// on teste l'affichage du zoom
									// on met à jour l'affichage du zoom
									if (m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
										m_pATPDocCreuset->m_AFFZoomPlan.Reset();

									continue;//on passe au format suivant
								}
							}
						}
					}
				}

				CInsertion Insertion;
				Insertion = KeySFD;

				// Ajout de l'insertion (avec tentative j + 2 si quotidien) 
				JUnt32 CptInsertion = 3;
				while (CptInsertion > 0)
				{
					// Vérifie que l'insertion est possible
					if (SetInfoInsertion(Insertion,
						m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD()))
					{

						// Si c'est un couplage il faut ajouter les insertions dans les fils
						if(CAjoutInsertion::IsCouplage(Insertion.m_IdSupport))
						{
							// Get Date inferior
							JDate DateMin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
							// Get Date superior
							JDate DateMax = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
							CInsKeySDF KeySDF;
							KeySDF = KeySFD;
							KeySDF.m_IdFormat = 0;

							CInsertion * pInsertion = 0;
							if(PLNInsertions.MoveTo_SDF(KeySDF,-1))
							{
								pInsertion = PLNInsertions.GetItem_SDF();
								if(pInsertion->m_IdSupport != Insertion.m_IdSupport)
								{
									pInsertion = 0;
									if(PLNInsertions.MoveTo_SDF(KeySDF,+1))
									{
										pInsertion = PLNInsertions.GetItem_SDF();
										if(pInsertion->m_IdSupport != Insertion.m_IdSupport)
											pInsertion = 0;
									}
								}
								if(pInsertion)
								{
									// Test si date nvlle insertion incluse dans dates trouvées
									if ((Insertion.m_DateD >= pInsertion->m_DateD) &&
										(Insertion.m_DateD < pInsertion->m_DateD + pInsertion->m_Duree))
									{
										// Récup date début et date fin insertion couplage existant
										DateMin = pInsertion->m_DateD;
										DateMax = pInsertion->m_DateD + pInsertion->m_Duree-1;

										DateFinIns = DateMax;
									}
									else
									{
										if ((pInsertion->m_DateD + pInsertion->m_Duree) <= Insertion.m_DateD)
											DateMin = pInsertion->m_DateD + pInsertion->m_Duree;
										else
										{
											DateMax = pInsertion->m_DateD ;
											DateFinIns = DateMax + pInsertion->m_Duree - 1;
										}
									}
								}
							}
							else
							{
								CInsertion * pInsertion = 0;
								if(PLNInsertions.MoveTo_SDF(KeySDF,+1))
								{
									pInsertion = PLNInsertions.GetItem_SDF();
									if(pInsertion->m_IdSupport == Insertion.m_IdSupport)
									{
										// Test si date nvlle insertion incluse dans dates trouvées
										if ((Insertion.m_DateD >= pInsertion->m_DateD) &&
											(Insertion.m_DateD < pInsertion->m_DateD + pInsertion->m_Duree))
										{
											// Récup date début et date fin insertion couplage existant
											DateMin = pInsertion->m_DateD;
											DateMax = pInsertion->m_DateD + pInsertion->m_Duree-1;
										}
										else
										{
											if (pInsertion->m_DateD > Insertion.m_DateD)
												DateMax = pInsertion->m_DateD ;
											else
												DateMin = pInsertion->m_DateD + pInsertion->m_Duree;
										}

										// Récup date fin
										DateFinIns = DateMax + pInsertion->m_Duree - 1;
									}
								}
							}

							if(!CAjoutInsertion::AddInsertionCouplage(Insertion,DateMin,DateMax,m_pATPDocCreuset))
								return false;
							// on vérifie la cohérence de la période de l'étude
							if ((Insertion.m_DateD >= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() && (Insertion.m_DateD + (Insertion.m_Duree - 1)) <= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()) &&
								(Insertion.m_DateD <= date && (Insertion.m_DateD + (Insertion.m_Duree - 1)) >= date))
							{
								// on teste les exceptions
								try
								{
									// on ajoute l'insertion Support
									Insertion.m_NbInsertions = 1;
									if(PLNInsertions.AddInsertion(Insertion,
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
									{
										Ret = 1;

										// Positionne date fin insertion (pour insertion suivante)
										DateFinIns = Insertion.m_DateD + Insertion.m_Duree - 1;
									}
									
								}
								catch (JException* pException)
								{
									// on affiche le message de l'exception
									::AfxMessageBox(pException->GetMessage(), MB_ICONERROR);
								}
								// on teste l'affichage du zoom
								if (Ret && m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
								{
									CInsKey Key;
									Key = Insertion;
									// on met à jour l'affichage du zoom
									m_pATPDocCreuset->m_AFFZoomPlan.SetAffZoomPlan(m_pATPDocCreuset->m_IdxHypotheseCourant, Key);
								}
							}
						}
						else
						{
							// on vérifie la cohérence de la période de l'étude
							if ((Insertion.m_DateD >= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() && Insertion.m_DateD <= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()) &&
								(Insertion.m_DateD <= date && (Insertion.m_DateD + (Insertion.m_Duree - 1)) >= date))
							{
								// on teste les exceptions
								try
								{

									// on ajoute l'insertion Support
									Insertion.m_NbInsertions = 1;
									if(PLNInsertions.AddInsertion(Insertion,
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
									{
										Ret = 1;

										// Positionne date fin insertion (pour insertion suivante)
										DateFinIns = Insertion.m_DateD + Insertion.m_Duree - 1;
									}
								}
								catch (JException* pException)
								{
									// on affiche le message de l'exception
									::AfxMessageBox(pException->GetMessage(), MB_ICONERROR);
								}
								// on teste l'affichage du zoom
								if (Ret && m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
								{
									CInsKey Key;
									Key = Insertion;
									// on met à jour l'affichage du zoom
									m_pATPDocCreuset->m_AFFZoomPlan.SetAffZoomPlan(m_pATPDocCreuset->m_IdxHypotheseCourant, Key);
								}
							}
						}

						// Stop insertion
						CptInsertion = 0;
					}

					// Tentative + 2jours pour les quotidiens (en effet jours week-end souvent non tarifés)
					else if (SupportQuotidien)
					{
						// On tente insertion au jour suivant pour les quotidiens
						Insertion.m_DateD++; 
						date++; 

						// On décrèmente le compteur tentative insertion
						CptInsertion--;
					}
					else
					{
						// Stop insertion et aucun traitement (rien n'a changé)- Résoud Track n° 50
						Ret = 0;
						CptInsertion = 0;
					}

				}
			}
		}//for formats actifs

		// on met à jour le document
		if (Ret)
		{ 
			// si on ajoute un spot à la dernière hypothèse, on en ajoute une nouvelle
			if (m_pATPDocCreuset->m_IdxHypotheseCourant <  m_pATPDocCreuset->m_MaxHypothese
				&& m_pATPDocCreuset->m_IdxHypotheseCourant  == m_pATPDocCreuset->m_TblHypotheses.GetCount() - 1) 
				m_pATPDocCreuset->AddNewHypo();

			//avant le calcul
			Invalidate();
			UpdateWindow();

			// Indique Recalcul échelle pour les apports  OPTIM 2.7
			m_pATPDocCreuset->UpdateAllViews(UPDATE_CALC_ECHELLE_SANS_CALCAPPORT);
			m_pATPDocCreuset->m_ApportARecalculer = true;

			// Recalcul les audiences
			m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO_LIGHT);

			// m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);

			// Si suppression insertion, test si hypothese avec au moins 1 insertion, sinon décalage
			if (FlagSuppInsertion)
			{
				// on récupère le plan de l'hypothèse courante après suppression élémnt
				m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
				CPlanInsertions & PLNInsert = m_pATPDocCreuset->m_TblHypotheses.GetItem();

				// Test si encore des insertions pour ce plan
				if (PLNInsert.GetCount() == 0 &&
					m_pATPDocCreuset->m_TblHypotheses.GetCount() > 2 &&
					m_pATPDocCreuset->m_IdxHypotheseCourant == m_pATPDocCreuset->m_TblHypotheses.GetCount()-2)
				{
					// Suppression de l'hypothèse dans document
					m_pATPDocCreuset->UpdateAllViews(UPDATE_SUPP_HYPOTHESECUR);
				}
			}

			// le plan creuset est modifié
			m_pATPDocCreuset->SetModifiedFlag(TRUE); 
			//m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(false);
			//Invalidate();
			//m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
		}


	}

//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	
//////////////////////////////////////////////////////////////////////////////////////////////////////	

	//insertion dans un couplage
	if(	m_pATPDocCreuset->m_TblHypotheses.IsValid() && Support.m_IdParent)
	{
		//pas en cours d'édition
		if(m_indCplEdited == -1)
		{
			//on recherche l'insertion

			for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				JUnt32 EtatF = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
				
				if(!(EtatF & 0x0001))
					continue;

				CPlanInsertions * pPLNInsertions = &m_pATPDocCreuset->m_TblHypotheses.GetItem();

				JUnt32 IdFormatCur = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
				CInsKeySFD KeySFD;
				KeySFD.m_IdFormat  = IdFormatCur;
				KeySFD.m_DateD     = date;

				// on initialise la clé

				KeySFD.m_IdSupport	= Support.m_IdParent;
				KeySFD.m_IdParent	= 0;

				// on recherche l'insertion
				pPLNInsertions->MoveTo_SFD(KeySFD,-1);

				// on teste si on a trouvé une insertion
				if (!pPLNInsertions->MoveTo_SFD(KeySFD,-1))
					continue;

				//on recupre le plan du couplage
				CPlanInsertions * pPLNInsertCPL;
				m_insertCpl = pPLNInsertions->GetItem_SFD();
				pPLNInsertCPL = &(m_insertCpl->GetCouplageInsertions());

				//on recherche les dates limites
				m_dateCplMin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
				m_dateCplMax = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
				if (pPLNInsertions->MovePrev_SFD())
				{
					if(pPLNInsertions->GetItem_SFD()->m_IdSupport == KeySFD.m_IdSupport)
					{
						m_dateCplMin = pPLNInsertions->GetItem_SFD()->m_DateD + pPLNInsertions->GetItem_SFD()->m_Duree;
						CTarifs* pTarif = CTarifs::GetInstance();
						JDate date = m_dateCplMin;
						while (pTarif->GetStatutTarif(Support.m_IdSupport, IdFormatCur, date, m_dateCplMin) < 0)
							date++;
						m_dateCplMin = date;
					}
				}
				//On retourne sur notre insertion
				pPLNInsertions->MoveTo_SFD(KeySFD,-1);
				if (pPLNInsertions->MoveNext_SFD())
				{
					if(pPLNInsertions->GetItem_SFD()->m_IdSupport == KeySFD.m_IdSupport)
					{
						m_dateCplMax= pPLNInsertions->GetItem_SFD()->m_DateD-1;
						CTarifs* pTarif = CTarifs::GetInstance();
						JDate date = m_dateCplMax;
						JUnt32 duree, trf;
						
						// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
						pTarif->GetTarifByDate(Support.m_IdSupport, IdFormatCur, m_pATPDocCreuset->m_MapTarifManuel, date-1, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(), date, duree, trf);
						if(date+duree-1 > m_dateCplMax)
							m_dateCplMax = date-1;
					}
				}


				KeySFD.m_IdSupport	= Support.m_IdSupport;
				KeySFD.m_IdParent	= Support.m_IdParent;

				// on recherche l'insertion
				// on teste si on a trouvé une insertion
				if(pPLNInsertCPL->MoveTo_SFD(KeySFD,-1))
				{
					// on récupère l'insertion
					CInsertion * pInsertion = pPLNInsertCPL->GetItem_SFD();

					// on récupère la durée de l'insertion
					JInt32 Duree	= pInsertion->m_Duree;
					JDate Fin		= pInsertion->m_DateD+Duree-1;

					// on teste si l'insertion correspond à notre demande
					if (pInsertion->m_IdSupport	== KeySFD.m_IdSupport &&
						pInsertion->m_IdParent	== KeySFD.m_IdParent &&
						pInsertion->m_IdFormat	== KeySFD.m_IdFormat &&
						!(date < pInsertion->m_DateD || date > Fin))
					{
						
						//on vire l'insertion
						pPLNInsertCPL->DeleteInsertion_SFD(m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
													          m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
						m_indCplEdited = indice;
						AllowSelection(false);
						//on interdit le bouton effacer et autre opérations
					}
				}
			}
			Invalidate();
			
			//
		}
		//on remet l'insertion déplacée
		else
		{
			
			//on rejette la date si hors zone
			if(m_dateCplMin> date || date > m_dateCplMax )
				return false;

			//on recherche l'insertion
			for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				JUnt32 EtatF = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
				
				if(!(EtatF & 0x0003))
					continue;

				CPlanInsertions * pPLNInsertions = &m_pATPDocCreuset->m_TblHypotheses.GetItem();

				JUnt32 IdFormatCur = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
				CInsKeySFD KeySFDCpl;
				CInsKeySFD KeySFD;
				
				
				CPlanInsertions & PLNInsertCpl = m_insertCpl->GetCouplageInsertions();
				KeySFDCpl = *m_insertCpl;
				if(!pPLNInsertions->MoveTo_SFD(KeySFDCpl))
					return false;
		
				KeySFD.m_IdFormat  = IdFormatCur;
				KeySFD.m_DateD     = date;
				KeySFD.m_IdSupport	= Support.m_IdSupport;
				KeySFD.m_IdParent	= Support.m_IdParent;

				// on recherche l'insertion
				if(PLNInsertCpl.MoveTo_SFD(KeySFD,-1))
				{

					// on récupère l'insertion
					CInsertion * pInsertion = PLNInsertCpl.GetItem_SFD();

					// on récupère la durée de l'insertion
					JInt32 Duree	= pInsertion->m_Duree;
					JDate Fin		= pInsertion->m_DateD+Duree-1;

					// on teste si l'insertion correspond à notre demande
					if (pInsertion->m_IdSupport	== KeySFD.m_IdSupport &&
						pInsertion->m_IdParent	== KeySFD.m_IdParent &&
						pInsertion->m_IdFormat	== KeySFD.m_IdFormat &&
						!(date < pInsertion->m_DateD || date > Fin))
					{
						
						//on ajoute
						PLNInsertCpl.AddInsertion(*pInsertion, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
													          m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
						
						
						//CInsertion inserCpl(*pInsertionCpl);
						
						//resize de l'insertion cpl
						CInsertion inserCpl(*m_insertCpl);
						JDate dMin, dMax;
						dMin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
						dMax = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
						for(PLNInsertCpl.MoveFirst_SDF(); PLNInsertCpl.IsValid_SDF(); PLNInsertCpl.MoveNext_SDF())
						{
							CInsertion* ins = PLNInsertCpl.GetItem_SDF();
							if (ins->m_DateD < dMin)
								dMin = ins->m_DateD;
							if (ins->m_DateD+ins->m_Duree > dMax)
								dMax = ins->m_DateD+ins->m_Duree;
						}
						inserCpl.m_DateD = dMin;
						inserCpl.m_Duree = inserCpl.m_Data.m_Duree = dMax - dMin;
						pPLNInsertions->DeleteInsertion_SFD(m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
															m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
						pPLNInsertions->AddInsertion(inserCpl, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
															m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());

						m_indCplEdited = -1;

						// Positionne date fin insertion (pour insertion suivante)
						DateFinIns = inserCpl.m_DateD + inserCpl.m_Duree - 1;

						//AllowSelection();
					}
					else
					{
						CInsertion newinsert;
						newinsert = KeySFD;
						if (SetInfoInsertion(newinsert, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD()))
						{
							if ((newinsert.m_DateD >= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() && newinsert.m_DateD <= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()) &&
								(newinsert.m_DateD <= date && (newinsert.m_DateD + (newinsert.m_Duree - 1)) >= date))
							{
								// on teste les exceptions
								try
								{
									// on ajoute l'insertion Support
									newinsert.m_NbInsertions = 1;
									if(PLNInsertCpl.AddInsertion(newinsert,
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
										
										{
											//Ret = 1;
											
											CInsertion inserCpl(*m_insertCpl);

											JDate dMin, dMax;
											dMin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
											dMax = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
											for(PLNInsertCpl.MoveFirst_SDF(); PLNInsertCpl.IsValid_SDF(); PLNInsertCpl.MoveNext_SDF())
											{
												CInsertion* ins = PLNInsertCpl.GetItem_SDF();
												if (ins->m_DateD < dMin)
													dMin = ins->m_DateD;
												if (ins->m_DateD+ins->m_Duree > dMax)
													dMax = ins->m_DateD+ins->m_Duree;
											}
											inserCpl.m_DateD = dMin;
											inserCpl.m_Duree = inserCpl.m_Data.m_Duree = dMax - dMin;


											pPLNInsertions->DeleteInsertion_SFD(m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																				m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
											pPLNInsertions->AddInsertion(inserCpl, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																				m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());

											m_indCplEdited = -1;

											// Positionne date fin insertion (pour insertion suivante)
											DateFinIns = inserCpl.m_DateD + inserCpl.m_Duree - 1;


											//AllowSelection();
										}
										
								}
								catch (JException* pException)
								{
									// on affiche le message de l'exception
									::AfxMessageBox(pException->GetMessage(), MB_ICONERROR);
								}
							}
						}	
					}
				}
				else
				{
					CInsertion newinsert;
					newinsert = KeySFD;
					if (SetInfoInsertion(newinsert, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD()))
					{
						if ((newinsert.m_DateD >= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() && newinsert.m_DateD <= m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()) &&
							(newinsert.m_DateD <= date && (newinsert.m_DateD + (newinsert.m_Duree - 1)) >= date))
						{
							// on teste les exceptions
							try
							{
								// on ajoute l'insertion Support
								newinsert.m_NbInsertions = 1;
								if(PLNInsertCpl.AddInsertion(newinsert,
															  m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
															  m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
								{
									//Ret = 1;
									
									CInsertion inserCpl(*m_insertCpl);
									JDate dMin, dMax;
									dMin = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
									dMax = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
									for(PLNInsertCpl.MoveFirst_SDF(); PLNInsertCpl.IsValid_SDF(); PLNInsertCpl.MoveNext_SDF())
									{
										CInsertion* ins = PLNInsertCpl.GetItem_SDF();
										if (ins->m_DateD < dMin)
											dMin = ins->m_DateD;
										if (ins->m_DateD+ins->m_Duree > dMax)
											dMax = ins->m_DateD+ins->m_Duree;
									}
									inserCpl.m_DateD = dMin;
									inserCpl.m_Duree = inserCpl.m_Data.m_Duree = dMax - dMin;
									pPLNInsertions->DeleteInsertion_SFD(m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());
									pPLNInsertions->AddInsertion(inserCpl, m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF());

									m_indCplEdited = -1;

									// Positionne date fin insertion (pour insertion suivante)
									DateFinIns = inserCpl.m_DateD + inserCpl.m_Duree - 1;

									//AllowSelection();
								}
							}
							catch (JException* pException)
							{
								// on affiche le message de l'exception
								::AfxMessageBox(pException->GetMessage(), MB_ICONERROR);
							}
						}
					}


				}
			}
			// le plan creuset est modifié
			m_pATPDocCreuset->SetModifiedFlag(TRUE); 
			//m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(false);
			Invalidate();
			UpdateWindow();

			m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);

		}
	}

	// à priori ajout insertion réussi
	if (Ret)
		return true;
	else
		return false;
}

JBool CATPCreusetPlan_C2::SetInfoInsertion(CInsertion & Insertion, JDate & DateF)
{
	// Add all children
	CRegroupements * pReg = CRegroupements::GetInstance();

	// Récup id support
	IElem *pElem = pReg->GetIElemByID(Insertion.m_IdSupport);

	// Test si c'est un couplage
	//if(pElem->IsCouplage())
	//	// Rien a faire ici
	//	return true;

	CTarifs * pTarifs = CTarifs::GetInstance();

	if(pTarifs)
	{
		JUnt32 Tarif = 0;
		JDate DateReal = Insertion.m_DateD;

		JUnt32 IdTitre;
		if(pElem->IsTitreApparie())
			IdTitre = pElem->AsTitreApparie()->GetIDTitre();
		else
			IdTitre = Insertion.m_IdSupport;

		// Attention si c'est un titre apparenté
		// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
		RETTRF ret2 = pTarifs->GetTarifByDateNew(IdTitre, Insertion.m_IdFormat, m_pATPDocCreuset->m_MapTarifManuel, Insertion.m_DateD,
												 m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(), m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF(), DateReal, Insertion.m_Duree, Tarif);

		if(ret2 != NOTRF && ret2 != TRFOUT)
		{
			Insertion.m_DateD			=	DateReal;
			Insertion.m_Data.m_Duree	=	Insertion.m_Duree;
			Insertion.m_Data.m_TarifN	=	Tarif;
			Insertion.m_Data.m_TarifP	=	Tarif;
			return true;
		}
	}
	return false;
}

////////////////////////////////////////////////////////
// la fonction pour capter les déplacements de la souris

JVoid CATPCreusetPlan_C2::OnMouseMove(LONG indice, JDate date)
{
	// on teste l'affichage du zoom
	if (m_pATPDocCreuset && m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
	{
		
		if(indice == -1)
		{
			//dès qu'on est en dehors, on RAZ le zoom
			m_pATPDocCreuset->m_AFFZoomPlan.Reset();
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_ZOOM_PLAN);
			return;
		}
		
		// on initialise le code
		JInt32 Ret = 0;

		// on récupère l'identifiant du Support
		JUnt32 IndicePlateau = indice;
		if (m_pATPDocCreuset->m_AFFPlateauGrille == 0) 
			IndicePlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(indice);

		// on récupère l'identifiant du Support
		const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(IndicePlateau);

		// on récupère l'hypothèse courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		// on vérifie la validité de l'hypothèse
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// on crée la clé de l'insertion
			CInsKeySFD Key;

			// on initialise la clé
			Key.m_IdSupport	=	Support.m_IdSupport ;
			Key.m_IdParent	=	Support.m_IdParent ;
			Key.m_DateD      =	date;
			JList<CInsKey> Tmp;
				
			for (m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveFirst();
				m_pATPDocCreuset->m_Offre.m_TBLIdFormats.IsValid();
				m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveNext())
			{

				Key.m_IdFormat = m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetItem();
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(Key.m_IdFormat);
				if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
				{
					// Récupére état du format
					JUnt32 Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
					//if (Etat & 0x0003)
					{
						// on récupère le plan de l'hypothèse courante
						CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

						// on recherche les insertions
						// on teste si on a trouvé au moins une insertion
						PLNInsertions.MoveTo_SFD(Key,-1);
						if (!PLNInsertions.IsValid_SFD())
							PLNInsertions.MoveTo_SFD(Key,+1);
						if (PLNInsertions.IsValid_SFD())
						{
							CInsertion* pInsertion = PLNInsertions.GetItem_SFD();
							if ( pInsertion->m_IdSupport == Key.m_IdSupport
							&& pInsertion->m_IdFormat == Key.m_IdFormat
							&& pInsertion->m_DateD <= Key.m_DateD
							&& pInsertion->m_DateD + pInsertion->m_Duree >= Key.m_DateD)
							{
								//Tmp.AddTail() = PLNInsertions.m_KeySFDMap.GetKey();
								CInsKeySDF clef; clef = *pInsertion;
								Tmp.AddTail() = clef;
							}
						}
					}
				}
			}
			// on met à jour l'affichage du zoom
			if(Tmp.GetCount())
			{
				m_pATPDocCreuset->m_AFFZoomPlan.SetAffZoomPlan(m_pATPDocCreuset->m_IdxHypotheseCourant, Tmp);
				// on modifie le code
				Ret = +1;
			}
 		}
		// on met à jour l'affichage du zoom
		if (Ret == 0)
			m_pATPDocCreuset->m_AFFZoomPlan.Reset();
		// on met à jour le document
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_ZOOM_PLAN);
	}
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetPlan_C2::OnUpdate()
{
	// on initialise la période
	JDate DateD;
	JDate DateF;

	// on initialise le nombre d'éléments
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie le document
		if (m_pATPDocCreuset->IsValid())
		{
			bool bContinue = false;
			for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
				m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() >= 2)
					bContinue = true;
			}
			if(bContinue)
				{
				// on récupère la période d'action (période étude si plan étendu)
				// MODIF ALAIN
				if(m_pATPDocCreuset->m_IdxHypotheseCourant == 0)
				{
					DateD = m_pATPDocCreuset->m_KEYEtude.GetDateD();
					DateF = m_pATPDocCreuset->m_KEYEtude.GetDateF();
				}
				else
				{				
					DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
					DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();
				}

				// on récupère le nombre d'éléments
				if (m_pATPDocCreuset->m_AFFPlanEtendu)
				{
					// Cas du plan étendu
					if (m_pATPDocCreuset->m_AFFPlateauGrille) 
						// nb élmts plateau sélectionné en mode étendu
						cy = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.GetCount();
					else
					{
						// nb élmt plateau compacté (plan sélectionné en plan étendu)
						cy = m_pATPDocCreuset->OnNbLigPlanPlateau(true);
					}
				}
				else
				{
					// Cas des hypothèses
					if (m_pATPDocCreuset->m_AFFPlateauGrille) 
						// nb élmts plateau sélectionné
						cy = m_pATPDocCreuset->m_PlateauSupports.GetCount();
					else
					{
						// nb élmt plateau compacté (plan sélectionné)
						cy = m_pATPDocCreuset->OnNbLigPlanPlateau(false);
					}
				}
			}

		}
	}

	// on met à jour le composant
	this->SetRange(DateD, DateF, FALSE);
	this->SetCount(cy, TRUE);
}

/////////////////////////////////////
// CATPCreusetPlan_C2 message handlers

void CATPCreusetPlan_C2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCWaveSelect::PreSubclassWindow();
}




void CATPCreusetPlan_C2::SetInsertStatus(JATPDATInsertion::InsertStatus stt)
{
	LONG iStart, jStart, iStop, jStop;
	GetSelRange( iStart, jStart, iStop, jStop);
	JDate dateStart, dateStop;
	dateStart = m_Debut+iStart;
	dateStop = 	m_Debut+iStop;

	for(LONG indice = jStart; indice<=jStop; indice++)
	{
		CKeySupport Support;
		Support.m_IdSupport = 0;
		if (m_pATPDocCreuset->m_AFFPlateauGrille)
		{
			// Mode affichage plateau (tous les supports sélectionnées)
			if (m_pATPDocCreuset->m_AFFPlanEtendu) 
				// Récupère identificateur support sur le plateau plan étendu
				Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(indice);
			else
				// Récupère identificateur support sur le plateau hypothèses
				Support = m_pATPDocCreuset->m_PlateauSupports.Item(indice);
		}
		else
		{
			// Récupère index plateau
			JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(indice);
			if (InxPlateau >=0)
				if (m_pATPDocCreuset->m_AFFPlanEtendu) 
					// Récupère identificateur support sur le plateau plan étendu
					Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
				else
					// Récupère identificateur support sur le plateau
					Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
		}

		// on récupère l'hypothèse courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// On boucle sur tous les formats actifs 
			for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
				m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				// Récupére état du format
				JUnt32 EtatF = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();

				if(!(EtatF & 0x0003))
					continue;

				// on récupère le plan de l'hypothèse courante
				CPlanInsertions * pPLNInsertions = &m_pATPDocCreuset->m_TblHypotheses.GetItem();


				// Id format courant
				JUnt32 IdFormatCur = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();

				// on crée la clé de l'insertion
				CInsKeySFD KeySFD;
				KeySFD.m_IdFormat  = IdFormatCur;
				
				
				for(JDate date = dateStart; date<=dateStop; date ++)
				{

					KeySFD.m_DateD     = date;

					//if(Support.m_IdParent)
					//{
					//	KeySFD.m_IdSupport	= Support.m_IdParent;
					//	KeySFD.m_IdParent	= 0;

					//	// on recherche l'insertion
					//	pPLNInsertions->MoveTo_SFD(KeySFD,-1);

					//	// on teste si on a trouvé une insertion
					//	if (!pPLNInsertions->MoveTo_SFD(KeySFD,-1))
					//		continue;

					//	pPLNInsertions = &(pPLNInsertions->GetItem_SFD()->GetCouplageInsertions());
					//}

					// on initialise la clé
					KeySFD.m_IdSupport	= Support.m_IdSupport;
					KeySFD.m_IdParent	= Support.m_IdParent;

					// on recherche l'insertion
					pPLNInsertions->MoveTo_SFD(KeySFD,-1); //  -1);

					// on teste si on a trouvé une insertion
					if (pPLNInsertions->IsValid_SFD())
					{
						// on récupère l'insertion
						CInsertion * pInsertion = pPLNInsertions->GetItem_SFD();

						// on récupère la durée de l'insertion
						JInt32 Duree	= pInsertion->m_Duree;
						JDate Fin		= pInsertion->m_DateD+Duree-1;

						// on teste si l'insertion correspond à notre demande
						if (pInsertion->m_IdSupport	== KeySFD.m_IdSupport &&
							pInsertion->m_IdParent	== KeySFD.m_IdParent &&
							pInsertion->m_IdFormat	== KeySFD.m_IdFormat &&
							!(date < pInsertion->m_DateD || date > Fin))
								pInsertion->m_Data.m_status = stt;
					}//valid insert
				}//for date
			}//for formats
		}//if hypo valid
	}//for indice

	ResetSelection();
	Invalidate();
}


void CATPCreusetPlan_C2::OnCrPopupgrilleItem0()
{
	SetInsertStatus(JATPDATInsertion::NOTHING);
}

void CATPCreusetPlan_C2::OnCrPopupgrilleItem1()
{
	SetInsertStatus(JATPDATInsertion::STATUT1);
}

void CATPCreusetPlan_C2::OnCrPopupgrilleItem2()
{
	SetInsertStatus(JATPDATInsertion::STATUT2);
}

// Traitement menu contextuel

//////////////////////////////////////////////////////////////////////////////////////////
// Menu contextuel pour ajouter + de 1 insertions en série
void CATPCreusetPlan_C2::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// Récupère indice et date correspondant à ma position curseur
	CPoint pt;
	pt = point;
	ScreenToClient(&pt);
	LONG  IndiceSupport;
	LONG  IndiceDate;
	JDate DateDeb;
	JBool Couplage = false;

	// Aurelien : corrigé exception ici car si IndicePlateau est nul const CKeySupport & Support devient une ref invalide
	// donc verif du nombre de supports du plateau
	if (m_pATPDocCreuset->m_PlateauSupports.GetCount() > 0)
	{
		// on récupère l'élément sous le curseur
		if (this->GetIdxAtPos(pt.x, pt.y, IndiceDate, IndiceSupport))
		{
			// on récupère l'identifiant du Support
			JInt32 IndicePlateau = IndiceSupport;
			if (m_pATPDocCreuset->m_AFFPlateauGrille == 0) 
				IndicePlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(IndiceSupport);

			// Récupération du support
			const CKeySupport & Support = m_pATPDocCreuset->m_PlateauSupports.Item(IndicePlateau);

			// attention si couplage, on ne fait rien, car problème à résoudre auparavant
			// en effet insertion suivante d'un couplage n'est pas forcément à la date suivante !!!!)
			if (CAjoutInsertion::IsCouplage(Support.m_IdSupport))
				return;

			// Récupère la date
			DateDeb = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD() + IndiceDate;

			CMenu Menu;
			CString Label;

			// Avant la création ndu menu popup, il nous faut tester la validité de la zone insertion
			// Il faut que les insertions soient possibles (zone tarifée etc ...)
			// Et que l'on soit sur un support
			//Menu contextuel
			if(HasSelection())
			{
				// Pas possible sur une zone avec déjà des insertions
				return;
			}

			//Création Menu
			Menu.CreatePopupMenu();

			// Options Ajoute insertions en série
			// Label.LoadString(IDS_PU_SUPP_SEL_LIGNES);
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_1INS, " 1 Insertion");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_2INS, " 2 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_3INS, " 3 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_4INS, " 4 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_5INS, " 5 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_6INS, " 6 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_7INS, " 7 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_8INS, " 8 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_9INS, " 9 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_10INS,"10 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_11INS,"11 Insertions");
			Menu.AppendMenu(MF_STRING,IDM_CR_AJOUT_12INS,"12 Insertions");

			JDate DateFin;

			switch(Menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON|TPM_RETURNCMD|TPM_NONOTIFY,point.x,point.y,this))
			{
			case IDM_CR_AJOUT_1INS:
				{
					// Ajoute 1 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 1);
				}
				break;

			case IDM_CR_AJOUT_2INS:
				{
					// Ajoute 2 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 2);
				}
				break;

			case IDM_CR_AJOUT_3INS:
				{
					// Ajoute 3 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 3);
				}
				break;

			case IDM_CR_AJOUT_4INS:
				{
					// Ajoute 2 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 4);
				}
				break;

			case IDM_CR_AJOUT_5INS:
				{
					// Ajoute 5 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 5);
				}
				break;

			case IDM_CR_AJOUT_6INS:
				{
					// Ajoute 6 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 6);
				}
				break;

			case IDM_CR_AJOUT_7INS:
				{
					// Ajoute 7 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 7);
				}
				break;

			case IDM_CR_AJOUT_8INS:
				{
					// Ajoute 8 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 8);
				}
				break;

			case IDM_CR_AJOUT_9INS:
				{
					// Ajoute 9 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 9);
				}
				break;

			case IDM_CR_AJOUT_10INS:
				{
					// Ajoute 10 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 10);
				}
				break;

			case IDM_CR_AJOUT_11INS:
				{
					// Ajoute 11 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 11);
				}
				break;

			case IDM_CR_AJOUT_12INS:
				{
					// Ajoute 12 insertions
					AjouterNInsertion(IndiceSupport, DateDeb, DateFin, 12);
				}
				break;

			default:
				break;
			}	
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// Ajout insertion en série (N fois) sur support indicé et à la date en cours
JBool CATPCreusetPlan_C2::AjouterNInsertion(LONG indice, JDate DateDeb, JDate &DateFin, JUnt32 NbInsertions)
{
	// Ajout N insertions consécutives
	DateFin = DateDeb;
	for (JUnt32 InxInsertion = 0; InxInsertion < NbInsertions; InxInsertion++)
	{
		// On essaie insérer une insertion
		if (!AjouterInsertion(indice, DateDeb, DateFin, -1, 0))
		{
			if (InxInsertion == 0)
				// Insertion impossible sur la 1ere date
				AfxMessageBox("Insertion impossible à cette date",MB_ICONINFORMATION);
			else
			{
				// Toutes les insertions n'ont pas pu être appliquées
				CString TxtMess;
				TxtMess.Format("%s %d %s","Seulement", InxInsertion,"insertions valides"); 
				AfxMessageBox(TxtMess,MB_ICONINFORMATION);
			}
			return false;
		}
		else
		{
			// On se positionne sur la date juste aprés
			DateDeb = DateFin+1;
		}
	}

	// Toutes les insertions se sont bien effectuées
	return true;
}	