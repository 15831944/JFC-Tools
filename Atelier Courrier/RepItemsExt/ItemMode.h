
#pragma once

#include "AbstractMode.h"
#include "ItemConstruit.h"
#include "IItem.h"
#include "IModalite.h"
#include "IBuilder.h"
#include "IBQuestion.h"
#include "DimensionDlg.h"
#include "ValuesDlg.h"
#include "IBItem.h"
#include "Pseudo.h"

/**
 * \ingroup RepItems
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CItemMode : public CAbstractMode  
{
public:
						CItemMode(CDlgItems *pDlgItems);
	virtual				~CItemMode();

public:
	// *************************************************************************************************************************
	// **************************** Abstract base class overrides **************************************************************
	// *************************************************************************************************************************
	/// This is used to StartMode the Mode
	virtual void		StartMode();
	/// This is used to stop the Mode
	virtual void		StopMode();

	// Récupération d'un mode en cours
	virtual void		RecupMode();

	// Updates the current display
	virtual void		UpdateDisplay();
	/// Button Accept pushed
	virtual bool		OnAccept(CString &Name);
	/// Button Genese pushed
	virtual bool		OnGenese();
	/// Button Modalite Ok pushed
	virtual void		OnDim_1_Ok();
	/// Button Modalite Et pushed
	virtual void		OnDim_1_Et();
	/// Button Modalite XOr pushed
	virtual void		OnDim_1_Xor();
	/// Button Modalite Ni pushed
	virtual void		OnDim_1_Ni();
	/// Button Modalite Ou pushed
	virtual void		OnDim_1_Ou();
	/// Button Modalite Raz pushed
	virtual void		OnDim_1_Raz();
	/// Button Element Et pushed
	virtual void		OnDim_2_Et();
	/// Button Element XOr pushed
	virtual void		OnDim_2_Xor();
	/// Button Element Ni pushed
	virtual void		OnDim_2_Ni();
	/// Button Element Ou pushed
	virtual void		OnDim_2_Ou();
	/// Button Element Raz pushed
	virtual void		OnDim_2_Raz();
	/// Button Question Et pushed
	virtual void		OnSAS_Et();
	/// Button Question Ou pushed
	virtual void		OnSAS_Ou();
	/// Button Segment Et pushed
	virtual void		OnSegment_Et();
	/// Button Segment Ou pushed
	virtual void		OnSegment_Ou();
	/// Button Raz Tout pushed
	virtual void		OnTout_Raz();
	// Clearing all the current item && palettes sauf tree questions
	virtual void		OnTout_Raz_ItemPalette();
	/// Button Item Raz pushed
	virtual void		OnItem_Raz();
	/// Button SAS Raz pushed
	virtual void		OnSAS_Raz();
	/// Suppression segment actif
	virtual void		OnSegment_Raz();
	// Clear seulement sélection item
	virtual void		OnDeselect_Items();
	// Update the Modalite list box
	virtual void		UpdateDimensionsList();
	/// Update the Modalite list box 1 uniquement
	virtual void		UpdateFiltreDimension1List();
	/// Message sent from theme tree control
	virtual LRESULT		OnUpdated_QuestionList(WPARAM wParam, LPARAM lParam);
	/// Message sent from rubrique tree control
	virtual LRESULT		OnUpdated_ItemList(WPARAM wParam, LPARAM lParam);
	/// Tab Segment Selection Changed
	virtual void		OnSelchange_TabSegment(NMHDR* pNMHDR, LRESULT* pResult);
	/// Segment calculation button Clicked
	virtual void		OnChange_SegmentCalc();
	/// List Question Selection changed
	virtual void		OnSelchange_ListQuestions();
	/// List Sas selection changed
	virtual void		OnSelchange_ListSas();
	/// List modalites selection changed
	virtual void		OnSelchange_ListDimension_1();
	/// List Dimension2 selection changed
	virtual void		OnSelchange_ListDimension_2();
	/// Message sent from modeless Modality and Element dialogs
	virtual LRESULT		OnSelChange_ListDimension_x(WPARAM wParam, LPARAM lParam);

	/// List Items selection changed
	virtual void		OnSelchange_ListItems();
	
	/// Mise en place des pseudos items sélectionnés
	virtual void		UpdatePseudoItem(JList<JInt32> &Values);

	// Du mode line / pupitre
	virtual void		UpdatePseudoItemLineMode(JList<JInt32> &Values);

	/// Context menu selected
	virtual void		OnContextMenu(CWnd* pWnd, CPoint point);

	// Affichage du menu selon mode, nombre de sélection et type de questions
	virtual void		OptionMenuContextuelDim1(CMenu *pMenu);

	/// Message sent from values dialog
	virtual void		OnSelchange_ListValue();
	/// Message sent from values dialog
	virtual void		OnChange_Value_PaletteSel();
	/// Message sent from values dialog
	virtual void		OnValuesQuantile(LONG Type);
	// Prepares and calculates all seperate target items
	virtual void		DoVisualisation(void);
	// Récupération des élmts constituant l'item sous forme de texte
	virtual CString GetVisualisation();
	/// Population Segment has Changed
	virtual void		OnChange_PopSegment();
	/// Type Poids has Changed
	virtual void		OnChange_TypePoids();
	/// Reset the colors
	virtual void		SetColors();
	/// Reset the icones
	virtual void		SetIcones();
	/// Displays the selected theme, support window
	virtual void		UpdateTheme(int ID);
	/// Update the items construits list
	virtual void		UpdateListItems();
	/// Button Modalite Filtrage
	virtual void		OnDim_Filtre();
	/// Button Modalite Down pushed
	virtual void		OnDim_1_Up();
	/// Button Modalite Down pushed
	virtual void		OnDim_1_Down();
	/// Returns a string to the current dimension mode
	virtual CString		GetDimensionModeString(int Mode);
	/// Returns nb élmts sélectionnés
	virtual int			GetNbSelectUserItem();

	// Build List all current questions in all segment 
	virtual void 		Find_IBListQuestionSAS(JList <JInt32> &ListQuestionSAS);

	// Récupère les infos segments et IBuilder via les infos Questions et Items
	virtual void		SetSegmentIBuilder();

	// Positionne le pseudo item
	virtual void		SetPseudoItem();

	// *************************************************************************************************************************
	// **************************** Unused functions in item mode **************************************************************
	// *************************************************************************************************************************

	/// Returns an Item wrapper list with the current configuration
	virtual void		GetSelectedItems(JList< CItemWrapper *> &Items){};

	virtual void		SetSelectedItems(JList< CItemWrapper *> &Items){};	

	/// Generate Items based on the current valid configuration
	virtual bool		BuildSelectedItems(bool RecupSel = false)	{ return false;};
	virtual bool		BuildSelectedItemsPalette() { return false;};

	/// Button Dimension2 pushed
	virtual void		OnChange_Dim_2_PaletteSel(){};
	virtual void		OnSetDimSelection2(){};

	/// Button Dimension1 pushed
	virtual void		OnChange_Dim_1_PaletteSel(){};
	virtual void		OnSetDimSelection1(){};

	/// Message sent from modeless Modality and Element dialogs
	virtual LRESULT		OnChange_Dim_x_PaletteSel(WPARAM wParam, LPARAM lParam) { return 1; } ;
	/// Questions to palette
	virtual void		OnQuestions_Tous(){};

	// Quel mode pour le Sas
	virtual bool		ModeCroisementPalette();

	// Sauvegarde palette (ligne ou colonne ou page)
	virtual bool		SendPalette(JStream &Stream, int Mode);

	// Récupération palette (ligne ou colonne ou page)
	virtual bool		RecvPalette(JStream &Stream, int Mode);

	// Recopie elmt palette
	virtual bool		CopyPalette(CAbstractMode * pPaletteMode);

	// Sauvegarde et récupération des palettes temporaires
	virtual bool		SaveMemento(JMemento	* pMemento, int Mode);
	virtual bool		LoadMemento(JMemento	* pMemento, int Mode);

	// Nb fenêtre dimension ouverte
	virtual JUnt32		NbDimDlg()	{ return m_DimDlg.GetCount();};

	// Delete the selected item construits (seulement les items) sans afficher message
	virtual bool		DeleteItemConstruit(JInt32 ID);

	// Visualiser the selected item construits
	virtual void		ViewItem(JInt32 ID);

	// Test si item quantitatif
	virtual JBool		ItemQuantitatif();

	// Filtarge modalité
	virtual void FiltreModaliteListDimension1();

	// *************************************************************************************************************************
	// **************************** End of Abstract base class overrides *******************************************************
	// *************************************************************************************************************************

protected:
	// *************************************************************************************************************************
	// ********************************************* overridables **************************************************************
	// *************************************************************************************************************************

	/// Rename the item construit
	virtual void		RenameItem(JInt32 ID);

	/// Delete the item construits
	virtual void		DeleteItem(JInt32 ID, bool DeleteForce);
	
	/// Export the item construits
	virtual void		ExportItem(JInt32 ID);
	/// Export liste des items construits
	virtual void		ExportItems(JList <JInt32> & ListID);
	/// Preview the selected item construits
	virtual void		PreviewItem();
	/// Rename modalites
	virtual void		RenameDimension1(JInt32 ID);
	/// Preview the selected item construits
	virtual void		PreviewDimension1();
	/// Update the Question list box
	virtual void		UpdateQuestionList(long BlockId=0);
	/// Update the Question list box via Genèse
	virtual void		UpdateQuestionListGenese(long BlockId=0);
	/// Updates the number of segments
	virtual void		UpdateSegmentTabs(int Sel);
	/// Update the Ibuilder selections
	virtual void		UpdateIBuilder(bool Changed, bool bValues);
	/// Updates the Values from values dlg
	virtual bool		UpdateItemValues(bool bValues);
	/// Set the selected question
	virtual void		QuestionChanged(IQuestion * pCurQuestion);

public:
	bool				OrderValues(bool bUp,JList<JInt32> & Values, JInt32 Move);

protected:
	/// Check is item is part of the search mode
	bool				IsSearched(const IItem * item) const;
	/// Enable/Disable the operators
	void				EnableOperators(BOOL bEnable);
	/// Updates the current item configuration Modalities Dimension2 etc.... (et renvoi si au moins 1 item valide)
	void				UpdateItemDetails(bool & bValid);
	/// Calculate the target effective
	bool				CalculateEffectif(CString & SEffectif,CString & SPercent,CString & SNbCas);
	/// Finds the current segment or creates a new segment
	/// return true if found
	bool				FindSet_IBsegment(int Index, bool bCopy = false,bool UpdateQuLst = true);
	/// Finds the IBquestion or IBitem in the segment or creates a new IBquestion or IBitem
	/// returns true if found
	void				FindSet_IBcontainerByType(const IQuestion * pQuestion);
	/// Finds the question in the segment or creates a new question
	/// returns true if found
	bool				FindSet_IBquestion(int Quid);
	/// Finds the Pseudo items in the segment or creates a new one
	/// returns true if found
	void				FindSet_Pseudo(JList<JInt32> * pValues=0, bool MajOpe = true);
	/// Finds the item container in the segment or creates a new quantile
	/// returns true if found
	bool				FindSet_IBitem(int id);
	/// Sets the active questions in the question list box
	virtual void		SetActiveSegmentQuestions();
	/// Sets the selected modalties in the modalites list boxes
	bool				SetSelectedDimensions();
	/// Sets the Values dialog
	bool				SetValuesDlg();
	/// Gets the question quanti values
	bool				GetQuestionNumericValues(CIBQuestion * pIQuestion, JVector<JFlt32> & Values, JInt32 & Divider);
	/// Sets the order of modalities
	// bool				OrderValues(bool bUp,JList<JInt32> & Values, JInt32 Move);
	// Preview the selected item construits
	void				PreviewItems(JList<JInt32> &Values);
	/// Get and set the modalite operator
	void				SetDimension1Operator(IBloc::OPERATOR Operator);
	IBloc::OPERATOR		GetDimension1Operator();
	/// Get and set the element operator
	void				SetDimension2Operator(IBloc::OPERATOR Operator);
	IBloc::OPERATOR		GetDimension2Operator();
	/// Get and set the question operator
	void				SetQuestionOperator(IBloc::OPERATOR Operator);
	IBloc::OPERATOR		GetQuestionOperator();
	/// get and set the segment operator
	void				SetSegmentOperator(IBloc::OPERATOR Operator);
	IBloc::OPERATOR		GetSegmentOperator();
	/// Format the numeric string
	void				Format(const CString &Fmt, CString &Num, const char Decimal='.');
	/// Get the Operator string
	CString				OperatorAsString(IBloc::OPERATOR Op);

	// Vire une question du SAS
	// void				SuppQuestionSAS(JInt32 IdQuestion);
	// Finds all current questions in all segment or delete them
	virtual bool		FindDelete_IBMapquestion(JMap <JInt32,JInt32> &MapQuestion, int &SelSeg);

	// Finds all current questions in all segment or delete them  (via m_pPseudoItem)
	bool				FindDelete_IBMapItem(int &SelSeg);

	// Mise à jour Segments suite à suppression élmt
	void				MajSegments();

	// Balayage des segments
	void				MajTabSegment(); 

	// Suppression des fenêtres mobiles
	void				EffaceFenDimDlg(bool MajSelectedUserItem = true);

public:
	IQuestion const *				m_pSelectedQuestion;

protected:

	/// IBuilder Objects / construction des cibles.....
	CIBuilder						m_IBuilder;
	CIBSegment *					m_pIBsegment;
	CIBQuestion *					m_pIBquestion;
	CIBItem	*						m_pIBitem;
	bool							m_bItemValid;

	/// Current Question
	// IQuestion const *				m_pSelectedQuestion;

	/// Pseudo Question
	CPseudo *						m_pPseudoItem;

	/// Selected User items
	JMap< JInt32, JBool >			m_Selected_UserItems;

	/// Dimensions lists
	JList< JVector<const IModalite *> >	m_DimList;
	JList< CDimensionDlg >				m_DimDlg;
	JList< RECT >						m_DimDlgPos;
	JList< JList<JInt32> >				m_Dim_Sel;
	JList< JInt32 >						m_DimType_Old;
	JList< JInt32 >						m_DimType_New;

	/// Values Dialog
	// CValuesDlg *						m_pValuesDlg;
	CRect								m_ValuesDlgPos;

	// Garde 1ere sélection dans liste dimension 1
	JUnt32								m_PremSelListDimension_1;

	// Type moyennable ou pas
	JBool								m_MoyenneActive;

public:
	/// Values Dialog
	CValuesDlg *						m_pValuesDlg;

};
