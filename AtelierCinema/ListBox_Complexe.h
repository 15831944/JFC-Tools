// ListBox_Complexe.h : main header file for the LISTBOX_COMPLEXE application
//

#if !defined(AFX_LISTBOX_COMPLEXE_H__91200EE4_1C0A_468C_8125_04BF7D1DACA0__INCLUDED_)
#define AFX_LISTBOX_COMPLEXE_H__91200EE4_1C0A_468C_8125_04BF7D1DACA0__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// ListBox_Complexe window

class ListBox_Complexe : public CObj_LBox
{
// Construction
public:
	ListBox_Complexe();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ListBox_Complexe)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Tableau info taille colonne
	// ... avec nb colonnes pour chaque libellé
	//     taille de chaque colonne
	//     libelle associé à chaque colonne

	typedef struct InfoColListe
	{
		int TailleCol;
		CString LibCol;
	};

	CArray<InfoColListe,InfoColListe&> m_TabListe;

	// Définition nombre de colonnes
	bool InitNbColonne(int NbColonne);

	// Définition taille des colonnes
	bool InitSizeColonne(int InxCol,int SizeCol);
	
	// Lecture libelle liste (nIndex : index liste, ColIndex : index colonne)
	void GetText(int nIndex, int ColIndex,CString &rString) const;

	// Gestion liste complexe
	int GetSelCount();	
	int GetSelItems(int nMaxItems, LPINT rgIndex);

	virtual ~ListBox_Complexe();

	// Generated message map functions
protected:

	//{{AFX_MSG(ListBox_Complexe)
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private :
	// type d'alignement du texte 
	UINT m_AlignType;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOX_COMPLEXE_H__91200EE4_1C0A_468C_8125_04BF7D1DACA0__INCLUDED_)
