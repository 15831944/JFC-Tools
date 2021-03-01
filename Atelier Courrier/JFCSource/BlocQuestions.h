
#pragma once

#pragma warning( disable : 4786 )

#include <memory>

#include "ITheme.h"
#include "IQuestion.h"
// #include "Terrain.h"

class CTerrain;

/**
 * \ingroup JFCSOURCE
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
class AFX_EXT_CLASS CBlocQuestions : public ITheme, public IQuestion
{
public:
	CBlocQuestions();
	virtual ~CBlocQuestions();

	JArchive &		Recv		(JArchive &lhs, JInt32 Version);
	JBool			FillTree	(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM hParent, JBool AllLeaves = false, JBool OnlyQuanti = false);
	void			SetID		(JInt32 ID) { m_ID = ID; }
	void			SetLabel	(JLabel & Label) { m_Label = Label; }

	// Complete chacune des dernieres branches thèmes avec les questions correspondantes
	void			ToggleItemState			(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM &hti);
	void			UpdateQuestionTreeQuanti(CTerrain *pTerrain, CTreeCtrl &Tree, HTREEITEM &hti, JBool OnlyQuanti = true);
	void			ToggleChildItemState	(CTreeCtrl &Tree, HTREEITEM hti, int iImage);
	void			ToggleParentItemState	(CTreeCtrl &Tree, HTREEITEM hti, int iImage);
	bool			HasInactifChildren		(CTreeCtrl &Tree, HTREEITEM hti);
	bool			HasActifChildren		(CTreeCtrl &Tree, HTREEITEM hti);

public:
	/// ITheme interface & IQuestion interface overrides

	/// Gets the label
	virtual const JLabel &	GetLabel		() const { return m_Label; };
	/// Gets the id
	virtual JInt32			GetID			() const { return m_ID; };
	/// Return true if the object is a question bloc
	virtual JBool			IsBlocQuestion	() const { return true; } ;

	/// Returns the objects Bloc ID
	virtual JInt32			GetBlocID		() const { return m_ID; };
	/// Returns the number of dimensions
	virtual JInt32 			HasDimensions	() const {return 0; };
	/// Returns the dimension type
	virtual JInt32			DimensionType(JInt32 Dim = 0) const { return IQuestion::NODIM; } ; 
	/// Gets the Modalites
	virtual JBool			GetIModalites(JVector<const IModalite *> &Vector, JInt32 ModaliteID=0, JInt32 Dim=0, JUnt32 ContingentMask=0)	const {return false; } ;
	/// Returns true if the Terme is valid
	virtual JBool			IsValidTerme(const JVector<JInt32> & IDTerme)		const {return false; } ;
	/// Transforms the object into a string form
	virtual JBool			ToItemString(JStringEx &String)						const {return false; } ;
	/// Returns true if this object can be ordered
	virtual JBool			CanModify		() const {return false; } ;
	/// Returns True if the modalites have been reordered
	virtual JBool			SetModaliteOrder(const JList<JInt32> & IDOrder) {return false; } ;
	/// Rename an Item returns false if name is invalid
	virtual JBool			RenameItem(JLabel &Label, JInt32 ItemID) {return false; };
	/// Returns True if this object is in the contingent
	virtual JBool			IsInContingent(JUnt32 ContingentMask) const {return true; };

private:

	JInt32				m_ID;
	JLabel				m_Label;

private:
	/// assignment operator
	CBlocQuestions & operator =(const CBlocQuestions & rhs);

	/// copy constructor
	CBlocQuestions(const CBlocQuestions & rhs);
};

typedef std::auto_ptr<CBlocQuestions> PTR_BLOCQUESTIONS;
typedef JVector<PTR_BLOCQUESTIONS> BLOCVECTOR;

