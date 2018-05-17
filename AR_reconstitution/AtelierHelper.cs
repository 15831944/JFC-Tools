using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AR_reconstitution
{
    public class AtelierHelper
    {

        public static string GetNameFilePlanXml(short NoReplace, string NamePlan, ulong NoPlan, short Etat)
        {
            short ShortNoPlan = (short)NoPlan;   // Pour garder le format utilisé auparavant
            if (NoPlan == 0 || NoReplace == 1)
            {
                // Ici il s'agit d'un plan étude ou campagne classique
                string NameFileXml = "T" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan + ".xml";
                return NameFileXml;
            }
            else if (NoPlan < 100000)
            {

                // Ici il s'agit d'un plan étude ou campagne classique
                string NameFileXml = "T" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-')
                                                                                        .Replace(':', '_')
                                                                                        .Replace('[', '(')
                                                                                        .Replace(']', ')')
                                                                                        .Replace('#', '&') + ".xml";
                return NameFileXml;
            }
            else
            {
                // Ici il s'agit d'un plan echange EDI
                // string NameFileXml = "EDI" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-').Replace(':', '_') + ".xml";

                // Pour le moment on ne change pas le nom du fichier  (seule la relationship "Type" sera différente)
                string NameFileXml = "T" + Etat.ToString() + "No" + ShortNoPlan + "_" + NamePlan.Replace('/', '-')
                                                                                        .Replace(':', '_')
                                                                                        .Replace('[', '(')
                                                                                        .Replace(']', ')')
                                                                                        .Replace('#', '&') + ".xml";
                return NameFileXml;
            }
        }

        public static String NameRlt(String name)
        {
            return System.Convert.ToBase64String(System.Text.Encoding.UTF8.GetBytes(name)).Replace('+', '1')
                    .Replace('/', '2')
                    .Replace('\\', '2')
                    .Replace('%', '3')
                    .Replace('[', '4')
                    .Replace(']', '5')
                    .Replace('&', '6')
                    .Replace('#', '7')
                    .Replace('|', '8')
                    .Replace('@', '9')
                    .Replace('_', '1')
                    .Replace('"', '2')
                    .Replace('\'', '3')
                    .Replace('{', '4')
                    .Replace('}', '5')
                    .Replace('`', '6')
                    .Replace('^', '7')
                    .Trim('=');
        }
    }
}
