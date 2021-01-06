// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPPupitre_A6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A6::CATPPupitre_A6()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPPupitre_A6::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPPupitre_A6::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A6::~CATPPupitre_A6()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A6, JFCButtonList)
	//{{AFX_MSG_MAP(CATPPupitre_A6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// la fonction pour r�cup�rer l'�tat des �l�ments
void CATPPupitre_A6::OnGetItemParams(LONG i, LONG j, BtnParams & Params)
{
	// on r�cup�re l'�tat de l'�l�ment
	Params.m_State = 0;

	// Indice format (tiens compte position 1er elmt format via ascenseur)
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;

	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j ; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// R�cup Table des formats
	const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
	
	srcpresse.m_TBLFormats.MoveTo(m_pATPDocCreuset->m_MapIdFormatsSel.GetKey()); // R�cup item correspondant � l'indice
	if(srcpresse.m_TBLFormats.IsValid())
	{
		IElem *pElem = srcpresse.m_TBLFormats.GetItem();
		if(pElem)
			Params.m_Text = pElem->GetLabel().AsJCharPtr();  
	}

	// V�rifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
			Params.m_State = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		else
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				Params.m_State = BUT_DOWN;
	}

	if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		Params.m_Style = (JFCButtonList::BUT_MULTI_SEL|JFCButtonList::LED_SEL);
	else
		Params.m_Style = (JFCButtonList::BUT_SINGLE_SEL);

	// Multis�lection avec led forme ronde
	//Params.m_Style			=	(JFCButtonList::BUT_MULTI_SEL|JFCButtonList::LED_SEL);
	Params.m_Color_BtnDown	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Color_BtnHover	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_BtnUp	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	//Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	//Params.m_Color_LedOn	=	CATPColors::GetColorDark(CATPColors::COLORCREUSET);
	Params.m_Color_LedBorder=	CATPColors::GetColorSelect(CATPColors::COLORCREUSET);
	Params.m_Color_LedOff	=	CATPColors::GetColorMedium(CATPColors::COLORCREUSET);
	Params.m_Color_LedOn	=	CATPColors::GetColorLight(CATPColors::COLORCREUSET);
	Params.m_Design_Led     =   4;
	Params.m_Color_Text		=	CATPColors::GetColorBlack();
}

void CATPPupitre_A6::OnButtonSel(LONG i, LONG j)
{
	// Rappel s�lection
	// Etat 0  : non s�lectionn�
	// Etat 1  : rien
	// Etat 2  : affichage uniquement
	// Etat 3  : mode insertion et affich�

	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();
	// V�rifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 key = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
		//JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();


		if(m_pATPDocCreuset->m_AFFCreuset.GetPlanActif())
		{
			//On d�selectionne le format actif
			int oldEtat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();

			for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
				m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
				m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()==3)
					m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
			}
			//On selectionne le nouveau
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(key);
			if(oldEtat != 3)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 3;
		}
		else
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
			else
				//On rend le format visible
				m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 2;
		}
        m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
	}

	
	
/*	// Rappel s�lection
	// Etat 0  : non s�lectionn�
	// Etat 1  : s�lectionn� pour mode insertion
	// Etat 2  : affichage uniquement
	// Etat 3  : mode insertion et affich�
	

	// Mode s�lection (multi s�lection si touche CONTROL ou SHIFT enfonc�)
	short StateControl = GetKeyState(VK_CONTROL);
	short StateShift   = GetKeyState(VK_SHIFT);
	short State		   = 1;	
	if (StateControl < 0 || StateShift < 0) 
		State = -1;

	// Voir si d�j� un format s�lectionn�
	JInt32 NbFormatActif     = 0;
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if(Etat == 3 || Etat == 1)
		{
			// Ident format en cours de s�lection
			NbFormatActif ++; 
		}
	}


	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// V�rifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		switch(Etat)
		{
			case 0:
				if (NbFormatActif == 0 || State < 0)
				{
					// Si aucun format actif ou mode multi-s�lection
					Etat = 3; 
				}
				if (NbFormatActif > 0 && State >= 0)
				// d�selectionne tous les autres
				{
					Etat = 3;
					JUnt32 ifFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
					for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
					m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
					m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
					{
						if (m_pATPDocCreuset->m_MapIdFormatsSel.GetKey() != ifFormat)
						{
							if (m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 2 || m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 3)
								m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
						}
					}

				}
				break;

			case 1:
				Etat = 0; break;

			case 2:
				if (NbFormatActif == 0 || State < 0)
				{
					// Si aucun format actif ou mode multi-s�lection
					Etat = 3; 
				}
				if (NbFormatActif > 0 && State >= 0)
				// d�selectionne tous les autres
				{
					Etat = 3;
					JUnt32 ifFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
					for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
					m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
					m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
					{
						if (m_pATPDocCreuset->m_MapIdFormatsSel.GetKey() != ifFormat)
						{
							if (m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 2 || m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() == 3)
								m_pATPDocCreuset->m_MapIdFormatsSel.GetItem() = 0;
						}
					}

				}
				break;

			case 3:
				Etat = 0; break;
				//Etat = 2; break;
		}
	}

	// Mise � jour du bouton Tous // Voir nb formats d�j� s�lectionn�s
	NbFormatActif     = 0;
	for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if(Etat & 0x0001)
		{
			// Ident format en cours de s�lection
			NbFormatActif ++; 
		}
	}
	
	// Rafraichissement des formats
	if(m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()>1)
        m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
*/	
}

