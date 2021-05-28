using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Net;
using System.ServiceProcess;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;

namespace JFCUpdateService
{
    public class MonService : ServiceBase
    {
        public struct Service
        {
            public int Number;

            public string CompanyName;

            public string Cle;

            public string LocalDir;

            public string NameAppli;

            public string RemoteDir;

            public string CodeAppli;

            public string Patch;

            public string PathExecute;

            public string Filter;

            public string PatchCopy;

            public string Execute;

            public string CommandLine;

            public string CurrentDir;

            public bool Wait;

            public bool DeleteAfter;
        }

        public static string AssemblyName => "";
        public static string AssemblyVersion => "";

        //public static string AppPath = MyProject.Application.Info.DirectoryPath + "\\";
        public static string AppPath = Environment.CurrentDirectory + "\\";

        public static string svServiceIni = AppPath + AssemblyName + ".ini";

        public static string HostName = Dns.GetHostEntry(Dns.GetHostName()).HostName;

        public static string Serial;

        public static cConnection MaConnection = new cConnection();

        public static int TimeOut;

        public static int DelayReConnect;

        public static int DelayRefresh;

        public static int DelayService;

        public static string DisplayNameService = "JFC Update";

        public static string DisplayNameReStart = "ReStart";

        public static string DisplayNameZipLib = "ICSharpCode.SharpZipLib";

        public static string svCompanyNameService;

        public static string AppName;

        public static string Cle;

        public static string LocalDir;

        public static string Patch;

        public static string CompanyName;

        public static bool ServiceFound;

        public static bool bLogonUser = false;

        public static AliasAccount impersonator;

        private cDelegate MonDelegate;

        private Service[] Services;

        private IContainer components;

        public MonService()
        {
            InitializeComponent();
            base.ServiceName = DisplayNameService;
        }

        protected override void Dispose(bool disposing)
        {
            if (disposing && components != null)
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        [MTAThread]
        public static void Main()
        {
            ServiceBase.Run(new ServiceBase[1]
            {
            new MonService()
            });
        }

        [DebuggerStepThrough]
        private void InitializeComponent()
        {
            components = new Container();
            base.ServiceName = DisplayNameService;
        }

        protected override void OnStart(string[] args)
        {
            checked
            {
                try
                {
                    string NomModule = "Parametres";
                    string MotCle = "ServerName";
                    bool flag = Strings.StrComp(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni), "www.jfc-infomedia.fr", CompareMethod.Text) == 0;
                    if (flag)
                    {
                        mFileIni.Select_WriteIniString("Parametres", "ServerName", "mp.kantarmedia.fr", svServiceIni);
                    }
                    MotCle = "Parametres";
                    NomModule = "ServiceName";
                    bool flag2 = Strings.StrComp(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni), "Service", CompareMethod.Text) == 0;
                    if (flag2)
                    {
                        mFileIni.Select_WriteIniString("Parametres", "ServiceName", DisplayNameService, svServiceIni);
                    }
                    if (File.Exists(AppPath + AssemblyName + ".old"))
                    {
                        File.Delete(AppPath + AssemblyName + ".old");
                    }
                    if (File.Exists(AppPath + DisplayNameReStart + ".old"))
                    {
                        File.Delete(AppPath + DisplayNameReStart + ".old");
                    }
                    NomModule = "Parametres";
                    MotCle = "CompanyName";
                    CompanyName = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                    svCompanyNameService = Url_Encode(CompanyName);
                    Serial = mHddSerial.HddSerial() + MacAddress.GetValue();
                    InitServer();
                    MotCle = "Parametres";
                    NomModule = "Language";
                    string left = Strings.UCase(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni));
                    if (Operators.CompareString(left, "EN", TextCompare: false) == 0)
                    {
                        mLanguageAU.SetCurrentLanguage(0);
                    }
                    else if (Operators.CompareString(left, "FR", TextCompare: false) == 0)
                    {
                        mLanguageAU.SetCurrentLanguage(1);
                    }
                    else
                    {
                        mLanguageAU.SetCurrentLanguage(1);
                    }
                    NomModule = "Parametres";
                    MotCle = "ServiceName";
                    AppName = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                    MotCle = "Parametres";
                    NomModule = "AppBuild";
                    string left2 = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
                    string text = AssemblyVersion.ToString();
                    if (Operators.CompareString(left2, text, TextCompare: false) != 0)
                    {
                        left2 = text;
                        mFileIni.Select_WriteIniString("Parametres", "AppBuild", left2, svServiceIni);
                    }
                    string fileVersion = FileVersionInfo.GetVersionInfo(AppPath + AssemblyName + ".exe").FileVersion;
                    if (Operators.CompareString(mSendMessage.AppVersion, fileVersion, TextCompare: false) != 0)
                    {
                        mSendMessage.AppVersion = fileVersion;
                        mFileIni.Select_WriteIniString("Parametres", "AppVersion", mSendMessage.AppVersion, svServiceIni);
                    }
                    NomModule = "Parametres";
                    MotCle = "LogonUser";
                    string text2 = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni).Trim();
                    if (Operators.CompareString(text2, "", TextCompare: false) != 0)
                    {
                        int num = Strings.InStr(text2, "\\");
                        int num2 = Strings.InStr(text2, ":");
                        string domainname = Strings.Mid(text2, 1, num - 1);
                        string username = Strings.Mid(text2, num + 1, num2 - num - 1);
                        string password = Strings.Mid(text2, num2 + 1, Strings.Len(text2) - num2);
                        impersonator = new AliasAccount(username, password, domainname);
                        bLogonUser = true;
                    }
                    else
                    {
                        bLogonUser = false;
                    }

