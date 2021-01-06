#include "StdAfx.h"
#include "serialisepalettebuilder.h"

#include "IBQuestion.h"
#include "IBItem.h"
#include "IBModalite.h"
#include "IModalite.h"
#include "IBClass.h"
#include "IBloc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////
// Constructeur
CSerialisePaletteBuilder::CSerialisePaletteBuilder(void)
{
}

///////////////////////////////////////////////////////////////////////
// Destructeur
CSerialisePaletteBuilder::~CSerialisePaletteBuilder(void)
{
}

///////////////////////////////////////////////////////////////////////
// Stockage des composants palette pupitre
JVoid CSerialisePaletteBuilder::Send(JStream & Stream, JMap<int, CPaletteBuilder> * pMapPaletteBuilder, const JMap<JInt32,JBool> &m_SelUserItems, CString &DefaultPaletteXLabel)
{
	// Sauvegarde nb �l�ment map palette builder
	Stream.Send(pMapPaletteBuilder->GetCount()); 

	// Boucle sur les �lmts du map palette builder
	for (pMapPaletteBuilder->MoveFirst(); pMapPaletteBuilder->IsValid(); pMapPaletteBuilder->MoveNext())
	{
		// R�cup�re et sauve la clef
		JInt32 ClefPaletteBuilder = pMapPaletteBuilder->GetKey(); 
		Stream.Send(ClefPaletteBuilder);

		// R�cup�re �lmt CPaletteBuilder
		CPaletteBuilder &PaletteBuilder = pMapPaletteBuilder->GetItem();

        // Sauvegarde CIBQuestion 	
		Send_CIBQuestion	(Stream, PaletteBuilder);

		// Sauvegarde CIBItem
		Send_CIBItem		(Stream, PaletteBuilder);

		// Sauvegarde Map_pIBModalite
		Send_MapIBModalite	(Stream, PaletteBuilder);

		// Sauvegarde Map_Dim_Type
		Send_MapDimType		(Stream, PaletteBuilder);

		// Sauvegarde Map_pIModalite
		Send_MapIModalite	(Stream, PaletteBuilder);

		// Sauvegarde Liste des valeurs num�riques List_NumValue
		Send_MapListNumValue(Stream, PaletteBuilder);

		// Sauvegarde List_DimSel
		Send_ListDimSel		(Stream, PaletteBuilder);

		// Sauvegarde List_DimMode
		Send_ListDimMode	(Stream, PaletteBuilder);

		// Sauvegarde des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
		Send_Libelles		(Stream, PaletteBuilder);
	}

	// Sauvegarde de la s�lection des Users Items
	Send_SelUserItems	(Stream, m_SelUserItems);

	// Sauvegarde mode op�ration entre les �lmts du Sas
	JString ModeOpe;
	ModeOpe = DefaultPaletteXLabel;
	ModeOpe.Send(Stream); 
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde CIBQuestion 	
JVoid CSerialisePaletteBuilder::Send_CIBQuestion(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Liste de valeurs
	JList<JFlt32>	ValuesFlt32;
	JList<JInt32>	ValuesInt32;

	// Y-a t-il des questions
	JBool QuestionExist = (PaletteBuilder.m_pIBquestion != 0);
	Stream.Send(QuestionExist);
	
	// Sauvegarde des infos questions si valide
	if (QuestionExist)
	{
		// Sauve Ident Bloc Question
		Stream.Send(PaletteBuilder.m_pIBquestion->GetID());   

		// Sauve Liste Blocs CIBModalite  >> JList <CIBModalite *>
		JList <CIBModalite *> Lst_IBModalite;
		PaletteBuilder.m_pIBquestion->GetModalites(Lst_IBModalite); 

		// Nb elmt liste
		Stream.Send(Lst_IBModalite.GetCount()); 

		// Puis les �lmts de la liste
		for (Lst_IBModalite.MoveFirst(); Lst_IBModalite.IsValid(); Lst_IBModalite.MoveNext())
		{
			// R�cup liste CIBModalite
			CIBModalite *pIBModalite = Lst_IBModalite.GetItem();
			JBool IBModaliteExist = (pIBModalite != 0);
			Stream.Send(IBModaliteExist);
			
			// Si IBMOdalite existe, sauve les infos associ�es
			if (IBModaliteExist)
			{
				// Recup la liste
				pIBModalite->GetValues(ValuesInt32); 

				// Sauve nb elmt et les elmts liste valeur modalit�
				Stream.Send(ValuesInt32.GetCount());
				for (ValuesInt32.MoveFirst(); ValuesInt32.IsValid(); ValuesInt32.MoveNext())
				{
					JInt32 Valeur;
					Valeur = ValuesInt32.GetItem();
					Stream.Send(Valeur);
				}

				// Puis op�rateur du bloc
				JInt32 ValOpe = pIBModalite->GetOperator();
				Stream.Send(ValOpe);
			}
		}

		// Info Classe					>> CIBClass
		CIBClass * pClass = PaletteBuilder.m_pIBquestion->GetClass(); 
		bool ClassExist = (pClass != 0);
		Stream.Send(ClassExist);  

		// Info si pointeur
		if (pClass)
		{
			// Classe exist

			pClass->GetValues(ValuesFlt32); 

			// Sauve nb elmt valeurs
			Stream.Send(ValuesFlt32.GetCount());
			for (ValuesFlt32.MoveFirst(); ValuesFlt32.IsValid(); ValuesFlt32.MoveNext())
			{
				JFlt32 Valeur;
				Valeur = ValuesFlt32.GetItem();
				Stream.Send(Valeur); 
			}

			// Sauve liste des op�rateurs associ�s � chaque elmt de la classe
			JList<IBloc::OPERATOR>	Operators;
			JInt32 ValOpe;
			pClass->GetOperators(Operators);
			Stream.Send(Operators.GetCount());
			for (Operators.MoveFirst(); Operators.IsValid(); Operators.MoveNext())
			{
			
				ValOpe = Operators.GetItem();
				Stream.Send(ValOpe); 
			}

			// Sauve op�rateur g�n�ral
			IBloc::OPERATOR	Operator = pClass->GetOperator();
			ValOpe = Operator;
			Stream.Send(ValOpe);
		}
		
		// Info Quantile				>> CIBQuant
		CIBQuant *pQuant = PaletteBuilder.m_pIBquestion->GetQuant(); 
		bool QuantExist = (pQuant != 0);
		Stream.Send(QuantExist);  

		if (pQuant)
		{
			pQuant->GetValues(ValuesInt32); 

			// Sauve nb elmt valeurs
			Stream.Send(ValuesInt32.GetCount());
			for (ValuesInt32.MoveFirst(); ValuesInt32.IsValid(); ValuesInt32.MoveNext())
			{
				JInt32 Valeur;
				Valeur = ValuesInt32.GetItem();
				Stream.Send(Valeur); 
			}

			// Sauve op�rateur g�n�ral
			IBloc::OPERATOR	Operator = pQuant->GetOperator();
			JInt32 ValOpe = Operator;
			Stream.Send(ValOpe);
		}

		// Info Score					>> CIBScore
		CIBScore *pScore = PaletteBuilder.m_pIBquestion->GetScore(); 
		bool ScoreExist = (pScore != 0);
		Stream.Send(ScoreExist);  

		if (pScore)
		{
			pScore->GetValues(ValuesInt32); 
			// Sauve nb elmt valeurs
			Stream.Send(ValuesInt32.GetCount());
			for (ValuesInt32.MoveFirst(); ValuesInt32.IsValid(); ValuesInt32.MoveNext())
			{
				JInt32 Valeur;
				Valeur = ValuesInt32.GetItem();
				Stream.Send(Valeur); 
			}

			// Sauve op�rateur g�n�ral
			IBloc::OPERATOR	Operator = pScore->GetOperator();
			JInt32 ValOpe = Operator;
			Stream.Send(ValOpe);
		}

		// Info Valeurs num�riques		>> CIBNumValue
		CIBNumValue *pNumValue = PaletteBuilder.m_pIBquestion->GetNumValue();
		bool NumValueExist = (pNumValue != 0);
		Stream.Send(NumValueExist);  

		if (pNumValue)
		{
			// Sauve nb valeurs et les valeurs num�riques
			pNumValue->GetValues(ValuesFlt32); 
			Stream.Send(ValuesFlt32.GetCount());
			for (ValuesFlt32.MoveFirst(); ValuesFlt32.IsValid(); ValuesFlt32.MoveNext())
			{
				JFlt32 Valeur;
				Valeur = ValuesFlt32.GetItem();
				Stream.Send(Valeur); 
			}
			
			// Puis sauve op�rateur associ�
			IBloc::OPERATOR	Operator = pNumValue->GetOperator();
			JInt32 ValOpe = Operator;
			Stream.Send(ValOpe);
		}

		// Info Point					>> CIBPoint
		CIBPoint *pPoint = PaletteBuilder.m_pIBquestion->GetPoint(); 
		bool PointExist = (pPoint != 0);
		Stream.Send(PointExist);  

		if (pPoint)
		{
			// Sauve la valeur
			JInt32 ValPoint = pPoint->GetValue();
			Stream.Send(ValPoint);
			// Sauve l'op�rateur
			IBloc::OPERATOR	Operator = pPoint->GetOperator();
			JInt32 ValOpe = Operator;
			Stream.Send(ValOpe);
		}
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde CIBItem
JVoid CSerialisePaletteBuilder::Send_CIBItem(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	
	// Y-a t-il des items
	JBool ItemExist = (PaletteBuilder.m_pIBitem != 0);
	Stream.Send(ItemExist);
	
	// Sauvegarde des infos items si valide
	if (ItemExist)
	{
		// Sauve Ident Bloc Item
		Stream.Send(PaletteBuilder.m_pIBitem->GetID());

		// Les valeurs associ�es
		JList<JInt32>		ValuesInt32;
		PaletteBuilder.m_pIBitem->GetValues(ValuesInt32);

		// Sauve nb elmt valeurs
		Stream.Send(ValuesInt32.GetCount());

		// Sauve les valeurs
		for (ValuesInt32.MoveFirst(); ValuesInt32.IsValid(); ValuesInt32.MoveNext())
		{
			JInt32 Valeur;
			Valeur = ValuesInt32.GetItem();
			Stream.Send(Valeur); 
		}

		// Sauve op�rateur g�n�ral
		IBloc::OPERATOR	Operator = PaletteBuilder.m_pIBitem->GetOperator(); 
		JInt32 ValOpe = Operator;
		Stream.Send(ValOpe);

		// Sauve Info Point					>> CIBPoint
		CIBPoint *pPoint = PaletteBuilder.m_pIBitem->GetPoint(); 
		bool PointExist = (pPoint != 0);
		Stream.Send(PointExist);  

		if (pPoint)
		{
			// Sauve la valeur
			JInt32 ValPoint = pPoint->GetValue();
			Stream.Send(ValPoint);
			// Sauve l'op�rateur
			IBloc::OPERATOR Operator = pPoint->GetOperator();
			JInt32 ValOpe = Operator;
			Stream.Send(ValOpe);
		}
	}	
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde Map_pIBModalite 
JVoid CSerialisePaletteBuilder::Send_MapIBModalite(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Sauve nb elmt du map
	JInt32 NbElmtMap = PaletteBuilder.m_Map_pIBModalite.GetCount(); 
	Stream.Send(NbElmtMap);

	// Puis tous les elmts CIBModalite (que sont des JList <JInt32>)
	for (PaletteBuilder.m_Map_pIBModalite.MoveFirst();
		 PaletteBuilder.m_Map_pIBModalite.IsValid();
		 PaletteBuilder.m_Map_pIBModalite.MoveNext())
	{
		// Stocke la clef
		JInt32 Clef		  = PaletteBuilder.m_Map_pIBModalite.GetKey();
		Stream.Send(Clef); 

		// R�cup la structure CIBModalite
		CIBModalite *pCIBModalite = PaletteBuilder.m_Map_pIBModalite.GetItem();
		bool CIBModaliteExist = (pCIBModalite != 0);
		Stream.Send(CIBModaliteExist);  

		if (pCIBModalite)
		{
			// Recup liste de valeurs
			JList<JInt32>	ValuesInt32;
			pCIBModalite->GetValues(ValuesInt32); 

			// Sauve nb �lmt de chaque CIBModalite
			Stream.Send(ValuesInt32.GetCount());

			// Boucle pour r�cup�rer tous les valeurs CIBMOdalite
			for (ValuesInt32.MoveFirst(); ValuesInt32.IsValid(); ValuesInt32.MoveNext())
			{
				JInt32 Valeur = ValuesInt32.GetItem(); 
				Stream.Send(Valeur);
			}

			// Sauve op�rateur
			JInt32 Operateur = pCIBModalite->GetOperator();
			Stream.Send(Operateur);
		}
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde Map_Dim_Type	>> JMap <int, IQuestion::DIMTYPE>
JVoid CSerialisePaletteBuilder::Send_MapDimType(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Sauve nb elmts
	JInt32 NbElmt = PaletteBuilder.m_Map_DimType.GetCount();
	Stream.Send(NbElmt);

	// Puis sauve tous les elmts, de type IQuestion::DIMTYPE
	for (PaletteBuilder.m_Map_DimType.MoveFirst(); 
		 PaletteBuilder.m_Map_DimType.IsValid();
		 PaletteBuilder.m_Map_DimType.MoveNext())
	{
		// Stocke la clef
		JInt32 Clef		  = PaletteBuilder.m_Map_DimType.GetKey();
		Stream.Send(Clef); 

		// Valeur Dim Type
		JInt32 ValDimType = PaletteBuilder.m_Map_DimType.GetItem();
		Stream.Send(ValDimType); 
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde Map_pIModalite
JVoid CSerialisePaletteBuilder::Send_MapIModalite(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Sauve nb elmts
	JInt32 NbElmt = PaletteBuilder.m_Map_pIModalite.GetCount();
	Stream.Send(NbElmt);

	// Puis sauve tous les elmts, de type JList const IModalite*>
	for (PaletteBuilder.m_Map_pIModalite.MoveFirst();
		 PaletteBuilder.m_Map_pIModalite.IsValid();
		 PaletteBuilder.m_Map_pIModalite.MoveNext())
	{
		// Stocke la clef
		JInt32 Clef		  = PaletteBuilder.m_Map_pIModalite.GetKey();
		Stream.Send(Clef); 

		// R�cup �lmt liste
		const JList <const IModalite*> &Lst_pIModalite = PaletteBuilder.m_Map_pIModalite.GetItem();

		// Stocke nb elmt liste
		JInt32 NbElmtLst = Lst_pIModalite.GetCount();
		Stream.Send(NbElmtLst);

		// Puis stocke tous les elmts de la liste
		for (Lst_pIModalite.MoveFirst(); Lst_pIModalite.IsValid(); Lst_pIModalite.MoveNext())
		{
			// r�cup �lmt pointeur IModalite
			const IModalite * pIModalite = Lst_pIModalite.GetItem(); 

			// Stocke le label
			JLabel LibModalite = pIModalite->GetLabel(); 
			LibModalite.Send(Stream);

			// Stocke l'ident modalit�
			JInt32 IdModalite = pIModalite->GetID();
			Stream.Send(IdModalite);

			// Stocke le Contingent Mask
			JUnt32 Mask = pIModalite->GetContingentMask();
			Stream.Send(Mask);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// Sauvegarde Liste des valeurs num�riques List_NumValue >> JList <float>
JVoid CSerialisePaletteBuilder::Send_MapListNumValue(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Stocke nb elmt liste
	JInt32 NbElmtLst = PaletteBuilder.m_List_NumValue.GetCount();  
	Stream.Send(NbElmtLst);

	// Stocke les elmts liste
	for (PaletteBuilder.m_List_NumValue.MoveFirst();
		 PaletteBuilder.m_List_NumValue.IsValid();
		 PaletteBuilder.m_List_NumValue.MoveNext())
	{
		JFlt32 Valeur = PaletteBuilder.m_List_NumValue.GetItem();
		Stream.Send(Valeur);
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde List_DimSel  >> JList <int>
JVoid CSerialisePaletteBuilder::Send_ListDimSel(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Stocke nb elmt liste
	JInt32 NbElmtLst = PaletteBuilder.m_List_DimSel.GetCount();  
	Stream.Send(NbElmtLst);

	// Stocke les elmts liste
	for (PaletteBuilder.m_List_DimSel.MoveFirst();
		 PaletteBuilder.m_List_DimSel.IsValid();
		 PaletteBuilder.m_List_DimSel.MoveNext())
	{
		JInt32 Valeur = PaletteBuilder.m_List_DimSel.GetItem();
		Stream.Send(Valeur);
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde List_DimMode	>> JList <int>
JVoid CSerialisePaletteBuilder::Send_ListDimMode(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Stocke nb elmt liste
	JInt32 NbElmtLst = PaletteBuilder.m_List_DimMode.GetCount();  
	Stream.Send(NbElmtLst);

	// Stocke les elmts liste
	for (PaletteBuilder.m_List_DimMode.MoveFirst();
		 PaletteBuilder.m_List_DimMode.IsValid();
		 PaletteBuilder.m_List_DimMode.MoveNext())
	{
		JInt32 Valeur = PaletteBuilder.m_List_DimMode.GetItem();
		Stream.Send(Valeur);
	}
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
JVoid CSerialisePaletteBuilder::Send_Libelles(JStream & Stream, const CPaletteBuilder & PaletteBuilder)
{
	// Sauve le libell� par d�faut
	JString TitreDef = PaletteBuilder.m_DefaultLabel;
	TitreDef.Send(Stream); 

	// Sauve le libelle utilisateur
	JString TitreUtil = PaletteBuilder.m_DefaultLabel;
	TitreUtil.Send(Stream); 
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde des user items s�lectionn�s
JVoid CSerialisePaletteBuilder::Send_SelUserItems		(JStream & Stream, const JMap<JInt32,JBool> &m_SelUserItems)
{
	// Sauvegarde nb �l�ment map "Selection User Items"
	JInt32 NbElmt = m_SelUserItems.GetCount();			
	Stream.Send(m_SelUserItems.GetCount()); 		

	// Sauvegarde des �lmts du map "Selection User Items"
	for (m_SelUserItems.MoveFirst(); m_SelUserItems.IsValid(); m_SelUserItems.MoveNext())   
	{
		// Sauvegarde de la clef
		JInt32 Key = m_SelUserItems.GetKey();
		Stream.Send(Key);

		// Sauvegarde de l'�tat s�lection
		JBool Etat = m_SelUserItems.GetItem();
		Stream.Send(Etat);
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration des composants palette pupitre
JVoid CSerialisePaletteBuilder::Recv(JStream & Stream, JMap<int, CPaletteBuilder> *pMapPaletteBuilder, JMap<JInt32,JBool> &m_SelUserItems, CString &DefaultPaletteXLabel)
{
	// Reset du map
	pMapPaletteBuilder->Reset();

	// R�cup nb �l�ment map 
	JInt32 NbElmt;
	Stream.Recv(NbElmt); 

	// Boucle sur les �lmts du map palette builder
	for (JInt32 IndexMap = 0; IndexMap < NbElmt; IndexMap++)
	{
		// R�cup clef
		JInt32 ClefPaletteBuilder;
		Stream.Recv(ClefPaletteBuilder);

		// Cr�er un �lmt CPaletteBuilder
		CPaletteBuilder	&PaletteBuilder = pMapPaletteBuilder->Add(ClefPaletteBuilder);
		
		// Sauvegarde CIBQuestion 	
		Recv_CIBQuestion(Stream, PaletteBuilder);

		// Sauvegarde CIBItem
		Recv_CIBItem(Stream, PaletteBuilder);

		// Sauvegarde Map_pIBModalite
		Recv_MapIBModalite(Stream, PaletteBuilder);

		// Sauvegarde Map_Dim_Type
		Recv_MapDimType(Stream, PaletteBuilder);

		// Sauvegarde Map_pIModalite
		Recv_MapIModalite(Stream, PaletteBuilder);

		// Sauvegarde Liste des valeurs num�riques List_NumValue
		Recv_MapListNumValue(Stream, PaletteBuilder);
		
		// Sauvegarde List_DimSel
		Recv_ListDimSel(Stream, PaletteBuilder);

		// Sauvegarde List_DimMode
		Recv_ListDimMode(Stream, PaletteBuilder);

		// Sauvegarde des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
		Recv_Libelles(Stream, PaletteBuilder);
	}

	// R�cup�ration des s�lections User Items
	Recv_SelUserItems(Stream, m_SelUserItems);

	// R�cup�ration mode op�ration entre les �lmts du Sas
	JString ModeOpe;
	ModeOpe.Recv(Stream); 
	DefaultPaletteXLabel = ModeOpe.AsJCharPtr(); 
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration CIBQuestion 	
JVoid CSerialisePaletteBuilder::Recv_CIBQuestion(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Y-a t-il des questions
	JBool QuestionExist;
	Stream.Recv(QuestionExist);
 
	// Question valide, on r�cup�re les infos questions
	if (!QuestionExist)
	{
		// Init pointeur pIBQuestion
		PaletteBuilder.m_pIBquestion = 0;
	}
	else
	{
		// Creer le pointeur elmt CIBItem
		PaletteBuilder.m_pIBquestion = new CIBQuestion();	
		
		// Recup Ident Bloc Question
		JInt32 IdBQuestion;
		Stream.Recv(IdBQuestion);   
		PaletteBuilder.m_pIBquestion->SetID(IdBQuestion);

		// Recup JList <CIBModalite *>
		// recup nombre d'�lm�nt
		JInt32 NbElmtLst;
		Stream.Recv(NbElmtLst);

		// Recup Liste Blocs CIBModalite  >> JList <CIBModalite *>
		for (JInt32 InxElmtLst = 0; InxElmtLst<NbElmtLst; InxElmtLst++)
		{
			// R�cup existence IBModalite ???
			JBool IBModaliteExist;
			Stream.Recv(IBModaliteExist);

			// Si IBModalit� existe, r�cup les infos associ�es
			if (IBModaliteExist)
			{
				// Cr�e elmt liste CIBModalite
				CIBModalite *pIBModalite = new CIBModalite();

				// Recup nb elmt valeur
				JInt32 NbValeurs;
				Stream.Recv(NbValeurs);

				// D�finition liste de valeur
				JList <JInt32> ValuesInt32;
				ValuesInt32.Reset();
				
				// Recup elmt valeurs
				for (JInt32 InxVal = 0; InxVal < NbValeurs; InxVal++)
				{
					// R�cup valeur
					JInt32 Valeur;
					Stream.Recv(Valeur);

					// Ajout � la liste des valeurs de CIBModalite
					ValuesInt32.AddTail() = Valeur; 
				}
			
				// Recup�re la liste des valeurs dan CIBModalite
				pIBModalite->SetValues(ValuesInt32); 

				// Puis Recup op�rateur du bloc
				JInt32 ValOpe;
				Stream.Recv(ValOpe);
				IBloc::OPERATOR Operateur = (IBloc::OPERATOR)ValOpe;
				pIBModalite->SetOperator(Operateur);

				// Ajout � la liste
				PaletteBuilder.m_pIBquestion->AddModalite(pIBModalite);
			}
		}


		// R�cup existence Classe ???
		JBool pClassExist;
		Stream.Recv(pClassExist);
		
		if (!pClassExist)
		{
			// Init pointeur pClass
			PaletteBuilder.m_pIBquestion->SetClass(NULL);
		}
		else
		{
			// Info Classe					>> CIBClass
			CIBClass * pClass = new CIBClass();
			PaletteBuilder.m_pIBquestion->SetClass(pClass);

			// Recup nb elmt valeurs
			JInt32 NbValClasse;
			Stream.Recv(NbValClasse);

			// Recup Liste des valeurs
			JList <JFlt32> ValuesFlt32;
			ValuesFlt32.Reset();
			for (JInt32 InxLst = 0; InxLst < NbValClasse; InxLst++)
			{
				// Recup valeur
				JFlt32 Valeur;
				Stream.Recv(Valeur); 
				ValuesFlt32.AddTail() = Valeur; 
			}

			// Ajoute liste de valeurs au pointeur classe
			pClass->SetValues(ValuesFlt32);
			
			// Nombre d'op�rateurs
			JInt32 NbOperators;
			Stream.Recv(NbOperators);

			// Recup liste des op�rateurs associ�s � chaque elmt de la classe
			JList<IBloc::OPERATOR>	Operators;
			for (JInt32 InxOpe = 0; InxOpe < NbOperators; InxOpe++)
			{
				// Recup chaque op�rateur
				JInt32 ValOpe;
				Stream.Recv(ValOpe);
				IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe; 

				// Ajoute � la liste des op�rateurs de la classe
				Operators.AddTail() = Operateur; 
			}

			// Ajoute liste d'op�rateurs au pointeur classe
			pClass->SetOperators(Operators);

			// Recup op�rateur g�n�ral
			JInt32 ValOpe;
			Stream.Recv(ValOpe);
			IBloc::OPERATOR	Operator = (IBloc::OPERATOR) ValOpe;
			pClass->SetOperator(Operator);
		}

		// R�cup existence Quantile ???
		JBool pQuantExist;
		Stream.Recv(pQuantExist);
		
		if (!pQuantExist)
		{
			// Init pointeurQuantile
			PaletteBuilder.m_pIBquestion->SetQuant(NULL);
		}
		else
		{
			// Recup Info Quantile					>> CIBQuant
			CIBQuant * pQuant = new CIBQuant();
			PaletteBuilder.m_pIBquestion->SetQuant(pQuant);

			// Recup nb elmt valeurs
			JInt32 NbValQuant;
			Stream.Recv(NbValQuant);

			// Recup Liste des valeurs
			JList <JInt32> ValuesInt32;
			for (JInt32 InxLst = 0; InxLst < NbValQuant; InxLst++)
			{
				// Recup valeur
				JInt32 Valeur;
				Stream.Recv(Valeur); 
				ValuesInt32.AddTail() = Valeur; 
			}

			// Ajoute liste valuer au pointeur quantile
			pQuant->SetValues(ValuesInt32);
			
			// Recup op�rateur
			JInt32 ValOpe;
			Stream.Recv(ValOpe);
			IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe; 

			// Ajoute op�rateur au pointeur quantile
			pQuant->SetOperator(Operateur);
		}

		// R�cup existence Score ???
		JBool pScoreExist;
		Stream.Recv(pScoreExist);
		
		if (!pScoreExist)
		{
			// Init pointeur Score
			PaletteBuilder.m_pIBquestion->SetScore(NULL);
		}
		else
		{
			// Recup Info Score					>> CIBScore
			CIBScore * pScore = new CIBScore();
			PaletteBuilder.m_pIBquestion->SetScore(pScore);

			// Recup nb elmt valeurs
			JInt32 NbValScore;
			Stream.Recv(NbValScore);

			// Recup Liste des valeurs
			JList <JInt32> ValuesInt32;
			ValuesInt32.Reset();
			for (JInt32 InxLst = 0; InxLst < NbValScore; InxLst++)
			{
				// Recup valeur
				JInt32 Valeur;
				Stream.Recv(Valeur); 
				ValuesInt32.AddTail() = Valeur; 
			}

			// Ajoute liste valeurs au pointeur classe
			pScore->SetValues(ValuesInt32);
			
			// Recup op�rateur
			JInt32 ValOpe; 
			Stream.Recv(ValOpe);
			IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe; 

			// Ajoute op�rateur au pointeur score
			pScore->SetOperator(Operateur);
		}

		// R�cup existence Valeurs Num�riques ???
		JBool pValNumExist;
		Stream.Recv(pValNumExist);
		
		if (!pValNumExist)
		{
			// Init pointeur Valeurs Num�riques
			PaletteBuilder.m_pIBquestion->SetNumValue(NULL);
		}
		else
		{
			// Recup Info Valeurs num�riques			>> CIBNumValue
			CIBNumValue * pNumValue = new CIBNumValue();
			PaletteBuilder.m_pIBquestion->SetNumValue(pNumValue);
			
			// Recup nb elmt valeurs
			JInt32 NbValNum;
			Stream.Recv(NbValNum);

			// Recup Liste des valeurs
			JList <JFlt32> ValuesFlt32;
			ValuesFlt32.Reset();
			for (JInt32 InxLst = 0; InxLst < NbValNum; InxLst++)
			{
				// Recup valeur
				JFlt32 Valeur;
				Stream.Recv(Valeur); 
				ValuesFlt32.AddTail() = Valeur; 
			}

			// Ajoute liste valeurs num�riques au pointeur valeur num�rique
			pNumValue->SetValues(ValuesFlt32);
			
			// Recup op�rateur
			JInt32 ValOpe;
			Stream.Recv(ValOpe);
			IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe; 

			// Ajoute op�rateur au pointeur valeur num�rique
			pNumValue->SetOperator(Operateur);
		}

		// R�cup existence Valeurs Num�riques ???
		JBool pPointExist;
		Stream.Recv(pPointExist);
		
		if (!pPointExist)
		{
			// Init pointeur Point
			PaletteBuilder.m_pIBquestion->SetPoint(NULL);
		}
		else
		{
			// Recup Info Point			>> CIBPoint
			CIBPoint * pPoint = new CIBPoint();
			PaletteBuilder.m_pIBquestion->SetPoint(pPoint);
			
			// Recup la valeur
			JInt32 ValPoint;
			Stream.Recv(ValPoint);
			pPoint->SetValue(ValPoint);

			// Recup l'op�rateur
			JInt32 ValOpe;
			Stream.Recv(ValOpe);
			IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe;
			pPoint->SetOperator((IBloc::OPERATOR)ValOpe);
		}
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration CIBItem
JVoid CSerialisePaletteBuilder::Recv_CIBItem(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Y-a t-il des items
	JBool ItemExist;
	Stream.Recv(ItemExist);

	// Item valide, on r�cup�re les infos
	if (!ItemExist)
	{
		// Init pointeur item
		PaletteBuilder.m_pIBitem = 0;
	}
	else
	{
		// Creer le pointeur elmt CIBItem
		PaletteBuilder.m_pIBitem = new CIBItem();
		
		// Recup Ident Bloc Item
		JInt32 IdItem;
		Stream.Recv(IdItem);
		PaletteBuilder.m_pIBitem->SetID(IdItem);

		// Recup nb elmt valeur
		JInt32 NbValeurs;
		Stream.Recv(NbValeurs);

		// D�finition liste de valeur
		JList <JInt32> ValuesInt32;
		ValuesInt32.Reset();
				
		// Recup elmt valeurs
		for (JInt32 InxVal = 0; InxVal < NbValeurs; InxVal++)
		{
			// R�cup valeur
			JInt32 Valeur;
			Stream.Recv(Valeur);
			
			// Ajout � la liste des valeurs de CIBItem
			ValuesInt32.AddTail() = Valeur; 
		}
			
		// Recup�re la liste des valeurs dans CIBItem
		PaletteBuilder.m_pIBitem->SetValues(ValuesInt32); 

		// Puis Recup op�rateur du bloc
		JInt32 ValOpe;
		Stream.Recv(ValOpe);
		IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe;
		PaletteBuilder.m_pIBitem->SetOperator(Operateur);

		// Champs Point exist (jeux de mots)
		JBool PointExist;
		Stream.Recv(PointExist);

		if (PointExist)
		{
			// Cr�e elmt CIBPoint
			CIBPoint * pIBPoint = new CIBPoint();

			// Recup elmt CIBPoint
			JInt32 ValPoint;
			Stream.Recv(ValPoint);
			pIBPoint->SetValue(ValPoint); 

			// R�cup l'op�rateur
			Stream.Recv(ValOpe);
			Operateur = (IBloc::OPERATOR) ValOpe;
			pIBPoint->SetOperator(Operateur); 
			
			// Sauve l'elmt CIBPoint dan CIBItem
			PaletteBuilder.m_pIBitem->SetPoint(pIBPoint); 
		}
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration Map_pIBModalite
JVoid CSerialisePaletteBuilder::Recv_MapIBModalite(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup nb elmt du map
	JInt32 NbElmtMap;
	Stream.Recv(NbElmtMap);

	// Initialise le map
	PaletteBuilder.m_Map_pIBModalite.Reset();

	// Puis Recup tous les elmts, de type JList const IModalite*>
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmtMap; IndexElmt++)
	{
		// Recup la clef
		JInt32 Clef;
		Stream.Recv(Clef);

		// Est-elle valide
		bool CIBModaliteExist;
		Stream.Recv(CIBModaliteExist);  

		if (CIBModaliteExist)
		{
			// Ajoute au map
			PaletteBuilder.m_Map_pIBModalite.MoveTo(Clef);
			if (!PaletteBuilder.m_Map_pIBModalite.IsValid())
			{
				// Ajoute nouvel �lmt liste
				// CIBModalite * pIBModalite = PaletteBuilder.m_Map_pIBModalite.Add(Clef);
				PaletteBuilder.m_Map_pIBModalite.Add(Clef) = new CIBModalite();
				PaletteBuilder.m_Map_pIBModalite.MoveTo(Clef);
				if (PaletteBuilder.m_Map_pIBModalite.IsValid())
				{
					CIBModalite * pIBModalite = PaletteBuilder.m_Map_pIBModalite.GetItem();
				
					// Recup nb elmt de chaque liste
					JInt32 NbElmtLst;
					Stream.Recv(NbElmtLst);

					// Recup liste de valeurs
					JList <JInt32> ValuesInt32;
					ValuesInt32.Reset();

					// Boucle pour r�cup�rer tous les valeurs CIBMOdalite
					for (JInt32 IndexElmt = 0; IndexElmt < NbElmtLst; IndexElmt++)
					{
						JInt32 Valeur;
						Stream.Recv(Valeur);
						ValuesInt32.AddTail() = Valeur; 
					}
					
					// Ins�re les valeurs
					pIBModalite->SetValues(ValuesInt32); 

					// Recup l'op�rateur
					JInt32 ValOpe;
					Stream.Recv(ValOpe);
					IBloc::OPERATOR Operateur = (IBloc::OPERATOR) ValOpe;
					pIBModalite->SetOperator((IBloc::OPERATOR)ValOpe);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration Map_Dim_Type
JVoid CSerialisePaletteBuilder::Recv_MapDimType(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup nb elmts
	JInt32 NbElmt;
	Stream.Recv(NbElmt);

	// Initialise le Map
	PaletteBuilder.m_Map_DimType.Reset(); 
	
	// Puis Recup tous les elmts, de type JList const IModalite*>
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmt; IndexElmt++)
	{
		// Recup la clef
		JInt32 Clef;
		Stream.Recv(Clef);

		// Ajoute au map
		PaletteBuilder.m_Map_DimType.MoveTo(Clef);
		if (!PaletteBuilder.m_Map_DimType.IsValid())
		{
			// Ajoute nouvel �lmt liste
			JInt32 ValDimType;
			Stream.Recv(ValDimType);
			PaletteBuilder.m_Map_DimType.Add(Clef) = (IQuestion::DIMTYPE) ValDimType;
		}
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration Map_pIModalite
JVoid CSerialisePaletteBuilder::Recv_MapIModalite(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup nb elmts
	JInt32 NbElmt;
	Stream.Recv(NbElmt);
	
	// Initilaise map des IModalite
	PaletteBuilder.m_Map_pIModalite.Reset();
	
	// Puis Recup tous les elmts, de type JList const IModalite*>
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmt; IndexElmt++)
	{
		// Recup la clef
		JInt32 Clef;
		Stream.Recv(Clef);

		// Ajoute au map
		PaletteBuilder.m_Map_pIModalite.MoveTo(Clef);
		if (!PaletteBuilder.m_Map_pIModalite.IsValid())
		{
			// Ajoute nouvel �lmt liste
			JList <const IModalite*> &Lst_pIModalite = PaletteBuilder.m_Map_pIModalite.Add(Clef);

			// Initilaise la liste
			Lst_pIModalite.Reset(); 

			// R�cup nb elmt liste
			JInt32 NbElmtLst;
			Stream.Recv(NbElmtLst);

			// R�cup�re tous les elmts de la liste
			for (JInt32 IndexLst = 0; IndexLst < NbElmtLst; IndexLst++)
			{
				// R�cup le label
				JLabel LibModalite;
				LibModalite.Recv(Stream);
				
				// R�cup l'ident modalit�
				JInt32 IdModalite;
				Stream.Recv(IdModalite);
				
				// Stocke le Contingent Mask
				JUnt32 Mask;
				Stream.Recv(Mask);

				// Reconstruit l'�lmt associ� au pointeur
				// IModalite *pModalite =
				// @@@@@@@@@@@@@@@@@@ A FAIRE
				/*
				/!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				IModalite *pIModalite = new IModalite();
				
				// Ajoute �lmt liste
				Lst_pIModalite.AddTail() = pIModalite;  
				*/
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration Liste des valeurs num�riques List_NumValue
JVoid CSerialisePaletteBuilder::Recv_MapListNumValue(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup nb elmt liste
	JInt32 NbElmtLst;
	Stream.Recv(NbElmtLst);
	
	// Reset liste valeurs num�riques
	PaletteBuilder.m_List_NumValue.Reset();  
	
	// Stocke les elmts liste
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmtLst; IndexElmt++)
	{
		// Recup valeur
		JFlt32 Valeur;
		Stream.Recv(Valeur);

		// Ajoute � liste
		PaletteBuilder.m_List_NumValue.AddTail() = Valeur;
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration List_DimSel
JVoid CSerialisePaletteBuilder::Recv_ListDimSel(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup Nb Elmt
	JInt32 NbElmtLst;
	Stream.Recv(NbElmtLst);

	// Reset de la liste
	PaletteBuilder.m_List_DimSel.Reset(); 

	// Recup�re les elmts de la liste
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmtLst; IndexElmt++)
	{
		JInt32 Valeur;
		Stream.Recv(Valeur);
		PaletteBuilder.m_List_DimSel.AddTail() = Valeur; 
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration List_DimMode
JVoid CSerialisePaletteBuilder::Recv_ListDimMode(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Recup Nb Elmt
	JInt32 NbElmtLst;
	Stream.Recv(NbElmtLst);

	// Reset de la liste
	PaletteBuilder.m_List_DimMode.Reset(); 

	// Recup�re les elmts de la liste
	for (JInt32 IndexElmt = 0; IndexElmt < NbElmtLst; IndexElmt++)
	{
		JInt32 Valeur;
		Stream.Recv(Valeur);
		PaletteBuilder.m_List_DimMode.AddTail() = Valeur; 
	}
}

///////////////////////////////////////////////////////////////////////
// R�cup�ration des Libell� par d�faut  : m_DefautLabel et Libell� Utilisateur: m_Label	 
JVoid CSerialisePaletteBuilder::Recv_Libelles(JStream & Stream, CPaletteBuilder & PaletteBuilder)
{
	// Sauve le libell� par d�faut
	JString TitreDef;
	TitreDef.Recv(Stream);
	PaletteBuilder.m_DefaultLabel = TitreDef.AsJCharPtr();
	
	// Sauve le libelle utilisateur
	JString TitreUtil;
	TitreUtil.Recv(Stream);
	PaletteBuilder.m_DefaultLabel = TitreUtil.AsJCharPtr();
}

///////////////////////////////////////////////////////////////////////
// Sauvegarde des user items s�lectionn�s
JVoid CSerialisePaletteBuilder::Recv_SelUserItems(JStream & Stream, JMap<JInt32,JBool> &m_SelUserItems)
{
	// Reset du map
	m_SelUserItems.Reset();

	// R�cup�ration nb �l�ment map "Selection User Items"
	JInt32 NbElmt;
	Stream.Recv(NbElmt); 		

	// Boucle sur les �lmts du map palette builder
	for (JInt32 IndexMap = 0; IndexMap < NbElmt; IndexMap++)
	{
		// R�cup clef
		JInt32 Clef;
		Stream.Recv(Clef);

		// Construit l'�lmt �tat s�lection
		JBool &Etat = m_SelUserItems.Add(Clef);

		// R�cup l'�tat s�lection
		Stream.Recv(Etat);
	}
}