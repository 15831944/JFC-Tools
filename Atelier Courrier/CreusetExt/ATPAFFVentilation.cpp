#include "StdAfx.h"
#include ".\atpaffventilation.h"
#include "resource.h"
#include "JATPDocCreuset.h"
#include "JATPDocExport.h"
#include "ItemsConstruitsManager.h"

JATPAFFVentilation::JATPAFFVentilation(void)
{
	m_bVisible = false;
	m_pATPDocCreuset = NULL;
	m_TBLhypos.SetCount(0);
	m_bUpd = m_bUpdAll = false;
	m_lastBtnSel = 0;

	m_bShowNbContacts = m_bShowAudience000 = m_bShowGRP = m_bShowIndGRP = m_bShowCouv = m_bShowIndCouv = m_bShowRepet = true;

}

JATPAFFVentilation::~JATPAFFVentilation(void)
{
	for(m_lstItemVentil.MoveFirst(); m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext())
		if(m_lstItemVentil.GetItem()->GetID()<0)
			delete m_lstItemVentil.GetItem();
}

void JATPAFFVentilation::SetVisible(JBool bVisible)
{
	m_bVisible = bVisible;
}

JBool JATPAFFVentilation::IsVisible()
{
	return m_bVisible;
}

JVoid JATPAFFVentilation::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}

JInt32 JATPAFFVentilation::GetNbItem() const
{
	return m_lstItemVentil.GetCount();
}

const JChar* JATPAFFVentilation::GetLblItem(int rangItem) const
{
	if(rangItem == 0)
		return "Cible";

	m_lstItemVentil.MoveTo(rangItem);
	if(m_lstItemVentil.IsValid())
		return m_lstItemVentil.GetItem()->GetLabel().AsJCharPtr();
	else
		return NULL;
}

// Retourne le nombre de contacts en milliers
JFlt64 JATPAFFVentilation::GetNbContacts(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	if(m_TBLhypos.GetCount())
	{
		ret = m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_AudienceRef / 1000;
	}

	return ret;
}

// Retourne l'audience en milliers
JFlt64 JATPAFFVentilation::GetAudience000(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	if(m_TBLhypos.GetCount())
	{
		JFlt64 Couv =  m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_Couverture;
		ret = Couv / 1000;
	}

	return ret;
}

// Retourne GRP
JFlt64 JATPAFFVentilation::GetGRP(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	if(m_TBLhypos.GetCount())
	{
		JFlt64 VolLecture = m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_VolLecture;
		JFlt64 Effectif = m_TBLhypos.Item(idxHypo).Item(rangItem).m_Effectif;
		if(Effectif > 0.0 && VolLecture >= 0.0)
			ret = VolLecture*100.0 / Effectif;
	}

	return ret;
}

// Retourne Couverture
JFlt64 JATPAFFVentilation::GetCouv(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	if(m_TBLhypos.GetCount())
	{
		JFlt64 Couv = m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_Couverture;
		JFlt64 Effectif = m_TBLhypos.Item(idxHypo).Item(rangItem).m_Effectif;
		if(Effectif > 0.0 && Couv >= 0.0)
			ret = 100.0 * Couv / Effectif;
	}

	return ret;
}

// Retourne Répétition
JFlt64 JATPAFFVentilation::GetRepet(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	if(m_TBLhypos.GetCount())
	{
		JFlt64 VolLecture = m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_VolLecture;
		JFlt64 Couv =  m_TBLhypos.Item(idxHypo).Item(rangItem).GetDATHypoByFormat(idFormat).m_Couverture;
		if(Couv != 0.0)
			ret = VolLecture / Couv;
	}

	return ret;
}

// Retourne indice GRP
JFlt64 JATPAFFVentilation::GetIndGRP(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	
	if(m_TBLhypos.GetCount() && GetGRP(idxHypo, idFormat, 0) > 0.0)
		ret = GetGRP(idxHypo, idFormat, rangItem) / GetGRP(idxHypo, idFormat, 0) * 100.0;

	return ret;
}

