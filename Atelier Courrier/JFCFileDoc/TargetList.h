#pragma once

#include "JFCXlistBox.h"
#include "SupportListFunctors.h"

#include <list>
#include <map>
#include <set>

class IItem;
class CItemConstruit;
class COffre2MainDialog;
class CCalcItemParams;
class CItemsConstruitsBase;
class JSRCTables;
class JSource;
class JTerrain;

struct CTargetFilter
{
	CString	strSearch;	
	JUnt32	idSource;
	bool	supports_from_source;
	bool	modePalettes;

	CTargetFilter() : strSearch(""), supports_from_source (true), idSource (0),	modePalettes(false) {}
};

//=============================================================================	
//	

//=============================================================================	
// Foncteur de tri de libell�s de cibles
struct TargetLabelSort
	: std::binary_function<const IItem *, const IItem *, bool>
//=============================================================================	
{
	virtual bool operator()	(const IItem * pItem1, const IItem * pItem2) const;

private:
	static void RemoveAccents(char & c);
	const std::string & PrepareStr(std::string & s) const;
};

//bool operator < (const IItem & Item1, const IItem & Item2);

typedef std::set<const IItem*>		IItemList_t;

//=============================================================================	
// Bo�te de dialogue CTargetList
class CTargetList : public CJFCXListBox
//=============================================================================	
{
	DECLARE_DYNAMIC(CTargetList)

public:
	CTargetList(COffre2MainDialog * pDialogOffre);   // constructeur standard
	virtual ~CTargetList();

	// ajout d'un item � la listbox
	void AddItem(const IItem * pItem);

	// suppression de tous les items
	void ResetItems();

	// indique quel est le type d'affichage courant (items ou palettes)
	void SetCurrentDisplay(const int eDisplay);

	void Lock();
	void UpdateAndUnlock(const CTargetFilter & filter, IItemList_t & SelectedItems);

	void SetAppColor(int appColor);

	const JUnt32 GetReferenceAffiniteId() const;

	void SetReferenceAffiniteItem(const IItem * pItem);
	const IItem *GetReferenceAffiniteItem() const;

	// recharge les elements permettant de calculer les seuils (source, terrains, segments et types de poids s�lectionn�s)
	void ReloadCacheSeuilsElements();

	// indique que les seuils sont invalides et qu'ils faudra les recalculer
	void InvalidateSeuils();

	// d�selectionne d'office tous les elements ayant un seuil r�dhibitoire
	void DeselectItemsWithBadSeuils();

	// Affiche un message si le seuil de l'item est r�hibitoire ou si c'est un seuil d'avertissement
	// bUnselect vaudra true au retour si l'item doit �tre d�selectionn�
	void DisplaySingleItemMessage(const IItem * pItem, bool & bUnselect);

	// on d�clare l'impl�mentation priv�e en public pour acceder � tts les methode depuis des functeurs exterieurs
	struct Pimpl;

protected:
	virtual void PreSubclassWindow();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	afx_msg void OnMenuReferenceAffinite();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

	DECLARE_MESSAGE_MAP()

	// recherche de l'index de la reference affinit� (LB_ERR si la Ref. Affinit� n'est pas dans la liste)
	int FindReferenceAffiniteIndex();

private:

	std::multiset<const IItem*>		m_Targets;
	std::multiset<const IItem*>		m_SelectionCopy;
	
	bool	m_bIsSorted;
	int		m_eDisplay;
	TCHAR*	m_pchTip;
	WCHAR*	m_pwchTip;

	const IItem *	m_pRefAffinite;
	//int				m_IdxRefAffinite;
	int				m_RClickedItem;
	int				m_appColor;

	COffre2MainDialog * m_pDialogOffre;

	Pimpl * m_pImpl;		// impl�mentation priv�e de CTargetList
	friend struct Pimpl;
};