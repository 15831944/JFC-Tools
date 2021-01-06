//
// Fichier: JSRCDataExo.cpp
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JSRCDataExo.h"

///////////////////////////////////////////////
// l'unique instance des sources data exog�nes
JSRCDataExo JSRCDataExo::m_Instance;

////////////////////
// Constructeur
JSRCDataExo::JSRCDataExo()
{
	// on ne fait rien
}

JSRCDataExo::JSRCDataExo(const JSRCDataExo & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments
JSRCDataExo & JSRCDataExo::operator =(const JSRCDataExo & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer l'instance des sources exog�nes
const JSRCDataExo & JSRCDataExo::GetInstance()
{
	// on initialise l'indicateur
	static JBool m_Load = false;

	// Init Map Ident Grandeurs (pour v�rifiant non redondance identifiant grandeur)
	JSRCDataExo::m_Instance.m_MapIdGrandeur.Reset();

	// on teste si on doit faire le chargement
	if (!m_Load)
	{
		// R�cup�re le chemin des fichiers exog�nes
		CString CurrentDir = AfxGetApp()->GetProfileString("Data", "SourcesPath", 0);
		CurrentDir += "\\DataExo";
		SetCurrentDirectory(CurrentDir);

		// Init Liste des noms data exog�ne
		JSRCDataExo::m_Instance.m_ListNomDataExo.Reset();

		// Init Map Data Exog�ne
		JSRCDataExo::m_Instance.m_MapDataExo.Reset();

		// Puis on traite tous les fichiers csv appartenant au r�pertoire donn�es exog�nes
		CFileFind finder;

		// Cr�� le chemin de recherche avec extension csv
		CurrentDir += _T("\\*.csv");

		// Commence la recherche des fichiers data exog�ne
		BOOL OkFind = finder.FindFile(CurrentDir);

		// Boucle sur tous kes fichiers exog�nes (extension csv)
		while (OkFind)
		{
			// Recherche fichier
			OkFind = finder.FindNextFile();

			// Passe les . et ..
			if (finder.IsDots())
				continue;

			// Ignore les ss-directory
			if (!finder.IsDirectory())
			{
				// Nom du fichier trouv�
				CString NomFile =finder.GetFileName(); 

				// R�cup�re nom du fichier complet
				CString FichDataExo = finder.GetFilePath();

				// Traite le fichier
				CString TxtErr;
				if (!JSRCDataExo::m_Instance.ChargeFichExo(FichDataExo, TxtErr))
				{
					CString Message;
					Message = "Fichier " + NomFile;
					Message += " incorrectement charg� !!!... " + TxtErr;
					AfxMessageBox(Message, MB_ICONEXCLAMATION); 
					// throw JInternalError::GetInstance(); 
				}
			}
		}

		// Fin de la recherche
		finder.Close();

		// Chargement correct
		m_Load = true;

		// Remet le r�pertoire courant
		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
	}

	// on renvoie l'instance
	return (m_Instance);
}

//////////////////////////////////////////////////////////////////////
// Chargement d'un fichier exog�ne
JBool JSRCDataExo::ChargeFichExo(CString FichDataExo, CString &TxtErr)
{
	// D�finitions locales
	CString path;
	CStdioFile File;
	int Pos;

	// Init Texte erreur
	TxtErr = "";

	// Ouverture du fichier data exog�ne
	if (File.Open(FichDataExo, CFile::modeRead | CFile::typeText ))
	{
		// Lecture date fichier
		CString Data;
		if(!File.ReadString(Data))
		{
			// Manque l'info date fichier exog�ne
			TxtErr = "Manque date fichier"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Lecture version
		if(File.ReadString(Data))
		{
			// Recherche s�parateur
			Pos = Data.Find(";");
			if (Pos <= 0)
			{
				// Manque l'info version fichier exog�ne
				TxtErr = "Manque version fichier"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
			else if (Data.Left(Pos) != "Version")
			{
				// Manque l'info version fichier exog�ne
				TxtErr = "Manque version fichier"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}

		// Ajoute le nom de la donn�e exog�ne � la liste
		CString LabelDataExo	= "";
		CString AbrLabelDataExo = "";
		if(File.ReadString(Data))
		{
			// Voir si libell� abr�g�
			Pos = Data.Find(";");
			if (Pos > 0)
			{
				// R�cup�re libell� court et libell� complet
				AbrLabelDataExo = Data.Mid(Pos+1);
				AbrLabelDataExo.Remove(';');
				LabelDataExo    = Data.Left(Pos);
			}
			else
			{
				// Pas de libell� court, par d�faut le m�me
				LabelDataExo    = Data;
				AbrLabelDataExo = LabelDataExo;
			}

			// Ajoute info libell� complet � la liste
			// JSRCDataExo::m_Instance.m_ListNomDataExo.AddTail() = LabelDataExo; 
		}
		else
		{
			// Manque le nom du fichier exog�ne
			TxtErr = "Manque nom fichier exog�ne"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}

		// R�cup�re min et max des valeurs pour cette data exo
		JFlt64 MinVal, MaxVal;
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "MinMax")
			{
				// Min et max � tester pour les valeurs qui vont suivre
				Data.Remove(' ');
				Data = Data.Mid(Pos+1);
				Pos = Data.Find(";");
				if (Pos >= 0)
				{
					MinVal = atof(Data.Left(Pos));
					
					MaxVal = atof(Data.Mid(Pos+1));
				}
				else
				{
					// Manque l'info min et max valeurs exog�nes
					TxtErr =  "Manque valeur min max"; 
					return false;
					// throw JInternalError::GetInstance(); 
				}
			}
			else
			{
				// Manque l'info min et max valeurs exog�nes
				TxtErr = "Manque valeur min max"; 
				return false;
				//throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info min et max valeurs exog�nes
			TxtErr = "Manque valeur min max"; 
			return false;
			//throw JInternalError::GetInstance(); 
		}

		// D�clare �lmts data exog�ne pour ajout map
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.Add(LabelDataExo);

		// Ajoute le libell� abr�g�
		DataExo.m_AbrLabelDataExo = AbrLabelDataExo;

		// Lecture nb d�cimales pour les valeurs exog�nes
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "NbDec")
			{
				// R�cup�re info nb d�cimales
				DataExo.m_NbDecExo = atoi(Data.Mid(Pos+1));
			}	
			else
			{
				// Manque l'info nb d�cimales
				TxtErr = "Manque info nb d�cimales"; 
				return false;
				//throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info nb d�cimales
			TxtErr = "Manque info nb d�cimales"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Lecture type de valeurs attendues
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "Type")
			{
				// R�cup�re type valeur 
				DataExo.m_TypeDataExo = atoi(Data.Mid(Pos+1));
			}	
			else
			{
				// Manque l'info type valeur
				TxtErr = "Manque type de valeur exog�ne"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info type valeur
			TxtErr = "Manque type de valeur exog�ne"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}

		// Lecture ident d�but grandeur pour ces datas exog�nes
		if(File.ReadString(Data))
		{
			Pos = Data.Find(";");
			if (Pos >= 0 && Data.Left(Pos) == "IdVal")
			{
				// R�cup�re type valeur 
				DataExo.m_IdDepGrandeur = atoi(Data.Mid(Pos+1));

				// V�rifie si data non d�j� utilis� et compris entre 100 et 999
				if (DataExo.m_IdDepGrandeur < 100 || DataExo.m_IdDepGrandeur >= 1000)
				{
					TxtErr = "Une valeur IdVal n'est pas dans les bornes identificateurs grandeur (soit >=100 et < 1000)"; 
					return false;
				}
				
				// V�rifie non d�j� utilis�
				JSRCDataExo::m_Instance.m_MapIdGrandeur.MoveTo(DataExo.m_IdDepGrandeur);
				if (!m_MapIdGrandeur.IsValid())
				{
					// Ajout au map pour v�rif suivante
					JSRCDataExo::m_Instance.m_MapIdGrandeur.Add(DataExo.m_IdDepGrandeur);
				}
				else
				{
					TxtErr = "Valeur identifiant grandeur d�j� utilis�e"; 
					return false;
				}

			}	
			else
			{
				// Manque l'info ident d�but grandeur exog�ne
				TxtErr = "Manque ident d�but grandeur exog�ne"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
		}
		else
		{
			// Manque l'info ident d�but grandeur exog�ne
			TxtErr = "Manque ident d�but grandeur exog�ne"; 
			return false;
			// throw JInternalError::GetInstance(); 
		}
		
		// Charge les valeurs exog�nes par id support
		while (File.ReadString(Data))
		{
			// Lecture s�parateur csv
			Pos = Data.Find(";");

			if (Pos >= 0)
			{
				
				// Libell� support (non sauvegard�, juste l� pour clart� fichier)
				CString lblSupport;
				lblSupport = Data = Data.Mid(Pos+1);

				// Lecture id support
				Pos = Data.Find(";");
				if (Pos >= 0)
				{
					// Sauvegarde id support
					JInt32 IdSupport = atoi(Data.Left(Pos));

					// Test validit� support
					if (IdSupport <= 0)
					{
						// Valeur associ� au support non valide
						TxtErr = "Valeur associ� au support d�passe les bornes min et max";
						return false;
						// throw JInternalError::GetInstance(); 
					}								
					else
					{
						// Puis lecture valeur associ�
						Data = Data.Mid(Pos+1);
						Data.Remove(' ');
						JFlt64 ValSupp = atoi(Data);

						// V�rifie valeur incluse dans les bornes min et max
						if (ValSupp < MinVal || ValSupp > MaxVal)
						{
							// Valeur associ� au support non valide
							TxtErr = "Valeur associ� au support d�passe les bornes min et max";
							return false;
							// throw JInternalError::GetInstance(); 
						}
						else
						{
							// Stockage id support et valeur associ�
							DataExo.m_MapValExo.MoveTo(IdSupport);
							if(!DataExo.m_MapValExo.IsValid())
								DataExo.m_MapValExo.Add(IdSupport) = ValSupp; 
							else
							{
								TxtErr.Format("Valeur en doublon pour %s", lblSupport);
								return false;
							}

						}
					}
				}
				else
				{
					// Manque l'info ident support
					TxtErr = "Manque information identification support"; 
					return false;
					// throw JInternalError::GetInstance(); 
				}
			}
			else
			{
				// Manque l'info libell� support
				TxtErr = "Manque information libell� support"; 
				return false;
				// throw JInternalError::GetInstance(); 
			}
			
		}

		// ferme le fichier
		File.Close();

		// Ajoute info libell� complet � la liste
		JSRCDataExo::m_Instance.m_ListNomDataExo.AddTail() = LabelDataExo; 

		// Tout s'est bien pass�
		return true;
	}

	// Mauvais chargement ou fichier inexistant
	return false;
}

///////////////////
// le destructeur
JSRCDataExo::~JSRCDataExo()
{
	// on ne fait rien
}

/////////////////////////////////////////////////////////////////////////////
// R�cup�ration de la liste des noms date exog�nes disponibles
const JList <CString> & JSRCDataExo::GetListNomDataExo() const
{
	// Renvoi liste des libell�s data exog�nes
	return (m_Instance.m_ListNomDataExo);
}

/////////////////////////////////////////////////////////////////////////////
// R�cup�ration valeur d'un titre pour une data exo
JBool JSRCDataExo::GetValeur(CString NomDataExo, JUnt32 IdSupport, JFlt64 &Valeur) const
{
	// On se positionne via le libell� recherch�
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exog�ne existe et est bien charg�
	if (m_MapDataExo.IsValid())
	{
		// Puis on r�cup�re les infos pour cette data
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Et la valeur associ�e � cet ident support
		DataExo.m_MapValExo.MoveTo(IdSupport);

		// Test si id support bien renseign� pour cette data exog�ne
		if (DataExo.m_MapValExo.IsValid())
		{
			// R�cup�re la valeur
			Valeur = DataExo.m_MapValExo.GetItem(); 
			return true;
		}
	}

	// Instance non charg� ou data exog�ne non trouv�e
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// R�cup�ration type et nb d�cimal des valeurs inclues dans une data exo
JBool JSRCDataExo::GetTypeValeur(CString NomDataExo, JUnt32 &Type, JUnt32 &NbDec) const
{
	// On se positionne via le libell� recherch�
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exog�ne existe et est bien charg�
	if (m_MapDataExo.IsValid())
	{
		// Puis on r�cup�re les infos data exog�ne
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Puis le type de valeur attendue pour cette data
		Type = DataExo.m_TypeDataExo;

		// Puis le nombre de d�cimal attendue pour les valeurs de cette data
		NbDec = DataExo.m_NbDecExo;
		
		// Aucun probl�me
		return true;
	}

	// Instance non charg� ou data exog�ne non trouv�e
	return false;
}

////////////////////////////////////////////////////////////////////
// R�cup�ration 1er Id Grandeur pour chaque data exe
JBool JSRCDataExo::GetIdDepGrandeur(CString NomDataExo, JUnt32 &IdDepGrandeur) const
{
	// On se positionne via le libell� recherch�
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exog�ne existe et est bien charg�
	if (m_MapDataExo.IsValid())
	{
		// Puis on r�cup�re les infos data exog�ne
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Enfin l'id garndeur de d�part
		IdDepGrandeur = DataExo.m_IdDepGrandeur;

		// Aucun probl�me
		return true;
	}

	// Instance non charg� ou data exog�ne non trouv�e
	return false;
}

//////////////////////////////////////////////////////////////////////
// R�cup�ration libell� abr�g� data exog�ne
JBool JSRCDataExo::GetAbrLib(CString NomDataExo, CString &AbrNomDataExo) const
{
	// On se positionne via le libell� recherch�
	JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

	// Test si cette data exog�ne existe et est bien charg�
	if (m_MapDataExo.IsValid())
	{
		// Puis on r�cup�re les infos data exog�ne
		CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

		// Enfin le lilell� abr�g� de la data exog�ne
		AbrNomDataExo = DataExo.m_AbrLabelDataExo;

		// Aucun probl�me
		return true;
	}

	// Instance non charg� ou data exog�ne non trouv�e
	return false;
}

// R�cup�ration libell� via ident grandeur
CString JSRCDataExo:: GetLib(JUnt32 IdGrandeur) const
{
	// Recherche parmi tous les libell�s data exog�ne
	for (JSRCDataExo::m_Instance.m_ListNomDataExo.MoveFirst();
		 JSRCDataExo::m_Instance.m_ListNomDataExo.IsValid();
		 JSRCDataExo::m_Instance.m_ListNomDataExo.MoveNext())
	{
		// R�cup�re le libell� data
		CString &NomDataExo = JSRCDataExo::m_Instance.m_ListNomDataExo.GetItem();

		// On se positionne via le libell� recherch�
		JSRCDataExo::m_Instance.m_MapDataExo.MoveTo(NomDataExo);

		// Test si cette data exog�ne existe et est bien charg�
		if (m_MapDataExo.IsValid())
		{
			// Puis on r�cup�re les infos data exog�ne
			CDataExo &DataExo = JSRCDataExo::m_Instance.m_MapDataExo.GetItem();

			// Test si ident trouv�
			if (DataExo.m_IdDepGrandeur == IdGrandeur)
				return NomDataExo;
		}
	}

	// Pas de libell� trouv�
	return "";
}