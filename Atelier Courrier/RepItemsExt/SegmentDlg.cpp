// SegmentDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\segmentdlg.h"
#include "Segment.h"
#include "JSRCTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CSegmentDlg dialog

IMPLEMENT_DYNAMIC(CSegmentDlg, CDialog)
CSegmentDlg::CSegmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentDlg::IDD, pParent)
{
	m_pTerrain = 0;
	m_Id_PopSegments.Reset();
	m_PopulationSeg.SetSize(0);
	m_ContingentMask = 0;
	m_CATPColorsApp = CATPColors::COLORREPITEMS;
}

CSegmentDlg::~CSegmentDlg()
{
}

void CSegmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSegmentDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnOk)
END_MESSAGE_MAP()


bool CSegmentDlg::SetData(CTerrain * pTerrain, JVector<JInt32> & IdSegments)
{
	m_pTerrain = pTerrain;
	m_Id_PopSegments.Reset();
	m_Id_PopSegments.SetCount(IdSegments.GetCount());
	for(IdSegments.MoveFirst(),m_Id_PopSegments.MoveFirst(); IdSegments.IsValid(); IdSegments.MoveNext(), m_Id_PopSegments.MoveNext())
	{
		m_Id_PopSegments.GetItem() = IdSegments.GetItem ();
	}
	return true;
}

// CSegmentDlg message handlers
BOOL CSegmentDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	m_Button_OK.SubclassDlgItem(IDOK, this);
	m_Button_Cancel.SubclassDlgItem(IDCANCEL , this);
	m_Label.SubclassDlgItem(IDC_RI_LABEL, this);
	m_Box.SubclassDlgItem(IDC_RI_STATIC_BOX, this);
	m_ListSegments.SubclassDlgItem(IDC_RI_LIST_SEGMENT, this);

	m_ListSegments.SetSelectColor(CATPColors::GetColorSelect(m_CATPColorsApp));

	m_Label.SetFontName(CATPColors::GetFontLabelName());
	m_Label.SetFontSize(CATPColors::GetFontLabelSize());
	m_Label.SetBkColor(CATPColors::GetColorDark(m_CATPColorsApp),m_CATPColorsApp, CATPColors::DARK_DEGRAD);
	m_Label.SetBorder(false);
	m_Label.SetTextColor(CATPColors::GetColorSuperDark(m_CATPColorsApp)); 

	m_Box.SetBkColor(CATPColors::GetColorMedium(m_CATPColorsApp),CATPColors::COLORNOAPP, CATPColors::MEDIUM_DEGRAD);
	m_Box.SetTransparent(false);
	m_Box.SetSunken(true);
	m_Box.AddTopWindow(m_Label);
	m_Box.AddTopWindow(m_ListSegments);

	if(!m_pTerrain)
	{
		this->EndDialog(false);
		return false;
	}

	JVector<CSegment*> Segments;
	if(m_pTerrain->GetSegments(Segments))
	{
		const JSRCTables & srcTables = JSRCTables::GetInstance(); 
		m_ListSegments.ResetContent ();
		// Update the segment list
		for(Segments.MoveFirst(); Segments.IsValid(); Segments.MoveNext())
		{
			const JSegment* Seg = srcTables.m_TBLSegments.GetItem(Segments.GetItem()->GetID());
			m_ListSegments.AddString(Seg->m_Libelle.AsJCharPtr());
		}
		// Set the selected segments
		if(m_Id_PopSegments.GetCount())
		{
			for(m_Id_PopSegments.MoveFirst(); m_Id_PopSegments.IsValid(); m_Id_PopSegments.MoveNext())
			{
				int x = 0;
				for(x =0, Segments.MoveFirst() ; Segments.IsValid(); Segments.MoveNext(),x++)
				{
					if(Segments.GetItem()->GetID()==m_Id_PopSegments.GetItem ())
						m_ListSegments.SetSel(x,true);
				}
			}
		}
	}

	return 0;
}

void CSegmentDlg::OnOk()
{
	int Count = m_ListSegments.GetSelCount(); 
	if(Count == LB_ERR)
		return;

	int Items[50];
	m_ListSegments.GetSelItems(50,Items);

	JVector<CSegment*> Segments;
	m_pTerrain->GetSegments(Segments);

	m_ContingentMask = 0;
	m_Id_PopSegments.Reset();
	m_Id_PopSegments.SetCount(Count);
	m_PopulationSeg.SetSize(0);
	for(int x=0; x<Count ; x++)
	{
		Segments.MoveTo(Items[x]);
		m_ContingentMask |= Segments.GetItem()->GetContingentMask(); 
		m_Id_PopSegments.MoveTo(x);
		m_Id_PopSegments.GetItem() = Segments.GetItem()->GetID();
		if(x)
			m_PopulationSeg |= Segments.GetItem ()->GetPopSegment();
		else
			m_PopulationSeg = Segments.GetItem ()->GetPopSegment();
	}
	OnOK();
}
