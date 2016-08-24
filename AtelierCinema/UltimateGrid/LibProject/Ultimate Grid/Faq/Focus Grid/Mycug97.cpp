/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived MyCug 97

#include "stdafx.h"
#include "resource.h"
#include "MyCug97.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyCug,CUGCtrl)
	//{{AFX_MSG_MAP(MyCug)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// MyCug construction/destruction
MyCug::MyCug()
{
	m_bDroplistStarted = FALSE;
	m_lostFocusFlag = 0;
}

MyCug::~MyCug()
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void MyCug::OnSetup()
{
	// *Note: inorder to make this work properly we had to make some changed to 
	// the standard edit control and the drop list celltype.  You might have to
	// do similar changes if you are using a custom edit control or a celltype
	// that show a popup window just like the droplist.  We have implemented
	// these changes with new droplist and edit control classes.
	// Additional functionality is provided through :
	// CUGLstBox2::OnKillFocus, CUGEdit2::OnKillFocus

	// create and initialize the updated edit control
	m_newEditControl.Create( WS_CHILD, CRect(0,0,0,0), this, 245334 );
	m_newEditControl.m_ctrl = this;
	SetNewEditClass( &m_newEditControl );
	// create and add the new drop list celltype
	m_newDLIndex = AddCellType( &m_newDLType );

	int cols = 4,
		rows = 40;

	SetDefColWidth( 70 );

	SetNumberCols ( cols );
	SetNumberRows ( rows );
	// set a droplist celltype to one of the cells
	QuickSetCellType( 1, 1, m_newDLIndex );
	QuickSetLabelText( 1, 1, "1\n2\n3\n" );

	SetMultiSelectMode( TRUE );
	SetCancelMode( FALSE );
	SetCurrentCellMode( 2 );
	SetHighlightRow( TRUE );

	// fill cells with data
	CUGCell cell;
	GetCell(0,1,&cell);
	for (int xIndex = 0; xIndex < cols; xIndex++) 
	{
		for (int yIndex = 0; yIndex < rows; yIndex++) 
		{
			if ( xIndex != 1 || yIndex != 1 )
			{
				cell.SetNumberDecimals( 0 );
				cell.SetNumber( rand()%100 );
				SetCell(xIndex,yIndex,&cell);
			}
		}
	}
}
	
/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		This message is sent everytime the grid needs to
//		draw a cell in the grid. At this point the cell
//		class has been filled with the information to be
//		used to draw the cell. The information can now be
//		changed before it is used for drawing
void MyCug::OnGetCell(int col,long row,CUGCell *cell)
{
	if ( col == -1 && row >= 0 )
	{	// side heading setup
		if ( m_lostFocusFlag <= 1 )
			cell->SetBackColor( RGB(180,180,180));
		else
			cell->SetBackColor( RGB(195,195,195));
		cell->SetNumberDecimals ( 0 );
		cell->SetNumber ( row );
	}
	else if ( row == -1 && col >= 0 )
	{	// top heading setup
		if ( m_lostFocusFlag <= 1 )
			cell->SetBackColor( RGB(180,180,180));
		else
			cell->SetBackColor( RGB(195,195,195));
		cell->SetNumberDecimals ( 0 );
		cell->SetNumber ( col );
	}
	else if ( col < 0 && row < 0 )
	{	// corner button setup
		if ( m_lostFocusFlag <= 1 )
			cell->SetBackColor( RGB(180,180,180));
		else
			cell->SetBackColor( RGB(195,195,195));
	}
	else
	{
		if ( m_lostFocusFlag == 0 )
		{
			cell->SetHBackColor( cell->GetBackColor());
			cell->SetHTextColor( cell->GetTextColor());
		}
		else if ( m_lostFocusFlag == 1 )
			cell->SetHBackColor( RGB(195,195,195));
		else if ( m_lostFocusFlag == 2 )
			cell->SetHBackColor( RGB(0,0,200));
	}
}

/////////////////////////////////////////////////////////////////////////////
//	OnDClicked
void MyCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}

/////////////////////////////////////////////////////////////////////////////
//	OnCellTypeNotify
//		This message is sent from a cell type , message
//		depends on the cell type - check the information
//		on the cell type classes
//		- The ID of the cell type is given
int MyCug::OnCellTypeNotify(long ID,int col,long row,long msg,long param)
{
	if ( msg == UGCT_DROPLISTSTART )
	{
		m_bDroplistStarted = TRUE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetDefBackColor
COLORREF MyCug::OnGetDefBackColor(int section)
{
	return GetSysColor ( COLOR_APPWORKSPACE );
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetFocus
void MyCug::OnSetFocus(int section)
{
	m_lostFocusFlag = 2;
	m_CUGTopHdg->Update();
	m_CUGSideHdg->Update();
	m_CUGCnrBtn->Update();
}

/////////////////////////////////////////////////////////////////////////////
//	OnKillFocus
void MyCug::OnKillFocus(int section)
{
	if ( !m_editInProgress && !m_bDroplistStarted )
	{
		// instruct the OnGetCell notification that the grid is loosing focus
		m_lostFocusFlag = 1;
		// redraw the grid
		AdjustComponentSizes();
	}
	m_bDroplistStarted = FALSE;
}