// Retourne indice Couverture
JFlt64 JATPAFFVentilation::GetIndCouv(int idxHypo, int idFormat, int rangItem) const
{
	JFlt64 ret = 0.0;
	
	if(m_TBLhypos.GetCount() && GetCouv(idxHypo, idFormat, 0) > 0.0)
		ret = GetCouv(idxHypo, idFormat, rangItem) / GetCouv(idxHypo, idFormat, 0) * 100.0;

	return ret;

}

CString JATPAFFVentilation::GetLabel(int ligne) const
{
	CString ret;
	int rangItem = ligne / (NbValue()+1);
	int rangValue = ligne % (NbValue()+1);

	
	if(!m_bShowNbContacts && rangValue>=0)
		rangValue++;
	if(!m_bShowAudience000 && rangValue>=1)
		rangValue++;
	if(!m_bShowGRP && rangValue>=2)
		rangValue++;
	if(!m_bShowIndGRP && rangValue>=3)
		rangValue++;
	if(!m_bShowCouv && rangValue>=4)
		rangValue++;
	if(!m_bShowIndCouv && rangValue>=5)
		rangValue++;
	if(!m_bShowRepet && rangValue>=6)
		rangValue++;

	if(rangValue!=7)
	ret.Format("%s - ",m_pATPDocCreuset->m_AFFVentilation.GetLblItem(rangItem));

	CString Text;
	if(rangValue==0)
	{
		Text.LoadString(IDS_CR_CONTACTS);
	}
	if(rangValue==1)
	{
		Text.LoadString(IDS_CR_AUDIENCE);
	}
	if(rangValue==2)
	{
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			Text.FormatMessage(IDS_CR_GRP);
		else
			Text.LoadString(IDS_CR_GRPACTIF);
	}
	if(rangValue==3)
	{
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			Text.LoadString(IDS_CR_INDGRP);
		else
			Text.LoadString(IDS_CR_INDGRPACTIF);
	}
	if(rangValue==4)
	{
		Text.LoadString(IDS_CR_COUVERTURE);
	}
	if(rangValue==5)
	{
		Text.LoadString(IDS_CR_INDCOUV);
	}
	if(rangValue==6)
	{
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			Text.LoadString(IDS_CR_REPET);
		else
			Text.LoadString(IDS_CR_REPETPLUS);
	}

	ret.Append(Text);
	return ret;
}

