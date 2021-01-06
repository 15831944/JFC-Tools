#include "StdAfx.h"
#include ".\linemode.h"
#include "SerialisePaletteBuilder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLineMode::CLineMode(CDlgItems *pDlgItems) : CPaletteMode(pDlgItems)
{
	// Mode courant : ligne ou coloenne ou page
	JInt32 ModeCur =  m_pDlgItem->ValCurMode();

	// selon le mode on initialise les maps correspondants
	if (ModeCur == Line)
	{
		// Mode ligne en cours
		m_Map_PaletteBuilderLigne.Reset();
	}
	else if (ModeCur == Column) 
	{
		// Mode colonne en cours
		m_Map_PaletteBuilderColonne.Reset();
	}
	else if (ModeCur == Page)
	{
		// Mode page en cours
		m_Map_PaletteBuilderPage.Reset();
	}
}

CLineMode::~CLineMode(void)
{
}

void CLineMode::StartMode()
{
	CPaletteMode::StartMode();
	m_SelectedItems.Reset();
	m_pDlgItem->m_Button_Accept.EnableWindow(FALSE);
}

void CLineMode::RecupMode()
{
	CPaletteMode::RecupMode();
}

// returns the current modes label
void CLineMode::SetModeTitle(void)
{
	CString Label;
	Label.LoadString(IDS_RI_LINE);
	if(m_pDlgItem->m_iModality == 4)
	{
		CString Filtre;
		Filtre.LoadString(IDS_RI_FILTRES);
		Label.Append(Filtre);
	}
	// m_pDlgItem->m_Mode_Title.SetText(Label);
	Label.LoadString(IDS_RI_TITLE_PUPITRE);
	m_pDlgItem->m_Mode_Title.SetText(Label);
	m_pDlgItem->m_Mode_Title.Invalidate();
}

/// Returns a string to the current dimension mode
CString CLineMode::GetDimensionModeString(int Mode)
{
	CString St;
	St.LoadString(IDS_RI_LINE_DIM);
	return St;
}

/// Generate Items based on the current valid configuration
bool CLineMode::BuildSelectedItems(bool RecupSel)
{
	/* ????
	// Bidouille pour éviter le bug de m_PIBQuestion qui est détruit  (en fait il sert à construire 
	// la palette, et n'est pas restitué tel qu'il était au début
	JList <CIBModalite *> TmpLst_IBModalite;
	if (RecupSel && m_CurPaletteBuilder.m_pIBquestion)
	{
		// Sauve Liste Blocs CIBModalite  >> JList <CIBModalite *>
		JList <CIBModalite *> Lst_IBModalite;
		m_CurPaletteBuilder.m_pIBquestion->GetModalites(Lst_IBModalite);

		// List temporaire pour construction
		for (Lst_IBModalite.MoveFirst() ; Lst_IBModalite.IsValid(); Lst_IBModalite.MoveNext())  
		{
			// Récup valeurs
			CIBModalite *pIBModalite = Lst_IBModalite.GetItem(); 

			JList <JInt32> Values;
			pIBModalite->GetValues(Values); 

			CIBModalite *pTmpIBModalite = new CIBModalite();
			pTmpIBModalite->SetValues(Values);

			// Ajoute à la liste temporaire
			TmpLst_IBModalite.AddTail() = pTmpIBModalite; 
		}
	}
	*/

	// Reset des élmts sélectionnés
	m_SelectedItems.Reset();

	if(!m_bItemValid)
		return false;

	if(!m_pIBquestion && !m_pIBitem)
		return false;

	// If the current question is not valid remove it
	if(m_pIBquestion && !m_pIBquestion->IsValid())
		return false;
	// Remove any Invalide items
	if(m_pIBitem && !m_pIBitem->IsValid())
		return false;

	// Get first palette
	m_Map_PaletteBuilder.MoveFirst();
	m_Map_PaletteBuilder.GetItem().m_List_DimSel.MoveFirst(); 

	// Move to first dimension
	CString FieldName = "";
	JList<CItemWrapper *> Items;
	// TODO get the mode
	bool Mode = (GetQuestionOperator()==IBloc::Or);
	if(BuildPalette(Items, m_Map_PaletteBuilder, FieldName, Mode) && Items.GetCount())
	{
		for(Items.MoveFirst(); Items.IsValid(); Items.MoveNext())
		{
			if(Items.GetItem()->GetHeader().IsEmpty())
			{
				Items.GetItem()->SetHeader(CString(_T("?")));
			}
			m_SelectedItems.AddTail() = Items.GetItem();
		}
	}
	
	return true;
}

