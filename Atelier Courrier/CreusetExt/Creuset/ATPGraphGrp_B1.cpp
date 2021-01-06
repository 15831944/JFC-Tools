// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JFCGraduation.h"
#include "ATPGraphGRP_B1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphGRP_B1::CATPGraphGRP_B1()
{
}

/////////////////
// le destructeur

CATPGraphGRP_B1::~CATPGraphGRP_B1()
{
	// on ne fait rien
}

void CATPGraphGRP_B1::CalcValues(const JITPDATHypoFormat & DATHypoFormat, int i, JFlt64 &Value)
{
	if (DATHypoFormat.m_Effectif > 0.0)
		Value = (100.0 * DATHypoFormat.GetDATHypoFormatActif().m_NbContactsJour.Item(i)) / DATHypoFormat.m_Effectif;
}