CString JATPAFFVentilation::GetValue(int idxHypo, int idFormat, int ligne) const
{
	int rangItem = ligne / (NbValue()+1);
	int rangValue = ligne % (NbValue()+1);

	if(rangValue == NbValue())
		return "";

	CString ret;
	JFlt64 val;

	if(!m_bShowNbContacts && rangValue>=0)
		rangValue++;
	if(!m_bShowAudience000 && rangValue>=1)
		rangValue++;
	if(!m_bShowGRP && rangValue>=2)
		rangValue++;
	if(!m_bShowIndGRP && rangValue>=3)
		rangValue++;
	if(!m_bShowCouv && rangValue>=4)
		rangValue++;
	if(!m_bShowIndCouv && rangValue>=5)
		rangValue++;
	if(!m_bShowRepet && rangValue>=6)
		rangValue++;


	if(rangValue==0)
	{
		val = GetNbContacts(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.0f  "),val);
	}
	if(rangValue==1)
	{
		val = GetAudience000(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.0f  "),val);
	}
	if(rangValue==2)
	{
        val = GetGRP(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.1f  "),val);
	}
	if(rangValue==3)
	{
		val = GetIndGRP(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.0f  "),val);
	}
	if(rangValue==4)
	{
		val = GetCouv(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.1f  %% "),val);
	}
	if(rangValue==5)
	{
		val = GetIndCouv(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.0f  "),val);
	}
	if(rangValue==6)
	{
		val = GetRepet(idxHypo, idFormat, rangItem);
		if(val>0.0)
			ret.Format(_T("%.1f  "),val);
	}

	return ret;
}

bool JATPAFFVentilation::ExportVentil(CString & exportStr)
{

//entete
	JATPDocExport docExport(*m_pATPDocCreuset);

	exportStr += "VERSION 1\nEXPORT VENTILATION\n\nBLOC 1\nTITRE:\tVENTILATION\nETUDE:\t";
	exportStr.AppendFormat("%s", docExport.m_LabelEtude.AsJCharPtr());
	exportStr += "\nGROUPE:\t";
	exportStr.AppendFormat("%s", docExport.m_LabelGroupe.AsJCharPtr());
	exportStr += "\nANNONCEUR:\t";
	exportStr.AppendFormat("%s", docExport.m_LabelAnnonceur.AsJCharPtr());
	exportStr += "\nPRODUIT:\t";
	exportStr.AppendFormat("%s", docExport.m_LabelProduit.AsJCharPtr());
	exportStr += "\nPERIODE:\t";
	// on vérifie la validité des dates
	if (docExport.m_DateDebPeriode.IsValid() && docExport.m_DateFinPeriode.IsValid())
	{
		// on décompose les dates
		JInt32 DayD, MonthD, YearD; docExport.m_DateDebPeriode.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF;docExport.m_DateFinPeriode.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		exportStr.AppendFormat("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"",DayD, MonthD, YearD, DayF, MonthF, YearF);
	}
	
	exportStr += "\nPERIODE ACTION:\t";
	// on vérifie la validité des dates
	if (docExport.m_DateDebPeriodeAction.IsValid() && docExport.m_DateFinPeriodeAction.IsValid())
	{
		// on décompose les dates
		JInt32 DayD, MonthD, YearD; docExport.m_DateDebPeriodeAction.GetDate(DayD, MonthD, YearD);
		JInt32 DayF, MonthF, YearF; docExport.m_DateFinPeriodeAction.GetDate(DayF, MonthF, YearF);

		// on exporte les dates
		exportStr.AppendFormat("\"%02d/%02d/%04d\"\t\"%02d/%02d/%04d\"",DayD, MonthD, YearD, DayF, MonthF, YearF);
	}
	exportStr += "\nCIBLE:\t";
	exportStr.AppendFormat("\"%s\"\t\"%d\"\t\"%d\"", docExport.m_LabelCible.AsJCharPtr(), docExport.m_EffectifCible, docExport.m_EchantillonCible);
	exportStr += "\nREFERENCE AFFINITE:\t";
	exportStr.AppendFormat("\"%s\"",docExport.m_LabelRefAffinite.AsJCharPtr());
	exportStr += "\nFILTRE AUDIENCE:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelFiltreAudience.AsJCharPtr()); 
	exportStr += "\nSOURCE:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelSource.AsJCharPtr());  
	exportStr += "\nTERRAIN:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelTerrain.AsJCharPtr());
	exportStr += "\nSEGMENT:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelSegment.AsJCharPtr());
	exportStr += "\nTYPE POIDS:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelTypePoids.AsJCharPtr());
	exportStr += "\nMETHODE DE CALCUL DES COUVERTURES:\t";
	exportStr.AppendFormat("%s",docExport.m_LabelMethodeCalcCouv.AsJCharPtr());
	exportStr += "\nFORMATS ACTIFS:\t";
	for (docExport.m_ListFormatActif.MoveFirst();
		 docExport.m_ListFormatActif.IsValid();
		 docExport.m_ListFormatActif.MoveNext())
	{
		// Ecriture ident et libellé format dans buffer
		exportStr.AppendFormat("%d\t",docExport.m_ListFormatActif.GetItem());

		// puis les libellés format
		JString LblCompletFormat = "*";
		JString LblAbregeFormat  = "*";
		const JSRCPresse * pSrcPresse = &JSRCPresse::GetInstance();
		if(pSrcPresse)
		{
			// libellé long format
			LblCompletFormat = pSrcPresse->m_TBLFormats.GetLabelByID(docExport.m_ListFormatActif.GetItem()); 

			// puis le libellé abrégé format
			LblAbregeFormat = pSrcPresse->m_TBLFormats.GetLabelCourtByID(docExport.m_ListFormatActif.GetItem()); 
		}

		// Ecriture libellé complet format dans buffer
		exportStr.AppendFormat("%s\t",LblCompletFormat.AsJCharPtr());

		// Ecriture libellé abrégé format dans buffer
		exportStr.AppendFormat("%s\t",LblAbregeFormat.AsJCharPtr());
	}
	
	exportStr += "\n\nCONTEXTE DE LECTURE:\n";
	exportStr += "Aucun contexte";
	exportStr += "\n";

	exportStr += "\n";

	exportStr += "BLOC 2\nCIBLE\tGRANDEUR\tPlan Maître";//	H1	H2	H3	H4	H5"
	for(int i = 1; i< m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
		exportStr.AppendFormat("\tH%d", i );
	exportStr += "\n";


//valeurs
	for(m_lstItemVentil.MoveFirst();m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext())
	{
		int rangItem = m_lstItemVentil.GetIndex();
		exportStr += GetLblItem(rangItem);
		
		CString Text;

		// Nombre de contacts
		Text.LoadString(IDS_CR_CONTACTS);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetNbContacts(i,-1, rangItem) );

		// Audience en milliers (000)
		Text.LoadString(IDS_CR_AUDIENCE);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetAudience000(i,-1, rangItem) );

		// Les GRPs
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			Text.LoadString(IDS_CR_GRP);
		else
			Text.LoadString(IDS_CR_GRPACTIF);
		exportStr.AppendFormat("\t%s", Text);
		for(int i = 0; i< m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetGRP(i,-1, rangItem) );
		
		//
		Text.LoadString(IDS_CR_COUVERTURE);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetCouv(i,-1, rangItem) );
		
		//
		Text.LoadString(IDS_CR_INDCOUV);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetIndCouv(i,-1, rangItem) );

		//
		Text.LoadString(IDS_CR_REPET);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetRepet(i,-1, rangItem) );

		//
		if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			Text.LoadString(IDS_CR_INDGRP);
		else
			Text.LoadString(IDS_CR_INDGRPACTIF);
		exportStr.AppendFormat("\n\t%s", Text);
		for(int i = 0; i<m_pATPDocCreuset->m_ITRHypotheses.GetCount(); i++)
			exportStr.AppendFormat("\t%f", GetIndGRP(i,-1, rangItem) );


		exportStr += "\n\n";
	}

	return true;
}

CString JATPAFFVentilation::ExportVentil(int nHypo)
{
	CString exportStr;

	if(m_lstItemVentil.GetCount()>1)
	{
		exportStr = "BLOC 6\n";

		for(m_lstItemVentil.MoveFirst();m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext())
		{
			if(m_lstItemVentil.GetItem()->GetID() == m_pATPDocCreuset->m_IdItemSel)
				continue;

			int rangItem = m_lstItemVentil.GetIndex();
			exportStr += GetLblItem(rangItem);
			
			CString Text;
	
			// Nombre de contacts
			Text.LoadString(IDS_CR_CONTACTS);
			exportStr.AppendFormat("\t%s", Text);
			exportStr.AppendFormat("\t%f", GetNbContacts(nHypo,-1, rangItem) );

			// Audience en milliers (000)
			Text.LoadString(IDS_CR_AUDIENCE);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetAudience000(nHypo,-1, rangItem) );

			//
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				Text.LoadString(IDS_CR_GRP);
			else
				Text.LoadString(IDS_CR_GRPACTIF);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetGRP(nHypo,-1, rangItem) );
			
			// Couverture
			Text.LoadString(IDS_CR_COUVERTURE);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetCouv(nHypo,-1, rangItem) );
			
			// Indice couverture
			Text.LoadString(IDS_CR_INDCOUV);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetIndCouv(nHypo,-1, rangItem) );

			// Repet
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				Text.LoadString(IDS_CR_REPET);
			else
				Text.LoadString(IDS_CR_REPETPLUS);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetRepet(nHypo,-1, rangItem) );

			// Indice Grp
			if(m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
				Text.LoadString(IDS_CR_INDGRP);
			else
				Text.LoadString(IDS_CR_INDGRPACTIF);
			exportStr.AppendFormat("\n\t%s", Text);
			exportStr.AppendFormat("\t%f", GetIndGRP(nHypo,-1, rangItem) );

			exportStr += "\n";
		}

		exportStr += "\n";
	}

	return exportStr;
}