void CLineMode::UpdateIBuilder(bool Changed, bool bValues)
{
	CItemMode::UpdateIBuilder(false, bValues);
	UpdateDisplay();
}

void CLineMode::GetSelectedItems(JList< CItemWrapper *> &Items)
{
	Items.Reset();
	if(m_SelectedItems.GetCount())
		Items.Swap(m_SelectedItems);
}

// Positionne les items sélectionnés directement
void CLineMode::SetSelectedItems(JList< CItemWrapper *> &Items)
{
	m_SelectedItems.Reset();
	if(Items.GetCount())
		m_SelectedItems.Swap(Items);
}

bool CLineMode::BuildPalette(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label, bool Mode)
{
	if(!PBuilders.IsValid())
		return false;

	if(Mode)
	{
		for(PBuilders.MoveFirst(); PBuilders.IsValid(); PBuilders.MoveNext())
		{
			// Palette en cours
			// CPaletteBuilder Palette = PBuilders.GetItem();

			// Set the questions selection to the beginning
			PBuilders.GetItem().m_List_DimSel.MoveFirst();

			// Build Palette linear
			BuildPaletteL(IItems,PBuilders,Label);
		}
		return true;
	}
	else
		// Build Palette compiled
		return BuildPaletteX(IItems,PBuilders,Label);
}

