// STDBOX.CPP
// Bo�tes de dialogue standard JFC
#include "stdafx.h"

void AfficheMessage(LPCSTR * msg)
{
	MessageBox(msg,"Message",MB_ICONINFORMATION);
}