JVoid JATPAFFVentilation::Update(const JFCUpdate & Message)
{
	if(Message == UPDATE_INIPROGRESS
		||Message == UPDATE_PROGRESS
		||Message == UPDATE_ENDPROGRESS)
		return;
	
	
	if(Message == UPDATE_INITIAL)
	{
		//chargement d'une étude: raz
		m_TBLhypos.Reset();
		m_TBLhypos.SetCount(0);
		m_bUpd = m_bUpdAll = false;

		for(m_lstItemVentil.MoveFirst(); m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext())
			if(m_lstItemVentil.GetItem()->GetID()<0)
				delete m_lstItemVentil.GetItem();

		m_lstItemVentil.Reset();
		m_listIDQST.Reset();
		m_listIDItem.Reset();
		m_listIDPal.Reset();
		return;
	}

	if( !m_bVisible || m_pATPDocCreuset->m_AFFCreuset.GetAffPlan() )
	{
		if( Message == UPDATE_FILTREAUDIENCE
			|| Message == UPDATE_CTXLECTURE
			|| Message == UPDATE_VENTIL
			|| Message == UPDATE_SEL_CIBLE
			|| Message == UPDATE_ETUDE)
			m_bUpdAll = true;

		if(  Message == UPDATE_KEY_PERIODE_ACTION
			|| Message == UPDATE_OPTIONSMOTEUR
			//|| Message == UPDATE_SEL_FORMAT
			|| Message == UPDATE_SEL_TERRAIN
			|| Message == UPDATE_CURRENT_HYPO)
			m_bUpd = true;


		if(Message != UPDATE_OFFRE && Message != UPDATE_FORCEVENTIL)
			return;
	}

	bool bProgressInit = false;
	CWaitCursor wait;

	if(Message == UPDATE_SEL_CIBLE
		|| m_bUpdAll)
	{
		if(m_lstItemVentil.GetCount())
		{
			//on change la 1ere cible
			m_lstItemVentil.MoveFirst();
			m_lstItemVentil.Remove();

			// Cas d'une cible normale sélectionnée
			if (!m_pATPDocCreuset->m_CiblesPondsActives)
			{
				m_pATPDocCreuset->m_MAPpItems.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				m_lstItemVentil.AddHead() = m_pATPDocCreuset->m_MAPpItems.GetItem()->AsItemConstruit();
			}
			else
			{
				// Cas d'une cibe pondérée sélectionnée A FAIRE ALAIN - HK 
				m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				CItemPondere *pItemPondere = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->AsItemPondere();

				// Réaffecte les pondérations 
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst ();
				if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
				{
					m_pATPDocCreuset->m_MoteurPresse.SetPonderations(m_pATPDocCreuset->m_IdItemSel,m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
				}	

				// Ajoute l'item construit correspondant à l'item de base
				CItemsConstruitsBase *pItemsConstruits = CItemsConstruitsManager::GetInstance()->GetItemsInstance(); 
				m_lstItemVentil.AddHead() = pItemsConstruits->GetItemConstruit(pItemPondere->GetIDItemBase());				
			}
		}
	}


	if( Message == UPDATE_FILTREAUDIENCE
		|| Message == UPDATE_CTXLECTURE
		|| Message == UPDATE_VENTIL
		|| Message == UPDATE_SEL_CIBLE
		|| Message == UPDATE_OFFRE
		|| Message == UPDATE_FORCEVENTIL
		|| m_bUpdAll)
	{
		m_pATPDocCreuset->m_Progress.m_Range = (10 * m_pATPDocCreuset->m_TblHypotheses.GetCount() * m_lstItemVentil.GetCount()) +10;
		m_pATPDocCreuset->m_Progress.m_Step = 10;
		if( Message != UPDATE_OFFRE)
		{
			m_pATPDocCreuset->UpdateAllViews(UPDATE_INIPROGRESS);
			m_pATPDocCreuset->UpdateAllViews(UPDATE_PROGRESS);
			bProgressInit = true;
		}
		else
		{
			TestItemsVentil();//if faut valider les items sur les nouveaux terrains
		}


		// Ajoute les items ventilation
		JUnt32 NbItemsVentil = m_lstItemVentil.GetCount(); 

		// Cas d'une cible normale sélectionnée
		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			m_pATPDocCreuset->m_MoteurPresse.AddCibleVentilation(m_pATPDocCreuset->m_Offre, m_lstItemVentil);
		}
		else
		{
			if (m_lstItemVentil.GetCount())
			{
				//on change la 1ere cible
				m_lstItemVentil.MoveFirst();
				m_lstItemVentil.Remove();

				// Cas d'une cibe pondérée sélectionnée A FAIRE ALAIN - HK 
				m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				CItemPondere *pItemPond = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->AsItemPondere();

				// Réaffecte les pondérations 
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst ();
				if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
				{
					m_pATPDocCreuset->m_MoteurPresse.SetPonderations(m_pATPDocCreuset->m_IdItemSel,m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
				}	

				// Récupère item de base associé
				// CItemsConstruits *pItemsConstruits = CItemsConstruits::GetInstance();
				// m_lstItemVentil.AddHead() = pItemsConstruits->GetItemConstruit(pItemPond->GetIDItemBase());;					
				m_lstItemVentil.AddHead() = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(pItemPond->GetIDItemBase());;					
			}

			m_pATPDocCreuset->m_MoteurPresse.AddCibleVentilation(m_pATPDocCreuset->m_Offre, m_lstItemVentil);
		}
	}

	if (Message == UPDATE_CURRENT_HYPO || Message == UPDATE_CURRENT_HYPO_LIGHT )
	{
		m_bUpd = m_bUpdAll = false;
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
		CPlanInsertions planIns =  m_pATPDocCreuset->m_TblHypotheses.GetItem();
		m_TBLhypos.Item(m_pATPDocCreuset->m_IdxHypotheseCourant).Reset();
		m_TBLhypos.Item(m_pATPDocCreuset->m_IdxHypotheseCourant).SetCount(m_lstItemVentil.GetCount());

		if(!bProgressInit)
		{
			m_pATPDocCreuset->m_Progress.m_Range = 10 * m_lstItemVentil.GetCount();
			m_pATPDocCreuset->m_Progress.m_Step = 10;
			m_pATPDocCreuset->UpdateAllViews(UPDATE_INIPROGRESS);
		}

		int j;
		for( j = 0, m_lstItemVentil.MoveFirst(); m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext(), j++)
		{
			m_TBLhypos.Item(m_pATPDocCreuset->m_IdxHypotheseCourant).Item(j).Reset();
			m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
					m_pATPDocCreuset->m_KEYPeriodeActionHyp,
					m_pATPDocCreuset->m_KEYPeriodeActionTablier,
					planIns,
					m_TBLhypos.Item(m_pATPDocCreuset->m_IdxHypotheseCourant).Item(j),
					m_lstItemVentil.GetItem()->GetID()+10000000,true);

			m_pATPDocCreuset->UpdateAllViews(UPDATE_PROGRESS);
		}
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ENDPROGRESS);

	}

	if(  Message == UPDATE_KEY_PERIODE_ACTION
		|| Message == UPDATE_OPTIONSMOTEUR
		|| Message == UPDATE_FILTREAUDIENCE
		|| Message == UPDATE_CTXLECTURE
		//|| Message == UPDATE_SEL_FORMAT
		|| Message == UPDATE_VENTIL
		|| Message == UPDATE_SEL_CIBLE
		|| Message == UPDATE_SEL_TERRAIN
		|| Message == UPDATE_OFFRE
		|| Message == UPDATE_FORCEVENTIL
		|| Message == UPDATE_ETUDE
		|| m_bUpdAll
		|| m_bUpd)
	{
		m_bUpd = m_bUpdAll = false;
		m_TBLhypos.Reset();
		m_TBLhypos.SetCount(m_pATPDocCreuset->m_MaxHypothese+1);
		m_pATPDocCreuset->m_Progress.m_Range = 10 * m_pATPDocCreuset->m_TblHypotheses.GetCount() * m_lstItemVentil.GetCount();
		m_pATPDocCreuset->m_Progress.m_Step = 10;
		if( Message != UPDATE_OFFRE)
			m_pATPDocCreuset->UpdateAllViews(UPDATE_INIPROGRESS);
	
		//calcul des hypo pour chaque item
		
		// Positionne les pondérations
		// Réaffecte les pondérations 
		m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst ();
		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
		{
			m_pATPDocCreuset->m_MoteurPresse.SetPonderations(m_pATPDocCreuset->m_IdItemSel,m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
		}
		
		int i = 0;
		for(m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext(),i++)
		{
			CPlanInsertions planIns =  m_pATPDocCreuset->m_TblHypotheses.GetItem();
			m_TBLhypos.Item(i).Reset();
			m_TBLhypos.Item(i).SetCount(m_lstItemVentil.GetCount());
			
			int j = 0;
			for( m_lstItemVentil.MoveFirst(); m_lstItemVentil.IsValid(); m_lstItemVentil.MoveNext(), j++)
			{
		
				m_TBLhypos.Item(i).Item(j).Reset();
				//m_TBLhypos.Item(i).Item(j).
				//m_pDocCreuset->m_Offre.m_TBLIdFormats
				m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
						m_pATPDocCreuset->m_KEYPeriodeActionHyp,
						m_pATPDocCreuset->m_KEYPeriodeActionTablier,
						planIns,
						m_TBLhypos.Item(i).Item(j),
						m_lstItemVentil.GetItem()->GetID()+10000000, true);

				if( Message != UPDATE_OFFRE)
					m_pATPDocCreuset->UpdateAllViews(UPDATE_PROGRESS);
			}
		}
		if( Message != UPDATE_OFFRE)
			m_pATPDocCreuset->UpdateAllViews(UPDATE_ENDPROGRESS);


	}

}

