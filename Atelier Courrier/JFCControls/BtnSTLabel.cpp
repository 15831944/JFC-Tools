// BtnSTLabel.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "BtnSTLabel.h"
#include ".\btnstlabel.h"


// CBtnSTLabel

IMPLEMENT_DYNAMIC(CBtnSTLabel, CButton)
CBtnSTLabel::CBtnSTLabel()
{
}

CBtnSTLabel::~CBtnSTLabel()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CBtnSTLabel, CButton)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void CBtnSTLabel::UpdateSurface()
{
	CRect (rc);
	GetWindowRect(rc);
	RedrawWindow();

	GetParent()->ScreenToClient(rc);
	GetParent()->InvalidateRect(rc,true);
	GetParent()->UpdateWindow();
}


void CBtnSTLabel::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}

CBtnSTLabel& CBtnSTLabel::SetFontBold(BOOL bBold)
{	

	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	UpdateSurface();
	return *this;
}




CBtnSTLabel& CBtnSTLabel::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;
}


CBtnSTLabel& CBtnSTLabel::SetFontItalic(BOOL bSet)
{

	m_lf.lfItalic = bSet;
	ReconstructFont();
	UpdateSurface();

	return *this;	
}





CBtnSTLabel& CBtnSTLabel::SetFontSize(int nSize)
{

	CFont cf;
	LOGFONT lf;

	cf.CreatePointFont(nSize * 10, m_lf.lfFaceName);
	cf.GetLogFont(&lf);

	m_lf.lfHeight = lf.lfHeight;
	m_lf.lfWidth  = lf.lfWidth;

//	nSize*=-1;
//	m_lf.lfHeight = nSize;
	ReconstructFont();
	UpdateSurface();

	return *this;
}

CBtnSTLabel& CBtnSTLabel::SetFontName(const CString& strFont, BYTE byCharSet /* Default = ANSI_CHARSET */)
{	

	m_lf.lfCharSet = byCharSet;

	strcpy_s(m_lf.lfFaceName, 32, strFont);
	ReconstructFont();
	UpdateSurface();

	return *this;
}

/*
CBtnSTLabel& CBtnSTLabel::SetFont3D(BOOL bSet,Type3D type)
{

	m_bFont3d = bSet;
	m_3dType = type;
	UpdateSurface();


	return *this;
}

*/




void CBtnSTLabel::PreSubclassWindow() 
{

	CButton::PreSubclassWindow();

	CFont* cf = GetFont();
	if(cf !=NULL)
	{
		cf->GetObject(sizeof(m_lf),&m_lf);
	}
	else
	{
		GetObject(GetStockObject(SYSTEM_FONT),sizeof(m_lf),&m_lf);
	}

	ReconstructFont();
	
}


BOOL CBtnSTLabel::PreCreateWindow(CREATESTRUCT& cs) 
{	
	return CButton::PreCreateWindow(cs);
}
/*
CBtnSTLabel& CBtnSTLabel::SetFont(LOGFONT lf)
{
	CopyMemory(&m_lf, &lf, sizeof(m_lf));
	ReconstructFont();
	UpdateSurface();
	return *this;

}
*/