                    //mSendMessage.AppVersion = mSendMessage.AppVersion;

                    if (InitService())
                    {
                        MonDelegate = new cDelegate();
                        MonDelegate.Main(Services);
                        mLog.WriteLog(mLanguageAU.GetString(14));
                        mSendMessage.HttpSendMessage("", "", "", mLanguageAU.GetString(14), Conversions.ToString(23));
                        if (flag)
                        {
                            mSendMessage.HttpSendMessage("", "", "Parameter", "ServerName", Conversions.ToString(50));
                        }
                        if (flag2)
                        {
                            mSendMessage.HttpSendMessage("", "", "Parameter", "ServiceName", Conversions.ToString(50));
                        }
                    }
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    mLog.WriteLog(ex2.Message);
                    mSendMessage.HttpSendMessage("", "", "", mLanguageAU.GetString(13), Conversions.ToString(22));
                    mSendMail.SendMail("Error: " + ex2.Message, mLanguageAU.GetString(18));
                    ProjectData.ClearProjectError();
                }
            }
        }

        private string Url_Encode(string svUrl)
        {
            svUrl = svUrl.Replace(" ", "%20");
            svUrl = svUrl.Replace("!", "%21");
            svUrl = svUrl.Replace("$", "%24");
            svUrl = svUrl.Replace("&", "%26");
            svUrl = svUrl.Replace("'", "%27");
            svUrl = svUrl.Replace("(", "%28");
            svUrl = svUrl.Replace(")", "%29");
            svUrl = svUrl.Replace("@", "%40");
            svUrl = svUrl.Replace("À", "%C0");
            svUrl = svUrl.Replace("Á", "%C1");
            svUrl = svUrl.Replace("Â", "%C2");
            svUrl = svUrl.Replace("Ã", "%C3");
            svUrl = svUrl.Replace("Ä", "%C4");
            svUrl = svUrl.Replace("Å", "%C5");
            svUrl = svUrl.Replace("Æ", "%C6");
            svUrl = svUrl.Replace("Ç", "%C7");
            svUrl = svUrl.Replace("È", "%C8");
            svUrl = svUrl.Replace("É", "%C9");
            svUrl = svUrl.Replace("Ê", "%CA");
            svUrl = svUrl.Replace("Ë", "%CB");
            svUrl = svUrl.Replace("Ì", "%CC");
            svUrl = svUrl.Replace("Í", "%CD");
            svUrl = svUrl.Replace("Î", "%CE");
            svUrl = svUrl.Replace("Ï", "%CF");
            svUrl = svUrl.Replace("Ð", "%D0");
            svUrl = svUrl.Replace("Ñ", "%D1");
            svUrl = svUrl.Replace("Ò", "%D2");
            svUrl = svUrl.Replace("Ó", "%D3");
            svUrl = svUrl.Replace("Ô", "%D4");
            svUrl = svUrl.Replace("Õ", "%D5");
            svUrl = svUrl.Replace("Ö", "%D6");
            svUrl = svUrl.Replace("×", "%D7");
            svUrl = svUrl.Replace("Ù", "%D9");
            svUrl = svUrl.Replace("Ú", "%DA");
            svUrl = svUrl.Replace("Û", "%DB");
            svUrl = svUrl.Replace("Ü", "%DC");
            svUrl = svUrl.Replace("Ý", "%DD");
            svUrl = svUrl.Replace("à", "%E0");
            svUrl = svUrl.Replace("á", "%E1");
            svUrl = svUrl.Replace("â", "%E2");
            svUrl = svUrl.Replace("ã", "%E3");
            svUrl = svUrl.Replace("ä", "%E4");
            svUrl = svUrl.Replace("å", "%E5");
            svUrl = svUrl.Replace("æ", "%E6");
            svUrl = svUrl.Replace("ç", "%E7");
            svUrl = svUrl.Replace("è", "%E8");
            svUrl = svUrl.Replace("é", "%E9");
            svUrl = svUrl.Replace("ê", "%EA");
            svUrl = svUrl.Replace("ë", "%EB");
            svUrl = svUrl.Replace("ì", "%EC");
            svUrl = svUrl.Replace("í", "%ED");
            svUrl = svUrl.Replace("î", "%EE");
            svUrl = svUrl.Replace("ï", "%EF");
            svUrl = svUrl.Replace("ð", "%F0");
            svUrl = svUrl.Replace("ñ", "%F1");
            svUrl = svUrl.Replace("ò", "%F2");
            svUrl = svUrl.Replace("ó", "%F3");
            svUrl = svUrl.Replace("ô", "%F4");
            svUrl = svUrl.Replace("õ", "%F5");
            svUrl = svUrl.Replace("ö", "%F6");
            svUrl = svUrl.Replace("ù", "%F9");
            svUrl = svUrl.Replace("ú", "%FA");
            svUrl = svUrl.Replace("û", "%FB");
            svUrl = svUrl.Replace("ü", "%FC");
            svUrl = svUrl.Replace("ý", "%FD");
            svUrl = svUrl.Replace("þ", "%FE");
            svUrl = svUrl.Replace("ÿ", "%FF");
            return svUrl;
        }

        private bool InitService()
        {
            int num = 0;
            int num2 = 1;
            checked
            {
                while (true)
                {
                    string NomModule = "Service" + Conversions.ToString(num2).Trim();
                    string MotCle = "CompanyName";
                    string text = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                    if (Operators.CompareString(text, null, TextCompare: false) == 0)
                    {
                        break;
                    }
                    MotCle = "Service" + Conversions.ToString(num2).Trim();
                    NomModule = "Key";
                    string right = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
                    if (Operators.CompareString(mCheckSum.AppendString(mEasyCrypt.Crypter(text, "sha512")), right, TextCompare: false) == 0)
                    {
                        ref Service[] services = ref Services;
                        services = (Service[])Utils.CopyArray(services, new Service[num + 1]);
                        Services[num].Number = num;
                        Services[num].CompanyName = text;
                        ref Service reference = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "RemoteDir";
                        reference.RemoteDir = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference2 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "LocalDir";
                        reference2.LocalDir = mFileFunction.StrAddLastSlash(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni));
                        ref Service reference3 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "NameAppli";
                        reference3.NameAppli = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference4 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "CodeAppli";
                        reference4.CodeAppli = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
                        ref Service reference5 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "Patch";
                        reference5.Patch = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference6 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "PathExecute";
                        reference6.PathExecute = mFileFunction.StrAddLastSlash(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni));
                        ref Service reference7 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "Filter";
                        reference7.Filter = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference8 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "PatchCopy";
                        reference8.PatchCopy = mFileFunction.StrAddLastSlash(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni));
                        ref Service reference9 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "Execute";
                        reference9.Execute = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference10 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "CommandLine";
                        reference10.CommandLine = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
                        ref Service reference11 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "CurrentDir";
                        reference11.CurrentDir = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
                        ref Service reference12 = ref Services[num];
                        MotCle = "Service" + Conversions.ToString(num2).Trim();
                        NomModule = "Wait";
                        reference12.Wait = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni), "1", TextCompare: false) == 0;
                        ref Service reference13 = ref Services[num];
                        NomModule = "Service" + Conversions.ToString(num2).Trim();
                        MotCle = "DeleteAfter";
                        reference13.DeleteAfter = Operators.CompareString(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni), "1", TextCompare: false) == 0;
                        num++;
                    }
                    else
                    {
                        mLog.WriteLog(mLanguageAU.GetString(17) + text + "\r\n\r\n" + mLanguageAU.GetString(1));
                    }
                    num2++;
                }
                bool flag = Services.Length != 0;
                if (!flag)
                {
                    mLog.WriteLog(mLanguageAU.GetString(16));
                }
                else
                {
                    TimeOut = DelayDefault("TimeOut", 300);
                    DelayReConnect = DelayDefault("DelayReConnect", 60);
                    DelayRefresh = DelayDefault("DelayRefresh", 300);
                    DelayService = DelayDefault("DelayService", 5);
                }
                return flag;
            }
        }

        private int DelayDefault(string szKey, int nvTimeDefault)
        {
            try
            {
                string NomModule = "Parametres";
                string text = mFileIni.Select_GetIniString(ref NomModule, ref szKey, ref svServiceIni).Trim();
                if (Operators.CompareString(text, "", TextCompare: false) == 0)
                {
                    return nvTimeDefault;
                }
                return Conversions.ToInteger(text);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                ProjectData.ClearProjectError();
                return nvTimeDefault;
            }
        }

        private string FormatString2Web(string szString)
        {
            string expression = Strings.Replace(szString, "&", "%26", 1, -1, CompareMethod.Text);
            return Strings.Replace(expression, " ", "%20", 1, -1, CompareMethod.Text);
        }

        private void InitServer()
        {
            string NomModule = "Parametres";
            string MotCle = "InternetBuffer";
            string text = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
            MotCle = "Parametres";
            NomModule = "InternetFlagsMask";
            string text2 = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
            cConnection maConnection = MaConnection;
            ref cConnection.StructProxy proxy = ref maConnection.Proxy;
            NomModule = "Parametres";
            MotCle = "Proxy";
            proxy.Proxy = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
            cConnection obj = maConnection;
            MotCle = "Parametres";
            NomModule = "DirectConnect";
            obj.DirectConnect = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni), "1", TextCompare: false) == 0;
            cConnection obj2 = maConnection;
            NomModule = "Parametres";
            MotCle = "PassiveMode";
            obj2.PassiveMode = Operators.CompareString(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni), "1", TextCompare: false) == 0;
            cConnection obj3 = maConnection;
            MotCle = "Parametres";
            NomModule = "Debug";
            obj3.DebugMode = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni), "1", TextCompare: false) == 0;
            cConnection obj4 = maConnection;
            NomModule = "Parametres";
            MotCle = "CodeAppli";
            obj4.CodeAppli = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
            maConnection.RemoteFileAcces = FormatString2Web(CompanyName) + "." + mCheckSum.AppendString(mEasyCrypt.Crypter(CompanyName, "sha1"));
            cConnection obj5 = maConnection;
            MotCle = "Parametres";
            NomModule = "UserName";
            obj5.UserName = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni).Trim();
            cConnection obj6 = maConnection;
            NomModule = "Parametres";
            MotCle = "Password";
            obj6.Password = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni).Trim();
            cConnection obj7 = maConnection;
            MotCle = "Parametres";
            NomModule = "RemoteDir";
            obj7.RemoteDir = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref svServiceIni);
            cConnection obj8 = maConnection;
            NomModule = "Parametres";
            MotCle = "ServerName";
            obj8.ServerName = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref svServiceIni);
            if (Operators.CompareString(maConnection.Proxy.Proxy, null, TextCompare: false) != 0)
            {
                maConnection.Proxy.InitProxy();
            }
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                maConnection.InternetBuffer = 8;
            }
            else
            {
                maConnection.InternetBuffer = Conversions.ToShort(text);
            }
            if (Operators.CompareString(text2, null, TextCompare: false) == 0)
            {
                maConnection.InternetFlagsMask = 0;
            }
            else
            {
                maConnection.InternetFlagsMask = Conversions.ToInteger(text2);
            }
            maConnection = null;
        }

        protected override void OnStop()
        {
            if (MonDelegate != null)
            {
                MonDelegate.StopThread();
            }
            mLog.WriteLog(mLanguageAU.GetString(15));
            mSendMessage.HttpSendMessage("", "", "", mLanguageAU.GetString(15), Conversions.ToString(24));
        }
    }
}