void JATPAFFVentilation::TestItemsVentil()
{
	//m_pATPDocCreuset->m_Offre, m_lstItemVentil

	m_lstItemVentil.MoveFirst();
	while(m_lstItemVentil.IsValid())
	{
		// Récup item 
		IItem * pItem = m_lstItemVentil.GetItem();
		if(pItem)//on ne soucie pas de palette ou item construit (générique)
		{
			CKeyTerrain key;
			key.SetSource(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);
			//on teste l'item pour chaque terrain, invalide on sort de la boucle
			for(m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid();
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				key.SetTerrain(m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem());
				
				CTerrain * terrain = CTerrainManager::GetInstance()->GetTerrainNoRef(key);
				if( ! pItem->IsValid(terrain))
				{
					m_lstItemVentil.Remove(-1);
					break;
				}
			}
			//test sur la source de ciblage
		}
		m_lstItemVentil.MoveNext();
	}
}


void JATPAFFVentilation::ShowNbContacts(bool bShow)
{
	m_bShowNbContacts = bShow;
}
void JATPAFFVentilation::ShowAudience000(bool bShow)
{
	m_bShowAudience000 = bShow;
}
void JATPAFFVentilation::ShowGRP(bool bShow)
{
	m_bShowGRP = bShow;
}
void JATPAFFVentilation::ShowIndGRP(bool bShow)
{
	m_bShowIndGRP = bShow;
}
void JATPAFFVentilation::ShowCouv(bool bShow)
{
	m_bShowCouv = bShow;
}
void JATPAFFVentilation::ShowIndCouv(bool bShow)
{
	m_bShowIndCouv = bShow;
}
void JATPAFFVentilation::ShowRepet(bool bShow)
{
	m_bShowRepet = bShow;
}

bool JATPAFFVentilation::CanNbContacts()
{
	return m_bShowNbContacts;
}

bool JATPAFFVentilation::CanAudience000()
{
	return m_bShowAudience000;
}

bool JATPAFFVentilation::CanGRP()
{
	return m_bShowGRP;
}
bool JATPAFFVentilation::CanIndGRP()
{
	return m_bShowIndGRP;
}
bool JATPAFFVentilation::CanCouv()
{
	return m_bShowCouv;
}
bool JATPAFFVentilation::CanIndCouv()
{
	return m_bShowIndCouv;
}
bool JATPAFFVentilation::CanRepet()
{
	return m_bShowRepet;
}
int JATPAFFVentilation::NbValue() const
{
	int nbVal = 0;
	
	if(m_bShowNbContacts)
		nbVal++;
	if(m_bShowAudience000)
		nbVal++;
	if(m_bShowGRP)
		nbVal++;
	if(m_bShowIndGRP)
		nbVal++;
	if(m_bShowCouv)
		nbVal++;
	if(m_bShowIndCouv)
		nbVal++;
	if(m_bShowRepet)
		nbVal++;
	return nbVal;
}



