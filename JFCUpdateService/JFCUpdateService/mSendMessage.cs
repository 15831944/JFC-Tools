using JFCUpdateService;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    [StandardModule]
    internal sealed class mSendMessage
    {
        public static string AppVersion;

        public static string HttpSendMessage(string CompanyName, string NameAppli, string maj, string info, string etat, string versionspe = null)
        {
            if (cDelegate.SilentMode & !cDelegate.Ping)
            {
                return null;
            }
            string text = "https://mp.kantarmedia.fr/update.asp?";
            if (Operators.CompareString(versionspe, null, TextCompare: false) == 0)
            {
                versionspe = AppVersion;
            }
            string MotCle;
            string NomModule;
            if (Operators.CompareString(CompanyName, null, TextCompare: false) == 0)
            {
                NomModule = "Parametres";
                MotCle = "CompanyName";
                CompanyName = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref MonService.svServiceIni);
            }
            MotCle = "Parametres";
            NomModule = "ServiceName";
            string text2 = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref MonService.svServiceIni);
            if (Operators.CompareString(NameAppli, null, TextCompare: false) == 0)
            {
                NameAppli = text2;
            }
            else if (Operators.CompareString(info, null, TextCompare: false) == 0)
            {
                info = text2;
            }
            string text3 = CompanyName.Replace("&", "%26");
            text3 = text3.Replace(" ", "%20");
            text3 = text3.Replace("/", "%2F");
            text3 = text3.Replace("\\", "%5C");
            string stUrl = text + "sn=" + MonService.Serial + "&login=" + text3 + "&host=" + MonService.HostName + "&logiciel=" + NameAppli + "&maj=" + maj + "&etat=" + etat + "&version=" + versionspe + "&info=" + info;
            return MonService.MaConnection.SendMessage(ref stUrl);
        }
    }
}