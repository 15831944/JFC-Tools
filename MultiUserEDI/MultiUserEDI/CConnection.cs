// MultiUserEDI.CConnection
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using MultiUserEDI.My;
using System;
using System.IO;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace MultiUserEDI
{

    public class CConnection
    {
        public int hInternet;

        public int hConnection;

        public string UserName;

        public string Password;

        public string RemoteFile;

        public string LocalFile;

        public double RemoteSize;

        public string Proxy;

        public string ProxyLog;

        public string ProxyPwd;

        public bool DirectConnect;

        public string CheckSum;

        public int InternetBuffer;

        public int InternetFlagsMask;

        private const string scUserAgent = "AU-Serv Connexion";

        private const int INTERNET_OPEN_TYPE_PRECONFIG = 0;

        private const int INTERNET_OPEN_TYPE_DIRECT = 1;

        private const int INTERNET_OPEN_TYPE_PROXY = 3;

        private const int INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY = 4;

        private const int HTTP_QUERY_STATUS_CODE = 19;

        private const int HTTP_STATUS_PROXY_AUTH_REQ = 407;

        private const int HTTP_STATUS_OK = 200;

        private const int HTTP_STATUS_DENIED = 401;

        private const int HTTP_STATUS_NOT_FOUND = 404;

        private const int INTERNET_OPTION_PASSWORD = 29;

        private const int INTERNET_OPTION_USERNAME = 28;

        private const int INTERNET_OPTION_PROXY = 38;

        private const int INTERNET_OPTION_PROXY_USERNAME = 43;

        private const int INTERNET_OPTION_PROXY_PASSWORD = 44;

        private const int INTERNET_OPTION_CONNECT_TIMEOUT = 2;

        private const int INTERNET_FLAG_EXISTING_CONNECT = 536870912;

        private const int INTERNET_FLAG_RELOAD = int.MinValue;

        public const int INTERNET_FLAGS_MASK = -1610612736;

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetOpenA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetOpen([MarshalAs(UnmanagedType.VBByRefStr)] ref string sAgent, int lAccessType, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sProxyName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sProxyBypass, int lFlags);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "HttpQueryInfoA", ExactSpelling = true, SetLastError = true)]
        private static extern bool HttpQueryInfo(int hHttpRequest, int lInfoLevel, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sBuffer, ref int lBufferLength, ref int lIndex);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "HttpSendRequestA", ExactSpelling = true, SetLastError = true)]
        private static extern bool HttpSendRequest(int hHttpRequest, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sHeaders, int lHeadersLength, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sOptional, int lOptionalLength);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetCloseHandle(int hInet);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetOpenUrlA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetOpenUrl(int hInternetSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sUrl, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sHeaders, int lHeadersLength, int lFlags, long lContext);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetReadFile(int hFile, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sBuffer, int lNumBytesToRead, ref int lNumberOfBytesRead);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetSetOptionA", ExactSpelling = true, SetLastError = true)]
        private static extern bool InternetSetOption(int hInternet, int dwOption, [MarshalAs(UnmanagedType.VBByRefStr)] ref string lpBuffer, int dwBufferLength);

        [DllImport("wininet.dll", CharSet = CharSet.Ansi, EntryPoint = "InternetConnectA", ExactSpelling = true, SetLastError = true)]
        private static extern int InternetConnect(int hInternetSession, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sServerName, int nServerPort, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sUserName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string sPassword, int lService, int lFlags, int lContext);

        public void InitHTTP()
        {
            string FichierIni = MyProject.Forms.MultiUserEDI.AppPath + "AU-Serv.ini";
            checked
            {
                if (File.Exists(FichierIni))
                {
                    string NomModule = "Parametres";
                    string MotCle = "UserName";
                    UserName = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni).Trim();
                    MotCle = "Parametres";
                    NomModule = "Password";
                    Password = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref FichierIni).Trim();
                    NomModule = "Parametres";
                    MotCle = "DirectConnect";
                    DirectConnect = Operators.CompareString(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni).Trim(), "1", TextCompare: false) == 0;
                    MotCle = "Parametres";
                    NomModule = "Buffer";
                    int num = (int)Math.Round(Conversion.Val(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref FichierIni)));
                    if (num == 0)
                    {
                        num = 8;
                    }
                    InternetBuffer = num;
                    NomModule = "Parametres";
                    MotCle = "InternetFlagsMask";
                    InternetFlagsMask = (int)Math.Round(Conversion.Val(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni)));
                    ProxyLog = null;
                    ProxyPwd = null;
                    MotCle = "Parametres";
                    NomModule = "Proxy";
                    Proxy = mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref FichierIni).Trim();
                    if (Operators.CompareString(Proxy, "", TextCompare: false) != 0)
                    {
                        short num2 = (short)Strings.InStr(Proxy, "@");
                        if (num2 > 0)
                        {
                            short num3 = (short)Strings.InStr(Proxy, ":");
                            ProxyLog = Strings.Mid(Proxy, 1, num3 - 1);
                            ProxyPwd = Strings.Mid(Proxy, num3 + 1, (short)unchecked(num2 - num3) - 1);
                            Proxy = Strings.Mid(Proxy, num2 + 1, Strings.Len(Proxy) - num2);
                        }
                        Proxy = "http=http://" + Proxy;
                    }
                }
                else
                {
                    InternetBuffer = 8;
                }
            }
        }

        public int InternetOpenHandle(string Proxy = null)
        {
            string sAgent = "AU-Serv Connexion";
            int lAccessType = Conversions.ToInteger(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 1, 0)), RuntimeHelpers.GetObjectValue(Interaction.IIf(DirectConnect, 3, 4))));
            string sProxyBypass = Conversions.ToString(Interaction.IIf(Operators.CompareString(Proxy, "", TextCompare: false) == 0, null, "<local>"));
            return InternetOpen(ref sAgent, lAccessType, ref Proxy, ref sProxyBypass, 0);
        }

        public bool OpenInternet()
        {
            bool result = false;
            if (InternetFlagsMask == 0)
            {
                InternetFlagsMask = -1610612736;
            }
            if (Operators.CompareString(this.Proxy, null, TextCompare: false) == 0)
            {
                string Proxy = null;
                hInternet = InternetOpenHandle(Proxy);
            }
            else
            {
                hInternet = InternetOpenHandle(this.Proxy);
            }
            if (hInternet != 0)
            {
                string sUrl = "https://mp.kantarmedia.fr/";
                int hInternetSession = hInternet;
                string Proxy = null;
                int num = InternetOpenUrl(hInternetSession, ref sUrl, ref Proxy, 0, InternetFlagsMask, 0L);
                if (num != 0)
                {
                    InternetCloseHandle(num);
                }
                result = num != 0;
            }
            return result;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        public void Disconnect()
        {
            FileSystem.Reset();
            if (hConnection != 0)
            {
                InternetCloseHandle(hConnection);
                hConnection = 0;
            }
            if (hInternet != 0)
            {
                InternetCloseHandle(hInternet);
                hInternet = 0;
            }
        }

        private int GetErr(int hOpen)
        {
            string sBuffer = Strings.Space(1024);
            int lBufferLength = 1024;
            int lIndex = 0;
            HttpQueryInfo(hOpen, 19, ref sBuffer, ref lBufferLength, ref lIndex);
            sBuffer = Strings.Left(sBuffer, lBufferLength);
            return Conversions.ToInteger(sBuffer);
        }

        private string ReadDataToString(int hOpen)
        {
            int num = checked(1024 * InternetBuffer);
            string sBuffer = Strings.Space(num);
            string text = null;
            int lNumberOfBytesRead = default(int);
            while (InternetReadFile(hOpen, ref sBuffer, num, ref lNumberOfBytesRead))
            {
                Application.DoEvents();
                text += Strings.Mid(sBuffer, 1, lNumberOfBytesRead);
                if (lNumberOfBytesRead != num)
                {
                    break;
                }
            }
            Application.DoEvents();
            string result = text;
            InternetCloseHandle(hOpen);
            Application.DoEvents();
            return result;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void ReadDataToFile(int hOpen)
        {
            if (File.Exists(TF(ref LocalFile)))
            {
                File.Delete(TF(ref LocalFile));
            }
            if (File.Exists(LocalFile))
            {
                File.Delete(LocalFile);
            }
            int num = checked(1024 * InternetBuffer);
            string sBuffer = Strings.Space(num);
            int num2 = FileSystem.FreeFile();
            try
            {
                FileSystem.FileOpen(num2, TF(ref LocalFile), OpenMode.Binary, OpenAccess.Write, OpenShare.LockReadWrite);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", "\"" + TF(ref LocalFile) + "\"") + "\r\n" + ex2.Message, MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                InternetCloseHandle(hOpen);
                ProjectData.ClearProjectError();
                return;
            }
            bool flag = true;
            int lNumberOfBytesRead = default(int);
            double num3 = default(double);
            while (InternetReadFile(hOpen, ref sBuffer, num, ref lNumberOfBytesRead))
            {
                if (flag)
                {
                    string sNewHeader = "MZ" + Conversions.ToString(Strings.Chr(144)) + "\0\u0003\0\0\0\u0004\0\0\0" + Conversions.ToString(Strings.Chr(255)) + Conversions.ToString(Strings.Chr(255)) + "\0\0";
                    if (!ReconstructionHeader(ref sBuffer, sNewHeader))
                    {
                        sNewHeader = "MZ";
                        ReconstructionHeader(ref sBuffer, sNewHeader);
                    }
                    flag = false;
                }
                if (lNumberOfBytesRead != num)
                {
                    sBuffer = Strings.Left(sBuffer, lNumberOfBytesRead);
                }
                FileSystem.FilePut(num2, sBuffer, -1L);
                num3 += (double)lNumberOfBytesRead;
                if (lNumberOfBytesRead != num)
                {
                    break;
                }
            }
            FileSystem.FileClose(num2);
            InternetCloseHandle(hOpen);
        }

        private bool ReconstructionHeader(ref string sBuffer, string sNewHeader)
        {
            int num = Strings.Len(sNewHeader);
            string left = Strings.Mid(sBuffer, 1, num);
            string text = "";
            int num2 = num;
            checked
            {
                for (int i = 1; i <= num2; i++)
                {
                    text += "\0";
                }
                if (Operators.CompareString(left, text, TextCompare: false) == 0)
                {
                    sBuffer = sNewHeader + Strings.Mid(sBuffer, num + 1, Strings.Len(sBuffer) - num);
                    return true;
                }
                return false;
            }
        }

        public string SendMessage(ref string stUrl)
        {
            bool flag = false;
            string result = "";
            stUrl = stUrl.Replace(" ", "%20");
            if (hInternet == 0)
            {
                flag = OpenInternet();
            }
            if (hInternet != 0)
            {
                int hInternetSession = hInternet;
                string sHeaders = null;
                int hOpen = InternetOpenUrl(hInternetSession, ref stUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
                if (hOpen > 0)
                {
                    if (Status(ref hOpen))
                    {
                        result = ReadDataToString(hOpen);
                    }
                    InternetCloseHandle(hOpen);
                }
                if (flag)
                {
                    Disconnect();
                }
            }
            return result;
        }

        private bool SetOption(int hOpen)
        {
            if (!InternetSetOption(hOpen, 28, ref UserName, Strings.Len(UserName)))
            {
                return false;
            }
            return InternetSetOption(hOpen, 29, ref Password, Strings.Len(Password));
        }

        private bool SetOptionTimeOut(int hOpen)
        {
            int num = 1000;
            string lpBuffer = Conversions.ToString(Conversion.Int(num));
            return InternetSetOption(hOpen, 2, ref lpBuffer, Strings.Len(num));
        }

        private bool SetOptionProxy(int hOpen)
        {
            if (!InternetSetOption(hOpen, 43, ref ProxyLog, Strings.Len(ProxyLog)))
            {
                return false;
            }
            return InternetSetOption(hOpen, 44, ref ProxyPwd, Strings.Len(ProxyPwd));
        }

        public string DeleteZ(string sValue)
        {
            return Strings.Replace(sValue, "Z", "");
        }

        public bool Status(ref int hOpen)
        {
            int err = GetErr(hOpen);
            Application.DoEvents();
            switch (err)
            {
                case 404:
                    return false;
                case 407:
                    {
                        SetOptionProxy(hOpen);
                        int hHttpRequest = hOpen;
                        string sHeaders = null;
                        string sOptional = null;
                        HttpSendRequest(hHttpRequest, ref sHeaders, 0, ref sOptional, 0);
                        err = GetErr(hOpen);
                        break;
                    }
            }
            if (err == 401)
            {
                SetOption(hOpen);
                int hHttpRequest2 = hOpen;
                string sOptional = null;
                string sHeaders = null;
                HttpSendRequest(hHttpRequest2, ref sOptional, 0, ref sHeaders, 0);
                err = GetErr(hOpen);
            }
            return err == 200;
        }

        public string TF(ref string szFile)
        {
            return szFile + ".tmp";
        }

        public bool GetNewUpdate(string AppBuild)
        {
            string text = "https://mp.kantarmedia.fr/fret2/MultiUser/";
            bool flag = false;
            text = text.Replace(" ", "%20");
            LocalFile = Application.ExecutablePath;
            string text2 = mFileFunction.ParsePath(LocalFile, 1);
            int hInternetSession = hInternet;
            string sUrl = text + text2 + ".Update";
            string sHeaders = null;
            int hOpen = InternetOpenUrl(hInternetSession, ref sUrl, ref sHeaders, 0, InternetFlagsMask, 0L);
            if (hOpen != 0)
            {
                Application.DoEvents();
                string text3 = null;
                if (Status(ref hOpen))
                {
                    text3 = ReadDataToString(hOpen);
                }
                InternetCloseHandle(hOpen);
                if (Operators.CompareString(text3, "", TextCompare: false) != 0 && Strings.InStr(text3, "##", CompareMethod.Text) > 0)
                {
                    string[] array = Strings.Split(text3, ";");
                    if (mFunction.StrCompTextVersions(array[0], AppBuild) != 0)
                    {
                        CheckSum = array[2];
                        RemoteSize = Conversions.ToDouble(array[1]);
                        text = text.Replace(" ", "%20");
                        int hInternetSession2 = hInternet;
                        sHeaders = text + text2 + "." + CheckSum;
                        sUrl = null;
                        hOpen = InternetOpenUrl(hInternetSession2, ref sHeaders, ref sUrl, 0, InternetFlagsMask, 0L);
                        if (hOpen == 0)
                        {
                            goto IL_03ed;
                        }
                        string text4 = mFileFunction.ParsePath(LocalFile, 4);
                        if (!mFileFunction.IsDirectoryWritable(text4))
                        {
                            Interaction.MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", "\"" + text4 + "\""), MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                        }
                        string text5 = mFileFunction.ParsePath(LocalFile, 8);
                        string text6 = text5 + ".old";
                        _ = text5 + ".new";
                        string text7 = text5 + ".exe";
                        if (File.Exists(text6))
                        {
                            try
                            {
                                File.Delete(text6);
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                Interaction.MsgBox("Delete file:\r\n" + ex2.Message, MsgBoxStyle.Exclamation);
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (File.Exists(LocalFile))
                        {
                            try
                            {
                                File.Move(text7, text6);
                            }
                            catch (Exception ex3)
                            {
                                ProjectData.SetProjectError(ex3);
                                Exception ex4 = ex3;
                                Interaction.MsgBox("backup file:\r\n" + ex4.Message, MsgBoxStyle.Exclamation);
                                InternetCloseHandle(hOpen);
                                flag = false;
                                ProjectData.ClearProjectError();
                                return flag;
                            }
                        }
                        if (!File.Exists(text7))
                        {
                            if (Status(ref hOpen))
                            {
                                ReadDataToFile(hOpen);
                            }
                            InternetCloseHandle(hOpen);
                            if (!File.Exists(TF(ref LocalFile)))
                            {
                                flag = false;
                                goto IL_03ed;
                            }
                            if (RemoteSize != (double)FileSystem.FileLen(TF(ref LocalFile)))
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex5)
                                {
                                    ProjectData.SetProjectError(ex5);
                                    Exception ex6 = ex5;
                                    Interaction.MsgBox("Delete temporary file (SIZE):\r\n" + ex6.Message, MsgBoxStyle.Exclamation);
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                                try
                                {
                                    File.Move(text6, text7);
                                }
                                catch (Exception ex7)
                                {
                                    ProjectData.SetProjectError(ex7);
                                    Exception ex8 = ex7;
                                    Interaction.MsgBox("Restore file (SIZE):\r\n" + ex8.Message, MsgBoxStyle.Exclamation);
                                    flag = false;
                                    ProjectData.ClearProjectError();
                                    return flag;
                                }
                            }
                            else if (Operators.CompareString(mCheckSum.Append(TF(ref LocalFile)), DeleteZ(CheckSum), TextCompare: false) != 0)
                            {
                                try
                                {
                                    File.Delete(TF(ref LocalFile));
                                }
                                catch (Exception ex9)
                                {
                                    ProjectData.SetProjectError(ex9);
                                    Exception ex10 = ex9;
                                    Interaction.MsgBox("Delete temporary file (CRC):\r\n" + ex10.Message, MsgBoxStyle.Exclamation);
                                    ProjectData.ClearProjectError();
                                }
                                try
                                {
                                    File.Move(text6, text7);
                                }
                                catch (Exception ex11)
                                {
                                    ProjectData.SetProjectError(ex11);
                                    Exception ex12 = ex11;
                                    Interaction.MsgBox("Restore file (CRC):\r\n" + ex12.Message, MsgBoxStyle.Exclamation);
                                    ProjectData.ClearProjectError();
                                }
                            }
                            else
                            {
                                try
                                {
                                    File.Move(TF(ref LocalFile), LocalFile);
                                }
                                catch (Exception ex13)
                                {
                                    ProjectData.SetProjectError(ex13);
                                    Exception ex14 = ex13;
                                    Interaction.MsgBox("Install file:\r\n" + ex14.Message, MsgBoxStyle.Exclamation);
                                    ProjectData.ClearProjectError();
                                }
                                flag = true;
                            }
                        }
                    }
                }
                flag = flag;
            }
            goto IL_03ed;
            IL_03ed:
            return flag;
        }
    }
}
