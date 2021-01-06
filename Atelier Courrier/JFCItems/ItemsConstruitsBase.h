#pragma once

#pragma warning( disable : 4786 )

#include <FileHeader.h>
#include "ItemConstruit.h"
#include "Palettes.h"
#include "ItemPondere.h"
#include "QuestionInduite.h"
#include "CritereModifie.h"
#include "CritereQuantile.h"
#include "CritereScore.h"
#include "Pseudo.h"
#include "JStream.h"


#include "..\Config\BuildConfig.h"


/**
* \ingroup JFCITEMS EXTERNAL
*
* This is a singleton class with reference counting
* and must be used accordingly.
* A user gets the instance of this object by calling GetInstance
* when the user has finished with the object he must call Release
* To reload this object all references must be released before Calling
* GetInstance again.
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

#if defined(BUILD_FOR_ATELIER_INTERNET)
#include "ItemsMessage.h"

#define _GETSTRING(id)			ItemsMessage::GetString(ItemsMessage::MSG_##id)

#else

#define IDS_ENSEMBLE	"Ensemble"

#define _GETSTRING(id)	IDS_ENSEMBLE

#endif


class AFX_EXT_CLASS CItemsConstruitsBase  
{
public:
	/// Item Owners IDs
	enum OWNER
	{
		ITEMSINDEPENDANT_ID =	1, ///< Independant Item
		PALETTE_ID			=	2, ///< Palette
		QUESTIONIND_ID		=	3, ///< QuestionInduite
		CRITEREMOD_ID		=	4, ///< Critere modifie
		CRITEREQUANT_ID		=	5, ///< Critere Quantile
		CRITERESCORE_ID		=	6, ///< Critere Score
		ITEMSPONDERE_ID		=   7, ///< Independant Item Pondéré

		SYSTEM_ID			=	10 ///< System component
	};

public:
	///// Get an instance of this class
	///// this adds one to the reference counter and
	///// thus must be released by the caller after use
	//static CItemsConstruitsBase *	GetInstance();
	///// Decrements the reference count
	//static JBool				Release();

private:
	///// The reference counter
	//static JInt32				m_RefCount;
	///// The instance to return
	//static CItemsConstruitsBase		m_Instance;

protected:
	CItemsConstruitsBase();
	virtual ~CItemsConstruitsBase();

public:
	/// Checks if the Data is dirty / needs saving
	JBool	IsDirty()		{ return m_bDirty; } ;
	/// Sets the Data as dirty / and saves
	void	SetDirty()	
	{ 
		m_bDirty=true;
	} ;
	/// Saves the items construit return true is sucessful
	JBool	Save();
	/// Check if the Items Construit have been loaded
	JBool	IsLoaded()	{ return m_bLoaded; } ;

	/// Create new Pseudo question / this pointer must be deleted by the caller
	CPseudo *			GetNewPseudo			() { return new CPseudo(this); } ;
	/// Create new ItemsConstruit / this pointer must be deleted by the caller
	CItemConstruit *	GetNewItemConstruit		() 
	{
		JInt32 Val = m_tMaxItem;
		return new CItemConstruit(this, m_tMaxItem--); 
	} ;

	/// Create new ItemsConstruit / this pointer must be deleted by the caller
	CItemConstruit *	GetNewItemConstruit		(const CItemConstruit * rhs) ;
	/// Create new Palette / this pointer must be deleted by the caller
	CPalettes *			GetNewPalette			(JBool bDeleteTemp = true) { return new CPalettes(this, m_tMaxPalettes--, bDeleteTemp); };
	/// Create new Critere Induite / this pointer must be deleted by the caller
	CQuestionInduite *	GetNewQuestionInduite	(JBool bDeleteTemp = true) { return new CQuestionInduite(this,m_tMaxQuestionInd--, bDeleteTemp); };	
	/// Create new CritereModifie / this pointer must be deleted by the caller
	CCritereModifie *	GetNewCritereModifie	(JBool bDeleteTemp = true) { return new CCritereModifie(this,m_tMaxCritereMod--, bDeleteTemp); };	
	/// Create new Critere Quantile / this pointer must be deleted by the caller
	CCritereQuantile *	GetNewCritereQuantile	(JBool bDeleteTemp = true) { return new CCritereQuantile(this,m_tMaxCritereQuant--, bDeleteTemp); };	
	/// Create new Critere Score / this pointer must be deleted by the caller
	CCritereScore *		GetNewCritereScore		(JBool bDeleteTemp = true) { return new CCritereScore(this,m_tMaxCritereScore--, bDeleteTemp); };	

	/// Create new item pondéré / this pointer must be deleted by the caller
	CItemPondere *		GetNewItemPondere		(JBool bDeleteTemp = true) { return new CItemPondere(this,m_tMaxItemPondere--, bDeleteTemp); };	

	/// Get pointer to an ItemConstruit
	CItemConstruit *		GetItemConstruit	(JInt32 ID) const;
	/// Get pointer to an ItemConstruit
	CItemConstruit *		GetItemEnsemble		() const { return GetItemConstruit(ITEM_BASE); };
	/// Get pointer to a Palette
	CPalettes *				GetPalette			(JInt32 ID) const;
	/// Get pointer to a Critere Induite
	CQuestionInduite *		GetQuestionInduite	(JInt32 ID) const;	
	/// Get pointer to a Critere Modifié
	CCritereModifie *		GetCritereModifie	(JInt32 ID) const;	
	/// Get pointer to a Critere Quantile
	CCritereQuantile *		GetCritereQuantile	(JInt32 ID) const;	
	/// Get pointer to a Critere Score
	CCritereScore *			GetCritereScore		(JInt32 ID) const;	

	/// Renvoi nb items pondérés existants
	const JUnt32			GetNbItemsPonderes();

	/// Get pointer to a Ponderate Item
	CItemPondere *			GetItemPondere		(JInt32 ID) const;

	/// Get pointer to an IQuestion Object with the given ID
	IQuestion *				GetQuestionByID		(JInt32 ID) const;
	/// Gets a pointer to an IItem Object with the given Id
	IItem *					GetIItemByID		(JInt32 ID) const;

	/// Get pointer to an ItemConstruit
	const CItemConstruit *		GetItemConstruit	(const JLabel & Label) const;
	/// Get constant pointer to an ItemIndependant
	const CItemConstruit *		GetItemIndependant	(const JLabel & Label) const;
	/// Get constant pointer to a Palette
	const CPalettes *			GetPalette			(const JLabel & Label) const;
	/// Get constant pointer to a Critere Induite
	const CQuestionInduite *	GetQuestionInduite	(const JLabel & Label) const;	
	/// Get constant pointer to a Critere Modifié
	const CCritereModifie *		GetCritereModifie	(const JLabel & Label) const;	
	/// Get constant pointer to a Critere Quantile
	const CCritereQuantile *	GetCritereQuantile	(const JLabel & Label) const;	
	/// Get constant pointer to a Critere Score
	const CCritereScore *		GetCritereScore		(const JLabel & Label) const;
	/// Get constant pointer to a Ponderate item
	const CItemPondere *		GetItemPondere		(const JLabel & Label) const;

	/// Returns the default "Ensemble" Item Id
	JInt32	GetDefaultItemId() const { return ITEM_BASE; }
	/// Get a vector of Items Independant as CItemConstruit
	JBool	GetItemsIndependant	(JList<const CItemConstruit *> & vItems, CTerrain *pTerrain = 0) const;
	/// Get a vector of Items Independant as IItems
	JBool	GetItemsIndependant	(JList<const IItem *> & vItems, CTerrain *pTerrain = 0) const;
	/// Get a vector of Items Independant as IItems
	JBool	GetItemsIndependant	(JList<const IItem *> & vItems, JList<CTerrain*> & lstTerrains) const;

	/// Get a vector of Items Independant as IItems
	template<class C1, class C2>
	bool	GetItemsIndependant	(C1 & vItems, const C2 & terrains) const;

	/// Get a vector of Items Independant as IModalites
	JBool	GetItemsIndependant	(JList<const IModalite *> & vItems, CTerrain *pTerrain = 0) const;
	/// Get a vector of Palettes
	JBool	GetPalettes			(JList<const IQuestion *> &vItems, CTerrain *pTerrain = 0) const;
	/// Get a vector of Palettes
	JBool	GetPalettes			(JList<const IItem *>  &vItems, JList<CTerrain*> & lstTerrains) const;
	/// Get a vector of Palettes
	JBool	GetPalettes			(JList<const IItem *>  &vItems, CTerrain *pTerrain = 0) const;

	/// Get a vector of Palettes
	template<class C1, class C2>
	bool	GetPalettes	(C1 & vItems, const C2 & terrains) const;

	/// Get a vector of Criteres Induit by BlocID 
	JBool	GetQuestionsInduiteByBlocID (JList<const IQuestion *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	JBool	GetQuestionsInduiteByBlocID (JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID = 0, CTerrain *pTerrain = 0, int iModality = 0) const;
	/// Get a vector of Criteres Induit by BlocID 
	JBool	GetQuestionsInduiteByBlocID (JList<const IItem *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	/// Get a vector of Criteres Modifie by BlocID 
	JBool	GetCriteresModifieByBlocID (JList<const IQuestion *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	JBool	GetCriteresModifieByBlocID (JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID = 0, CTerrain *pTerrain = 0, int iModality = 0) const;
	/// Get a vector of Criteres Modifie by BlocID 
	JBool	GetCriteresModifieByBlocID (JList<const IItem *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	/// Get a vector of Criteres Quantile by BlocID
	JBool	GetCriteresQuantileByBlocID	(JList<const IQuestion *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	JBool	GetCriteresQuantileByBlocID	(JList<const IQuestion *> &vItems, const CString & Filter, JInt32 BlocID = 0, CTerrain *pTerrain = 0, int iModality = 0) const;
	/// Get a vector of Criteres Quantile by BlocID
	JBool	GetCriteresQuantileByBlocID	(JList<const IItem *> &vItems, JInt32 BlocID = 0, CTerrain *pTerrain = 0) const;	
	/// Get a vector of Criteres Score
	JBool	GetCriteresScore			(JList<const IQuestion *> &vItems) const;	

	/// Get a vector of Ponderate Items
	JBool	GetItemsPonderes			(JList<const IItem *>  &vItems, JList<CTerrain*> & lstTerrains) const;
	/// Get a vector of Ponderate Items
	JBool	GetItemsPonderes			(JList<const IItem *>  &vItems, CTerrain *pTerrain = 0) const;


	/// Register new item construit / returns a constant pointer to the registered version
	const CItemConstruit *		RegisterItemConstruit	(const CItemConstruit * pItem, JInt32 OwnerID = ITEMSINDEPENDANT_ID) ;
	/// Register new Palette / returns a constant pointer to the registered version
	const CPalettes *			RegisterPalette			(const CPalettes * pItem) ;
	/// Register new Critere Induite / returns a constant pointer to the registered version
	const CQuestionInduite *	RegisterQuestionInduite	(const CQuestionInduite * pItem) ;
	/// Register new Critere Modifié / returns a constant pointer to the registered version
	const CCritereModifie *		RegisterCritereModifie	(const CCritereModifie * pItem);
	/// Register new Critere Quantile / returns a constant pointer to the registered version
	const CCritereQuantile *	RegisterCritereQuantile (const CCritereQuantile * pItem) ;
	/// Register new Critere Score / returns a constant pointer to the registered version
	const CCritereScore *		RegisterCritereScore	(const CCritereScore * pItem) ;

	/// Register new item pondéré / returns a constant pointer to the registered version
	const CItemPondere *		RegisterItemPondere	(const CItemPondere * pItem) ;

	/// Can UnRegister an item construit / returns true if the item can be unregistered
	JBool	CanUnRegisterItemConstruit		(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0, JInt32 OwnerID = ITEMSINDEPENDANT_ID);
	/// Can UnRegister a Palette / returns true if the palette can be unregistered
	JBool	CanUnRegisterPalette			(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);
	/// Can UnRegister a Critere Induite / returns true if the critere induite can be unregistered
	JBool	CanUnRegisterQuestionInduite	(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);
	/// Can UnRegister a Critere Modifié / returns true if the critere modifié can be unregistered
	JBool	CanUnRegisterCritereModifie		(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);
	/// Can UnRegister a Critere Quantile / returns true if the critere quantile can be unregistered
	JBool	CanUnRegisterCritereQuantile	(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);
	/// Can UnRegister a Critere Score / returns true if the critere score can be unregistered
	JBool	CanUnRegisterCritereScore		(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);

	/// Can UnRegister a ponderate item / returns true if the ponderate item can be unregistered
	JBool	CanUnRegisterItemPondere		(JInt32 ID, JMap<JInt32, JLabel> * pUpwards = 0);

	/// UnRegister an item construit / returns true if unregistered
	JBool	UnRegisterItemConstruit		(JInt32 ID, JInt32 OwnerID = ITEMSINDEPENDANT_ID) ;
	/// UnRegister a Palette / returns true if unregistered
	JBool	UnRegisterPalette			(JInt32 ID) ;
	/// UnRegister a Critere Induite / returns true if unregistered
	JBool	UnRegisterQuestionInduite	(JInt32 ID) ;
	/// UnRegister a Critere Modifié / returns true if unregistered
	JBool	UnRegisterCritereModifie	(JInt32 ID) ;
	/// UnRegister a Critere Quantile / returns true if unregistered
	JBool	UnRegisterCritereQuantile	(JInt32 ID) ;
	/// UnRegister a Critere Score / returns true if unregistered
	JBool	UnRegisterCritereScore		(JInt32 ID) ;

	/// UnRegister a ponderate item / returns true if unregistered
	JBool	UnRegisterItemPondere		(JInt32 ID) ;

	/// returns true if registered
	JBool	IsRegisteredItemConstruit	(JInt32 ID) ;
	/// returns true if registered
	JBool	IsRegisteredPalette			(JInt32 ID) ;
	/// returns true if registered
	JBool	IsRegisteredQuestionInduite	(JInt32 ID) ;
	/// returns true if registered
	JBool	IsRegisteredCritereModifie	(JInt32 ID) ;
	/// returns true if registered
	JBool	IsRegisteredCritereQuantile	(JInt32 ID) ;
	/// returns true if registered
	JBool	IsRegisteredCritereScore	(JInt32 ID) ;

	/// returns true if registered
	JBool	IsRegisteredItemPondere		(JInt32 ID) ;

	// Export a list of Items / Palettes / Classes / Quantiles etc...
	JBool	Export(JList<JInt32> & IdList, CWnd* pParent);

	JBool	Export(CStdioFile & ExportFile, JList<JInt32> & IdList, JMap<JInt32,JBool> & ExpMap);
	JBool	Export(CStdioFile & ExportFile, const IItem *pItem,		JMap<JInt32,JBool> & ExpMap);
	JBool	Export(CArchive & ExportFile, JList<JInt32> & IdList,	JMap<JInt32,JBool> & ExpMap);
	JBool	Export(CArchive & ExportFile, const IItem *pItem,		JMap<JInt32,JBool> & ExpMap);

	// Import a files of exported items / Palettes / Classes / Quantiles ...
	JBool	Import(CWnd* pParent);
	JBool	Import(CStdioFile & ImportFile, JMap<JInt32,const IItem *> &ImpMap);
	JBool	Import(CArchive & ImportFile, JMap<JInt32,const IItem *> &ImpMap);

	/// Get Downwards (pour retrouver la composition d'un item)
	CString GetCompoItem(JInt32 ID, CTerrain *pTerrain);

protected:

	friend class CItemConstruit;
	friend class CItemPondere;
	friend class CPalettes;
	friend class CQuestionInduite;
	friend class CCritereModifie;
	friend class CCritereQuantile;
	friend class CCritereScore;

	/// Check if new item already exists / returns the Id of the existing version
	JInt32		CheckExists(CItemConstruit * pItem) ;
	/// Check if new palette already exists / returns the Id of the existing version
	JInt32		CheckExists(CPalettes * pItem) ;
	/// Check if new Critere Induite already exists / returns the Id of the existing version
	JInt32		CheckExists(CQuestionInduite * pItem) ;
	/// Check if new Critere Modifié already exists / returns the Id of the existing version
	JInt32		CheckExists(CCritereModifie * pItem) ;
	/// Check if new Critere Quantile already exists / returns the Id of the existing version
	JInt32		CheckExists(CCritereQuantile * pItem) ;
	/// Check if new Ponderate Item already exists / returns the Id of the existing version
	JInt32		CheckExists(CItemPondere * pItem) ;

	/// Register new item construit / returns a constant pointer to the registered version
	CItemConstruit *	RegisterItemConstruit	(CItemConstruit * pItem, JMap<JInt32,const IItem *> &ImpMap, JInt32 OwnerID = ITEMSINDEPENDANT_ID) ;
	/// Register new Palette / returns a constant pointer to the registered version
	CPalettes *			RegisterPalette			(CPalettes * pItem, JMap<JInt32,const IItem *> &ImpMap) ;
	/// Register new Critere Induite / returns a constant pointer to the registered version
	CQuestionInduite *	RegisterQuestionInduite	(CQuestionInduite * pItem, JMap<JInt32,const IItem *> &ImpMap) ;
	/// Register new Critere Modifié / returns a constant pointer to the registered version
	CCritereModifie *	RegisterCritereModifie	(CCritereModifie * pItem, JMap<JInt32,const IItem *> &ImpMap) ;
	/// Register new Critere Quantile / returns a constant pointer to the registered version
	CCritereQuantile *	RegisterCritereQuantile (CCritereQuantile * pItem, JMap<JInt32,const IItem *> &ImpMap) ;

	/// Register new Ponderate Item / returns a constant pointer to the registered version
	CItemPondere *		RegisterItemPondere		(CItemPondere * pItem, JMap<JInt32,const IItem *> &ImpMap) ;

	/// Loads the Items construit
	virtual JBool Load(const char *Name) = 0;

	/// Creates the Item Ensemble
	JVoid CreateEnsemble();
	/// Loads the file header
	JVoid LoadFileHeader		(JArchive *pFile);
	/// Saves the file header
	JVoid SaveFileHeader		(JArchive *pFile);

	/// Load the Items
	JVoid LoadItems				(JArchive *pFile, JInt32 Version);
	/// Saves the Items
	JVoid SaveItems				(JArchive *pFile) ;//const;

	/// Loads List of Items independant
	JVoid LoadItemsIndependant	(JArchive *pFile, JInt32 Version);
	/// Saves List of Items independant
	JVoid SaveItemsIndependant	(JArchive *pFile) const;

	/// Loads the palettes
	JVoid LoadPalettess			(JArchive *pFile, JInt32 Version);
	/// Saves the palettes
	JVoid SavePalettess			(JArchive *pFile) const;

	/// Loads Critere Induites
	JVoid LoadQuestionInds		(JArchive *pFile, JInt32 Version);
	/// Saves Critere Induites
	JVoid SaveQuestionInds		(JArchive *pFile) const;

	/// Loads Critères modifiés
	JVoid LoadCritereMods		(JArchive *pFile, JInt32 Version);
	/// Saves Critères modifiés
	JVoid SaveCritereMods		(JArchive *pFile) const;

	/// Loads Critères de quantiles
	JVoid LoadCritereQuants		(JArchive *pFile, JInt32 Version);
	/// Saves Critères de quantiles
	JVoid SaveCritereQuants		(JArchive *pFile) const;

	/// Loads the Critères de scoring
	JVoid LoadCritereScores		(JArchive *pFile, JInt32 Version);
	/// Saves the Critères de scoring
	JVoid SaveCritereScores		(JArchive *pFile) const;

	/// Loads the Ponderate Items
	JVoid LoadItemsPonderes		(JArchive *pFile, JInt32 Version);
	/// Saves the Ponderate Items
	JVoid SaveItemsPonderes		(JArchive *pFile) const;

	/// Get Upwards
	JVoid GetUpwards(const JMap<JInt32, JInt32> & MapItems, JInt32 ID, JMap<JInt32, JLabel> & MapUpwards);

	/// Lecture Label via stream
	CString ReadStreamLabel(JStream &stream, JLabel &Type);

protected:

	/// File Name
	JStringEx				m_sName;

	/// File Name for proper file functions
	JStringEx				m_FileName;

	/// Time that the current file was loaded
	CTime					m_ThisTime;

	/// File Header
	CFileHeader				m_FileHeader;

	/// Dirty Flag, true if data needs saving
	JBool					m_bDirty;

	/// Loaded Flag, true if data loaded
	JBool					m_bLoaded;


private:

	/// File Name
	// JStringEx				m_sName;

	/// File Header
	// CFileHeader				m_FileHeader;

	/// Current item count
	JInt32					m_MaxItem;
	JInt32					m_tMaxItem;
	/// Items construit map
	ITEMCONSTRUITMAP		m_ItemsConstruits;

	/// List of items independant
	JList<JInt32>			m_ItemsIndependant;

	/// Current palette count
	JInt32					m_MaxPalettes;
	JInt32					m_tMaxPalettes;
	/// Palette map
	PALETTEMAP				m_Palettes;

	/// Current Critere induite count
	JInt32					m_MaxQuestionInd;
	JInt32					m_tMaxQuestionInd;
	/// Critere Induite map
	QUESTIONINDMAP			m_QuestionInds;

	/// Current Critere modifié count
	JInt32					m_MaxCritereMod;
	JInt32					m_tMaxCritereMod;
	/// Critere Modifie map
	CRITEREMODMAP			m_CritereMods;

	/// Current Critere Quantile count
	JInt32					m_MaxCritereQuant;
	JInt32					m_tMaxCritereQuant;
	/// Critere Quantile map
	CRITEREQUANTMAP			m_CritereQuants;

	/// Current Critere Score count
	JInt32					m_MaxCritereScore;
	JInt32					m_tMaxCritereScore;
	/// Critere Score map
	CRITERESCOREMAP			m_CritereScores;

	/// Current Ponderate Item Score count
	JInt32					m_MaxItemPondere;
	JInt32					m_tMaxItemPondere;
	/// Ponderate Item Score map
	ITEMPONDERMAP			m_ItemsPonderes;

	/// Dirty Flag, true if data needs saving
	// JBool					m_bDirty;

	/// Loaded Flag, true if data loaded
	// JBool					m_bLoaded;

	/// File Name for proper file functions
	// JStringEx				m_FileName;

	/// Time that the current file was loaded
	// CTime					m_ThisTime;

private:
	/// assignment operator
	CItemsConstruitsBase & operator =(const CItemsConstruitsBase & rhs);

	/// copy constructor
	CItemsConstruitsBase(const CItemsConstruitsBase & rhs);
};


#include <iterator>

/// Get a vector of Items Independant as IItems
template<class C1, class C2>
bool CItemsConstruitsBase::GetItemsIndependant	(C1 & vItems, const C2 & terrains) const
{
	// C1 : container<const IItem*>
	// C2 : container<CTerrain*>

	// on nettoie le container dans lequel on insère les items indépendants correspondants aux terrains
	vItems.clear();

	// on crée un itérateur d'insertion
	std::insert_iterator<C1> pos(vItems, vItems.end());

	for (m_ItemsIndependant.MoveFirst(); m_ItemsIndependant.IsValid(); m_ItemsIndependant.MoveNext())
	{
		m_ItemsConstruits.MoveTo(m_ItemsIndependant.GetItem());
		if(m_ItemsConstruits.IsValid())
		{
			CItemConstruit * pItem = m_ItemsConstruits.GetItem().get();
			bool bAdd = true;

			// on itère sur tous les terrains passés en paramêtre
			C2::const_iterator ter_it = terrains.begin();		
			while(ter_it != terrains.end())
			{
				if(! pItem->IsValid(*ter_it))
				{
					// on sort dès que l'on en a trouvé un
					bAdd = false;
					break;
				}
				++ter_it;
			}
			// on insère l'item
			if(bAdd)
				*pos = pItem->AsIItem();
		}
		else
			return false;
	}
	return true;
}

/// Get a vector of Palettes
template<class C1, class C2>
bool CItemsConstruitsBase::GetPalettes	(C1 & vItems, const C2 & terrains) const
{
	// on nettoie le container dans lequel on insère les items indépendants correspondants aux terrains
	vItems.clear();

	// on crée un itérateur d'insertion
	std::insert_iterator<C1> pos(vItems, vItems.end());

	for (m_Palettes.MoveFirst(); m_Palettes.IsValid(); m_Palettes.MoveNext())
	{
		CPalettes *pItem = m_Palettes.GetItem().get();
		if(pItem->IsRegistered())
		{
			bool bAdd = true;

			// on itère sur tous les terrains passés en paramêtre
			C2::const_iterator ter_it = terrains.begin();		
			while(ter_it != terrains.end())
			{
				if(! pItem->IsValid(*ter_it))
				{
					// on sort dès que l'on en a trouvé un
					bAdd = false;
					break;
				}
				++ter_it;
			}
			if(bAdd)
				*pos = pItem->AsIItem();
		}
	}
	return true;
}