void CATPPupitre_A6::OnLedSel(LONG i, LONG j)
{
	// Indice format
	j = j + m_pATPDocCreuset->m_DecalAffichFormat;
	// Test si indice Ok
	if (j >= m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		j = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount()-1;

	m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	for(JInt32 inc = 0; inc < j ; inc++)
		m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext();

	// V�rifie si le format est valide
	if(m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		switch(Etat)
		{
			case 0:
				Etat = 2; break;
			//case 1:
			//	Etat = 3; break;
			case 2:
				Etat = 0; break;
			//case 3:
			//	Etat = 0; break;
		}
	}

	//// Mise � jour du bouton Tous // Voir nb formats d�j� allum�s
	//JUnt32 NbFormatLedAllume     = 0;
	//for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
	//	m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	//{
	//	JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
	//	if(Etat & 0x0002)
	//	{
	//		// Ident format en cours de s�lection
	//		NbFormatLedAllume ++; 
	//	}
	//}


	// Raffraichissement fen�tre format	
	m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
}


bool CATPPupitre_A6::OnBeginDrawing(CDC & dc)
{
	if(m_pATPDocCreuset && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount())
		return true;
	return false;
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPPupitre_A6::OnUpdate()
{

	// Si aucun format s�lectionn�, on s�lectionne le 1er
/*	JBool AucunFormatSel = true;
	for (m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
		 m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
		 m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
	{
		// r�cup�re �tat du format
		JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
		if (Etat & 0x0001)
		{
			// format actif
			AucunFormatSel = false;
			break;
		}
	}

	if (AucunFormatSel && m_pATPDocCreuset->m_MapIdFormatsSel.GetCount() > 0)
	{
		// On se positionne sur le 1er format de l'offre
		m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveFirst();
		
		// R�cup id format
		JUnt32 Id = m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetItem();
		if(Id)
		{
			// Positionnement dans le map des formats
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(Id);

			// On le s�lectionne
			JUnt32 & Etat = m_pATPDocCreuset->m_MapIdFormatsSel.GetItem();
			Etat = 3;

			// Rafraichissement des formats
			m_pATPDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
		}
	}
*/
	// Nombre d'�l�ment � afficher selon position ascenseur
	JUnt32 NbElmtAff = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
	NbElmtAff -= m_pATPDocCreuset->m_DecalAffichFormat;

	// Initialisation item range
	if(m_pATPDocCreuset)
	{
		if(NbElmtAff)
			SetVertItemRange(0,NbElmtAff-1,true);
		else
			SetVertItemRange(-1,-1,true);
	}
	SetIcons(IDI_CR_LEDON, IDI_CR_LEDNEUTRE);
	
	// on redessine la fen�tre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A6 message handlers

void CATPPupitre_A6::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	JFCButtonList::PreSubclassWindow();

	// on initialise la taille des cellules
	SetHorzItemSize(133,2,2, FALSE);
	SetVertItemSize(24,2,2, FALSE);
	//SetDefaultColors(CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::GetColorBlack());   
}