bool CLineMode::BuildPaletteL(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		// Mode linear add the Item
		if(PB.m_pIBitem || PB.m_pIBquestion)
		{
			const IQuestion * pQ = 0;
			if(PB.m_pIBquestion)
			{
				JInt32 Id = PB.m_pIBquestion->GetID();
				pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
			}
			else
				pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());


			// Attention à l'ordre Label / Header
			if(Label.IsEmpty())
			{
				if (PB.GetNumValues().GetCount() == 0) 
				{
					if(pQ)
					{
						Label = pQ->GetLabel().AsJCharPtr();
					}
					else
						Label = "Error"; 
				}
			}
			else
			{
				if(m_pDlgItem->m_iModality == 4 && pQ)
				{
					Label.Insert(0," - ");
					Label.Insert(0,pQ->GetLabel().AsJCharPtr());
				}
			}
			if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
			{
				CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
				CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
				const JList<JFlt32> & List_NumValues = PB.GetNumValues();
				for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
				{
					JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
					CIBNumValue* pNumValue = new CIBNumValue;
					pNumValue->SetOperator(IBloc::Nop);
					pNumValue->SetValues(Value);
					PB.m_pIBquestion->SetNumValue(pNumValue);
					CIBuilder	tIBuilder;
					CIBSegment	tISegment;
					tIBuilder.SetOperator(IBloc::Or);
					tIBuilder.Add(&tISegment); 
					tISegment.SetOperator(IBloc::Or);
					tISegment.Add(PB.m_pIBquestion);
					CString NomValue = Label;
					if (List_NumValues.GetItem() >= 0.0) {
						if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
						else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
					}
					else {
						if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
						else                    NomValue.AppendFormat(" - %s", "#");
					}
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();

					
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
					{
						
						CString StrHeader = "";
						CItemWrapper *pIW = new CItemWrapper();
						IItems.AddTail() = pIW;
						if(m_pDlgItem->m_iModality == 4)
						{
							StrHeader = m_pDlgItem->m_QFilter;
						}
						else
						{
							if(pQ)
							{
								StrHeader = pQ->GetLabel().AsJCharPtr(); 

								// Ajoute info catégorie item
								const CQuestion * pQuestion = pQ->AsQuestion();
								if (pQuestion)
								{
									if (pQuestion->IsQuantitative())
									{
										// Positionne la catégorie
										pIW->SetCategorie(pQuestion->GetCategorie());
									}
								}
							}
							if(!PB.m_Label.IsEmpty())
								StrHeader = PB.m_Label;
							else if(!PB.m_DefaultLabel.IsEmpty())
								StrHeader = PB.m_DefaultLabel;
						}
						pIW->SetItemConstruit(pItem);
						pIW->SetHeader(StrHeader);
					}
					else
						delete pItem;
					tIBuilder.RemoveSegment(&tISegment);
					if(PB.m_pIBquestion)
						tISegment.RemoveQuestion(PB.m_pIBquestion);
					else
						tISegment.RemoveItem(PB.m_pIBitem);
				}
				PB.m_pIBquestion->SetNumValue(pOldNumValue);
				PB.m_pIBquestion->SetClass(pOldClass);
			}
			else
			{
				CIBuilder	tIBuilder;
				CIBSegment	tISegment;
				tIBuilder.SetOperator(IBloc::Or);
				tIBuilder.Add(&tISegment); 
				tISegment.SetOperator(IBloc::Or);
				if(PB.m_pIBquestion)
					tISegment.Add(PB.m_pIBquestion);
				else
					tISegment.Add(PB.m_pIBitem);
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),tIBuilder))
				{
					CString StrHeader = "";
					CItemWrapper *pIW = new CItemWrapper();
					IItems.AddTail() = pIW;
					if(m_pDlgItem->m_iModality == 4)
					{
						StrHeader = m_pDlgItem->m_QFilter;
					}
					else
					{
						if(pQ)
						{
							StrHeader = pQ->GetLabel().AsJCharPtr(); 
							const CQuestion * pQuestion = pQ->AsQuestion();

							// Ajoute info catégorie item
							if (pQuestion)
							{
								if (pQuestion->IsQuantitative())
								{
									// Positionne la catégorie
									pIW->SetCategorie(pQuestion->GetCategorie());

									// Réécriture des libellés pour les questions quantitatives (un vrai bordel avant !!!!)
									JLabel LblFinal;
									if (pItem->GetLabel().AsJCharPtr() != StrHeader)
									{
										// Cas des questions quantitatives à réponse multiple (type nb quotidens lus avec nom des quotidiens)
										CString Union = " - ";
										LblFinal = pItem->GetLabel().AsJCharPtr(); // + Union;

										// Modif Pb Libelles dans les tris croises (8/07/2013)
										if (PB.m_Label != StrHeader && PB.m_Label != "")
											LblFinal = LblFinal.AsJCharPtr() + Union + PB.m_Label;
									}
									else
										// Cas de question avec "Valeurs Numériques" 1 seule modalité
										LblFinal = PB.m_Label;

									pItem->SetLabel(LblFinal, true);
									PB.m_Label = StrHeader;
								}
							}
						}
						if(!PB.m_Label.IsEmpty())
							StrHeader = PB.m_Label;
						else if(!PB.m_DefaultLabel.IsEmpty())
							StrHeader = PB.m_DefaultLabel;
					}

					pIW->SetItemConstruit(pItem);
					pIW->SetHeader(StrHeader);
				}
				else
					delete pItem;
				tIBuilder.RemoveSegment(&tISegment);
				if(PB.m_pIBquestion)
					tISegment.RemoveQuestion(PB.m_pIBquestion);
				else
					tISegment.RemoveItem(PB.m_pIBitem);
			}
		}
		return true;
	}
	else
	{
		// If the current modality is valid and we have values
		if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
		{
			// Retreive data
			CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
			JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();

			// Sauve temporairement
			JList <JInt32> TmpValues;
			pModalite->GetValues(TmpValues); 

			// For each modality build its item
			for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
			{
				CString Nom	= Label;
				// Add the modality label
				if(Nom.GetLength()>0)	Nom += " - "; 
				Nom += Modas.GetItem()->GetLabel().AsJCharPtr();

				// Set the current modality ID
				JList<JInt32> Values;
				Values.AddTail() = Modas.GetItem()->GetID();
				
				pModalite->SetValues(Values);
				BuildPaletteL(IItems, PBuilders, Nom);
			}

			// Remet les modalités tel qu'ils étaient avant la construction
			pModalite->SetValues(TmpValues); 

			if(PB.m_List_DimSel.IsValid())
				PB.m_List_DimSel.MovePrev();
			else
				PB.m_List_DimSel.MoveLast();
		}
		else
			return false;
	}
	return true;
}

