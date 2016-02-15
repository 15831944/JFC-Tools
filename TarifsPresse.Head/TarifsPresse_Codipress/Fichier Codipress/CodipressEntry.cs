using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

namespace TarifsPresse_Codipress
{
	public static class CodipressEntry_Extensions
	{
		public static string UniqueSupport(this CodipressEntry insert)
		{
			return insert.nomSupportField + (insert.typeEditionField == "" ? "" : " EDITION " + insert.typeEditionField);
		}

		public static bool HasRubrique(this CodipressEntry insert)
		{
			return insert.rubriqueField != "";
		}

		public static bool HasDiffusionNotNational(this CodipressEntry insert)
		{
			return insert.diffusionField != "" && insert.diffusionField != "NATIONAL";
		}

		public static bool HasTypePubliciteNotCommerciale(this CodipressEntry insert)
		{
			return insert.typePubliciteField != "COMMERCIALE" && insert.typePubliciteField != "";
		}

		public static string UniqueFormat(this CodipressEntry insert)
		{
			var encart = insert.typeEncartField == "" ? "" : insert.typeEncartField + " ";
			var rubrique = insert.rubriqueField == "" ? "" : "RUBRIQUE " + insert.rubriqueField + " ";
			var pagination = insert.paginationField == "" ? "" : insert.paginationField + " ";
			var publicite = (insert.typePubliciteField == "" || insert.typePubliciteField == "COMMERCIALE")
															? "" : "TYPE " + insert.typePubliciteField + " ";
			var emplacement = insert.emplacementField == "" ? "" : insert.emplacementField + " ";
			var diffusion = insert.diffusionField == "" || insert.diffusionField == "NATIONAL" ? "" : "DIFFUSION " + insert.diffusionField + " ";
			var implantation = insert.implantationField == "" ? "" : insert.implantationField + " ";

			if (insert.formatField == "")
			{
				if (insert.paginationField == "")
					return insert.nom_ligne_offreField + " " + implantation + emplacement + insert.couleurField;
				else
					return encart + insert.paginationField + " " + implantation + emplacement + diffusion + publicite + rubrique + insert.couleurField;
			}
			return encart + insert.formatField + " " + implantation + emplacement + diffusion + publicite + rubrique + insert.couleurField;
		}
	}

	public struct CodipressEntry
	{
		public string nomSupportField;
		public string nomPeriodeField;
		public DateTime dateAppliPeriodeField;
		public DateTime dateFinPeriodeField;
		public string nom_ligne_offreField;
		public string statutField;
		public string codeTypePubliciteField;
		public string typePubliciteField;
		public string codeTypeEditionField;
		public string typeEditionField;
		public string codeCouleurField;
		public string couleurField;
		public string codeFormatField;
		public string formatField;
		public string codeEmplacementField;
		public string emplacementField;
		public string codeRubriqueField;
		public string rubriqueField;
		public string codeImplantationField;
		public string implantationField;
		public string codeTypeEncartField;
		public string typeEncartField;
		public string codeTypeTraitementField;
		public string typeTraitementField;
		public string codePaginationField;
		public string paginationField;
		public string codeDiffusionField;
		public string diffusionField;
		public string codeSupportField;
		public string supportField;
		public string tarifField;
		public DateTime dateModificationField;

		public CodipressEntry(DataRow row)
		{
			var fields = row.ItemArray;

			nomSupportField = fields[0] as String;
			nomPeriodeField = fields[1] as String;
			dateAppliPeriodeField = DateTime.Parse(fields[2] as String);
			dateFinPeriodeField = DateTime.Parse(fields[3] as String);
			nom_ligne_offreField = fields[4] as String;
			statutField = fields[5] as String;
			codeTypePubliciteField = fields[6] as String;
			typePubliciteField = fields[7] as String;
			codeTypeEditionField = fields[8] as String;
			typeEditionField = fields[9] as String;
			codeCouleurField = fields[10] as String;
			couleurField = fields[11] as String;
			codeFormatField = fields[12] as String;
			formatField = fields[13] as String;
			codeEmplacementField = fields[14] as String;
			emplacementField = fields[15] as String;
			codeRubriqueField = fields[16] as String;
			rubriqueField = fields[17] as String;
			codeImplantationField = fields[18] as String;
			implantationField = fields[19] as String;
			codeTypeEncartField = fields[20] as String;
			typeEncartField = fields[21] as String;
			codeTypeTraitementField = fields[22] as String;
			typeTraitementField = fields[23] as String;
			codePaginationField = fields[24] as String;
			paginationField = fields[25] as String;
			codeDiffusionField = fields[26] as String;
			diffusionField = fields[27] as String;
			codeSupportField = fields[28] as String;
			supportField = fields[29] as String;
			tarifField = fields[30] as String;
			dateModificationField = DateTime.Parse(fields[31] as String);
		}
	}
}
