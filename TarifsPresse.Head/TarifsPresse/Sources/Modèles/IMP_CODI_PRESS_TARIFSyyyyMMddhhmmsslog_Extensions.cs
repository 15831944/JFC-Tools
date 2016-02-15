using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace IMP_CODI_PRESS_TARIFS_LOG
{
    public static class IMP_CODI_PRESS_TARIFSyyyyMMddhhmmsslog_Extensions
    {
        public static string UniqueSupport(this NewDataSetTARIF_INSERT insert)
        {
            return insert.NomSupport + (insert.TypeEdition == "" ? "" : " EDITION " + insert.TypeEdition);
        }

        public static bool HasRubrique(this NewDataSetTARIF_INSERT insert)
        {
            return insert.Rubrique != "";
        }

        public static bool HasDiffusionNotNational(this NewDataSetTARIF_INSERT insert)
        {
            return insert.Diffusion != "" && insert.Diffusion != "NATIONAL";
        }

        public static bool HasTypePubliciteNotCommerciale(this NewDataSetTARIF_INSERT insert)
        {
            return insert.TypePublicite != "COMMERCIALE" && insert.TypePublicite != "";
        }

        public static string UniqueFormat(this NewDataSetTARIF_INSERT insert)
        {
            var encart      = insert.TypeEncart == ""       ? "" : insert.TypeEncart    + " ";
            var rubrique    = insert.Rubrique   == ""       ? "" : "RUBRIQUE " + insert.Rubrique + " ";
            var pagination  = insert.Pagination == ""       ? "" : insert.Pagination + " ";
            var publicite   = (insert.TypePublicite == "" || insert.TypePublicite == "COMMERCIALE")
                                                            ? "" : "TYPE " + insert.TypePublicite + " ";
            var emplacement = insert.Emplacement == "" ? "" : insert.Emplacement + " ";
            var diffusion = insert.Diffusion == "" || insert.Diffusion == "NATIONAL" ? "" : "DIFFUSION " + insert.Diffusion + " ";
            var implantation = insert.Implantation == "" ? "" : insert.Implantation + " "; 

            if (insert.Format == "")
            {
                if (insert.Pagination == "")
                    return insert.Nom_ligne_offre + " " + implantation + emplacement + insert.Couleur;
                else
                    return encart + insert.Pagination + " " + implantation + emplacement + diffusion + publicite + rubrique + insert.Couleur;
            }
            return encart + insert.Format + " " + implantation + emplacement + diffusion + publicite + rubrique + insert.Couleur;
         }
    }
}
