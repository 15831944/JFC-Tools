/***************************************************
Ultimate Grid 3.0 for MFC
Dundas Software Ltd. 1994-1996

	Cell Type Class
****************************************************/

#include "stdafx.h"
#include "UGCtrl.h"
#include "UGDLType2.h"

/***************************************************
****************************************************/
CUGDropListType2::CUGDropListType2()
{
	if ( m_listBox != NULL )
		// the list box has already been created in the parent
		delete m_listBox;

	m_listBox = new CUGLstBox2;
}

CUGDropListType2::~CUGDropListType2()
{
}