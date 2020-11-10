using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ARProbaProcessing
{
    public enum Enquete
    {
        PanelNational,
        PanelCadre,
        PanelIleDeFrance
    }

    public static class EnqueteTools
    {
        public static string GetSupFileChar(Enquete enquete)
        {
            switch (enquete)
            {
                case Enquete.PanelNational:
                    return "U";
                case Enquete.PanelCadre:
                    return "C";
            }
            return "*";
        }
    }
}