bool CLineMode::BuildPaletteX(JList<CItemWrapper *> & IItems, JMap<int,CPaletteBuilder> & PBuilders, CString Label)
{
	if(!PBuilders.IsValid())
		return false;

	bool bValidDimSel		= false;
	bool bHasDimSel			= false;
	int Sel					= 0; 
	CPaletteBuilder & PB	= PBuilders.GetItem();

	if(PB.m_List_DimSel.GetCount()>0)
	{
		bHasDimSel = true;
		// Check for Dimension selection
		if(PB.m_List_DimSel.IsValid())
		{
			// Get current selection
			Sel = PB.m_List_DimSel.GetItem();
			// Step over current selection
			PB.m_List_DimSel.MoveNext();
			// Position all dimension data
			PB.m_Map_pIBModalite	.MoveTo(Sel);
			PB.m_Map_pIModalite		.MoveTo(Sel);
			PB.m_Map_DimType		.MoveTo(Sel);
			// This Question has dimensions
			bValidDimSel = true;
		}
	}

	if(!bHasDimSel || !bValidDimSel)
	{
		if (PB.m_pIBquestion && PB.GetNumValues().GetCount() > 0)
		{
			CIBClass*    pOldClass    = PB.m_pIBquestion->DetachClass();
			CIBNumValue* pOldNumValue = PB.m_pIBquestion->DetachNumValue();
			const JList<JFlt32> & List_NumValues = PB.GetNumValues();
			for (List_NumValues.MoveFirst(); List_NumValues.IsValid(); List_NumValues.MoveNext())
			{
				JList<JFlt32> Value; Value.AddTail() = List_NumValues.GetItem();
				CIBNumValue* pNumValue = new CIBNumValue;
				pNumValue->SetOperator(IBloc::Nop);
				pNumValue->SetValues(Value);
				PB.m_pIBquestion->SetNumValue(pNumValue);
				CString NomValue = Label;
				if (List_NumValues.GetItem() >= 0.0) {
					if (NomValue.IsEmpty()) NomValue.Format("%.0f", (double)List_NumValues.GetItem());
					else                    NomValue.AppendFormat(" - %.0f", (double)List_NumValues.GetItem());
				}
				else {
					if (NomValue.IsEmpty()) NomValue.Format("%s", "#");
					else                    NomValue.AppendFormat(" - %s", "#");
				}
				// Step to the next question
				PBuilders.MoveNext();
				// Is there another question
				if(PBuilders.IsValid())
				{
					// Set the questions selection to the beginning
					PBuilders.GetItem().m_List_DimSel.MoveFirst();
					// Build the Question
					BuildPaletteX(IItems, PBuilders, NomValue);
					PBuilders.MovePrev();
				}
				else
				{
					// Geet the Question or Item
					const IQuestion * pQ = 0;
					if(PB.m_pIBquestion)
						pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
					else
						pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
					// Validate the label
					this->ValidateLabel(NomValue);
					// Build the Item with all questions
					JLabel Lbl = NomValue;
					// Create the Target Here and Add it to the Palette
					CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
					pItem->SetLabel(Lbl,true);
					// Set the builder object
					if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
					{
						CItemWrapper *pIW = new CItemWrapper();
						pIW->SetItemConstruit(pItem);
						if(!m_PaletteXLabel.IsEmpty())
							pIW->SetHeader(m_PaletteXLabel);
						else if(!m_DefaultPaletteXLabel.IsEmpty())
							pIW->SetHeader(m_DefaultPaletteXLabel);
						IItems.AddTail() = pIW;
					}
					else
						delete pItem;
					PBuilders.MoveLast();
				}
			}
			PB.m_pIBquestion->SetNumValue(pOldNumValue);
			PB.m_pIBquestion->SetClass(pOldClass);
		}
		else
		{
			// Step to the next question
			PBuilders.MoveNext();
			// Is there another question
			if(PBuilders.IsValid())
			{
				// Set the questions selection to the beginning
				PBuilders.GetItem().m_List_DimSel.MoveFirst();
				// Build the Question
				BuildPaletteX(IItems, PBuilders, Label);
				PBuilders.MovePrev();
			}
			else
			{
				// Geet the Question or Item
				const IQuestion * pQ = 0;
				if(PB.m_pIBquestion)
					pQ = (const IQuestion *)m_pDlgItem->GetTerrain()->GetQuestionByID(PB.m_pIBquestion->GetID());
				else
					pQ = (const IQuestion *)m_pDlgItem->GetItems()->GetQuestionByID(PB.m_pIBitem->GetID());
				// Validate the label
				this->ValidateLabel(Label);
				// Build the Item with all questions
				JLabel Lbl = Label;
				// Create the Target Here and Add it to the Palette
				CItemConstruit *pItem = m_pDlgItem->GetItems()->GetNewItemConstruit();
				pItem->SetLabel(Lbl,true);
				// Set the builder object
				if(pItem->SetItemBuilder(m_pDlgItem->GetTerrain(),m_IBuilder))
				{
					CItemWrapper *pIW = new CItemWrapper();
					pIW->SetItemConstruit(pItem);
					if(!m_PaletteXLabel.IsEmpty())
						pIW->SetHeader(m_PaletteXLabel);
					else if(!m_DefaultPaletteXLabel.IsEmpty())
						pIW->SetHeader(m_DefaultPaletteXLabel);
					IItems.AddTail() = pIW;
				}
				else
					delete pItem;
				PBuilders.MoveLast();
			}
		}
	}
	else
	{
		// If the current modality is valid and we have values
		if(PB.m_Map_pIBModalite.IsValid() && PB.m_Map_pIModalite.IsValid())
		{
			// Retreive data
			CIBModalite * pModalite				= PB.m_Map_pIBModalite.GetItem();
			JList<const IModalite *> & Modas	= PB.m_Map_pIModalite.GetItem();

			// Sauve temporairement
			JList <JInt32> TmpValues;
			pModalite->GetValues(TmpValues); 

			// For each modality build its item
			for(Modas.MoveFirst(); Modas.IsValid(); Modas.MoveNext())
			{
				// Add the modality label
				CString Nom = Label;
				if(Label.GetLength()>0) Nom += " - "; 
				// Add the modality label
				Nom += Modas.GetItem()->GetLabel().AsJCharPtr();
				// Set the current modality ID
				JList<JInt32> Values;
				Values.AddTail() = Modas.GetItem()->GetID();
				pModalite->SetValues(Values);
				BuildPaletteX(IItems, PBuilders, Nom);
			}

			// Remet les modalités tel qu'ils étaient avant la construction
			pModalite->SetValues(TmpValues); 

			if(PB.m_List_DimSel.IsValid())
				PB.m_List_DimSel.MovePrev();
			else
				PB.m_List_DimSel.MoveLast();
		}
		else
			return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////
// Renvoi si on est en mode croisemnt de palette
bool CLineMode::ModeCroisementPalette()
{
	if (m_DefaultPaletteXLabel.IsEmpty())
		// On est en mode suite de composants
		return false;
	else
		// On est en mode croisement de composants
		return true;
}

bool CLineMode::OnAccept(CString &Name) 
{
	bool Mode = (GetQuestionOperator()==IBloc::Or);

	if(Mode)
	{
		if(Name != m_CurPaletteBuilder.m_DefaultLabel)
			m_CurPaletteBuilder.m_Label = Name;
	}
	else
	{
		if(Name != m_DefaultPaletteXLabel)
			m_PaletteXLabel = Name;
	}
	UpdateDisplay();
	return false;
}

/// Updates the current display
void CLineMode::UpdateDisplay()
{
	BOOL State = TRUE;

	CPaletteMode::UpdateDisplay();

	m_DefaultPaletteXLabel            .Empty();
	m_CurPaletteBuilder.m_DefaultLabel.Empty();

	if(m_CurPaletteBuilder.m_List_DimSel.GetCount() == 0)
	{
		if (m_CurPaletteBuilder.m_List_NumValue.GetCount() == 0)
			State = FALSE;
		else
		{
			if (m_pSelectedQuestion)
				m_CurPaletteBuilder.m_DefaultLabel = m_pSelectedQuestion->GetLabel().AsJCharPtr();
		}
	}
	else if(m_CurPaletteBuilder.m_Map_DimType.GetCount() == m_CurPaletteBuilder.m_List_DimSel.GetCount())
	{
		if(m_pSelectedQuestion && !m_pSelectedQuestion->IsPseudo())
		{
			if (m_CurPaletteBuilder.m_pIBquestion && (m_CurPaletteBuilder.m_pIBquestion->GetClass() || m_CurPaletteBuilder.m_pIBquestion->GetNumValue()))
			{
				if (m_CurPaletteBuilder.m_List_NumValue.GetCount() == 0)
					m_CurPaletteBuilder.m_DefaultLabel = m_pSelectedQuestion->GetLabel().AsJCharPtr();
				else
					State = FALSE;
			}
			else
				State = FALSE;
		}
		else
			m_CurPaletteBuilder.m_DefaultLabel.LoadString(IDS_RI_DEFAULT_LABEL_ITEMS_INDEPENDANTS);
	}
	else if(m_CurPaletteBuilder.m_Map_DimType.GetCount() > m_CurPaletteBuilder.m_List_DimSel.GetCount())
	{
		if (m_pSelectedQuestion)
		{
			CString Txt; Txt.LoadString(IDS_RI_DEFAULT_LABEL_ITEMS_CONSTRUITS);
			m_CurPaletteBuilder.m_DefaultLabel.Format(Txt, m_pSelectedQuestion->GetLabel().AsJCharPtr());
		}
	}
	else
		State = FALSE;

	if (GetQuestionOperator()==IBloc::Or)
	{
		if (m_CurPaletteBuilder.m_Label.IsEmpty())
			m_pDlgItem->m_Edit_Name.SetWindowText(m_CurPaletteBuilder.m_DefaultLabel);
		else
			m_pDlgItem->m_Edit_Name.SetWindowText(m_CurPaletteBuilder.m_Label);
	}
	else
	{
		State = TRUE;
		m_DefaultPaletteXLabel.LoadString(IDS_RI_DEFAULT_LABEL_CROISEMENT_PALETTES);
		if (m_PaletteXLabel.IsEmpty())
			m_pDlgItem->m_Edit_Name.SetWindowText(m_DefaultPaletteXLabel);
		else
			m_pDlgItem->m_Edit_Name.SetWindowText(m_PaletteXLabel);
	}

	if(m_pSelectedQuestion)
	{
		m_Map_PaletteBuilder.MoveTo(m_pSelectedQuestion->GetID());
		if(m_Map_PaletteBuilder.IsValid())
			m_Map_PaletteBuilder.GetItem().m_DefaultLabel = m_CurPaletteBuilder.m_DefaultLabel;
	}

	// Modif Filtre/Filtre Janv 2007 (permet de valider choix items avec filtre dim 1 et filtre dim 2 A voir !!!!)
	// m_pDlgItem->m_Button_Accept.EnableWindow(State);
	// m_pDlgItem->m_Edit_Name    .EnableWindow(State);

	int nb = m_pDlgItem->m_List_SAS.GetCount();
	if (!m_pDlgItem->GetTypeRepItem()&& m_pDlgItem->m_List_SAS.GetCount() > 0)
	{
		m_pDlgItem->m_Button_Accept.EnableWindow(true);
		m_pDlgItem->m_Edit_Name    .EnableWindow(true);
	}
	else
	{
		m_pDlgItem->m_Button_Accept.EnableWindow(State);
		m_pDlgItem->m_Edit_Name    .EnableWindow(State);
	}

	if (m_pDlgItem->GetTypeRepItem())
	{
		// Cas deu répertoire des items
		if(!IsValidPalette())
			m_pDlgItem->m_Button_OK.EnableWindow(FALSE);
		else
			m_pDlgItem->m_Button_OK.EnableWindow(TRUE);
	}
	else
	{	
		// Cas du pupitre, on laisse toujours bouton Ok actif
		m_pDlgItem->m_Button_OK.EnableWindow(TRUE);
	}

}

bool CLineMode::IsValidPalette()
{
	if (m_Map_PaletteBuilder.IsValid())
		bool Ok = 1;

	if(m_Map_PaletteBuilder.GetCount() == 0)
		return (false);

	for(m_Map_PaletteBuilder.MoveFirst(); m_Map_PaletteBuilder.IsValid(); m_Map_PaletteBuilder.MoveNext())
	{
		CPaletteBuilder & Palette = m_Map_PaletteBuilder.GetItem();
		if(Palette.m_List_DimSel.GetCount() == 0)
		{
			if (Palette.m_List_NumValue.GetCount() == 0 || (Palette.m_Label.IsEmpty() && Palette.m_DefaultLabel.IsEmpty()))
                return false;
		}
		else if(Palette.m_Map_DimType.GetCount() == Palette.m_List_DimSel.GetCount())
		{
			if(Palette.m_pIBquestion != 0 && (Palette.m_pIBquestion->GetClass() != 0 || Palette.m_pIBquestion->GetNumValue() != 0) && Palette.GetNumValues().GetCount() == 0 && (Palette.m_Label.IsEmpty() && Palette.m_DefaultLabel.IsEmpty()))
				return false;
			if(Palette.m_pIBitem != 0 && Palette.m_pIBitem->GetID() == 0 && (Palette.m_Label.IsEmpty() && Palette.m_DefaultLabel.IsEmpty()))
				return false;
		}
		else if(Palette.m_Map_DimType.GetCount() > Palette.m_List_DimSel.GetCount())
		{
			if(Palette.m_Label.IsEmpty() && Palette.m_DefaultLabel.IsEmpty())
				return false;
		}
	}
	if(GetQuestionOperator()!=IBloc::Or)
	{
		if(m_PaletteXLabel.IsEmpty() && m_DefaultPaletteXLabel.IsEmpty())
			return false;
	}
	return true;
}

void CLineMode::StopMode()
{
	m_Map_PaletteBuilder.Reset();
	m_CurPaletteBuilder .Reset();
	
	// Récupère le map courant sélection
	int Mode = m_pDlgItem->ValCurMode();
	RecupSelMapCur(Mode);
		
	CItemMode::StopMode();

}

// Récupération palette builder(ligne/colonne ou page) et transfert dans map builder courant
void CLineMode::RecupSelMapCur(int Mode)
{
	// A VOIR
	m_Map_PaletteBuilder.Reset();

	if (Mode == Line)
	{
		// Recopie les infos lignes dans map courant
		for (m_Map_PaletteBuilderLigne.MoveFirst();
			 m_Map_PaletteBuilderLigne.IsValid();
			 m_Map_PaletteBuilderLigne.MoveNext())
		{
			m_Map_PaletteBuilder.Add(m_Map_PaletteBuilderLigne.GetKey()) = m_Map_PaletteBuilderLigne.GetItem();   
		}
	}
	else if (Mode == Column) 
	{
		// Mode colonne en cours
		// Recopie les infos colonnes dans map courant
		for (m_Map_PaletteBuilderColonne.MoveFirst();
			 m_Map_PaletteBuilderColonne.IsValid();
			 m_Map_PaletteBuilderColonne.MoveNext())
		{
			m_Map_PaletteBuilder.Add(m_Map_PaletteBuilderColonne.GetKey()) = m_Map_PaletteBuilderColonne.GetItem();   
		}
	}
	else if (Mode == Page)
	{
		// Mode page en cours
		// Recopie les infos pages dans map courant
		for (m_Map_PaletteBuilderPage.MoveFirst();
			 m_Map_PaletteBuilderPage.IsValid();
			 m_Map_PaletteBuilderPage.MoveNext())
		{
			m_Map_PaletteBuilder.Add(m_Map_PaletteBuilderPage.GetKey()) = m_Map_PaletteBuilderPage.GetItem();   
		}
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde palette ligne
bool CLineMode:: SendPalette(JStream &Stream, int Mode)
{
	switch (Mode)
	{
		case Line:
		{
			// On sauve la palette ligne
			CSerialisePaletteBuilder::Send(Stream, &m_Map_PaletteBuilderLigne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}

		case Column:
		{
			// On sauve la palette colonne
			CSerialisePaletteBuilder::Send(Stream, &m_Map_PaletteBuilderColonne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}
		
		case Page:
		{
			// On sauve la palette page
			CSerialisePaletteBuilder::Send(Stream, &m_Map_PaletteBuilderPage, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}
	}

	// En attente
	return true;
}

///////////////////////////////////////////////////////////////////////
// Récupération palette ligne
bool CLineMode:: RecvPalette(JStream &Stream, int Mode)
{
	switch (Mode)
	{
		case Line:
		{
			// Init Map Palette Ligne
			m_Map_PaletteBuilderLigne.Reset(); 

			// On récupère la palette ligne
			CSerialisePaletteBuilder::Recv(Stream, &m_Map_PaletteBuilderLigne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			RecupSelMapCur(Mode);

			// Reconstruit le IBuilder
			// RecupIBuilder();
			break;
		}

		case Column:
		{
			// Init Map Palette Colonne
			m_Map_PaletteBuilderColonne.Reset(); 

			// On sauve la palette colonne
			CSerialisePaletteBuilder::Recv(Stream, &m_Map_PaletteBuilderColonne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			RecupSelMapCur(Mode);

			// Reconstruit le IBuilder
			// RecupIBuilder();
			break;
		}
		
		case Page:
		{
			// Init Map Palette Page
			m_Map_PaletteBuilderPage.Reset(); 

			// On sauve la palette page
			CSerialisePaletteBuilder::Recv(Stream, &m_Map_PaletteBuilderPage,m_Selected_UserItems, m_DefaultPaletteXLabel);
			RecupSelMapCur(Mode);

			// Reconstruit le IBuilder
			// RecupIBuilder();
			break;
		}
	}

	// En attente
	return true;
}

///////////////////////////////////////////////////////////////////////
// Copie infos elmt palette vers la palette destination
bool CLineMode::CopyPalette(CAbstractMode * pPaletteModeDst)
{
	/* ??? ne sert plus à priori
	// Nb élément map palette builder
	JInt32 NbElmtSrc = this->m_Map_PaletteBuilder.GetCount();		

	// Test si palette en cours existante
	if (pPaletteModeDst)
	{
		// Elmt Mode Ligne existant
		// CLineMode * pPaletteDst = dynamic_cast <CLineMode *> (pPaletteModeDst);
		
		CLineMode * pPaletteDst;
		pPaletteDst = (CLineMode *) pPaletteModeDst;

		JInt32 NbElmtDst = pPaletteDst->m_Map_PaletteBuilder.GetCount(); 

		// Init Map palette destination 
		pPaletteDst->m_Map_PaletteBuilder.Reset();

		// Boucle sur le map palette builder en cours, et recopie les elmts dans le map destination
		// Boucle sur les élmts du map palette builder
		for (this->m_Map_PaletteBuilder.MoveFirst();   
			 this->m_Map_PaletteBuilder.IsValid();
			 this->m_Map_PaletteBuilder.MoveNext()) 
		{
			// Récupére et sauve la clef
			JInt32 ClefPaletteBuilder = this->m_Map_PaletteBuilder.GetKey(); 

			// Récupére élmt CPaletteBuilder
			CPaletteBuilder &PaletteBuilderSrc = this->m_Map_PaletteBuilder.GetItem();

			// Recopie l'élmt dans palette destination
			pPaletteDst->m_Map_PaletteBuilder.Add(ClefPaletteBuilder) = PaletteBuilderSrc; 
		}

		// puis recopie m_CurPaletteBuilder
		pPaletteDst->m_CurPaletteBuilder	= this->m_CurPaletteBuilder;  

		// Les selected user_items
		pPaletteDst->m_Selected_UserItems.Reset();
		for (this->m_Selected_UserItems.MoveFirst(); this->m_Selected_UserItems.IsValid(); this->m_Selected_UserItems.MoveNext())
		{
			JInt32 Key = this->m_Selected_UserItems.GetKey();
			pPaletteDst->m_Selected_UserItems.Add(Key) = this->m_Selected_UserItems.GetItem();
		}

		// Les selecteds items
		pPaletteDst->m_SelectedItems.Reset();
		for (this->m_SelectedItems.MoveFirst(); this->m_SelectedItems.IsValid(); this->m_SelectedItems.MoveNext())
		{
			pPaletteDst->m_SelectedItems.AddTail() =  this->m_SelectedItems.GetItem(); 
		}

		// Les autres infos item
		pPaletteDst->m_bItemValid			= this->m_bItemValid; 

		// Update Segment Builder Associé
		pPaletteDst->SetSegmentIBuilder(); 

		return true;
	}
	else
		// Problème de recopie palette
		return false;
	*/	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Sauvegarde des palettes temporaires
bool CLineMode::SaveMemento(JMemento	* pMemento, int Mode)
{
	// Mise à jour des maps récupérés
	switch (Mode)
	{
		// Sauvegarde map palette mode ligne
		case Line:
		{			
			CSerialisePaletteBuilder::Send(*pMemento, &m_Map_PaletteBuilderLigne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}

		// Sauvegarde map palette mode colonne
		case Column:
		{			
			CSerialisePaletteBuilder::Send(*pMemento, &m_Map_PaletteBuilderColonne, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}

		// Sauvegarde map palette mode page
		case Page:
		{			
			CSerialisePaletteBuilder::Send(*pMemento, &m_Map_PaletteBuilderPage, m_Selected_UserItems, m_DefaultPaletteXLabel);
			break;
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Récupération des palettes temporaires
bool CLineMode::LoadMemento(JMemento	* pMemento, int Mode)
{
	// Mise à jour des maps récupérés
	switch (Mode)
	{
		case CPaletteMode::Line:
		{			
			// Récupération memento
			CSerialisePaletteBuilder::Recv(*pMemento, &m_Map_PaletteBuilderLigne, m_Selected_UserItems, m_DefaultPaletteXLabel);

			this->RecupSelMapCur(Mode);
			break;
		}

		case Column:
		{			
			// Récupération memento
			CSerialisePaletteBuilder::Recv(*pMemento, &m_Map_PaletteBuilderColonne, m_Selected_UserItems, m_DefaultPaletteXLabel);

			this->RecupSelMapCur(Mode);
			break;
		}

		case Page:
		{	
			// Récupération memento
			CSerialisePaletteBuilder::Recv(*pMemento, &m_Map_PaletteBuilderPage, m_Selected_UserItems, m_DefaultPaletteXLabel);

			this->RecupSelMapCur(Mode);
			break;
		}
	}

	return true;
}
